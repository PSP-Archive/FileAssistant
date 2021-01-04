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

  Module :	CDebugDraw.h

  Description :

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 08 September 2005 71M

***********************************************************************************/

#ifndef CDEBUGDRAW_H_
#define CDEBUGDRAW_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CTypes.h"
#include "CSphere.h"

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
class CDebugDraw
{
	public:

		static void			Open();
		static void			Close();

		static void			Render();

		static void			DrawLine( const V3 & p0, const V3 & p1, ARGB col0, ARGB col1 );
		static void			DrawSphere( const V3 & centre, float radius, ARGB color );

	private:

		static const u32	SPHERE_VERTICAL_SLICES = 5;
		static const u32	SPHERE_HORIZONTAL_SLICES = 7;
		static const u32	SPHERE_FACETS = SPHERE_HORIZONTAL_SLICES * SPHERE_VERTICAL_SLICES;

		static V3			s_SphereCoords[ SPHERE_FACETS ][ 3 ];
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CDEBUGDRAW_H_ */
