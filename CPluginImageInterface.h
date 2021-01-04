/***********************************************************************************

  Module :	CPluginImageInterface.h

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

#ifndef CPLUGINIMAGEINTERFACE_H_
#define CPLUGINIMAGEINTERFACE_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************

//**********************************************************************************
//   Macros
//**********************************************************************************

//**********************************************************************************
//   Types
//**********************************************************************************
typedef struct
{
	u32		m_nWidth;
	u32		m_nHeight;
	u32		m_nCanvasWidth;
	u32		m_nCanvasHeight;
	u8 *	m_pBuffer;
	u8 *	m_pOriginalAddress;
} sTextureInfo;

typedef	const char *		( * GetPluginNameFunc )();
typedef	sTextureInfo *		( * LoadTextureFunc )( const char * p_filename );

typedef struct
{
	GetPluginNameFunc	name_func;
	LoadTextureFunc		load_func;
} sImagePlugInterface;

typedef	sImagePlugInterface *	( * GetInterfaceFunc )();

//**********************************************************************************
//   Constants
//**********************************************************************************

//**********************************************************************************
//   Class definitions
//**********************************************************************************

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CPLUGINIMAGEINTERFACE_H_ */
