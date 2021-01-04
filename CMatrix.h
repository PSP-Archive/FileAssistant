/***********************************************************************************

  Module :	CMatrix.h

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

#ifndef CMATRIX_H_
#define CMATRIX_H_

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
struct MATRIX_4X4
{
	public:

		union
		{
			struct
			{
				float	_11, _12, _13, _14;
				float	_21, _22, _23, _24;
				float	_31, _32, _33, _34;
				float	_41, _42, _43, _44;
			};

			float	m[ 4 ][ 4 ];
		};

	public:

		MATRIX_4X4()
		{
		}

		MATRIX_4X4( const float * p_mtx )
		: _11( p_mtx[  0 ] ), _12( p_mtx[  1 ] ), _13( p_mtx[  2 ] ), _14( p_mtx[  3 ] )
		, _21( p_mtx[  4 ] ), _22( p_mtx[  5 ] ), _23( p_mtx[  6 ] ), _24( p_mtx[  7 ] )
		, _31( p_mtx[  8 ] ), _32( p_mtx[  9 ] ), _33( p_mtx[ 10 ] ), _34( p_mtx[ 11 ] )
		, _41( p_mtx[ 12 ] ), _42( p_mtx[ 13 ] ), _43( p_mtx[ 14 ] ), _44( p_mtx[ 15 ] )
		{
		}

		MATRIX_4X4( float _11, float _12, float _13, float _14,
					float _21, float _22, float _23, float _24,
					float _31, float _32, float _33, float _34,
					float _41, float _42, float _43, float _44 )
		: _11( _11 ), _12( _12 ), _13( _13 ), _14( _14 )
		, _21( _21 ), _22( _22 ), _23( _23 ), _24( _24 )
		, _31( _31 ), _32( _32 ), _33( _33 ), _34( _34 )
		, _41( _41 ), _42( _42 ), _43( _43 ), _44( _44 )
		{
		}
};

//**********************************************************************************
//   Constants
//**********************************************************************************

//**********************************************************************************
//   Class definitions
//**********************************************************************************
class M44 : public MATRIX_4X4
{
	public:

		M44();
		M44( const MATRIX_4X4 & matrix );
		M44( const MATRIX_4X4 * p_matrix );
		M44( const M44 & matrix );
		M44( const M44 * p_matrix );
		M44( float _11, float _12, float _13, float _14,
			 float _21, float _22, float _23, float _24,
			 float _31, float _32, float _33, float _34,
			 float _41, float _42, float _43, float _44 );

	public:

		operator ScePspFMatrix4 * ();
		operator const ScePspFMatrix4 * () const;

		V3			GetPos() const;
		void		SetPos( const V3 & pos );

		V3			GetAt() const;
		void		SetAt( const V3 & at );

		V3			GetUp() const;
		void		SetUp( const V3 & up );

		V3			GetRight() const;
		void		SetRight( const V3 & right );

		M44 &		SetIdentity();

		M44 &		SetTranslation( float x, float y, float z );

		M44 &		SetTranslation( const V3 & v );

		M44 &		SetScaling( float s );
		M44 &		SetScaling( float sx, float sy, float sz );

		M44 &		SetRotationX( float x_rot );
		M44 &		SetRotationY( float y_rot );
		M44 &		SetRotationZ( float z_rot );

		M44 &		SetRotationAxis( const V3 & axis, float r );

		M44 &		Multiply( const M44 & matrix );

		M44			Inverse( float * const p_det = NULL ) const;

		M44			Transpose() const;

		M44 &		Scale( const float & s );
		M44 &		Scale( const V3 & s );
		M44 &		Scale( const float xval, const float yval, const float zval );

		M44 &		NormaliseScale();

		V3			TransformCoord( const V3 & vector ) const;
		V3			TransformNormal( const V3 & vector ) const;

		void		Projection( float fovy, float aspect, float near, float far );
};


//**********************************************************************************
//   Externs
//**********************************************************************************
const M44	gIdentity(	1.f, 0.f, 0.f, 0.f,
						0.f, 1.f, 0.f, 0.f,
						0.f, 0.f, 1.f, 0.f,
						0.f, 0.f, 0.f, 1.f	);

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* _MATRIX_H_ */
