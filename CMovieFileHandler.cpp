/***********************************************************************************

  Module :	CMovieFileHandler.cpp

  Description :	

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 04 November 2005 71M

***********************************************************************************/

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CMovieFileHandler.h"
#include "CMusicFileHandler.h"
#include "CMessageBox.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************

//**********************************************************************************
//   Static Prototypes
//**********************************************************************************

//**********************************************************************************
//   Global Variables
//**********************************************************************************
int PlayMovie( const char * const p_filename );

//**********************************************************************************
//   Static Variables
//**********************************************************************************
static const sFileExtensionInfo	s_FileExtensionInfo =
{
	"Play",
	CSkinManager::SC_ICON_MOVIE
};

//**********************************************************************************
//   Class Definition
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
bool	CMovieFileHandler::Execute( const CString & file )
{
	CFile * const	p_file( CFileSystem::Open( file, "rt" ) );

	if ( p_file != NULL )
	{
		CFileSystem::Close( p_file );

		CMusicFileHandler::Stop();

		if ( PlayMovie( file ) == 0 )
		{
			return true;
		}
	}

	CErrorMessage( "Invalid movie file" );

	return true;
}

//**********************************************************************************
//
//**********************************************************************************
const sFileExtensionInfo &	CMovieFileHandler::Information( const CString & file )
{
	return s_FileExtensionInfo;
}

//*******************************  END OF FILE  ************************************
