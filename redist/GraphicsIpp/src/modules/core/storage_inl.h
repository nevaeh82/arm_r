#ifndef MODULES_CORE_STORAGE_H_
#define MODULES_CORE_STORAGE_H_

#include "../../modules/core/preprocessor.h"
#include <string>
#include <assert.h>
#include <limits.h>
#include <tchar.h>
#include <stdint.h>

class StdErrReportingPolicy
{
public:
    void AllocError(const size_t size) const
    {
        TCHAR debug_buffer[128];
		_stprintf_s(debug_buffer, _T("Error! Can't allocate %i bytes\n"), size);
        _ftprintf_s(stderr, debug_buffer);
        return;
    }
    void GuardBlocksError() const
    {
        _ftprintf_s(stderr, _T("Guard blocks check error\n"));
        return;
    }
    void BufferOverflowError() const
    {
        _ftprintf_s(stderr, _T("Buffer overflow error\n"));
        return;
    }
    void BufferUnderflowError() const
    {
        _ftprintf_s(stderr, _T("Buffer underflow error\n"));
        return;
    }
    void BufferDefaultSizeError() const
    {
        _ftprintf_s(stderr, _T("Buffer default size error\n"));
        return;
    }
    void SizeError() const
    {
        _ftprintf_s(stderr, _T("Size error\n"));
        return;
    }
};

class ThrowExceptionErrorReportingPolicy
{
public:
    void AllocError(const size_t size) const
    {
        TCHAR debug_buffer[128];
		_stprintf_s(debug_buffer, _T("Error! Can't allocate %i bytes\n"), size);
        throw std::basic_string<TCHAR>(debug_buffer);
    }
    void GuardBlocksError() const
    {
        throw std::basic_string<TCHAR>(_T("Guard blocks check error\n"));
    }
    void BufferOverflowError() const
    {
        throw std::basic_string<TCHAR>(_T("Buffer overflow error\n"));
    }
    void BufferUnderflowError() const
    {
        throw std::basic_string<TCHAR>(_T("Buffer underflow error\n"));
    }
    void BufferDefaultSizeError() const
    {
        throw std::basic_string<TCHAR>(_T("Buffer default size error\n"));
    }
    void SizeError() const
    {
        throw std::basic_string<TCHAR>(_T("Size error\n"));
    }
};

#pragma warning (disable: 279)

class AssertErrReportingPolicy
{
public:
    void AllocError(const size_t size) const
    {
        size;
        assert(_T("Error! Can't allocate memory\n") && size && 0);
    }
    void GuardBlocksError() const
    {
        assert(_T("Guard blocks check error") && 0);
    }
    void BufferOverflowError() const
    {
        assert(_T("Buffer overflow error") && 0);
    }
    void BufferUnderflowError() const
    {
        assert(_T("Buffer underflow error") && 0);
    }
    void BufferDefaultSizeError() const
    {
        assert(_T("Buffer default size error") && 0);
    }
    void SizeError() const
    {
        assert(_T("Size error\n") && 0);
    }
};
#pragma warning (default: 279)

#pragma warning (disable: 4702)

//************************************
// Class:       Storage
// FullName:    Storage<T, ErrorReportingPolicy>
// Parameter:   class T                 - тип хранимых данных
// Parameter:   ErrorReportingPolicy    - стратегия реагирования на ошибки выделения памяти и перезаписи границ
//************************************
template<
            class T,
            class ErrorReportingPolicy = ThrowExceptionErrorReportingPolicy
        >
class Storage: public ErrorReportingPolicy
{
public:
    //  default_buffer_size - минимальный размер буфера по-умолчанию
    //  file_name           - имя файла (для отладки)
    //  line                - номер строки в файле (для отладки)
    Storage(size_t default_buffer_size = (4096-4*2)):
        ErrorReportingPolicy(),
        guard_block_size_(sizeof(T) < 16 ? 16 : ((sizeof(T)+63) & -64)),
        default_size_(default_buffer_size)
    {
        
        if(default_buffer_size <= 0 || default_buffer_size > INT_MAX)
            BufferDefaultSizeError();
        InitBuffer(default_buffer_size);
    }

