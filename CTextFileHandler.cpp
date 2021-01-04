/***********************************************************************************

  Module :	CTextFileHandler.cpp

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
#include "CTextFileHandler.h"
#include "CRenderable.h"
#include "CHUD.h"
#include "CInput.h"
#include "CFileSystem.h"
#include "CProcess.h"
#include "CFrameWork.h"
#include "CMessageBox.h"
#include "CGfx.h"
#include "CFont.h"
#include "CTextureManager.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************
static const u32	SCROLL_INCR( 1 );
static const u32	PAGE_SIZE( 10 );
static const V2		TEXT_WINDOW_SIZE( 400.f, 240.f );

//**********************************************************************************
//   Static Prototypes
//**********************************************************************************

//**********************************************************************************
//   Global Variables
//**********************************************************************************

//**********************************************************************************
//   Static Variables
//**********************************************************************************
static CTextBuffer *			s_TextBuffer;

static const sFileExtensionInfo	s_FileExtensionInfo =
{
	"Read",
	CSkinManager::SC_ICON_TEXT
};


//**********************************************************************************
//   Class Definition
//**********************************************************************************
class CTextWindow : public CWindow
{
	public:

		CTextWindow( CTextBuffer * const p_text_buffer );

		virtual void	Render();

		virtual void	ProcessInput();

	private:

		 CTextBuffer *	m_pTextBuffer;
		 u32			m_nXPos;
		 u32			m_nYPos;
		 u32			m_nNumLines;
};

//**********************************************************************************
//	
//**********************************************************************************
CTextWindow::CTextWindow( CTextBuffer * const p_text_buffer )
:	m_pTextBuffer( p_text_buffer )
,	m_nXPos( 0 )
,	m_nYPos( 0 )
{
	CFont * const	p_font( CFont::GetDefaultFont() );
	const V2 &		font_size( p_font->m_Size );

	m_nNumLines = static_cast< u32 >( TEXT_WINDOW_SIZE.y / ( font_size.y + 1 ) );

	SetPos( V2( ( CGfx::s_ScreenWidth - TEXT_WINDOW_SIZE.x ) * 0.5f, ( CGfx::s_ScreenHeight - TEXT_WINDOW_SIZE.y ) * 0.5f ) );
	SetSize( TEXT_WINDOW_SIZE );
}

//**********************************************************************************
//	
//**********************************************************************************
void	CTextWindow::Render()
{
	CWindow::Render();

	CFont * const	p_font( CFont::GetDefaultFont() );
	const V2 &		font_size( p_font->m_Size );
	const ARGB		text_color( CWindow::GetTextColor() );
	V2				screen_pos( GetScreenPos() );

	screen_pos += V2( font_size.x, font_size.y );

	V2				background_pos( screen_pos );

	CGfx::SetClipRegion( screen_pos, TEXT_WINDOW_SIZE - ( font_size * 2.f ) );

	screen_pos.x -= font_size.x * m_nXPos;

	for ( u32 i = 0; i < m_nNumLines; ++i )
	{
		if ( ( i & 1 ) == 0 )
		{
			CGfx::DrawQuad( background_pos, V2( TEXT_WINDOW_SIZE.x - ( font_size.x * 2.f ), font_size.y ), 0x60000000 );
		}
		else
		{
			CGfx::DrawQuad( background_pos, V2( TEXT_WINDOW_SIZE.x - ( font_size.x * 2.f ), font_size.y ), 0x50000000 );
		}

		if ( ( i + m_nYPos ) < m_pTextBuffer->GetMaxHeight() )
		{
			p_font->Print( m_pTextBuffer->GetLine( i + m_nYPos ), screen_pos, text_color );
		}

		screen_pos.y += ( font_size.y + 1 );
		background_pos.y += ( font_size.y + 1 );
	}

	CGfx::DisableClipRegions();
}

//**********************************************************************************
//	
//**********************************************************************************
void	CTextWindow::ProcessInput()
{
	const u32	max_lines( m_nNumLines - 2 );

	if ( CInput::IsButtonRepeat( CInput::LEFT, 2 ) == true )
	{
		if ( m_nXPos >= SCROLL_INCR )
		{
			m_nXPos -= SCROLL_INCR;
		}
	}
	else if ( CInput::IsButtonRepeat( CInput::RIGHT, 2 ) == true )
	{
		if ( m_nXPos < ( s_TextBuffer->GetMaxWidth() - SCROLL_INCR ) )
		{
			m_nXPos += SCROLL_INCR;
		}
	}

	if ( CInput::IsButtonRepeat( CInput::UP, 2 ) == true )
	{
		if ( m_nYPos >= SCROLL_INCR )
		{
			m_nYPos -= SCROLL_INCR;
		}
		else
		{
			m_nYPos = 0;
		}
	}
	else if ( CInput::IsButtonRepeat( CInput::DOWN, 2 ) == true )
	{
		if ( m_nYPos < ( s_TextBuffer->GetMaxHeight() - SCROLL_INCR - max_lines ) )
		{
			m_nYPos += SCROLL_INCR;
		}
		else
		{
			m_nYPos = s_TextBuffer->GetMaxHeight() - max_lines;
		}
	}

	if ( CInput::IsButtonClicked( CInput::LTRIGGER ) == true )
	{
		if ( m_nYPos >= PAGE_SIZE )
		{
			m_nYPos -= PAGE_SIZE;
		}
		else
		{
			m_nYPos = 0;
		}
	}
	else if ( CInput::IsButtonClicked( CInput::RTRIGGER ) == true )
	{
		if ( m_nYPos < ( s_TextBuffer->GetMaxHeight() - PAGE_SIZE - max_lines ) )
		{
			m_nYPos += PAGE_SIZE;
		}
		else
		{
			m_nYPos = s_TextBuffer->GetMaxHeight() - max_lines;
		}
	}
}

//**********************************************************************************
//**********************************************************************************
//
//	CTextFileHandler class implementation
//
//**********************************************************************************
//**********************************************************************************

//**********************************************************************************
//	
//**********************************************************************************
bool	CTextFileHandler::Execute( const CString & file )
{
	// Allocate a new TextBuffer
	s_TextBuffer = new CTextBuffer();

	if ( s_TextBuffer->ProcessTextFile( file )  == true )
	{
		CProcess::Pause( true );

		CHUD::SetButton( CHUD::BUTTON_START, "Hide UI" );
		CHUD::SetButtons( "", "Quit", "", "" );

		CTextWindow *	p_text_window( new CTextWindow( s_TextBuffer ) );

		p_text_window->SetTitle( file );
		p_text_window->SetFocus( true );
		p_text_window->SetVisible( true );

		while ( CInput::IsButtonClicked( CInput::CIRCLE ) == false )
		{
			CFrameWork::Process();

			p_text_window->ProcessInput();

			if ( CInput::IsButtonClicked( CInput::START ) == true )
			{
				CHUD::Show( ( CHUD::IsVisible() == false ) );
			}
		}

		CProcess::Pause( false );

		CHUD::Show( true );

		SAFE_DELETE( p_text_window );
		SAFE_DELETE( s_TextBuffer );

		return true;
	}

	SAFE_DELETE( s_TextBuffer );

	CErrorMessage	box( "Invalid text file" );

	box.Show();

	return false;
}

//**********************************************************************************
//
//**********************************************************************************
const sFileExtensionInfo &	CTextFileHandler::Information( const CString & file )
{
	return s_FileExtensionInfo;
}

//*******************************  END OF FILE  ************************************
