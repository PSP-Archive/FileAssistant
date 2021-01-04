/***********************************************************************************

  Module :	CImageFileHandler.cpp

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
#include "CImageFileHandler.h"
#include "CMessageBox.h"
#include "CTextureManager.h"
#include "CGfx.h"
#include "CInput.h"
#include "CFrameWork.h"
#include "CRenderable.h"
#include "CFont.h"
#include "CHUD.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************
static const s32	MAX_UV( 512 );
static const float	SCALE_SPEED( 0.5f );
static const float	OFFSET_SPEED( 64.f );

//**********************************************************************************
//   Static Prototypes
//**********************************************************************************

//**********************************************************************************
//   Global Variables
//**********************************************************************************

//**********************************************************************************
//   Static Variables
//**********************************************************************************
static const sFileExtensionInfo	s_FileExtensionInfo =
{
	"View",
	CSkinManager::SC_ICON_IMAGE
};

CTexture *	CImageFileHandler::s_pTexture( NULL );
V2			CImageFileHandler::s_Size( 0.f, 0.f );
float		CImageFileHandler::s_fScale( 1.f );
V2			CImageFileHandler::s_Offset( 0.f, 0.f );

static bool	s_bFlip( false );
static bool	s_bMirror( false );

//**********************************************************************************
//   Class Definition
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
bool	CImageFileHandler::Execute( const CString & file )
{
	s_pTexture = CTextureManager::Create( file );

	if ( s_pTexture != NULL )
	{
		//
		//	Register our render callback
		//
		CRenderable::Register( CRenderable::RO_WINDOWS, CImageFileHandler::Render );

		//
		//	Draw the image in our own special loop
		//
		CProcess::Pause( true );

		s_fScale = 1.f;
		s_bFlip = false;
		s_bMirror = false;
		s_Offset = V2( 0.f, 0.f );

		CalculateSize();

		CHUD::SetButton( CHUD::BUTTON_START, "Hide UI" );
		CHUD::SetButtons( "", "Quit", "Flip", "Mirror" );

		while ( CInput::IsButtonClicked( CInput::CIRCLE ) == false )
		{
			const float	delta( CFrameWork::GetElapsedTime() );

			CFrameWork::Process();

			if ( CInput::IsButtonDown( CInput::UP ) == true )
			{
				s_fScale += SCALE_SPEED * delta;
			}

			if ( CInput::IsButtonDown( CInput::DOWN ) == true )
			{
				s_fScale -= SCALE_SPEED * delta;

				SETMIN( s_fScale, 1.f );
			}

			if ( CInput::GetAnalogStick().LengthSq() > SQUARE( 0.4f ) )
			{
				s_Offset += CInput::GetAnalogStick() * OFFSET_SPEED * delta * s_fScale;
			}

			if ( CFileHandler::MultiSelection() == true )
			{
				CString	file( "" );

				if ( CInput::IsButtonClicked( CInput::LTRIGGER ) == true )
				{
					file = CFileHandler::GetPrevFile();
				}

				if ( CInput::IsButtonClicked( CInput::RTRIGGER ) == true )
				{
					file = CFileHandler::GetNextFile();
				}

				if ( file.IsEmpty() == false )
				{
					SAFE_DELETE( s_pTexture );

					s_pTexture = CTextureManager::Create( file );

					if ( s_pTexture == NULL )
					{
						break;
					}

					s_fScale = 1.f;
					s_bFlip = false;
					s_bMirror = false;
					s_Offset = V2( 0.f, 0.f );

					CalculateSize();
				}
			}

			if ( CInput::IsButtonClicked( CInput::SQUARE ) == true )
			{
				s_bFlip = ( s_bFlip == false );
			}

			if ( CInput::IsButtonClicked( CInput::TRIANGLE ) == true )
			{
				s_bMirror = ( s_bMirror == false );
			}

			if ( CInput::IsButtonClicked( CInput::START ) == true )
			{
				CHUD::Show( ( CHUD::IsVisible() == false ) );
			}
		}

		//
		//	Remove our render callback
		//
		CRenderable::UnRegister( CRenderable::RO_WINDOWS, CImageFileHandler::Render );

		SAFE_DELETE( s_pTexture );

		CProcess::Pause( false );

		CHUD::Show( true );

		return true;
	}

	CErrorMessage( "Invalid image file!" );

	return false;
}

//**********************************************************************************
//
//**********************************************************************************
const sFileExtensionInfo &	CImageFileHandler::Information( const CString & file )
{
	return s_FileExtensionInfo;
}

//**********************************************************************************
//	Calculate the scale to fit the image on screen
//**********************************************************************************
void	CImageFileHandler::CalculateSize()
{
	if ( s_pTexture->m_nWidth <= CGfx::s_ScreenWidth && s_pTexture->m_nHeight <= CGfx::s_ScreenHeight )
	{
		s_Size.x = s_pTexture->m_nWidth;
		s_Size.y = s_pTexture->m_nHeight;
	}
	else
	{
		s_Size.x = static_cast< float >( CGfx::s_ScreenWidth ) / s_pTexture->m_nWidth;
		s_Size.y = static_cast< float >( CGfx::s_ScreenHeight ) / s_pTexture->m_nHeight;

		if ( s_Size.x < s_Size.y )
		{
			s_Size.x = s_pTexture->m_nWidth * s_Size.x;
			s_Size.y = s_pTexture->m_nHeight * s_Size.x;
		}
		else
		{
			s_Size.x = s_pTexture->m_nWidth * s_Size.y;
			s_Size.y = s_pTexture->m_nHeight * s_Size.y;
		}
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CImageFileHandler::Render()
{
	s32			width( s_pTexture->m_nWidth );
	s32			height( s_pTexture->m_nHeight );
	const V2	pos( s_Offset.x + ( 0.5f * ( CGfx::s_ScreenWidth - ( s_Size.x * s_fScale ) ) ), s_Offset.y + ( 0.5f * ( CGfx::s_ScreenHeight - ( s_Size.y * s_fScale ) ) ) );
	const float	x_scale( ( s_Size.x * s_fScale ) / width );
	const float	y_scale( ( s_Size.y * s_fScale ) / height );

	CGfx::DrawQuad( V2( 0.f, 0.f ), V2( CGfx::s_ScreenWidth, CGfx::s_ScreenHeight ), 0x80000000 );

	//
	//	Render the image in 512x512 chunks as the PSP doesn't
	//	support textures greater than 512x512 :(
	//
	while( height > 0 )
	{
		width = s_pTexture->m_nWidth;

		while( width > 0 )
		{
			s32	rwidth( width );

			if ( s_bFlip )
			{
				const s32	remainder( rwidth % MAX_UV );

				if ( width == ( s32 )s_pTexture->m_nWidth )
				{
					rwidth = remainder;
				}
				else if ( width == remainder )
				{
					rwidth = MAX_UV;
				}
			}

			sVertexTexturedColor *	p_vert;
			const s32				x_size( s_pTexture->m_nWidth - rwidth );
			const s32				y_size( s_pTexture->m_nHeight - height );
			s32						u_step( rwidth >= MAX_UV ? MAX_UV : rwidth );
			s32						v_step( height >= MAX_UV ? MAX_UV : height );
			s32						u[ 2 ], v[ 2 ];

			CGfx::GetPolyList( 2, &p_vert );

			u[ 0 ] = 0;
			v[ 0 ] = 0;
			u[ 1 ] = u_step;
			v[ 1 ] = v_step;

			if ( s_bFlip == true )
			{
				const s32	temp( u[ 0 ] );
				u[ 0 ] = u[ 1 ];
				u[ 1 ] = temp;
			}

			if ( s_bMirror == true )
			{
				const s32	temp( v[ 0 ] );
				v[ 0 ] = v[ 1 ];
				v[ 1 ] = temp;
			}

			p_vert[ 0 ].uv.x = u[ 0 ];
			p_vert[ 0 ].uv.y = v[ 0 ];
			p_vert[ 0 ].pos.x = pos.x + ( x_size * x_scale );
			p_vert[ 0 ].pos.y = pos.y + ( y_size * y_scale );
			p_vert[ 0 ].pos.z = 0.f;
			p_vert[ 0 ].color = 0xffffffff;

			p_vert[ 1 ].uv.x = u[ 1 ];
			p_vert[ 1 ].uv.y = v[ 1 ];
			p_vert[ 1 ].pos.x = pos.x + ( ( x_size + u_step ) * x_scale );
			p_vert[ 1 ].pos.y = pos.y + ( ( y_size + v_step ) * y_scale );
			p_vert[ 1 ].pos.z = 0.f;
			p_vert[ 1 ].color = 0xffffffff;

			sceGuEnable( GU_TEXTURE_2D );
			sceGuShadeModel( GU_SMOOTH );
			sceGuTexMode( GU_PSM_8888, 0, 0, 0 );
			sceGuTexImage( 0, MAX_UV, MAX_UV, s_pTexture->m_nCanvasWidth, s_pTexture->m_pBuffer + ( 4 * ( x_size + ( y_size * s_pTexture->m_nCanvasWidth ) ) ) );
			sceGuTexFunc( GU_TFX_MODULATE, GU_TCC_RGBA );
			sceGuTexWrap( GU_CLAMP, GU_CLAMP );
			sceGuTexFilter( GU_LINEAR, GU_LINEAR );
			sceGuTexScale( 1.0f, 1.0f );
			sceGuTexOffset( 0.0f, 0.0f );
			sceGuAmbientColor( 0xffffffff );
			sceGuDrawArray( GU_SPRITES, GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, p_vert );

			width -= MAX_UV;
		}

		height -= MAX_UV;
	}

	//
	//	Display some image information
	//
/*	CString	text;

	text.Printf( "Width = %d\nHeight = %d\n", s_pTexture->m_nWidth, s_pTexture->m_nHeight );

	CFont::GetDefaultFont()->Print( text, V2( 17.f, 17.f ), 0xff000000 );
	CFont::GetDefaultFont()->Print( text, V2( 16.f, 16.f ), 0xffffffff );*/
}

//*******************************  END OF FILE  ************************************
