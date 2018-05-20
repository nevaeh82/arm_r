#ifndef MODULES_CORE_FILE_INL_H
#define MODULES_CORE_FILE_INL_H

#include "modules\core\preprocessor.h"
#include "modules\core\result_codes.h"

#include <Windows.h>

#include <assert.h>
#include <tchar.h>
#include <stdint.h>


#pragma warning( disable : 869 )


//************************************
// Class:		FileMapper
// Description:	Class for reading files. Uses file mapping in memory.
//************************************
class FileMapper
{
private:
	uint8_t*	baseaddress_;
	uint64_t	size_;
	TCHAR*		filename_;

	HANDLE		filehandle_;
	HANDLE		filemapping_;
public:

	inline	FileMapper			( TCHAR* filename );
	inline	FileMapper			( const TCHAR* filename );
	inline	~FileMapper			( );

	inline	void		Close	( );
	inline	uint64_t	Size	( );

	inline	operator bool		( );

	template<typename Type>
	inline	operator Type*		( )	const;
};


//************************************
// Function:	SaveBufferToFile
//				AppendBufferToFile
// Description:	Some function to simple work with file.
//************************************
template<typename Type>		inline	Result		SaveBufferToFile		( const char*	filename, const Type* buffer, size_t itemscount );
template<typename Type>		inline	Result		SaveBufferToFile		( const TCHAR*	filename, const Type* buffer, size_t itemscount );
template<typename Type>		inline	Result		SaveBufferToFile		( const TCHAR*	filename, const TCHAR* fileextension, int file_id, const Type* buffer, size_t itemscount );

template<typename Type>		inline	Result		AppendBufferToFile		( const char*	filename, const Type* buffer, size_t itemscount );
template<typename Type>		inline	Result		AppendBufferToFile		( const TCHAR*	filename, const Type* buffer, size_t itemscount );
template<typename Type>		inline	Result		AppendBufferToFile		( const TCHAR*	filename, const TCHAR* fileextension, int file_id, const Type* buffer, size_t itemscount );


#pragma region FileMapper implementation
FileMapper::FileMapper( TCHAR* filename ):
	baseaddress_	( nullptr ), 
	size_			( 0 ), 
	filename_		( nullptr ), 
	filehandle_		( nullptr ), 
	filemapping_	( nullptr )
{
	if( filename )
	{
		size_t	len			= _tcslen( filename );
				filename_	= new TCHAR[len+1];

		_tcscpy_s( filename_, len+1, filename );
	}
}

FileMapper::FileMapper( const TCHAR* filename ):
	baseaddress_	( nullptr ), 
	size_			( 0 ), 
	filename_		( nullptr ), 
	filehandle_		( nullptr ), 
	filemapping_	( nullptr )
{
	if( filename )
	{
		size_t	len = _tcslen(filename);
		filename_	= new TCHAR[len+1];

		_tcscpy_s(filename_, len+1, filename);
	}
}

FileMapper::~FileMapper()
{
	if ( baseaddress_ )
		Close();
	else
	{
		delete[] filename_;
		filename_		= nullptr;
	}

	assert( size_			== 0 );

	assert( filehandle_		== nullptr );
	assert( filemapping_	== nullptr );
	assert( baseaddress_	== nullptr );
	assert( filename_		== nullptr );
}

