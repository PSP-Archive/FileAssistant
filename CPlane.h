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

  Module :	CPlane.h

  Description :

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 08 September 2005 71M

***********************************************************************************/

#ifndef CPLANE_H_
#define CPLANE_H_

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
class CPlane
{
	public:

		CPlane()																{}
		CPlane( const CPlane & plane );
		CPlane( const V3 & norm, const float d );
		CPlane( const V3 & norm, const V3 & pos );

		float					DistanceToPoint( const V3 & p ) const;

		const V3 &				GetNormal() const								{ return m_Normal; }
		float					GetScalar() const								{ return m_D; }

	public:

		V3						m_Normal;
		float					m_D;
};


//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CPLANE_H_ */
