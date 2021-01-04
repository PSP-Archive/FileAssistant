/***********************************************************************************

  Module :	CFrameWork.h

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

#ifndef CFRAMEWORK_H_
#define CFRAMEWORK_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CTypes.h"
#include "CString.h"

//**********************************************************************************
//   Macros
//**********************************************************************************

//**********************************************************************************
//   Types
//**********************************************************************************
class CFile;

//**********************************************************************************
//   Constants
//**********************************************************************************

//**********************************************************************************
//   Class definitions
//**********************************************************************************
class CFrameWork
{
	public:

		/* If the type is 0, then load the module in the kernel partition, otherwise load it
		in the user partition. */
		enum eModulePartition
		{
			KERNEL_PARTITION	= 0,
			USER_PARTITION		= 1,
		};

		enum eFirmwareVersion
		{
			PSP_v1_0,
			PSP_v1_5,
		};

	public:

		// Initialises the app
		static bool				Open();

		// Shuts down the app
		static void				Close();

		// Update the app
		static bool				Process();

		// Returns the time in milliseconds since the app was started
		static u32				GetTicks();

		// Returns the time in milliseconds of the last frame
		static float			GetElapsedTime();

		// Displays a debug message on the screen - note, this will be cleared by calls to GCfx::ClearScreen
		static void				DBGMessage( const char * fmt, ... );

		// Load and run the specifed executable file
		static SceUID			LoadModule( const CString & module, eModulePartition partition );
		static bool				LoadAndStartModule( const CString & module, eModulePartition partition );

		// Runs an executable (PBP/ELF) file
		static bool				RunExecutable( const CString & executable );

		// Runs an executable (UMD/BIN) file
		static bool				RunUMD( const CString & executable );

		// Returns the firmware version of the PSP
		static eFirmwareVersion	GetVersion();

		// Return a pointer to the module info
		static SceModuleInfo *	GetModuleInfo();

	private:

		static u64				s_Time;
		static u64				s_LastTime;
		static u64				s_TickResolution;
		static CFile *			s_pLogFile;
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CFRAMEWORK_H_ */
