#include "../../stdafx.h"
#include "filesystem.h"
#include "../../modules/core/preprocessor.h"

#include <ipps.h>
#include <QMutexLocker>
#include <stdint.h>
#include <QXmlDefaultHandler>
#include <sstream>

namespace filesystem
{
    FileIOBase::FileIOBase(): mutex_(QMutex::Recursive)
    {

    }

    bool FileIOBase::Open( const QString& file_name, bool share_read, bool share_write, bool read_only, bool truncate )
    {
        MutexLockerType locker(&mutex_);
        file_                   = std::make_shared<win32::File>(file_name.toStdWString());
        current_position_       = 0;
        capabilities_.read_only = read_only;

        if(file_->Open(read_only, share_read, share_write, truncate))
            return true;
        else
        {
            file_.reset();
            return false;
        }
    }

    QString FileIOBase::FileName()
    {
        MutexLockerType locker(&mutex_);
        if(file_)
            return QString::fromStdWString(file_->file_name());
        return QString();
    }

    bool FileIOBase::IsOpened()
    {
        MutexLockerType locker(&mutex_);
        if(!file_)
            return false;
        return file_->operator bool();
    }

    bool FileIOBase::Read( uint8_t* buffer, qint64 bytes_count )
    {
        MutexLockerType locker(&mutex_);
        if(!file_)
            return false;
        return file_->ReadFromPosition(current_position_, buffer, bytes_count, &current_position_);
    }

    FileCapabilities FileIOBase::Capabilities() const
    {
        return capabilities_;
    }

    double FileIOBase::TunerFrequency() const
    {
        return tuner_frequency_hz_;
    }

    double FileIOBase::ReferenceLevel_dB() const
    {
        return reference_level_db_;
    }

    bool FileIOBase::ReadOnly() const
    {
        return capabilities_.read_only;
    }

    bool FileIOBase::Append( uint8_t* buffer, int64_t bytes_count )
    {
        MutexLockerType locker(&mutex_);
        if ((!file_) || (ReadOnly()))
            return false;
        else
            if (file_->Append(buffer,bytes_count) == bytes_count)
                return true;

        return false;
    }

    PCMReader::PCMReader( dsp::SampleType sample_type, double sample_rate, double tuner_frequency_hz /*= 0.0*/ ):
        FileIOBase(),
        sample_type_(sample_type),
        sample_rate_(sample_rate),
        sample_size_(dsp::SampleSizeByType(sample_type_)),
        is_sample_complex_(dsp::IsComplexSampleType(sample_type_)),
        header_size_(0),
        tail_size_(0),
        postprocess_enabled_(false)
    {
        if(sample_rate <= 0)
            throw "Wrong sample rate";

        current_position_   = 0;
        reference_level_db_ = 0.0;
        tuner_frequency_hz_ = tuner_frequency_hz;
        if(tuner_frequency_hz_ != 0.0)
            capabilities_.tuner_frequency_available = true;
    }

    bool PCMReader::Seek( qint64 position )
    {
        if(position < 0)
            return false;

        MutexLockerType locker(&mutex_);
        if(!file_)
            return false;

        if( file_->size() < static_cast<int64_t>(header_size_ + position*sample_size_) )
            return false;

        current_position_ = header_size_ + position*sample_size_;
        return true;
    }

    bool ConvertSamplesTo32f(const unsigned char* in_buffer, dsp::SampleType sample_type, Ipp32f* out_buffer, const int samples_count)
    {
        if(samples_count == 0)
            return true;

        switch(sample_type)
        {
        case dsp::kSample16s:   _DBG_IPP_STATUS(ippsConvert_16s32f( reinterpret_cast<const Ipp16s*>(in_buffer), out_buffer, samples_count ));   return true;
        case dsp::kSample32f:   _DBG_IPP_STATUS(ippsCopy_32f      ( reinterpret_cast<const Ipp32f*>(in_buffer), out_buffer, samples_count ));   return true;
        case dsp::kSample64f:   _DBG_IPP_STATUS(ippsConvert_64f32f( reinterpret_cast<const Ipp64f*>(in_buffer), out_buffer, samples_count ));   return true;
        default:
            return false;
        }
    }

