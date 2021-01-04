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

  Module :	CSphere.h

  Description :

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 08 September 2005 71M

***********************************************************************************/

#ifndef CSPHERE_H_
#define CSPHERE_H_

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
class CSphere
{
	public:

		CSphere();
		CSphere( const CSphere & sphere );
		CSphere( const V3 & centre, float radius );

		void		Render( ARGB color ) const;

		const V3 &	GetCentre() const;
		void		SetCentre( const V3 & centre );

		float		GetRadius() const;
		float		GetRadiusSq() const;
		void		SetRadius( float radius );

		CSphere &	operator += ( const V3 & offset );

		bool		Collide( const CSphere & sphere );

	public:

		V3			m_Centre;
		float		m_fRadius;
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CSPHERE_H_ */
