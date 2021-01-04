/***********************************************************************************

  Module :	CTextInput.cpp

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
#include "CFrameWork.h"
#include "CTextInput.h"
#include "CHUD.h"
#include "CInput.h"
#include "CGfx.h"
#include "CFont.h"
#include "CSkinManager.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************
static const float	TEXT_INPUT_WIDTH( 256.f );
static const float	TEXT_INPUT_HEIGHT( 48.f );

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
//   Text input class implementation
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
CTextInput::CTextInput( const CString & title, const CString & text )
:	m_szText( text )
,	m_pKeyboard( NULL )
{
	m_pKeyboard = new CKeyboard( this );

	SetTitle( title );

	SetPos( V2( 0.5f * ( CGfx::s_ScreenWidth - TEXT_INPUT_WIDTH ), 0.5f * ( CGfx::s_ScreenHeight - TEXT_INPUT_HEIGHT ) ) );
	SetSize( V2( TEXT_INPUT_WIDTH, TEXT_INPUT_HEIGHT ) );
	SetFocus( true );

	CHUD::SetButtons( "Select", "Cancel", "Delete", "Accept" );
}

//**********************************************************************************
//
//**********************************************************************************
CTextInput::~CTextInput()
{
	SAFE_DELETE( m_pKeyboard );
}

//**********************************************************************************
//
//**********************************************************************************
void	CTextInput::Render()
{
	CWindow::Render();

	V2				pos( GetScreenPos() );
	CString			current_key;
	CFont * const	p_font( CFont::GetDefaultFont() );
	const V2		text_size( p_font->GetStringSize( m_szText ) );
	const ARGB		marker_color( CSkinManager::GetColor( "directory_list", "file_mark_color_on", 0x80404060 ) );

	current_key.Printf( "%c", m_pKeyboard->GetCurrentKey() );

	pos.x += 0.5f * ( TEXT_INPUT_WIDTH - text_size.x );
	pos.y += 0.5f * ( TEXT_INPUT_HEIGHT - text_size.y );

	p_font->Print( m_szText, pos, GetTextColor() );

	const V2	key_size( p_font->GetStringSize( current_key ) );

	CGfx::DrawQuad( V2( pos.x + text_size.x, pos.y ), key_size, marker_color );

	p_font->Print( current_key, V2( pos.x + text_size.x, pos.y ), ARGB( GetTextColor().a >> 1, GetTextColor().r, GetTextColor().g, GetTextColor().b ) );
}

//**********************************************************************************
//
//**********************************************************************************
CTextInput::eReturnCode	CTextInput::Show()
{
	CProcess::Pause( true );

	while ( 1 )
	{
		CFrameWork::Process();

		m_pKeyboard->ProcessInput();

		if ( CInput::IsButtonClicked( CInput::TRIANGLE ) == true )
		{
			CProcess::Pause( false );

			return RC_OK;
		}

		if ( CInput::IsButtonClicked( CInput::CIRCLE ) == true )
		{
			CProcess::Pause( false );

			return RC_CANCEL;
		}
	}

	return RC_CANCEL;
}

//**********************************************************************************
//
//**********************************************************************************
const CString &	CTextInput::GetText() const
{
	return m_szText;
}

//**********************************************************************************
//
//**********************************************************************************
void	CTextInput::Message( CKeyboardListener::eMessage message, const void * const p_data )
{
	char * const	p_string( const_cast< char * >( m_szText.GetPtr() ) );

	switch ( message )
	{
	case MSG_DELETE:
		{
			const u32	length( m_szText.Length() );

			if ( length > 0 )
			{
				p_string[ length - 1 ] = '\0';
			}
		}
		break;

	case MSG_CHARACTER:
		{
			const char	buffer[ 2 ] = { static_cast< char >( ( u32 )( p_data ) ), '\0' };

			m_szText += buffer;
		}
		break;

	case MSG_CURSOR:
		{
		}
		break;

	default:
		{
			ASSERT( 0, "Received unknown keyboard message" );
		}
		break;
	}
}

//*******************************  END OF FILE  ************************************
