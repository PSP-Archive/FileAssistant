/***********************************************************************************

  Module :	CInformationDialog.cpp

  Description :

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 2005 - File Assistant++ - Titan Demodivision

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

  Contact information
  71M - 71m@orange.net
  Shazz - shazz@titan-whq.info

***********************************************************************************/

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CInformationDialog.h"
#include "CFileSystem.h"
#include "CHUD.h"
#include "CFileAssistant.h"

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

//**********************************************************************************
//   Static Variables
//**********************************************************************************

//**********************************************************************************
//   Class Definition
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
CInformationDialog::CInformationDialog( const sDirEntry * const p_file_info )
:	CModalMessageBox( "File Information", "" )
{
	CString		text;
	CString		date( "" );

	text.Printf( "FileName: %s\n", p_file_info->m_szFileName.GetPtr() );

	//
	//	Display the size of the file
	//
	s32	size( 0 );

	if ( p_file_info->IsDirectory() == true )
	{
		CFileList	dir_files;
		CString		full_path( CFileAssistant::Get()->GetFocusList()->GetCurrentPath() );

		full_path += p_file_info->m_szFileName;

		if ( CFileSystem::GetDirectoryFiles( full_path, dir_files ) == true )
		{
			for ( CFileList::iterator it = dir_files.begin(); it != dir_files.end(); ++it )
			{
				if ( ( *it ).IsFile() == true )
				{
					CFile * const	p_file( CFileSystem::Open( ( *it ).m_szFileName, "rb" ) );

					if ( p_file != NULL )
					{
						size += p_file->GetLength();

						CFileSystem::Close( p_file );
					}
				}
			}
		}
	}
	else
	{
		if ( p_file_info->m_Stats.st_size > 0 )
		{
			size = p_file_info->m_Stats.st_size;
		}

		date.Printf( "Created: %.2d/%.2d/%.4d", p_file_info->m_Stats.st_ctime.day, p_file_info->m_Stats.st_ctime.month, p_file_info->m_Stats.st_ctime.year );
	}

	if ( size > 0 && size < 1024 )
	{
		text.Printf( "%sSize: %db\n", text.GetPtr(), size );
	}
	else if ( size < 1024 * 1024 )
	{
		text.Printf( "%sSize: %dkb\n", text.GetPtr(), size / 1024 );
	}
	else
	{
		text.Printf( "%sSize: %dmb\n", text.GetPtr(), size / ( 1024 * 1024 ) );
	}

	text += date;

	//
	//	Display the text
	//
	SetText( text );

	AddExitCode( EXIT_CROSS, "Continue" );

	CHUD::SetButtons( "Continue", "", "", "" );
}

//*******************************  END OF FILE  ************************************
