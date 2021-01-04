/***********************************************************************************

  Module :	CExecutableFileHandler.cpp

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
#include "CExecutableFileHandler.h"
#include "CFrameWork.h"
#include "CFileHandler.h"
#include "CFileAssistant.h"
#include "CTextureManager.h"
#include "CBackground.h"

#include <pspsdk.h>
#include <pspmoduleinfo.h>
#include <psploadcore.h>

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
static sFileExtensionInfo	s_FileExtensionInfo =
{
	"Run",
	CSkinManager::SC_ICON_EXECUTABLE
};

//**********************************************************************************
//**********************************************************************************
//
//   ELF execution class
//
//**********************************************************************************
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
bool	CELFFileHandler::Execute( const CString & file )
{
	CFrameWork::Close();

	if ( CFrameWork::RunExecutable( file ) == true )
	{
		return true;
	}

	ASSERT( 0, "Failed to run ELF!" );

	return false;
}

//**********************************************************************************
//
//**********************************************************************************
const sFileExtensionInfo &	CELFFileHandler::Information( const CString & file )
{
	return s_FileExtensionInfo;
}

//**********************************************************************************
//**********************************************************************************
//
//   PRX execution class
//
//**********************************************************************************
//**********************************************************************************

//**********************************************************************************
//	PRX should be executed in context, without freeing any resource
//      Contrary to BIN (which are PRX but...)
//**********************************************************************************
bool	CPRXFileHandler::Execute( const CString & file )
{
	return CFrameWork::LoadAndStartModule( file, CFrameWork::KERNEL_PARTITION );
}

//**********************************************************************************
//
//**********************************************************************************
const sFileExtensionInfo &	CPRXFileHandler::Information( const CString & file )
{
	return s_FileExtensionInfo;
}


//**********************************************************************************
//**********************************************************************************
//
//   BIN execution class
//
//**********************************************************************************
//**********************************************************************************

//**********************************************************************************
//	BIN should be executed in context, after freeing any resource
//      Contrary to PRX
//**********************************************************************************
bool	CBINFileHandler::Execute( const CString & file )
{
	CFrameWork::Close();

	//return CFrameWork::LoadAndStartModule( file, CFrameWork::USER_PARTITION );

	if ( CFrameWork::RunUMD( file ) == true )
	{
		return true;
	}

	ASSERT( 0, "Failed to run UMD!" );

	return false;


}

//**********************************************************************************
//
//**********************************************************************************
const sFileExtensionInfo &	CBINFileHandler::Information( const CString & file )
{
	return s_FileExtensionInfo;
}


//**********************************************************************************
//**********************************************************************************
//
//   PBP execution class
//
//**********************************************************************************
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
bool	CPBPFileHandler::Execute( const CString & file )
{
	CFrameWork::Close();

	if ( CheckArchive( file ) == false )
	{
		if ( UnpackAndRun( file ) == true )
		{
			return true;
		}
	}
	else // 1.5 ELF renamed as EBOOT.PBP
	{
		if ( CFrameWork::RunExecutable( file ) == true )
		{
			return true;
		}
	}

	ASSERT( 0, "Failed to run PBP!" );

	return false;
}

//**********************************************************************************
//
//**********************************************************************************
const sFileExtensionInfo &	CPBPFileHandler::Information( const CString & file )
{
	return s_FileExtensionInfo;
}

//**********************************************************************************
//
//**********************************************************************************
bool	CPBPFileHandler::CheckArchive( const CString & file )
{
	return true;
}

//**********************************************************************************
//
//**********************************************************************************
bool	CPBPFileHandler::UnpackAndRun( const CString & file )
{
	return false;
}

//**********************************************************************************
//
//**********************************************************************************
void	CPBPFileHandler::BackgroundCallback( const CString & file )
{
//	if ( CFrameWork::GetVersion() == CFrameWork::PSP_v1_0 )
	{
		CFileSystem::MakeReadOnly( "ms0:/71M.png" );

		if ( WriteIcon0( file, "ms0:/71M.PNG" ) == true )
		{
			CTexture * const	p_texture( CTextureManager::Create( "ms0:/71M.PNG" ) );

			if ( p_texture != NULL )
			{
				CBackground::SetBackgroundTexture( p_texture );

				CFileSystem::DeleteFile( "ms0:/71M.png" );
			}
		}
	}
}

//**********************************************************************************
//
//**********************************************************************************
bool	CPBPFileHandler::WriteIcon0( const CString & file, const CString & out_file )
{
	bool			success( false );
	CFile * const	p_in_file( CFileSystem::Open( file, "rb" ) );

	if ( p_in_file != NULL )
	{
		CFile * const	p_out_file( CFileSystem::Open( out_file, "wb" ) );

		if ( p_out_file != NULL )
		{
			PBP_HEADER	header;

			if ( p_in_file->Read( &header, sizeof( header ) ) == true )
			{
				if ( header.signature == 0x50425000 )
				{
					const u32	file_offset( header.offset[ PS_ICON0_PNG ] );

					if ( p_in_file->Seek( file_offset, SEEK_SET ) == true )
					{
						const u32	section_size( header.offset[ PS_ICON0_PNG + 1 ] - header.offset[ PS_ICON0_PNG ] );
						u8 *		p_buffer( new u8[ section_size ] );

//						BREAK_POINT( "%d", section_size );

						if ( p_in_file->Read( p_buffer, section_size ) == true )
						{
							if ( p_out_file->Write( p_buffer, section_size ) == true )
							{
								success = true;
							}
//							else
//							{
//								ASSERT( 0, "Out write FAILED!" );
//							}
						}
//						else
//						{
//							ASSERT( 0, "In read FAILED!" );
//						}

						SAFE_DELETE( p_buffer );
					}
//					else
//					{
//						ASSERT( 0, "Seek FAILED!" );
//					}
				}
//				else
//				{
//					ASSERT( 0, "Not a real PBP file!" );
//				}
			}
//			else
//			{
//				ASSERT( 0, "In read FAILED!" );
//			}

			CFileSystem::Close( p_out_file );
		}
//		else
//		{
//			ASSERT( 0, "Out file FAILED!" );
//		}

		CFileSystem::Close( p_in_file );
	}
//	else
//	{
//		ASSERT( 0, "In file FAILED!" );
//	}

	return success;
}

//**********************************************************************************
//
//**********************************************************************************
bool	CPBPFileHandler::WriteIcon1( const CString & file, const CString & out_file )
{
	return false;
}

//*******************************  END OF FILE  ************************************
