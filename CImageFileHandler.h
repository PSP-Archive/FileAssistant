/***********************************************************************************

  Module :	CImageFileHandler.h

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

#ifndef CIMAGEFILEHANDLER_H_
#define CIMAGEFILEHANDLER_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
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
class CImageFileHandler
{
	public:

		static bool							Execute( const CString & file );
		static const sFileExtensionInfo &	Information( const CString & file );

	private:

		static void							Render();
		static void							CalculateSize();

	private:

		static CTexture *					s_pTexture;
		static V2							s_Size;
		static float						s_fScale;
		static V2							s_Offset;
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CIMAGEFILEHANDLER_H_ */