    Storage(Storage&& src):
        ErrorReportingPolicy    (),
        start_data_ptr_         (src.start_data_ptr_),
        data_                   (src.data_),
        guard_block_size_       (src.guard_block_size_),
        real_data_ptr_          (src.real_data_ptr_),
        buffer_size_            (src.buffer_size_),
        end_data_ptr_           (src.end_data_ptr_),
        default_size_           (src.default_size_)
    {
        src.start_data_ptr_ = size_t();
        src.data_           = nullptr;
        src.real_data_ptr_  = nullptr;
        src.buffer_size_    = size_t();
        src.end_data_ptr_   = size_t();
        src.InitBuffer(default_size_);
    }

    Storage& operator=(Storage&& src)
    {
        if(this != &src)
        {
            _aligned_free(real_data_ptr_);
            start_data_ptr_     = src.start_data_ptr_;
            data_               = src.data_;
            guard_block_size_   = src.guard_block_size_;
            real_data_ptr_      = src.real_data_ptr_;
            buffer_size_        = src.buffer_size_;
            end_data_ptr_       = src.end_data_ptr_;
            default_size_       = src.default_size_;

            src.start_data_ptr_ = size_t();
            src.data_           = nullptr;
            src.real_data_ptr_  = nullptr;
            src.buffer_size_    = size_t();
            src.end_data_ptr_   = size_t();
            src.InitBuffer(default_size_);
        }
        return *this;
    }

    ~Storage()
    {
        CheckGuardBlocksOrFail();
        _aligned_free(real_data_ptr_);
    }

    explicit Storage(const Storage& src):
    ErrorReportingPolicy(),
        start_data_ptr_     (),
        data_               (),
        guard_block_size_   (src.guard_block_size_),
        real_data_ptr_      (),
        buffer_size_        (),
        end_data_ptr_       (),
        default_size_       (src.default_size_)
    {
        WriteData(src.operator const T *(), src.AvailRead());
    }

    Storage& operator=(const Storage& src)
    {
        CheckGuardBlocksOrFail();
        _aligned_free(real_data_ptr_);
        start_data_ptr_ = 0;
        data_           = 0;
        real_data_ptr_  = 0;
        buffer_size_    = 0;
        end_data_ptr_   = 0;
        WriteData(src.operator const T *(), src.AvailRead());

        return *this;
    }

    //  Очищает буфер, проверяет границы. Если было выделено в 32 раза больше памяти чем изначально, приводит буфер
    //  к начальному размеру
    void Clear()
    {
        CheckGuardBlocksOrFail();

        if(buffer_size_ > 32*default_size_)
        {
            _aligned_free(real_data_ptr_);
            InitBuffer(default_size_);
        }
        else
        {
            start_data_ptr_ = 0;
            end_data_ptr_   = 0;
        }   
    }

    //  Проверка перезаписи границ буфера, не вызывает GuardBlocksError()
    bool CheckGuardBlocks() const
    {
        if(real_data_ptr_ == nullptr)
            return true;

        auto CheckGuardData = [&](const unsigned char* ptr, size_t length) -> bool
        {
            size_t size = length/4;
            const uint32_t* uint_ptr = reinterpret_cast<const uint32_t*>(ptr);
            bool OK = true;
            for(size_t i = 0; i < size; ++i, ++uint_ptr)
                OK &= *uint_ptr == 0x3AEBA100;

            return OK;
        };

        return CheckGuardData(real_data_ptr_, guard_block_size_) && CheckGuardData(real_data_ptr_+buffer_size_*sizeof(T)+guard_block_size_, guard_block_size_);
    }

