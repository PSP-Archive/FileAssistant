/***********************************************************************************

  Module :	CExecutableFileHandler.h

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

#ifndef CEXECUTABLEFILEHANDLER_H_
#define CEXECUTABLEFILEHANDLER_H_

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
enum ePBPSection
{
	PS_PARAM_SFO,
	PS_ICON0_PNG,
	PS_ICON1_PMF,
	PS_UNKNOWN_PNG,
	PS_PIC1_PNG,
	PS_SND0_AT3,
	PS_UNKNOWN_PSP,
	PS_UNKNOWN_PSAR,

	MAX_PBP_SECTIONS
};

struct PBP_HEADER
{
	u32		signature;
	int		version;
	int		offset[ MAX_PBP_SECTIONS ];
};

//**********************************************************************************
//   Constants
//**********************************************************************************

//**********************************************************************************
//**********************************************************************************
//
//   PRX execution class
//
//**********************************************************************************
//**********************************************************************************
class CPRXFileHandler
{
	public:

		static bool							Execute( const CString & file );
		static const sFileExtensionInfo &	Information( const CString & file );
};

//**********************************************************************************
//**********************************************************************************
//
//   BIN execution class
//
//**********************************************************************************
//**********************************************************************************
class CBINFileHandler
{
	public:

		static bool							Execute( const CString & file );
		static const sFileExtensionInfo &	Information( const CString & file );
};

//**********************************************************************************
//**********************************************************************************
//
//   ELF execution class
//
//**********************************************************************************
//**********************************************************************************
class CELFFileHandler
{
	public:

		static bool							Execute( const CString & file );
		static const sFileExtensionInfo &	Information( const CString & file );
};


//**********************************************************************************
//**********************************************************************************
//
//   PBP execution class
//
//**********************************************************************************
//**********************************************************************************
class CPBPFileHandler
{
	public:

		static bool							Execute( const CString & file );
		static const sFileExtensionInfo &	Information( const CString & file );

		static void							BackgroundCallback( const CString & file );

	protected:

		static bool							CheckArchive( const CString & file );
		static bool							UnpackAndRun( const CString & file );

		static bool							WriteIcon0( const CString & file, const CString & out_file );
		static bool							WriteIcon1( const CString & file, const CString & out_file );
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CEXECUTABLEFILEHANDLER_H_ */
