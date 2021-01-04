/***********************************************************************************

  Module :	CLUAFileHandler.cpp

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
#include <sys/unistd.h>
#include "CLUAFileHandler.h"
#include "CMessageBox.h"
#include "CFrameWork.h"

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
static const sFileExtensionInfo	s_FileExtensionInfo =
{
	"Run",
	CSkinManager::SC_ICON_LUA
};

//**********************************************************************************
//   Class Definition
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
bool	CLUAFileHandler::Execute( const CString & file )
{
	CFile * const	p_file( CFileSystem::Open( file, "rt" ) );

	if ( p_file != NULL )
	{
		CString					args;
		CString					cwd( CFileSystem::GetCurrentFolder() );
		SceKernelLoadExecParam	execParam;
		const CString			fa_executable( cwd + "/EBOOT.PBP" );
		const CString			lua_executable( cwd + "/Data/Plugins/LUAPlayer.elf" );

		args.Printf( "%s;%s;%s", lua_executable.GetPtr(), file.GetPtr(), fa_executable.GetPtr() );

		CFrameWork::Close();

		execParam.size = sizeof( execParam );
		execParam.argp = ( void * )( args.GetPtr() );
		execParam.args = args.Length() + 1;
		execParam.key = NULL;

		sceKernelLoadExec( lua_executable.GetPtr(), &execParam );

		return true;
	}

	CErrorMessage( "Invalid LUA file" );

	return true;
}

//**********************************************************************************
//
//**********************************************************************************
const sFileExtensionInfo &	CLUAFileHandler::Information( const CString & file )
{
	return s_FileExtensionInfo;
}

//*******************************  END OF FILE  ************************************