    //  Проверка перезаписи границ буфера, в случае ошибки вызывает GuardBlocksError()
    bool CheckGuardBlocksOrFail() const
    {
        if(!CheckGuardBlocks())
        {
            GuardBlocksError();
            return false;
        }
        return true;
    }

    //  Возвращает объем данных доступных для чтения (в единициах T)
    size_t AvailRead() const
    {
        return end_data_ptr_ - start_data_ptr_;
    }

    size_t AvailReadBytes() const
    {
        return AvailRead()*sizeof(T);
    }

    //  Возвращает объем зарезервированной для записи области (в единициах T)
    size_t AvailWrite() const
    {
        return buffer_size_-end_data_ptr_;
    }

    //  Подгатавливает буфер к записи item_count элементов и возвращает указатель на область памяти для записи
    T* PrepareForWrite(int item_count)
    {
        if(item_count < 0)
        {
            SizeError();
#ifdef _DEBUG
            return nullptr;
#endif // _DEBUG
        }

        size_t AvailWr = buffer_size_ - end_data_ptr_;
        if((int)AvailWr < item_count)
        {
            if(AvailRead()+item_count <= start_data_ptr_)
            {
                //  Копируем блок доступный для чтения в начало
                CheckGuardBlocksOrFail();
                auto Avail = AvailRead();
                if(Avail)
                    memcpy(data_, &data_[start_data_ptr_], Avail*sizeof(T));

                end_data_ptr_   -= start_data_ptr_;
                start_data_ptr_ = 0;
                assert(end_data_ptr_ >= start_data_ptr_);
            }
            else
            {
                CheckGuardBlocksOrFail();

                //  Перераспределение памяти
                size_t size_scale_factor = AvailRead() + item_count + buffer_size_ > 1*1024*1024 ? 1 : 2;
                auto NewItemCount = (AvailRead() + item_count + buffer_size_)*size_scale_factor > default_size_ ? (AvailRead() + item_count + buffer_size_)*size_scale_factor : default_size_;

                unsigned char* NewRealPtr = (unsigned char*)_aligned_malloc(NewItemCount*sizeof(T)+guard_block_size_*2, 32);    //  2 guard-блока

                if(NewRealPtr == NULL)
                    AllocError(NewItemCount*sizeof(T)+guard_block_size_*2);

                WriteGuardData(NewRealPtr, guard_block_size_);
                WriteGuardData(NewRealPtr+NewItemCount*sizeof(T)+guard_block_size_, guard_block_size_);

                auto Avail = AvailRead();
                if(Avail)
                    memcpy(NewRealPtr+guard_block_size_, &data_[start_data_ptr_], Avail*sizeof(T));

                _aligned_free(real_data_ptr_);

                real_data_ptr_  = NewRealPtr;
                data_           = (T*)&real_data_ptr_[guard_block_size_];
                start_data_ptr_ = 0;
                end_data_ptr_   = Avail;
                real_data_ptr_  = NewRealPtr;
                buffer_size_    = NewItemCount;
            }
        }
        return &data_[end_data_ptr_];
    }

    T* PrepareForWrite(size_t item_count)
    {
        return PrepareForWrite((int)item_count);
    }

    //  Сообщает классу о том, что в выделенный буфер было записано item_count элементов
    void WriteComplete(int item_count)
    {
        if(item_count < 0)
        {
            SizeError();
            return;
        }

        end_data_ptr_ += item_count;
        if(end_data_ptr_ > buffer_size_)
            BufferOverflowError();
    }

    void WriteComplete(size_t item_count)
    {
        WriteComplete((int)item_count);
    }

    //  Копирует в буфер item_count элементов из массива data
    void WriteData(const T* data, int item_count)
    {
        if(item_count < 0)
        {
            SizeError();
            return;
        }

        if(PrepareForWrite(item_count))
        {
            memcpy(&data_[end_data_ptr_], data, item_count*sizeof(T));
            WriteComplete(item_count);
        }
    }
    void WriteData(const T* data, size_t item_count)
    {
        WriteData(data, (int)item_count);
    }

