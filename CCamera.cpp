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

  Module :	CCamera.cpp

  Description :

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 08 September 2005 71M

***********************************************************************************/

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CCamera.h"
#include "CFrameWork.h"

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
CCamera::CCamera()
:	m_Pos( gZeroVector )
,	m_Focus( gZeroVector )
{
}

//**********************************************************************************
//
//**********************************************************************************
void	CCamera::SetView()
{
	const V3	at( ( m_Pos - m_Focus ).Normal() );
	const V3	right( gUpVector.UnitCross( at ) );
	const V3	up( at.UnitCross( right ) );
	M44			projection_mtx, view_mtx, texture_mtx( gIdentity ), model_mtx( gIdentity );
	M44			rot_mtx( gIdentity );
	M44			trans_mtx( gIdentity );

	rot_mtx.SetAt( at );
	rot_mtx.SetUp( up );
	rot_mtx.SetRight( right );
	rot_mtx = rot_mtx.Inverse();
	trans_mtx.SetPos( -m_Pos );

	view_mtx = trans_mtx.Multiply( rot_mtx );

	projection_mtx.Projection( 45.0f, ( 16.0 / 9.0f ), 0.5f, 1000.0f );

	sceGuSetMatrix( GU_VIEW, view_mtx );
	sceGuSetMatrix( GU_MODEL, model_mtx );
	sceGuSetMatrix( GU_TEXTURE, texture_mtx );
	sceGuSetMatrix( GU_PROJECTION, projection_mtx );
}

//**********************************************************************************
//
//**********************************************************************************
const V3 &	CCamera::GetPos() const
{
	return m_Pos;
}

//**********************************************************************************
//
//**********************************************************************************
void	CCamera::SetPos( const V3 & pos )
{
	m_Pos = pos;
}

//**********************************************************************************
//
//**********************************************************************************
const V3 &	CCamera::GetFocus() const
{
	return m_Focus;
}

//**********************************************************************************
//
//**********************************************************************************
void	CCamera::SetFocus( const V3 & focus )
{
	m_Focus = focus;
}

//*******************************  END OF FILE  ************************************
