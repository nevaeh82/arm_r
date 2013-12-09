#ifndef MODULES_CORE_PREPROCESSOR_H_
#define MODULES_CORE_PREPROCESSOR_H_	1

#include <intrin.h>	//  microsoft __debugbreak();

#ifndef DISALLOW_COPY_AND_ASSIGN
#ifdef __INTEL_COMPILER
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&) = delete; \
	template<class T> T& operator=(const TypeName&) = delete;
#else
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&); \
	template<class T> T& operator=(const TypeName&)
#endif
#endif

#ifdef _DEBUG
    #ifndef	_DBG_CONDITION
        #ifdef	ASSERT_ON_ERROR
            #include <assert.h>
        #endif
    #endif
#endif

#ifdef _DEBUG
	#ifndef	_DBG_CONDITION
		#ifdef	ASSERT_ON_ERROR
			#define _DBG_CONDITION(x)				if ( (bool)(x) == false ) {  assert((bool)x); }
		#else
			#define _DBG_CONDITION(x)				if ( (bool)(x) == false ) {  __debugbreak(); }
		#endif
	#endif
#else
	#ifndef	_DBG_CONDITION
	#define _DBG_CONDITION(x)				(x)
	#endif
#endif

#ifndef	_DBG_POINTER
#define _DBG_POINTER(x)						_DBG_CONDITION( (x) != NULL );
#endif

#ifndef	_DBG_ALIGN
#define _DBG_ALIGN(x, n)					_DBG_CONDITION( (((UINT)(x)) & (n)) == 0 );
#endif

#ifndef	_DBG_ALIGN_4
#define _DBG_ALIGN_4(x)						_DBG_CONDITION( (((UINT)(x)) & 3) == 0 );
#endif

#ifndef	_DBG_ALIGN_8
#define _DBG_ALIGN_8(x)						_DBG_CONDITION( (((UINT)(x)) & 7) == 0 );
#endif

#ifndef	_DBG_ALIGN_16
#define _DBG_ALIGN_16(x)					_DBG_CONDITION( (((UINT)(x)) & 15) == 0 );
#endif

#ifndef	_DBG_NON_ZERO
#define _DBG_NON_ZERO(x)					_DBG_CONDITION( (x) != 0 );
#endif

#ifndef	_DBG_HRESULT
#define _DBG_HRESULT(x)						_DBG_CONDITION( (x) == NOERROR );
#endif

#ifndef	_DBG_IPP_STATUS
#define _DBG_IPP_STATUS(x)					_DBG_CONDITION( (x) == ippStsNoErr );
#endif

#ifndef _DBG_RETURN 
#define _DBG_RETURN(condition, errorcode)	{ bool	answer = condition; _DBG_CONDITION( !answer ); if ( answer ) return (errorcode); }
#endif


// This template function declaration is used in defining arraysize.
// Note that the function doesn't need an implementation, as we only
// use its type.
#ifndef	ARRAY_SIZE
	template <typename T, size_t N>
	char (&ArraySizeHelper(T (&array)[N]))[N];

	#define ARRAY_SIZE(array) (sizeof(ArraySizeHelper(array)))
#endif

//  CHECK_MODULE_VERSION ( MODULES_CORE_PREPROCESSOR_H_, 1 )
#define		CHECK_MODULE_VERSION( module_version, required_module_version )		\
	static_assert( module_version >= required_module_version, "Version of the module lower than requered version." );

#endif