    static IppStatus Convert_12sc32fc_slow(const uint8_t* in_buffer, Ipp32fc* out_buffer, const int samples_count)
    {
        if(in_buffer == nullptr || out_buffer == nullptr)
            return ippStsNullPtrErr;

        for(int i = 0; i < samples_count; i++)
        {
            uint32_t Data = *((uint32_t*)in_buffer);
            short Re = Data & 0xFFF;
            if(Re & 0x800)
                Re = Re | 0xF000;
            short Im = (Data >> 12) & 0xFFF;
            if(Im & 0x800)
                Im = Im | 0xF000;
            out_buffer->re = Re;
            out_buffer->im = Im;
            out_buffer++;
            in_buffer += 3;
        }
        return ippStsNoErr;
    }

    IppStatus Convert_12sc32fc_fast(const uint8_t* in_buffer, Ipp32fc* out_buffer, const int samples_count)
    {
        if(in_buffer == nullptr || out_buffer == nullptr)
            return ippStsNullPtrErr;

        size_t          sse_cycles      = samples_count / 4;
        int             remainder       = samples_count % 4;

        __m128i         re_sign_mask    = _mm_set1_epi32(0x00000800);
        __m128i         im_sign_mask    = _mm_set1_epi32(0x00800000);

        __m128i         sign_mask       = _mm_set1_epi32(0xFFFFF000);

        if(((ptrdiff_t)out_buffer) % 16 == 0)
        {
            for(size_t i = 0; i < sse_cycles; i++)
            {
                __m128i re                  = _mm_set_epi32(*((uint32_t*)(in_buffer+9)), *((uint32_t*)(in_buffer+6)), *((uint32_t*)(in_buffer+3)), *((uint32_t*)(in_buffer+0)));
                __m128i im                  = _mm_srli_epi32(re, 12);

                __m128i re_mask             = _mm_and_si128(re, re_sign_mask);
                __m128i im_mask             = _mm_and_si128(re, im_sign_mask);

                        re_mask             = _mm_cmpeq_epi32(re_mask, re_sign_mask);
                        im_mask             = _mm_cmpeq_epi32(im_mask, im_sign_mask);

                        re_mask             = _mm_and_si128(sign_mask, re_mask);
                        im_mask             = _mm_and_si128(sign_mask, im_mask);

                        re                  = _mm_andnot_si128(sign_mask, re);
                        im                  = _mm_andnot_si128(sign_mask, im);

                        re                  = _mm_or_si128(re_mask, re);
                        im                  = _mm_or_si128(im_mask, im);

                __m128i result1             = _mm_unpacklo_epi32(re, im);
                __m128i result2             = _mm_unpackhi_epi32(re, im);

                _mm_store_ps((float*)out_buffer,        _mm_cvtepi32_ps(result1));
                _mm_store_ps((float*)(out_buffer+2),    _mm_cvtepi32_ps(result2));

                out_buffer          += 4;
                in_buffer           += 3*4;
            }
        }
        else
            for(size_t i = 0; i < sse_cycles; i++)
            {
                __m128i re                  = _mm_set_epi32(*((uint32_t*)(in_buffer+9)), *((uint32_t*)(in_buffer+6)), *((uint32_t*)(in_buffer+3)), *((uint32_t*)(in_buffer+0)));
                __m128i im                  = _mm_srli_epi32(re, 12);

                __m128i re_mask             = _mm_and_si128(re, re_sign_mask);
                __m128i im_mask             = _mm_and_si128(re, im_sign_mask);

                        re_mask             = _mm_cmpeq_epi32(re_mask, re_sign_mask);
                        im_mask             = _mm_cmpeq_epi32(im_mask, im_sign_mask);

                        re_mask             = _mm_and_si128(sign_mask, re_mask);
                        im_mask             = _mm_and_si128(sign_mask, im_mask);

                        re                  = _mm_andnot_si128(sign_mask, re);
                        im                  = _mm_andnot_si128(sign_mask, im);

                        re                  = _mm_or_si128(re_mask, re);
                        im                  = _mm_or_si128(im_mask, im);

                __m128i result1             = _mm_unpacklo_epi32(re, im);
                __m128i result2             = _mm_unpackhi_epi32(re, im);

                _mm_storeu_ps((float*)out_buffer,        _mm_cvtepi32_ps(result1));
                _mm_storeu_ps((float*)(out_buffer+2),    _mm_cvtepi32_ps(result2));

                out_buffer          += 4;
                in_buffer           += 3*4;
            }


        if(remainder)
            Convert_12sc32fc_slow(in_buffer, out_buffer, remainder);

        return ippStsNoErr;
    }


