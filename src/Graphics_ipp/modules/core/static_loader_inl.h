#ifndef MODULES_CORE_STATIC_LOADER_INL_H
#define MODULES_CORE_STATIC_LOADER_INL_H

#include <stdint.h>
#include <assert.h>
#include <map>
#include <memory>
#include <vector>


template<typename Interface>
class	StaticLoader
{
public:
	typedef		typename	std::vector<uint64_t>							hash_vector;
	typedef		typename	std::shared_ptr<Interface>						ptr_shared;
	typedef		typename	Interface*										ptr_simple;

public:
	template<typename ModuleType>		static			void				registering			( uint64_t class_hash );
										static			hash_vector			get_hash_vector		( );
										static			ptr_shared			get_module			( uint64_t class_hash );
										static			ptr_simple			get_module_ptr		( uint64_t class_hash );


private:
	class	AuxiliaryBase
	{
	public:
		virtual									~AuxiliaryBase	( ) {};
		virtual		std::shared_ptr<Interface>	get				( )	= 0;
		virtual		Interface*					get_ptr			( )	= 0;
	};

	template <class Type>
	class Type_class : public AuxiliaryBase
	{
	public:
		virtual		std::shared_ptr<Interface>	get				( )	{ return	std::shared_ptr<Interface>( new Type() ); };
		virtual		Interface*					get_ptr			( )	{ return	new Type(); };
	};


	typedef		typename	std::map<uint64_t, std::shared_ptr<AuxiliaryBase> >		map_type;


	static	StaticLoader	static_object;
	map_type				interface_;


	inline		StaticLoader	( );
	inline		~StaticLoader	( );
};

template<typename Interface>
StaticLoader<Interface>::StaticLoader()
{
}

template<typename Interface>
StaticLoader<Interface>::~StaticLoader()
{
}

template<typename Interface>
template<typename ModuleType>
void									StaticLoader<Interface>::registering		( uint64_t class_hash )
{
	auto		iter	= static_object.interface_.find( class_hash );


	if ( iter == static_object.interface_.end() )
	{
		static_object.interface_[ class_hash ]					= std::shared_ptr< Type_class<ModuleType> >( new Type_class<ModuleType>( ) );
	}
	else
	{
// 		std::shared_ptr<AuxiliaryBase>	object					= static_object.interface_[ class_hash ];
// 		std::shared_ptr<Interface>		interface_created		= object->get();
// 		std::shared_ptr<Interface>		interface_new			= std::shared_ptr<Interface>( new ModuleType() );
// 
// 
// 		if ( interface_new->get_version() > interface_created->get_version() )
// 		{
			static_object.interface_[ class_hash ]			= std::shared_ptr< Type_class<ModuleType> >( new Type_class<ModuleType>( ) );
//		}
	}


	return	;
}

template<typename Interface>
typename	std::vector<uint64_t>		StaticLoader<Interface>::get_hash_vector	( )
{
	hash_vector		vector;

	for (auto iter = static_object.interface_.cbegin(); iter != static_object.interface_.cend(); iter++)
		vector.push_back( (*iter).first );

	return	vector;
}

template<typename Interface>
typename	std::shared_ptr<Interface>	StaticLoader<Interface>::get_module			( uint64_t class_hash )
{
	auto			iter	= static_object.interface_.find( class_hash );
	ptr_shared		result;

	if ( iter != static_object.interface_.end() )
	{
		std::shared_ptr<AuxiliaryBase>	object		= static_object.interface_[ class_hash ];
										result		= object->get();
	}

	return	result;
}

template<typename Interface>
typename	Interface*					StaticLoader<Interface>::get_module_ptr			( uint64_t class_hash )
{
	auto			iter	= static_object.interface_.find( class_hash );
	ptr_simple		result;

	if ( iter != static_object.interface_.end() )
	{
		std::shared_ptr<AuxiliaryBase>	object		= static_object.interface_[ class_hash ];
										result		= object->get_ptr();
	}

	return	result;
}



template<typename ModuleType>
class	Registrator
{
public:
	explicit	Registrator					( const char* str_objectname );
	explicit	Registrator					( uint64_t hash );

private:
				uint64_t		FNVHash		( const char* str );

private:
	uint64_t	hash_;
};

template<typename ModuleType>		Registrator<ModuleType>::Registrator( const char* str_objectname )
{
	hash_	= FNVHash( str_objectname );

	StaticLoader<ModuleType::BaseClass>::registering<ModuleType>( hash_ );
}

template<typename ModuleType>		Registrator<ModuleType>::Registrator( uint64_t hash )
{
	hash_	= hash;

	StaticLoader<ModuleType::BaseClass>::registering<ModuleType>( hash_ );
}

template<typename ModuleType>		uint64_t Registrator<ModuleType>::FNVHash( const char* str )
{
	const	uint64_t	fnv_prime	= 0x00000100000001b3ui64;
			uint64_t	hash		= 0xcbf29ce484222325ui64;

	for (size_t i = 0; i < strlen(str); i++)
	{
		hash ^= str[i];
		hash *= fnv_prime;
	}

	return hash;
}



/*
#ifdef STATIC_LOADER_INL_H
	#define	RegisterClass_ByObjectName( ClassName, ObjectName )		static	Registrator<ClassName>		object_##ClassName( ObjectName );
	#define	RegisterClass_ByClassName( ClassName )					RegisterClass_ByObjectName( ClassName, #ClassName );
#else
	#define	RegisterClass_ByObjectName( ClassName, ObjectName )
	#define	RegisterClass_ByClassName( ClassName )
#endif
*/


#endif