#ifndef MODULES_CORE_RESULT_CODES_H_
#define MODULES_CORE_RESULT_CODES_H_	1

// I see you ( ಠ_ಠ)

#pragma warning( push )
#pragma warning( disable : 4480 )


enum	Result : int
{
	noerror				= 0,						// no error

	e_pointer			= 1,
	e_invalidarg		= 2,
	e_outofmemory		= 3,
	e_outofrange		= 4,
	e_fail				= 5,
	e_notimplemented	= 6,
	e_notinitialized	= 7,
	e_nointerface		= 8,
	e_pointernotaligned	= 9,
};


#pragma warning( pop ) 


#endif