FileMapper::operator bool( )
{
	assert( filehandle_ == nullptr );

	if( (filehandle_ = CreateFile(filename_, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr)) == INVALID_HANDLE_VALUE )
	{
		_tprintf_s(_T("Can't open file %s for reading...\n"), filename_);
		filehandle_		= nullptr;

		return false;
	}

	LARGE_INTEGER	hFileInSize;

	if ( GetFileSizeEx( filehandle_, &hFileInSize ) == FALSE )
	{
		_tprintf_s(_T("File size %s is zero...\n"), filename_);

		CloseHandle( filehandle_ );				
		filehandle_		= nullptr;

		return	false;
	}

	DWORD	highsizepart	= hFileInSize.HighPart;
	DWORD	lowsizepart		= hFileInSize.LowPart;
			size_			= hFileInSize.QuadPart;

	if ( size_ == (uint64_t)0 )			
	{
		_tprintf_s(_T("File size %s is zero...\n"), filename_);

		CloseHandle( filehandle_ );		
		filehandle_		= nullptr;

		return	false;
	}


	filemapping_ = CreateFileMapping(filehandle_, nullptr, PAGE_READONLY, highsizepart, lowsizepart, nullptr);

	if (filemapping_ == nullptr)
	{
		_tprintf_s(_T("Can't create file mapping for %s...\n"), filename_);

		CloseHandle( filehandle_ );		
		filehandle_		= nullptr;

		return false;		
	}

	baseaddress_ = (unsigned char*)MapViewOfFile(filemapping_, FILE_MAP_READ, 0, 0, lowsizepart);

	if (baseaddress_ == nullptr)
	{
		_tprintf_s(_T("Can't map view of file for %s...\n"), filename_);
		
		CloseHandle( filemapping_ );
		filemapping_	= nullptr;

		CloseHandle( filehandle_ );
		filehandle_		= nullptr;

		return false;
	}

	return true;
}

void FileMapper::Close()
{
	UnmapViewOfFile( baseaddress_ );

	assert( filemapping_ );
	CloseHandle( filemapping_ );
	filemapping_	= nullptr;

	assert( filehandle_ );
	CloseHandle( filehandle_ );
	filehandle_		= nullptr;

	assert( filename_ );
	delete[] filename_;
	filename_		= nullptr;

	baseaddress_	= nullptr;
	size_			= 0;
}

UINT64 FileMapper::Size()
{
	return	size_;
}

template<typename Type>
FileMapper::operator Type*() const
{
	return	(Type*) baseaddress_;
}
#pragma endregion


#pragma region Write/Append BufferToFile implementation
template<typename Type>
inline	Result		SaveBufferToFile	( const char* filename, const Type* buffer, size_t itemscount )
{
	_DBG_RETURN( filename == nullptr,	e_pointer );
	_DBG_RETURN( buffer == nullptr,		e_pointer );
	_DBG_RETURN( itemscount == 0,		e_invalidarg );

	HANDLE	hFile = CreateFileA( filename, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr );

	_DBG_RETURN( (hFile == INVALID_HANDLE_VALUE) || (hFile == 0),	e_invalidarg );

	DWORD	io	= 0;

	BOOL	resultwrite	= WriteFile( hFile, buffer, itemscount * sizeof(Type), &io, nullptr );
	BOOL	resultclose	= CloseHandle( hFile );

	return	((resultwrite && resultclose) == TRUE) ? noerror : e_invalidarg;
}