    template<class Functor>
    void WriteData(size_t item_count, Functor func)
    {
        if(func( PrepareForWrite(item_count) ) == true)
            WriteComplete(item_count);
    }

    //  Побыстрому удаляет из буфера первые item_count элементов, сложность O(1)
    //  если удалены все данные, то вызывается Clear()
    void RemoveFirst(int item_count)
    {
        if(item_count < 0)
        {
            SizeError();
            return;
        }

        if(start_data_ptr_+item_count > end_data_ptr_)
        {
            BufferUnderflowError();
            return;
        }

        start_data_ptr_ += item_count;
        if(IsEmpty())
            Clear();
    }
    void RemoveFirst(size_t item_count)
    {
        RemoveFirst((int)item_count);
    }

    //  Побыстрому удаляет из буфера последние item_count элементов, сложность O(1)
    //  если удалены все данные, то вызывается Clear()
    void RemoveLast(int item_count)
    {
        if(item_count < 0)
        {
            SizeError();
            return;
        }

        if(AvailRead() < static_cast<size_t>(item_count))
            BufferUnderflowError();

        end_data_ptr_ -= item_count;

        if(IsEmpty())
            Clear();
    }
    void RemoveLast(size_t item_count)
    {
        RemoveLast((int)item_count);
    }

    //  Возвращает указатель на данные для чтения. Объем данных можно узнать вызвав AvailRead()
    operator T*()
    {
        return &data_[start_data_ptr_];
    }

    //  Возвращает константный указатель на данные для чтения. Объем данных можно узнать вызвав AvailRead()
    operator const T*() const
    {
        return &data_[start_data_ptr_];
    }

    //  Возвращает указатель на данные для чтения. Объем данных можно узнать вызвав AvailRead()
    T* data()
    {
        return &data_[start_data_ptr_];
    }

    //  Возвращает константный указатель на данные для чтения. Объем данных можно узнать вызвав AvailRead()
    T* const_data() const
    {
        return &data_[start_data_ptr_];
    }

    //  Копирует все данные из буфера src
    void CopyFrom(const Storage& src)
    {
        WriteData(src.const_data(), src.AvailRead());
    }

    //  Возвращает true, если нет данных для чтения
    bool IsEmpty() const { return AvailRead() == 0; }

private:
    //  Это реальный указатель на выделенную память.
    //  В начале и в конце находятся защитные слова
    size_t          start_data_ptr_;
    T*              data_;
    size_t          guard_block_size_;
    unsigned char*  real_data_ptr_;
    size_t          buffer_size_;
    size_t          end_data_ptr_;
    size_t          default_size_;  //  размер буфера по-умолчанию

    //  Инициализация буфера размера size
    void InitBuffer(size_t size)
    {
        buffer_size_ = size;
        real_data_ptr_ = (unsigned char*)_aligned_malloc(buffer_size_*sizeof(T)+guard_block_size_*2, 32);
        
        if(real_data_ptr_ == NULL)
            AllocError(buffer_size_*sizeof(T)+guard_block_size_*2);

        data_ = (T*)&real_data_ptr_[guard_block_size_];

        WriteGuardData(real_data_ptr_, guard_block_size_);
        WriteGuardData(real_data_ptr_+buffer_size_*sizeof(T)+guard_block_size_, guard_block_size_);

        start_data_ptr_ = 0;
        end_data_ptr_   = 0;
    }

    void WriteGuardData(unsigned char* ptr, size_t length)
    {
        size_t size = length/4;
        uint32_t* uint_ptr = reinterpret_cast<uint32_t*>(ptr);
        for(size_t i = 0; i < size; ++i, ++uint_ptr)
            *uint_ptr = 0x3AEBA100;
    }
};

#pragma warning (default: 4702)

typedef Storage<unsigned char> StdStorage;

#endif
