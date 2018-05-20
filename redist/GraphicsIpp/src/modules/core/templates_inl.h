#ifndef MODULES_CORE_TEMPLATES_INL_H_
#define MODULES_CORE_TEMPLATES_INL_H_

#include "../../modules/core/preprocessor.h"

template<int Value>
struct	IntToType
{
	enum	{ Value };
};

template<typename T, class Functor, typename SizeT>
bool ProcessDataByBlocks(T* in_data, const SizeT element_count, const SizeT block_size, Functor t)
{
	SizeT data_remains = element_count;
	SizeT index = 0;
	while(data_remains)
	{
		bool Last = false;
		SizeT size;
		if(data_remains > block_size)
			size = block_size;
		else
		{
			size = data_remains;
			Last = true;
		} 

		bool Continue = t(&in_data[index], size, Last);

		index += size;
		data_remains -= size;
		if(!Continue)
			return false;
	}

    return true;
}

template<typename T, class Functor, typename SizeT>
bool ProcessDataByBlocksWithStep(T* in_data, const SizeT element_count, const SizeT block_size, const SizeT step, Functor t)
{
    SizeT data_remains = element_count;
    SizeT index = 0;
    while(data_remains >= block_size)
    {
        bool    last    = (index + step + block_size) > element_count;
        SizeT   size    = block_size;

        bool Continue = t(&in_data[index], size, last);

        index        += step;
        data_remains -= step;
        if(!Continue)
            return false;
    }

    return true;
}

template<class LockableClass>
class ScopedLock
{
public:
	ScopedLock(LockableClass& m): mutex_(m)	{	mutex_.Lock();		}
	~ScopedLock()							{	mutex_.Unlock();	}
private:
	LockableClass& mutex_;

	DISALLOW_COPY_AND_ASSIGN(ScopedLock);
};

template<class To, class From> To* checked_cast( From* from ) {
    _DBG_CONDITION( dynamic_cast<To*>(from) == static_cast<To*>(from) && "checked_cast failed" );
    return static_cast<To*>( from );
}

#endif