template<typename Type>
inline	Result		SaveBufferToFile	( const TCHAR* filename, const Type* buffer, size_t itemscount )
{
	_DBG_RETURN( filename == nullptr,	e_pointer );
	_DBG_RETURN( buffer == nullptr,		e_pointer );
	_DBG_RETURN( itemscount == 0,		e_invalidarg );

	HANDLE	hFile = CreateFileW( filename, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	_DBG_RETURN( (hFile == INVALID_HANDLE_VALUE) || (hFile == 0),	e_invalidarg );

	DWORD	io	= 0;

	BOOL	resultwrite	= WriteFile(hFile, buffer, (DWORD)(itemscount * sizeof(Type)), &io, nullptr);	
	BOOL	resultclose	= CloseHandle(hFile);

	return	((resultwrite && resultclose) == TRUE) ? noerror : e_invalidarg;
}

template<typename Type>
inline	Result		SaveBufferToFile	( const TCHAR*	filename, const TCHAR* fileextension, int file_id, const Type* buffer, size_t itemscount )
{
	_DBG_RETURN( filename == nullptr,		e_pointer );
	_DBG_RETURN( fileextension == nullptr,	e_pointer );
	_DBG_RETURN( buffer == nullptr,			e_pointer );
	_DBG_RETURN( itemscount == 0,			e_invalidarg );

	TCHAR	fullfilename[240];

	_stprintf_s( fullfilename, _T("%s_%u.%s"), filename, file_id, fileextension );

	HANDLE	hFile = CreateFileW( fullfilename, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	_DBG_RETURN( (hFile == INVALID_HANDLE_VALUE) || (hFile == 0),	e_invalidarg );

	DWORD	io	= 0;

	BOOL	resultwrite	= WriteFile(hFile, buffer, itemscount * sizeof(Type), &io, nullptr);	
	BOOL	resultclose	= CloseHandle(hFile);

	return	((resultwrite && resultclose) == TRUE) ? noerror : e_invalidarg;
}

template<typename Type>
inline	Result		AppendBufferToFile	( const char* filename, const Type* buffer, size_t itemscount )
{
	_DBG_RETURN( filename == nullptr,	e_pointer );
	_DBG_RETURN( buffer == nullptr,		e_pointer );
	_DBG_RETURN( itemscount == 0,		e_invalidarg );

	HANDLE	hFile = CreateFileA(filename, GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	_DBG_RETURN( (hFile == INVALID_HANDLE_VALUE) || (hFile == 0),	e_invalidarg );

	SetFilePointer( hFile, 0, nullptr, FILE_END );

	DWORD	io	= 0;

	BOOL	resultwrite	= WriteFile(hFile, buffer, itemscount * sizeof(Type), &io, nullptr);	
	BOOL	resultclose	= CloseHandle(hFile);

	return	((resultwrite && resultclose) == TRUE) ? noerror : e_invalidarg;
}

template<typename Type>
inline	Result		AppendBufferToFile	( const TCHAR* filename, const Type* buffer, size_t itemscount )
{
	_DBG_RETURN( filename == nullptr,	e_pointer );
	_DBG_RETURN( buffer == nullptr,		e_pointer );
	_DBG_RETURN( itemscount == 0,		e_invalidarg );

	HANDLE	hFile = CreateFileW(filename, GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	_DBG_RETURN( (hFile == INVALID_HANDLE_VALUE) || (hFile == 0),	e_invalidarg );

	SetFilePointer( hFile, 0, nullptr, FILE_END );

	DWORD	io	= 0;

	BOOL	resultwrite	= WriteFile(hFile, buffer, itemscount * sizeof(Type), &io, nullptr);	
	BOOL	resultclose	= CloseHandle(hFile);

	return	((resultwrite && resultclose) == TRUE) ? noerror : e_invalidarg;
}
//////////////////////////////////////////////////////////////////////////
template<typename Type>
inline	Result		AppendBufferToFile	( const TCHAR*	filename, const TCHAR* fileextension, int file_id, const Type* buffer, size_t itemscount )
{
	_DBG_RETURN( filename == nullptr,	e_pointer );
	_DBG_RETURN( buffer == nullptr,		e_pointer );
	_DBG_RETURN( itemscount == 0,		e_invalidarg );

	TCHAR	fullfilename[240];

	_stprintf_s( fullfilename, _T("%s_%u.%s"), filename, file_id, fileextension );

	HANDLE	hFile = CreateFileW(fullfilename, GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	_DBG_RETURN( (hFile == INVALID_HANDLE_VALUE) || (hFile == 0),	e_invalidarg );

	SetFilePointer( hFile, 0, nullptr, FILE_END );

	DWORD	io	= 0;

	BOOL	resultwrite	= WriteFile(hFile, buffer, itemscount * sizeof(Type), &io, nullptr);	
	BOOL	resultclose	= CloseHandle(hFile);

	return	((resultwrite && resultclose) == TRUE) ? noerror : e_invalidarg;
}
#pragma endregion


#endif // MODULES_CORE_FILE_INL_H