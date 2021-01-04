/***********************************************************************************

  Module :	CSizedItem.cpp

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

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CSizedItem.h"

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

//*************************************************************************************
//
//*************************************************************************************
CSizedItem::CSizedItem()
:	m_Pos( 0.f, 0.f )
,	m_Size( 0.f, 0.f )
,	m_fScale( 1.f )
{
}

//*************************************************************************************
//
//*************************************************************************************
CSizedItem::~CSizedItem()
{
}

//*************************************************************************************
//
//*************************************************************************************
const V2 &	CSizedItem::GetPos() const
{
	return m_Pos;
}

//*************************************************************************************
//
//*************************************************************************************
void	CSizedItem::SetPos( const V2 & pos )
{
	m_Pos.x = static_cast< float >( static_cast< s32 >( pos.x ) );
	m_Pos.y = static_cast< float >( static_cast< s32 >( pos.y ) );
}

//*************************************************************************************
//
//*************************************************************************************
V2	CSizedItem::GetSize() const
{
	return ( m_Size * GetScale() );
}

//*************************************************************************************
//
//*************************************************************************************
void	CSizedItem::SetSize( const V2 & size )
{
	m_Size = size;
}

//*************************************************************************************
//
//*************************************************************************************
V2	CSizedItem::GetScreenPos() const
{
	return ( m_Pos + ( ( m_Size - ( m_Size * m_fScale ) ) * 0.5f ) );
}

//**********************************************************************************
//
//**********************************************************************************
float	CSizedItem::GetScale() const
{
	return m_fScale;
}

//**********************************************************************************
//
//**********************************************************************************
void	CSizedItem::SetScale( float scale )
{
	m_fScale = scale;
}

//*******************************  END OF FILE  ************************************
