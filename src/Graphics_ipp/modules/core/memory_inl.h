#ifndef MODULES_CORE_MEMORY_INL_H_
#define MODULES_CORE_MEMORY_INL_H_		1


#include <stdint.h>
#include <malloc.h>

#include "modules/core/preprocessor.h"
#include "modules/core/result_codes.h"


#pragma region	New style
template <typename Type>
Type*		_safe_malloc( const size_t itemscount )
{
	Type*	ptr	= (Type*) _aligned_malloc( itemscount * sizeof(Type), 32 );

	_DBG_CONDITION( ptr != nullptr );	

	return ptr; 
}

template <typename Type>
Type*		_safe_malloc_zero( const size_t itemscount)
{ 
	Type*	ptr	= (Type*) _aligned_malloc( itemscount * sizeof(Type), 32 ) ;

	_DBG_CONDITION( ptr != nullptr );

	memset( ptr, 0, itemscount * sizeof(Type));

	return ptr;
}

template <typename Type>
void		_safe_free( Type* &ptr )				
{
	_aligned_free(ptr);

	ptr = nullptr;
}

template <typename Type>
Type*		_safe_new( )
{ 
	Type*	ptr	= new Type();

	_DBG_CONDITION( ptr != nullptr );

	return ptr;
}

template <typename Type>
Result		_safe_copy( const Type* src, Type* dst, const size_t itemscount )
{
	_DBG_CONDITION( src != nullptr );
	_DBG_CONDITION( dst != nullptr );

	if ( src == nullptr )		return e_pointer;
	if ( dst == nullptr )		return e_pointer;

	memcpy( dst, src, itemscount * sizeof(Type) );

	return noerror;
}

template <typename Type>
Result		_safe_delete( Type* &ptr )
{
	if ( ptr != nullptr )
	{
		delete	ptr;
		ptr = nullptr;
	}

	return noerror;
}

template <typename Type>
Result		_safe_delete_array( Type* &ptr )
{
	if ( ptr != nullptr )
	{
		delete[]	ptr;
		ptr = nullptr;
	}

	return noerror;
}

template <typename Type>
Result		_safe_fillmass( Type* src, const size_t itemscount, const Type word )
{
	_DBG_CONDITION( src != nullptr );
	_DBG_CONDITION( itemscount != 0 );

	if ( src == nullptr )		return e_pointer;
	if ( itemscount == 0 )		return e_invalidarg;

	for (size_t i = 0; i < itemscount; i++)
		src[i] = word;

	return	noerror;
}

class	MemmoryAlloc
{
public:
	inline	 MemmoryAlloc( size_t items_qnt ) :	items_qnt_( items_qnt )			{}
	inline	~MemmoryAlloc( )													{}

	template<typename Type>
	operator Type*	( )
	{
		return	_safe_malloc<Type>( items_qnt_ );
	}

private:
	size_t	items_qnt_;
};


class	MemmoryAllocZero
{
public:
	inline	 MemmoryAllocZero( size_t items_qnt ) :	items_qnt_( items_qnt )		{}
	inline	~MemmoryAllocZero( )												{}

	template<typename Type>
	operator Type*	( )
	{
		return	_safe_malloc_zero<Type>( items_qnt_ );
	}

private:
	size_t	items_qnt_;
};


#define		_safe_alloc( items_qnt )			MemmoryAlloc( items_qnt )
#define		_safe_alloc_zero( items_qnt )		MemmoryAllocZero( items_qnt )
#pragma endregion



#pragma region	Old style
// template <typename Type>
// Type*		_SAFE_MALLOC( size_t itemscount )
// {
// 	Type*	ptr	= (Type*) _aligned_malloc( itemscount * sizeof(Type), 32 );
// 
// 	_DBG_CONDITION( ptr != nullptr );	
// 
// 	return ptr; 
// }
// 
// template <typename Type>
// Type*		_SAFE_MALLOC_ZERO( size_t itemscount)
// { 
// 	Type*	ptr	= (Type*) _aligned_malloc( itemscount * sizeof(Type), 32 ) ;
// 
// 	_DBG_CONDITION( ptr != nullptr );
// 
// 	memset( ptr, 0, itemscount * sizeof(Type));
// 
// 	return ptr;
// }
// 
// template <typename Type>
// void		_SAFE_FREE( Type* &ptr )				
// {
// 	_aligned_free(ptr);
// 
// 	ptr = nullptr;
// }
// 
// template <typename Type>
// Type*		_SAFE_NEW( )
// { 
// 	Type*	ptr	= new Type();
// 
// 	_DBG_CONDITION( ptr != nullptr );
// 
// 	return ptr;
// }
// 
// template <typename Type>
// Result		_SAFE_COPY( const Type* src, Type* dst, size_t itemscount )
// {
// 	_DBG_CONDITION( src != nullptr );
// 	_DBG_CONDITION( dst != nullptr );
// 
// 	if ( src == nullptr )		return e_pointer;
// 	if ( dst == nullptr )		return e_pointer;
// 
// 	memcpy( dst, src, itemscount * sizeof(Type) );
// 
// 	return noerror;
// }
// 
// template <typename Type>
// Result		_SAFE_DELETE( Type* &ptr )
// {
// 	if ( ptr != nullptr )
// 	{
// 		delete	ptr;
// 		ptr = nullptr;
// 	}
// 
// 	return noerror;
// }
// 
// template <typename Type>
// Result		_SAFE_DELETE_ARRAY( Type* &ptr )
// {
// 	if ( ptr != nullptr )
// 	{
// 		delete[]	ptr;
// 		ptr = nullptr;
// 	}
// 
// 	return noerror;
// }
// 
// template <typename Type>
// Result		_SAFE_FILLMASS( Type* src, size_t itemscount, const Type word )
// {
// 	_DBG_CONDITION( src != nullptr );
// 	_DBG_CONDITION( itemscount != 0 );
// 
// 	if ( src == nullptr )		return e_pointer;
// 	if ( itemscount == 0 )		return e_invalidarg;
// 
// 	for (size_t i = 0; i < itemscount; i++)
// 		src[i] = word;
// 
// 	return	noerror;
// }
#pragma endregion


#endif
