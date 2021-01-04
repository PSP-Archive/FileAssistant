/***********************************************************************************

  Module :	CMusicFileHandler.h

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

#ifndef CMUSICFILEHANDLER_H_
#define CMUSICFILEHANDLER_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CTypes.h"
#include "CFileHandler.h"

//**********************************************************************************
//   Macros
//**********************************************************************************

//**********************************************************************************
//   Types
//**********************************************************************************

//**********************************************************************************
//   Constants
//**********************************************************************************

//**********************************************************************************
//   Class definitions
//**********************************************************************************
class CMusicFileHandler
{
	public:

		enum eError
		{
			SUCCESS,
			NO_CODEC,
			REPLAY_ERROR,
		};

	public:

		static void							Open();
		static void							Close();

		static void							ProcessInput();

		static bool							Execute( const CString & file );
		static const sFileExtensionInfo &	Information( const CString & file );

		static eError						Play( const CString & file );
		static void							Stop();
		static void							Pause();

		static bool							IsPlaying();

		static void							ShowVisualiser( bool show );
		static bool							VisualiserVisible();

		static CString						GetInfo();
		static const CString &				GetFilePlaying();
		static CString						GetPlayingTime();

		static void							ToggleEcho();
		static void							ToggleSpatialSound();

	private:

		static void							Render();
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CMUSICFILEHANDLER_H_ */