    static bool ConvertSamplesTo32fc(const unsigned char* in_buffer, dsp::SampleType sample_type, Ipp32fc* out_buffer, const int samples_count)
    {
        if(samples_count == 0)
            return true;

        switch(sample_type)
        {
        case dsp::kSample16sc:  _DBG_IPP_STATUS(ippsConvert_16s32f      ( reinterpret_cast<const Ipp16s*>(in_buffer), reinterpret_cast<Ipp32f*>(out_buffer), samples_count*2 ));   return true;
        case dsp::kSample32fc:  _DBG_IPP_STATUS(ippsCopy_32f            ( reinterpret_cast<const Ipp32f*>(in_buffer), reinterpret_cast<Ipp32f*>(out_buffer), samples_count*2 ));   return true;
        case dsp::kSample64fc:  _DBG_IPP_STATUS(ippsConvert_64f32f      ( reinterpret_cast<const Ipp64f*>(in_buffer), reinterpret_cast<Ipp32f*>(out_buffer), samples_count*2 ));   return true;
        case dsp::kSample12sc:  _DBG_IPP_STATUS(Convert_12sc32fc_fast   (                                 in_buffer,                             out_buffer, samples_count   ));   return true;
        case dsp::kSample32sc:  _DBG_IPP_STATUS(ippsConvert_32s32f      ( reinterpret_cast<const Ipp32s*>(in_buffer), reinterpret_cast<Ipp32f*>(out_buffer), samples_count*2 ));   return true;
        default:
            return false;
        }
    }

    bool PCMReader::Read( Ipp32fc* buffer, qint64 samples_count )
    {
        if(samples_count < 0)
            return false;

        if(samples_count == 0)
            return true;

        MutexLockerType locker(&mutex_);

        if(!is_sample_complex_)
            return false;

        if(sample_type_ == dsp::kSample32fc)
            return file_->ReadFromPosition(current_position_, buffer, samples_count*sample_size_, &current_position_) != 0;
        else
        {
            auto temp_buffer                = work_buffer_.PrepareForWrite(static_cast<size_t>(samples_count*sample_size_+16)); //  16 байт на использование SSE

            _DBG_POINTER(temp_buffer);

            if(current_position_+samples_count >= SampleCount())
                memset(temp_buffer, 0, static_cast<size_t>(samples_count*sample_size_+16)*sizeof(temp_buffer[0]));

            auto old_position               = current_position_;

            if(file_->ReadFromPosition(current_position_, temp_buffer, samples_count*sample_size_, &current_position_) == 0)
                return false;
            
            _DBG_CONDITION(old_position <= current_position_);

            auto result                     = ConvertSamplesTo32fc(temp_buffer, sample_type_, buffer, samples_count);

            if(postprocess_enabled_)
                PostprocessSamples(buffer, samples_count);

            const size_t max_buffer_size    = 16*1024*1024;
            if(work_buffer_.AvailWrite() > max_buffer_size)
                work_buffer_.Clear();

            return result;
        }
    }

    bool PCMReader::Read( Ipp32f* buffer, qint64 samples_count )
    {
        if(samples_count < 0)
            return false;

        if(samples_count == 0)
            return true;

        MutexLockerType locker(&mutex_);

        if(is_sample_complex_)
            return false;

        if(sample_type_ == dsp::kSample32f)
            return file_->ReadFromPosition(current_position_, buffer, samples_count*sample_size_, &current_position_) != 0;
        else
        {
            auto temp_buffer                = work_buffer_.PrepareForWrite(static_cast<size_t>(samples_count*sample_size_));
            _DBG_POINTER(temp_buffer);

            auto result                     = ConvertSamplesTo32f(temp_buffer, sample_type_, buffer, samples_count);

            const size_t max_buffer_size    = 16*1024*1024;
            if(work_buffer_.AvailWrite() > max_buffer_size)
                work_buffer_.Clear();

            return result;
        }
    }

