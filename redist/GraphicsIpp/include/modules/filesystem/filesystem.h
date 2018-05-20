#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include "../../sample_types.h"
#include "../../modules/core/preprocessor.h"
#include "../../modules/windows/windows_tools.h"
#include "../../modules/core/storage_inl.h"
#include <memory>
#include <QMutex>
#include <QBuffer>
#include <QString>

#undef min
#undef max

namespace filesystem
{
    typedef QMutex          MutexType;
    typedef QMutexLocker    MutexLockerType;

    union FileCapabilities
    {
        uint64_t flags_;
        struct
        {
            bool tuner_frequency_available: 1;
            bool reference_level_available: 1;
            bool read_only:                 1;
            bool remove_first_available:    1;
        };

        FileCapabilities(): flags_() {}
    };

    class ReaderInterface
    {
    public:

        virtual ~ReaderInterface() {}
        virtual bool                Open(const QString& file_name, bool share_read = false, bool share_write = false, bool read_only = true, bool truncate = true)  = 0;
        virtual QString             FileName()                                                                          = 0;
        virtual bool                IsOpened()                                                                          = 0;

        virtual bool                Seek(int64_t        position)                                                       = 0;
        virtual bool                Read(uint8_t*       buffer, int64_t bytes_count)                                    = 0;
        virtual bool                Read(Ipp32f*        buffer, int64_t samples_count)                                  = 0;
        virtual bool                Read(Ipp32fc*       buffer, int64_t samples_count)                                  = 0;
        virtual bool                Append(uint8_t*     buffer, int64_t bytes_count)                                    = 0;
        virtual bool                Append(Ipp32f*      buffer, int64_t samples_count)                                  = 0;
        virtual bool                Append(Ipp32fc*     buffer, int64_t samples_count)                                  = 0;
        virtual bool                RemoveFirst(int64_t samples_count)                                                  = 0;

        virtual FileCapabilities    Capabilities() const                                                                = 0;

        virtual bool                IsComplex()                                                                         = 0;
        virtual int64_t             SampleCount()                                                                       = 0;
        virtual double              SampleRate()                                                                        = 0;
        virtual dsp::SampleType     SampleType()                                                                        = 0;
        virtual double              TunerFrequency() const                                                              = 0;
        virtual double              ReferenceLevel_dB() const                                                           = 0;

        virtual ReaderInterface*    Clone() const                                                                       = 0;
    };

    typedef ReaderInterface IOInterface;
    typedef std::shared_ptr<IOInterface> IOInterfacePtr;

    class FileIOBase: public ReaderInterface
    {
    public:
        FileIOBase();
        virtual bool                Open(const QString& file_name, bool share_read, bool share_write, bool read_only, bool truncate);
        virtual QString             FileName();
        virtual bool                IsOpened();
        virtual bool                ReadOnly() const;

        virtual bool                Read(uint8_t* buffer, int64_t bytes_count);
        virtual bool                Append(uint8_t* buffer, int64_t bytes_count);

        virtual double              TunerFrequency() const;
        virtual double              ReferenceLevel_dB() const;

        virtual FileCapabilities    Capabilities() const;

    protected:
        typedef std::shared_ptr<win32::File>    FileType;

        MutexType                   mutex_;
        FileType                    file_;
        int64_t                     current_position_;

        FileCapabilities            capabilities_;

        double                      reference_level_db_; //  in dBm
        double                      tuner_frequency_hz_;

    private:
        virtual bool                RemoveFirst(int64_t samples_count) { return false; }
    };

    bool ConvertSamplesTo32f(const unsigned char* in_buffer, dsp::SampleType sample_type, Ipp32f* out_buffer, const int samples_count);

    template<dsp::SampleType EnumSampleType>
    class MemoryFile: public ReaderInterface
    {
    public:
        typedef typename dsp::SampleTraits<EnumSampleType>::SampleType  SampleT;
        typedef std::shared_ptr<Storage<SampleT> >                      StoragePtr;

        MemoryFile(const StoragePtr& buffer, double sample_rate): storage_ptr_(buffer), sample_rate_(sample_rate)
        {

        }

        virtual bool                Open(const QString& /*file_name*/, bool /*share_read*/, bool /*share_write*/, bool /*read_only*/, bool /*truncate*/)
        {
            return true;
        }

        virtual QString             FileName()
        {
            return file_name_;
        }

        virtual bool                IsOpened()
        {
            return true;
        }

        virtual bool                Seek(int64_t position)
        {
            MutexLockerType locker(&mutex_);

            if(position >= 0 && position < storage_ptr_->AvailRead())
            {
                current_file_pointer_ = position*sizeof(SampleT);
                return true;
            }
            else
                return false;
        }

        virtual bool                Read(uint8_t* buffer, int64_t bytes_count)
        {
            MutexLockerType locker(&mutex_);

            _DBG_CONDITION(current_file_pointer_ >= 0);
            _DBG_CONDITION(current_file_pointer_ < storage_ptr_->AvailReadBytes());
            _DBG_CONDITION(current_file_pointer_+bytes_count <= storage_ptr_->AvailReadBytes());

            if(current_file_pointer_ >= 0 && current_file_pointer_ <= storage_ptr_->AvailRead())
            {
                memset(buffer, 0, bytes_count);

                size_t to_copy = size_t(std::max(int64_t(0), std::min(bytes_count, int64_t(storage_ptr_->AvailReadBytes()-current_file_pointer_))));

                memcpy(buffer, &(((uint8_t*)storage_ptr_->const_data())[current_file_pointer_]), to_copy);

                current_file_pointer_ += to_copy / sizeof(SampleT);

                _DBG_CONDITION(current_file_pointer_ >= 0);
                _DBG_CONDITION(current_file_pointer_ <= storage_ptr_->AvailReadBytes());

                return true;
            }
            else
            {
                return false;
            }
        }

