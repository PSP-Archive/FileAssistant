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

  Module :	CSphere.cpp

  Description :

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 08 September 2005 71M

***********************************************************************************/

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CSphere.h"
#include "CGfx.h"
//#include "CDebugDraw.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************

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

//**********************************************************************************
//
//**********************************************************************************
CSphere::CSphere()
:	m_Centre( gZeroVector )
,	m_fRadius( 0.f )
{
}

//**********************************************************************************
//
//**********************************************************************************
CSphere::CSphere( const CSphere & sphere )
:	m_Centre( sphere.m_Centre )
,	m_fRadius( sphere.m_fRadius )
{
}

//**********************************************************************************
//
//**********************************************************************************
CSphere::CSphere( const V3 & centre, float radius )
:	m_Centre( centre )
,	m_fRadius( radius )
{
}

//**********************************************************************************
//
//**********************************************************************************
const V3 &	CSphere::GetCentre() const
{
	return m_Centre;
}

//**********************************************************************************
//
//**********************************************************************************
void	CSphere::SetCentre( const V3 & centre )
{
	m_Centre = centre;
}

//**********************************************************************************
//
//**********************************************************************************
float	CSphere::GetRadius() const
{
	return m_fRadius;
}

//**********************************************************************************
//
//**********************************************************************************
float	CSphere::GetRadiusSq() const
{
	return SQUARE( m_fRadius );
}

//**********************************************************************************
//
//**********************************************************************************
void	CSphere::SetRadius( float radius )
{
	m_fRadius = radius;
}

//**********************************************************************************
//
//**********************************************************************************
void	CSphere::Render( ARGB color ) const
{
//	CDebugDraw::DrawSphere( GetCentre(), GetRadius(), color );
}

//**********************************************************************************
//
//**********************************************************************************
CSphere &	CSphere::operator += ( const V3 & offset )
{
	m_Centre += offset;

	return *this;
}

//**********************************************************************************
//
//**********************************************************************************
bool	CSphere::Collide( const CSphere & sphere )
{
	const V3	diff( m_Centre - sphere.m_Centre );
	const float	distance_sq( diff.LengthSq() );
	const float	combined_radius( sphere.m_fRadius + m_fRadius );

	if ( distance_sq <= SQUARE( combined_radius ) )
	{
		m_Centre -= diff.Normal() * ( SQRTF( distance_sq ) - combined_radius );

		return true;
	}

	return false;
}

//*******************************  END OF FILE  ************************************
