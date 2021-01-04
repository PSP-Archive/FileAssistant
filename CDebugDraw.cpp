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

  Module :	CDebugDraw.cpp

  Description :

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 08 September 2005 71M

***********************************************************************************/

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CDebugDraw.h"
#include "CRenderable.h"
#include "CGfx.h"

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
V3	CDebugDraw::s_SphereCoords[ SPHERE_FACETS ][ 3 ];

//**********************************************************************************
//   Class Definition
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
void	CDebugDraw::Open()
{
	//
	// Lay out points around a unit sphere - this method essentiallyuses longitude and latitude.
	// From http://astronomy.swin.edu.au/pbourke/geometry/
	// There are some different methods on that page too.
	//
	u32			facet( 0 );
	const float	dtheta( G_PI / SPHERE_VERTICAL_SLICES );
	const float	dphi( G_PI_TIMES_2 / SPHERE_HORIZONTAL_SLICES );

	for ( u32 y = 0; y < SPHERE_VERTICAL_SLICES; y++ )
	{
		const float	theta( -G_PI_ON_2 + ( y * dtheta ) );

		for ( u32 x = 0; x < SPHERE_HORIZONTAL_SLICES; x++ )
		{
			const float	phi( -G_PI + ( x * dphi ) );

			s_SphereCoords[ facet ][ 0 ].x = cosf( theta ) * cosf( phi );
			s_SphereCoords[ facet ][ 0 ].y = cosf( theta ) * sinf( phi );
			s_SphereCoords[ facet ][ 0 ].z = sinf( theta );

			s_SphereCoords[ facet ][ 1 ].x = cosf( ( theta + dtheta ) ) * cosf( phi );
			s_SphereCoords[ facet ][ 1 ].y = cosf( ( theta + dtheta ) ) * sinf( phi );
			s_SphereCoords[ facet ][ 1 ].z = sinf( ( theta + dtheta ) );

			s_SphereCoords[ facet ][ 2 ].x = cosf( ( theta + dtheta ) ) * cosf( ( phi + dphi ) );
			s_SphereCoords[ facet ][ 2 ].y = cosf( ( theta + dtheta ) ) * sinf( ( phi + dphi ) );
			s_SphereCoords[ facet ][ 2 ].z = sinf( ( theta + dtheta ) );

			++facet;
		}
	}

	CRenderable::Register( CRenderable::RO_BACKGROUND, Render );
}

//**********************************************************************************
//
//**********************************************************************************
void	CDebugDraw::Close()
{
	CRenderable::UnRegister( CRenderable::RO_BACKGROUND, Render );
}

//**********************************************************************************
//
//**********************************************************************************
void	CDebugDraw::Render()
{
}

//**********************************************************************************
//
//**********************************************************************************
void	CDebugDraw::DrawLine( const V3 & p0, const V3 & p1, ARGB col0, ARGB col1 )
{
	sVertexColor *	p_line;

	CGfx::GetPolyList( 2, &p_line );

	p_line[ 0 ].color = col0;
	p_line[ 0 ].pos = p0;
	p_line[ 1 ].color = col1;
	p_line[ 1 ].pos = p1;

	CGfx::DrawPoly3D( GU_LINE_STRIP, p_line, 2 );
}

//**********************************************************************************
//
//**********************************************************************************
void	CDebugDraw::DrawSphere( const V3 & centre, float radius, ARGB color )
{
	V3	p[ 3 ];
	M44	mat_pos, mat_scale, mat_object;

	mat_pos.SetTranslation( centre );
	mat_scale.SetScaling( radius );

	mat_object = mat_scale.Multiply( mat_pos );

	for ( u32 f = 0; f < SPHERE_FACETS; f++ )
	{
		p [ 0 ] = mat_object.TransformCoord( s_SphereCoords[ f ][ 0 ] );
		p [ 1 ] = mat_object.TransformCoord( s_SphereCoords[ f ][ 1 ] );
		p [ 2 ] = mat_object.TransformCoord( s_SphereCoords[ f ][ 2 ] );

		DrawLine( p[ 0 ], p[ 1 ], color, color );
		DrawLine( p[ 1 ], p[ 2 ], color, color );
	}
}

//*******************************  END OF FILE  ************************************