        virtual bool                Read(Ipp32f* buffer, int64_t samples_count)
        {
            if(EnumSampleType == dsp::kSample32f)
            {
                MutexLockerType locker(&mutex_);

                _DBG_CONDITION(current_file_pointer_ >= 0);
                _DBG_CONDITION(current_file_pointer_ < storage_ptr_->AvailReadBytes());

                const auto requested_bytes_count = samples_count*sizeof(SampleT);

                _DBG_CONDITION(current_file_pointer_+requested_bytes_count <= storage_ptr_->AvailReadBytes());

                if(current_file_pointer_ >= 0 && current_file_pointer_ <= storage_ptr_->AvailReadBytes())
                {
                    memset(buffer, 0, samples_count*sizeof(buffer[0]));

                    size_t to_copy = size_t(std::max(int64_t(0), std::min(int64_t(requested_bytes_count), int64_t(storage_ptr_->AvailReadBytes()-current_file_pointer_))));

                    memcpy(buffer, &(((uint8_t*)storage_ptr_->const_data())[current_file_pointer_]), to_copy);

                    current_file_pointer_ += to_copy;

                    _DBG_CONDITION(current_file_pointer_ >= 0);
                    _DBG_CONDITION(current_file_pointer_ <= storage_ptr_->AvailReadBytes());

                    return true;
                }
                else
                    return false;
            }
            else
            {
                return false;
            }
        }

        virtual bool                Read(Ipp32fc*       buffer, int64_t samples_count)
        {
            return false;
        }

        virtual bool                Append(uint8_t*     buffer, int64_t bytes_count)
        {
            return false;
        }

        virtual bool                Append(Ipp32f*      buffer, int64_t samples_count)
        {
            return false;
        }

        virtual bool                Append(Ipp32fc*     buffer, int64_t samples_count)
        {
            return false;
        }

        virtual bool                RemoveFirst(int64_t samples_count)
        {
            return false;
        }

        virtual FileCapabilities    Capabilities() const
        {
            return FileCapabilities();
        }

        virtual bool                IsComplex()
        {
            return dsp::SampleTraits<EnumSampleType>::is_complex;
        }

        virtual int64_t             SampleCount()
        {
            MutexLockerType locker(&mutex_);
            return storage_ptr_->AvailRead();
        }

        virtual double              SampleRate()
        {
            return sample_rate_;
        }

        virtual dsp::SampleType     SampleType()
        {
            return EnumSampleType;
        }

        virtual double              TunerFrequency() const
        {
            return 0.0;
        }

        virtual double              ReferenceLevel_dB() const
        {
            return 0.0;
        }

        virtual ReaderInterface*    Clone() const
        {
            return nullptr;
        }

    protected:

        StoragePtr  storage_ptr_;
        MutexType   mutex_;
        int64_t     current_file_pointer_;  //  указатель на текущий элемент в массиве в байтах!

        QString     file_name_;
        double      sample_rate_;
        
    };

    class PCMReader: public FileIOBase
    {
    public:
        PCMReader(dsp::SampleType, double sample_rate, double tuner_frequency_hz = 0.0);

        virtual bool                Seek(int64_t     position);
        virtual bool                Read(Ipp32f*    buffer, int64_t samples_count);
        virtual bool                Read(Ipp32fc*   buffer, int64_t samples_count);

        virtual bool                IsComplex()     { return is_sample_complex_; };
        virtual int64_t             SampleCount();
        virtual double              SampleRate()    { return sample_rate_; }
        virtual dsp::SampleType     SampleType()    { return sample_type_; }
        virtual PCMReader*          Clone() const;

    protected:
        dsp::SampleType             sample_type_;
        double                      sample_rate_;
        size_t                      sample_size_;
        bool                        is_sample_complex_;
        size_t                      header_size_;
        size_t                      tail_size_;
        bool                        postprocess_enabled_;

        virtual void                PostprocessSamples(Ipp32fc* buffer, int64_t samples_count) {}

    private:
        StdStorage                  work_buffer_;
        DISALLOW_COPY_AND_ASSIGN(PCMReader);

        virtual bool                Append(uint8_t* buffer, int64_t bytes_count)    { return false; }
        virtual bool                Append(Ipp32f*  buffer, int64_t samples_count)  { return false; }
        virtual bool                Append(Ipp32fc* buffer, int64_t samples_count)  { return false; }
    };

    class ITMReader: public PCMReader
    {
    public:
        ITMReader();

        virtual bool                Open(const QString& file_name, bool share_read, bool share_write, bool read_only, bool truncate);

        using PCMReader::Read;
        virtual bool                Read(Ipp32f*   buffer, int64_t samples_count);

    private:
        DISALLOW_COPY_AND_ASSIGN(ITMReader);

        bool ParseTail(const uint8_t* list_chunk_data, size_t size, double &tuner_frequency_hz);
    };

    class TIQReader: public PCMReader
    {
    public:
        TIQReader();

        virtual bool                Open(const QString& file_name, bool share_read, bool share_write, bool read_only, bool truncate);

        using PCMReader::Read;
        virtual bool                Read(Ipp32f*   buffer, int64_t samples_count);

        virtual TIQReader*          Clone() const;

    protected:
        virtual void                PostprocessSamples(Ipp32fc* buffer, int64_t samples_count);

    private:
        DISALLOW_COPY_AND_ASSIGN(TIQReader);

        float                       scale_;

        bool                        ParseXML(QByteArray& xml, double& sample_rate, double& scale, double& tuner_frequency) const;
    };

}

#endif