    qint64 PCMReader::SampleCount()
    {
        MutexLockerType locker(&mutex_);
        if(!file_)
            return 0;

        _DBG_CONDITION(static_cast<size_t>(file_->size()) >= tail_size_);
        _DBG_CONDITION(static_cast<size_t>(file_->size()) >= header_size_);
        _DBG_CONDITION(static_cast<size_t>(file_->size()) >= (header_size_ + tail_size_));

        return (file_->size()-header_size_-tail_size_) / sample_size_;
    }

    PCMReader* PCMReader::Clone() const
    {
        PCMReader* result           = new PCMReader(sample_type_, sample_rate_);
        result->header_size_        = header_size_;
        result->file_               = file_;
        result->current_position_   = current_position_;
        result->capabilities_       = capabilities_;
        result->reference_level_db_ = reference_level_db_;
        result->tuner_frequency_hz_ = tuner_frequency_hz_;

        return result;
    }


    ITMReader::ITMReader(): PCMReader(dsp::kSample12sc, 1)
    {
    }

    bool ITMReader::Read( Ipp32f* buffer, qint64 samples_count )
    {
        return false;
    }

    bool ITMReader::Open( const QString& file_name, bool share_read, bool share_write, bool read_only, bool truncate )
    {
        MutexLockerType locker(&mutex_);
        file_                       = std::make_shared<win32::File>(file_name.toStdWString());
        current_position_           = 0;
        capabilities_.read_only     = read_only;

        int64_t current_position    = 0;

        if(file_->Open(read_only, share_read, share_write, truncate))
        {
            uint32_t RIFF_magic = 0;
            if(file_->ReadFromPosition(0, &RIFF_magic, sizeof(RIFF_magic), &current_position) && RIFF_magic == 0x46464952)
            {
                uint32_t WAVE_magic = 0;
                if(file_->ReadFromPosition(8, &WAVE_magic, 4, &current_position) && WAVE_magic == 0x45564157)
                {
                    uint32_t fmt_magic = 0;                    
                    if(file_->ReadFromPosition(current_position, &fmt_magic, 4, &current_position) && fmt_magic == 0x20746D66)
                    {
                        uint32_t fmt_size = 0;
                        if(file_->ReadFromPosition(current_position, &fmt_size, 4, &current_position) && fmt_size != 0 && fmt_size < 100)
                        {
                            struct WaveFormat
                            {
                                ushort	wFormatTag;
                                ushort	nChannels;
                                uint	nSamplesPerSec;
                                uint	nAvgBytesPerSec;
                                ushort	nBlockAlign;
                            };

                            WaveFormat wave_format = {0};
                            if(file_->ReadFromPosition(current_position, &wave_format, sizeof(wave_format), &current_position))
                            {
                                if(wave_format.wFormatTag == 1 && wave_format.nChannels == 2 && wave_format.nBlockAlign == 3)  //  PCM, stereo, 12-bit
                                {
                                    current_position    = fmt_size - sizeof(wave_format) + current_position;

                                    uint32_t data_magic = 0;

                                    if(file_->ReadFromPosition(current_position, &data_magic, 4, &current_position) && data_magic == 0x61746164)
                                    {
                                        uint64_t data_size = 0;
                                        if(file_->ReadFromPosition(current_position, &data_size, 4, &current_position) && data_size > 0)
                                        {
                                            header_size_        = current_position;
                                            current_position_   = current_position;
                                            sample_rate_        = wave_format.nSamplesPerSec;
                                            tail_size_          = file_->size() - header_size_ - data_size;

                                            if(tail_size_ > 0)
                                            {
                                                StdStorage  data_buffer(tail_size_);
                                                auto        size = file_->ReadFromPosition(current_position + data_size, data_buffer, tail_size_);
                                                data_buffer.WriteComplete(static_cast<size_t>(size));

                                                double tuner_frequency = 0;
                                                if(ParseTail(data_buffer, data_buffer.AvailRead(), tuner_frequency))
                                                {
                                                    tuner_frequency_hz_                     = tuner_frequency;
                                                    capabilities_.tuner_frequency_available = true;
                                                }
                                            }

                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        file_.reset();
        return false;
    }

    bool ITMReader::ParseTail(const uint8_t* list_chunk_data, size_t size, double &tuner_frequency_hz)
    {
        if(size > 20)
        {
            if(*(const uint32_t*)list_chunk_data == 0x5453494C) //  LIST
            {
                uint32_t list_size = *(const uint32_t*)(list_chunk_data+4);
                if(list_size <= (size-4-4))
                {
                    if(*(const uint32_t*)(list_chunk_data+4+4) == 0x4F464E49)  //  INFO
                        if(*(const uint32_t*)(list_chunk_data+4+4+4) == 0x544D4349)  //  ICMT
                        {
                            uint32_t comment_size = *(const uint32_t*)(list_chunk_data+4+4+4+4);
                            if(comment_size <= (size-4-4-4-4))
                            {
                                const char* comment_ptr = reinterpret_cast<const char*>(list_chunk_data+4+4+4+4+4);
                                
                                std::string central_freq_string = "÷ентральна€„астота=";
                                std::istringstream sstrm(comment_ptr);

                                while(!sstrm.eof())
                                {
                                    std::string str;
                                    sstrm >> str;
                                    if(str.find(central_freq_string) == 0)
                                    {
                                        str.erase(0, central_freq_string.length());
                                        std::istringstream sstrm2(str);

                                        double tuner_frequency = 0;
                                        sstrm2 >> tuner_frequency;

                                        if(tuner_frequency >= 0)
                                        {
                                            tuner_frequency_hz = tuner_frequency;
                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                }
            }
        }
        return false;
    }

    TIQReader::TIQReader():
        PCMReader(dsp::kSample32sc, 1),
        scale_(1)
    {
        postprocess_enabled_ = true;
    }

    bool TIQReader::Open( const QString& file_name, bool share_read, bool share_write, bool read_only, bool truncate )
    {
        static const char* header = "<DataFile offset=\"";

        MutexLockerType locker(&mutex_);
        file_                       = std::make_shared<win32::File>(file_name.toStdWString());
        current_position_           = 0;
        capabilities_.read_only     = read_only;

        int64_t current_position    = 0;

        if(file_->Open(true, share_read, share_write))
        {
            char magic_buffer[20] = {0};
            if(file_->ReadFromPosition(0, magic_buffer, 18, &current_position))
            {
                if(memcmp(magic_buffer, header, 18) == 0)
                {
                    char offset_buffer[10] = {0};
                    if(file_->ReadFromPosition(19, offset_buffer, 9, &current_position))
                    {
                        int offset = atoi(offset_buffer);
                        if(offset > 19)
                        {
                            QByteArray datafile_xml_text;
                            datafile_xml_text.resize(offset);

                            if(file_->ReadFromPosition(0, datafile_xml_text.data(), offset, &current_position))
                            {
                                double sample_rate;
                                double scale;
                                double tuner_frequency;

                                if(ParseXML(datafile_xml_text, sample_rate, scale, tuner_frequency))
                                {
                                    header_size_                            = offset;
                                    current_position_                       = offset;
                                    sample_rate_                            = sample_rate;
                                    scale_                                  = scale;
                                    tuner_frequency_hz_                     = tuner_frequency;

                                    capabilities_.tuner_frequency_available = true;

                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }

        file_.reset();
        return false;
    }

    bool TIQReader::Read( Ipp32f* buffer, qint64 samples_count )
    {
        return false;
    }

    void TIQReader::PostprocessSamples( Ipp32fc* buffer, qint64 samples_count )
    {
        if(samples_count)
            _DBG_IPP_STATUS( ippsMulC_32f_I(scale_, reinterpret_cast<Ipp32f*>(buffer), static_cast<int>(samples_count*2)) );
    }

    class DataFileXMLParser: public QXmlDefaultHandler
    {
    public:
        DataFileXMLParser():
            in_data_description_(),
            in_product_specific_(),
            in_sampling_frequency_(),
            in_samples_type_(),
            in_number_format_(),
            in_endian_(),
            in_scaling_(),
            in_frequency_(),
            sample_rate_(),
            scale_(),
            tuner_frequency_()
        {}

        bool endElement( const QString&, const QString&, const QString &name )
        {
            if ( name == "DataDescription" )    in_data_description_    = false;
            if ( name == "ProductSpecific" )    in_product_specific_    = false;
            if ( name == "SamplingFrequency" )  in_sampling_frequency_  = false;
            if ( name == "SamplesType" )        in_samples_type_        = false;
            if ( name == "NumberFormat" )       in_number_format_       = false;
            if ( name == "Endian" )             in_endian_              = false;
            if ( name == "Scaling" )            in_scaling_             = false;
            if ( name == "Frequency" )          in_frequency_           = false;

            return true;
        }

        bool characters(const QString& chars)
        {
            if(chars.size() == 0)
                return true;
            if(!in_data_description_ && !in_product_specific_)
                return true;

            if(in_sampling_frequency_)
            {
                bool ok;
                double sample_rate = chars.toDouble(&ok);
                    
                _DBG_CONDITION(ok);
                if(!ok || sample_rate < 1)
                    return false;

                sample_rate_ = sample_rate;
            }
            else
            if(in_samples_type_)
            {
                if(chars != "IQ")
                    return false;
            }
            else
            if(in_number_format_)
            {
                if(chars != "Int32")
                    return false;
            }
            else
            if(in_endian_)
            {
                if(chars != "Little")
                    return false;
            }
            else
            if(in_scaling_)
            {
                bool ok;
                double scaling = chars.toDouble(&ok);

                _DBG_CONDITION(ok);
                if(!ok || scaling <= 0)
                    return false;

                scale_ = scaling;
            }
            else
            if(in_frequency_)
            {
                bool ok;
                double frequency = chars.toDouble(&ok);

                _DBG_CONDITION(ok);
                if(!ok || frequency < 0)
                    return false;

                tuner_frequency_ = frequency;
            }

            return true;
        }

        bool startElement( const QString&, const QString&, const QString &name, const QXmlAttributes & )
        {
            if ( name == "DataDescription" )    in_data_description_    = true;
            if ( name == "SamplingFrequency" )  in_sampling_frequency_  = true;
            if ( name == "SamplesType" )        in_samples_type_        = true;
            if ( name == "NumberFormat" )       in_number_format_       = true;
            if ( name == "Endian" )             in_endian_              = true;
            if ( name == "Scaling" )            in_scaling_             = true;
            if ( name == "Frequency" )          in_frequency_           = true;
            if ( name == "ProductSpecific" )    in_product_specific_    = true;

            return true;
        }

        double  sample_rate() const     { return sample_rate_;      }
        double  scale() const           { return scale_;            }
        double  tuner_frequency() const { return tuner_frequency_;  }
        bool    is_correct() const      { return sample_rate_ > 0 && scale_ > 0 && tuner_frequency_ >= 0.0; }

    private:
        bool    in_data_description_;
        bool    in_product_specific_;
        bool    in_sampling_frequency_;
        bool    in_samples_type_;
        bool    in_number_format_;
        bool    in_endian_;
        bool    in_scaling_;
        bool    in_frequency_;
        double  sample_rate_;
        double  scale_;
        double  tuner_frequency_;
    };

    bool TIQReader::ParseXML( QByteArray& xml, double& sample_rate, double& scale, double& tuner_frequency ) const
    {
        QBuffer buffer(&xml);
        QXmlSimpleReader    reader;
        DataFileXMLParser   parser;
        reader.setContentHandler(&parser);
        if(reader.parse(QXmlInputSource(&buffer)))
        {
            if(parser.is_correct())
            {
                sample_rate     = parser.sample_rate();
                scale           = parser.scale();
                tuner_frequency = parser.tuner_frequency();
                return true;
            }
        }
        return false;
    }

    TIQReader* TIQReader::Clone() const
    {
        auto result                     = new TIQReader();
        result->header_size_            = header_size_;
        result->file_                   = file_;
        result->current_position_       = current_position_;
        result->capabilities_           = capabilities_;
        result->reference_level_db_     = reference_level_db_;
        result->tuner_frequency_hz_     = tuner_frequency_hz_;

        result->sample_type_            = sample_type_;
        result->sample_rate_            = sample_rate_;
        result->sample_size_            = sample_size_;
        result->is_sample_complex_      = is_sample_complex_;
        result->header_size_            = header_size_;
        result->postprocess_enabled_    = postprocess_enabled_;

        result->scale_                  = scale_;

        return result;
    }

}  //  namespace filesystem
