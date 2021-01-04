/*
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
*/
/***********************************************************************************

  Module :	CCamera.h

  Description :

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 08 September 2005 71M

***********************************************************************************/

#ifndef CCAMERA_H_
#define CCAMERA_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CTypes.h"

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
class CCamera
{
	public:

		CCamera();

		void			SetView();

		const V3 &		GetPos() const;
		void			SetPos( const V3 & pos );

		const V3 &		GetFocus() const;
		void			SetFocus( const V3 & focus );

	private:

		V3				m_Pos;
		V3				m_Focus;
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CCAMERA_H_ */
