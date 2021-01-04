/***********************************************************************************

  Module :	CKeyboard.cpp

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
#include "CKeyboard.h"
#include "CFont.h"
#include "CInput.h"
#include "CGfx.h"
#include "CSkinManager.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************
static const s32	KEY_REPEAT( 4 );
static const V2		KEYBOARD_POS( 0.5f * ( 480.f - 256.f ), 8.f );
static const V2		KEY_SIZE( 16.f, 16.f );
static const V2		WINDOW_BORDER( 16.f, 16.f );

//**********************************************************************************
//   Static Prototypes
//**********************************************************************************

//**********************************************************************************
//   Global Variables
//**********************************************************************************

//**********************************************************************************
//   Static Variables
//**********************************************************************************
static const CString	s_Keyboard0[ CKeyboard::NUM_ROWS ] =
{
	"1234567890-=",
	"qwertyuiop[]",
	"asdfghjkl;'#",
	"\\zxcvbnm,./ ",
};

static const CString	s_Keyboard1[ CKeyboard::NUM_ROWS ] =
{
	"!\"£$%^&*()_+",
	"QWERTYUIOP{}",
	"ASDFGHJKL:@~",
	"\\ZXCVBNM<>? ",
};

//**********************************************************************************
//   Class Definition
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
CKeyboard::CKeyboard( CKeyboardListener * const p_listener )
:	m_pListener( p_listener )
,	m_pCurrentLayout( s_Keyboard0 )
,	m_fWidestRow( 0.f )
,	m_nRow( 0 )
,	m_nColumn( 0 )
,	m_IRDA( -1 )
{
	SetTitle( "Keyboard" );

	for ( u32 i = 0; i < NUM_ROWS; ++i )
	{
		m_fRowWidth[ i ] = s_Keyboard0[ i ].Length() * KEY_SIZE.x;

		if ( m_fRowWidth[ i ] > m_fWidestRow )
		{
			m_fWidestRow = m_fRowWidth[ i ];
		}
	}

	const V2	border( WINDOW_BORDER * 2.f );
	const V2	size( V2( m_fWidestRow, NUM_ROWS * KEY_SIZE.y ) + border );

	SetPos( V2( 0.5f * ( CGfx::s_ScreenWidth - size.x ), KEYBOARD_POS.y ) );
	SetSize( size );
	SetFocus( true );

	//
	//	Open the IRDA device
	//
	m_IRDA = sceIoOpen( "irda0:", PSP_O_RDWR | PSP_O_NBLOCK | PSP_O_NOWAIT, 0 );
}

//**********************************************************************************
//
//**********************************************************************************
void	CKeyboard::Render()
{
	CGfx::DrawQuad( V2( 0.f, 0.f ), V2( CGfx::s_ScreenWidth, CGfx::s_ScreenHeight ), 0x80000000 );

	CWindow::Render();

	const V2		orig_pos( GetScreenPos() + WINDOW_BORDER );
	V2				pos( orig_pos );
	CFont * const	p_font( CFont::GetDefaultFont() );
	const ARGB		text_color( GetTextColor() );
	const ARGB		marker_color( CSkinManager::GetColor( "directory_list", "file_mark_color_on", 0x80404060 ) );

	for ( s32 r = 0; r < NUM_ROWS; ++r )
	{
		const CString &	row( m_pCurrentLayout[ r ] );

		for ( s32 k = 0; k < row.Length(); ++k )
		{
			const char	key[ 2 ] = { row[ k ], '\0' };
			const V2	text_size( p_font->GetStringSize( key ) );

			if ( r == m_nRow && k == m_nColumn )
			{
				CGfx::DrawQuad( pos, KEY_SIZE, marker_color );
			}

			p_font->Print( key, pos + ( ( KEY_SIZE - text_size ) * 0.5f ), text_color, 1.f );

			pos.x += KEY_SIZE.x;
		}

		pos.x = orig_pos.x;
		pos.y += KEY_SIZE.y;
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CKeyboard::ProcessInput()
{
	if ( CInput::IsButtonRepeat( CInput::UP, KEY_REPEAT ) == true )
	{
		--m_nRow;

		if ( m_nRow < 0 )
		{
			m_nRow = NUM_ROWS - 1;
		}
	}

	if ( CInput::IsButtonRepeat( CInput::DOWN, KEY_REPEAT ) == true )
	{
		++m_nRow;

		if ( m_nRow >= NUM_ROWS )
		{
			m_nRow = 0;
		}
	}

	const s32	row_width( m_pCurrentLayout[ m_nRow ].Length() );

	if ( CInput::IsButtonRepeat( CInput::LEFT, KEY_REPEAT ) == true )
	{
		--m_nColumn;

		if ( m_nColumn < 0 )
		{
			m_nColumn = row_width - 1;
		}
	}

	if ( CInput::IsButtonRepeat( CInput::RIGHT, KEY_REPEAT ) == true )
	{
		++m_nColumn;

		if ( m_nColumn >= row_width )
		{
			m_nColumn = 0;
		}
	}

	if ( CInput::IsButtonClicked( CInput::LTRIGGER ) == true || CInput::IsButtonClicked( CInput::RTRIGGER ) == true )
	{
		if ( m_pCurrentLayout == s_Keyboard0 )
		{
			m_pCurrentLayout = s_Keyboard1;
		}
		else
		{
			m_pCurrentLayout = s_Keyboard0;
		}
	}

	if ( CInput::IsButtonClicked( CInput::CROSS ) == true )
	{
		const char * const	p_row( m_pCurrentLayout[ m_nRow ] );

		m_pListener->Message( CKeyboardListener::MSG_CHARACTER, ( void * )( ( u32 )p_row[ m_nColumn ] ) );
	}

	if ( CInput::IsButtonClicked( CInput::SQUARE ) == true )
	{
		m_pListener->Message( CKeyboardListener::MSG_DELETE, NULL );
	}


	// check for pad info from other PSP
	if ( m_IRDA >= 0 )
	{
		u8	data;

		if ( sceIoRead( m_IRDA, &data, 1 ) == 1 )
		{

		}
	}

	CWindow::ProcessInput();
}

//**********************************************************************************
//
//**********************************************************************************
char	CKeyboard::GetCurrentKey() const
{
	const char * const	p_row( m_pCurrentLayout[ m_nRow ] );

	return p_row[ m_nColumn ];
}

//*******************************  END OF FILE  ************************************
