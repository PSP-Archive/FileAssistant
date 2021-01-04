/***********************************************************************************

  Module :	Main.cpp

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
#include "CFrameWork.h"
#include "CFileAssistant.h"
#include "CFileSystem.h"
#include "ftpclient.h"
#include "CConfigFile.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************
static CVarString	CVAR_SERVER_IP( "server_ip", "192.168.1.200" );
static CVarString	CVAR_SERVER_PORT( "server_port", "21" );
static CVarString	CVAR_SERVER_USER( "server_user", "psp" );
static CVarString	CVAR_SERVER_PASSWORD( "server_password", "psp" );

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

//*************************************************************************************
//
//*************************************************************************************
int	main( int argc, char * argv[] )
{
	//
	//	Set the working folder
	//
	if ( argc > 0 )
	{
		CString	drive, dir;

		CFileSystem::SplitPath( CString( argv[ 1 ] ), &drive, &dir, NULL, NULL );

		drive += dir;

		CFileSystem::SetRoot( drive );
	}

	//
	//	Open the framework
	//
	if ( CFrameWork::Open() == true )
	{
		//
		//	Create the application
		//
		CFileAssistant::Create();

		//
		//	The main loop
		//
		while ( CFrameWork::Process() == true )
		{
		}

		//
		//	Close the application
		//
		CFileAssistant::Destroy();

		//
		//	Close down the framework
		//
		CFrameWork::Close();
	}
	else
	{
		ASSERT( 0, "CFrameWork::Open()" );
	}

	return 0;
}

//*******************************  END OF FILE  ************************************

