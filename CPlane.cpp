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

  Module :	CPlane.cpp

  Description :

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 08 September 2005 71M

***********************************************************************************/

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CPlane.h"

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
CPlane::CPlane( const CPlane & plane )
:	m_Normal( plane.m_Normal )
,	m_D( plane.m_D )
{
}

//**********************************************************************************
//
//**********************************************************************************
CPlane::CPlane( const V3 & norm, const float d )
:	m_Normal( norm.Normal() )
,	m_D( d )
{
}

//**********************************************************************************
//
//**********************************************************************************
CPlane::CPlane( const V3 & norm, const V3 & pos )
:	m_Normal( norm.Normal() )
,	m_D( m_Normal.Dot( pos ) )
{
}

//**********************************************************************************
//
//**********************************************************************************
float	CPlane::DistanceToPoint( const V3 & p ) const
{
	return m_Normal.Dot( p ) - m_D;
}

//*******************************  END OF FILE  ************************************
