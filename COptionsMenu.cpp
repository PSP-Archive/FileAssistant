/***********************************************************************************

  Module :	COptionsMenu.cpp

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
#include "COptionsMenu.h"
#include "CWindowTable.h"
#include "CFont.h"
#include "CFileAssistant.h"
#include "CMessageBox.h"
#include "CInput.h"
#include "CFileSystem.h"
#include "CFrameWork.h"
#include "CFileHandler.h"
#include "CHUD.h"
#include "CTextInput.h"
#include "CGfx.h"
#include "CUSBManager.h"
#include "CMusicFileHandler.h"
#include "CSkinSelectMenu.h"
#include "CExecutableFileHandler.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************
static const float	WINDOW_BORDER( 16.f );

//**********************************************************************************
//   Static Prototypes
//**********************************************************************************

//**********************************************************************************
//   Global Variables
//**********************************************************************************

//**********************************************************************************
//   Static Variables
//**********************************************************************************
static const float	MIN_TABLE_WIDTH( 200.f );

const CString	COptionsMenu::s_szOptionNames[ MAX_OPTIONS ] =
{
	"Make Directory",		// OPTION_MAKE_DIRECTORY,
	"Toggle USB",			// OPTION_USB_CONNECTION,
	"Select Skin",			// OPTION_SKIN_SELECTION,
	"Hide Corrupt Files",	// OPTION_HIDE_CORRUPT_FILES,
	"Stop Music",			// OPTION_STOP_MUSIC,
	"Credits",				// OPTION_CREDITS,
	"Hide Visualiser",		// OPTION_HIDE_VISUALISER,
	"Toggle Echo FX",		// OPTION_TOGGLE_ECHO_FX,
	"Toggle Spatial FX",	// OPTION_TOGGLE_SPATIAL_FX,
};

//**********************************************************************************
//**********************************************************************************
//
//	Menu option item implementation
//
//**********************************************************************************
//**********************************************************************************
class COptionsMenuItem : public CWindowTableItem
{
	public:

		COptionsMenuItem( COptionsMenu::eOption option, const CString & text );

		virtual V2				Render( V2 pos, bool highlight, float scroll_offset );

		virtual V2				GetSize() const;

		COptionsMenu::eOption	GetOptionNumber() const;

	protected:

		CString					m_szText;
		COptionsMenu::eOption	m_OptionNumber;
};

//**********************************************************************************
//
//**********************************************************************************
COptionsMenuItem::COptionsMenuItem( COptionsMenu::eOption option, const CString & text )
:	m_szText( text )
,	m_OptionNumber( option )
{
}

//**********************************************************************************
//
//**********************************************************************************
V2	COptionsMenuItem::Render( V2 pos, bool highlight, float scroll_offset )
{
	CFont::GetDefaultFont()->Print( m_szText, pos, m_pParent->GetColor() );

	pos.y += GetSize().y + 1;

	return pos;
}

//**********************************************************************************
//
//**********************************************************************************
V2	COptionsMenuItem::GetSize() const
{
	return CFont::GetDefaultFont()->GetStringSize( m_szText );
}

//**********************************************************************************
//
//**********************************************************************************
COptionsMenu::eOption	COptionsMenuItem::GetOptionNumber() const
{
	return m_OptionNumber;
}


//**********************************************************************************
//**********************************************************************************
//
//	File option menu implementation
//
//**********************************************************************************
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
COptionsMenu::COptionsMenu()
:	m_pOptions( new CWindowTable( true ) )
{
	//
	//	Add all the available options
	//
	AddOptions();

	//
	//	Set the size depending on the number of items
	//
	V2	table_size( m_pOptions->GetAllItemsSize() );
	V2	window_size( MIN_TABLE_WIDTH, table_size.y + ( 3.f * WINDOW_BORDER ) );

	SetTitle( "Options" );
	SetPos( V2( ( CGfx::s_ScreenWidth - MIN_TABLE_WIDTH ) * 0.5f, 0.5f * ( CGfx::s_ScreenHeight - window_size.y ) ) );
	SetSize( window_size );

	CFileAssistant::Get()->SetListFocus( false );

	table_size.x = MIN_TABLE_WIDTH - ( 2.f * WINDOW_BORDER );

	m_pOptions->SetFocus( true );
	m_pOptions->SetPos( V2( WINDOW_BORDER, WINDOW_BORDER ) );
	m_pOptions->SetSize( table_size );

	AddItem( m_pOptions );
}

//**********************************************************************************
//
//**********************************************************************************
COptionsMenu::~COptionsMenu()
{
	SAFE_DELETE( m_pOptions );
}

//**********************************************************************************
//
//**********************************************************************************
void	COptionsMenu::Delete()
{
	CFileAssistant::Get()->SetListFocus( true );

	CWindow::Delete();
}

//**********************************************************************************
//
//**********************************************************************************
void	COptionsMenu::Process()
{
	CWindow::Process();
}

//**********************************************************************************
//
//**********************************************************************************
void	COptionsMenu::ProcessInput()
{
	if ( HasFocus() == true )
	{
		if ( CInput::IsButtonClicked( CInput::START ) == true || CInput::IsButtonClicked( CInput::CIRCLE ) == true )
		{
			Delete();
		}
	}

	CWindow::ProcessInput();
}

//**********************************************************************************
//
//**********************************************************************************
void	COptionsMenu::AddOptions()
{
	COptionsMenuItem *	p_item( NULL );

	//
	//	Make directory
	//
	if ( CFileAssistant::Get()->GetFocusList()->ReadOnly() == false )
	{
		p_item = new COptionsMenuItem( OPTION_MAKE_DIRECTORY, s_szOptionNames[ OPTION_MAKE_DIRECTORY ] );
		p_item->SetCallback( OptionSelectedCallback );
		m_pOptions->AddItem( p_item );
	}

	//
	//	USB toggle
	//
	p_item = new COptionsMenuItem( OPTION_USB_CONNECTION, s_szOptionNames[ OPTION_USB_CONNECTION ] );
	p_item->SetCallback( OptionSelectedCallback );
	m_pOptions->AddItem( p_item );

	//
	//	Skin selection
	//
	p_item = new COptionsMenuItem( OPTION_SKIN_SELECTION, s_szOptionNames[ OPTION_SKIN_SELECTION ] );
	p_item->SetCallback( OptionSelectedCallback );
	m_pOptions->AddItem( p_item );

	//
	//	Corrupt file selection
	//
	if ( CFileSystem::HideCorruptFiles() == true )
	{
		p_item = new COptionsMenuItem( OPTION_HIDE_CORRUPT_FILES, "Show Corrupt Files" );
	}
	else
	{
		p_item = new COptionsMenuItem( OPTION_HIDE_CORRUPT_FILES, "Hide Corrupt Files" );
	}
	p_item->SetCallback( OptionSelectedCallback );
	m_pOptions->AddItem( p_item );

	//
	//	Stop music
	//
	if ( CMusicFileHandler::IsPlaying() == true )
	{
		p_item = new COptionsMenuItem( OPTION_STOP_MUSIC, s_szOptionNames[ OPTION_STOP_MUSIC ] );
		p_item->SetCallback( OptionSelectedCallback );
		m_pOptions->AddItem( p_item );

		//
		//	Show/Hide visualiser
		//
		if ( CMusicFileHandler::VisualiserVisible() == true )
		{
			p_item = new COptionsMenuItem( OPTION_HIDE_VISUALISER, "Hide Visualiser" );
		}
		else
		{
			p_item = new COptionsMenuItem( OPTION_HIDE_VISUALISER, "Show Visualiser" );
		}
		p_item->SetCallback( OptionSelectedCallback );
		m_pOptions->AddItem( p_item );

		//
		//	Add spatial fx options
		//
		if ( pspAudioGetCurrentEffect() != FX_ECHO )
		{
			p_item = new COptionsMenuItem( OPTION_TOGGLE_ECHO_FX, "Toggle Echo FX On" );
		}
		else
		{
			p_item = new COptionsMenuItem( OPTION_TOGGLE_ECHO_FX, "Toggle Echo FX Off" );
		}
		p_item->SetCallback( OptionSelectedCallback );
		m_pOptions->AddItem( p_item );

		if ( pspAudioGetCurrentEffect() != FX_SPATIAL_SOUND )
		{
			p_item = new COptionsMenuItem( OPTION_TOGGLE_SPATIAL_FX, "Toggle Spatial FX On" );
		}
		else
		{
			p_item = new COptionsMenuItem( OPTION_TOGGLE_SPATIAL_FX, "Toggle Spatial FX Off" );
		}
		p_item->SetCallback( OptionSelectedCallback );
		m_pOptions->AddItem( p_item );
	}

	//
	//	Credits
	//
	p_item = new COptionsMenuItem( OPTION_CREDITS, s_szOptionNames[ OPTION_CREDITS ] );
	p_item->SetCallback( OptionSelectedCallback );
	m_pOptions->AddItem( p_item );
}

//**********************************************************************************
//
//**********************************************************************************
void	COptionsMenu::OptionSelectedCallback( CWindowTableItem * p_item, u32 item_no )
{
	COptionsMenuItem * const	p_option_item( static_cast< COptionsMenuItem * >( p_item ) );
	CWindowTable * const		p_table( p_option_item->GetParent() );
	COptionsMenu * const		p_window( static_cast< COptionsMenu * >( p_table->GetParent() ) );

	p_window->SetFocus( false );

	switch ( p_option_item->GetOptionNumber() )
	{
	case OPTION_MAKE_DIRECTORY:
		{
			CTextInput	text_input( "Make Directory", "" );

			if ( text_input.Show() == CTextInput::RC_OK )
			{
				const CString	new_dir( CFileAssistant::Get()->GetFocusList()->GetCurrentPath() + text_input.GetText() );

				CFileSystem::MakeDirectory( new_dir );

				CFileAssistant::Get()->GetFocusList()->RefreshList( true );

				p_window->Delete();
			}
		}
		break;

	case OPTION_USB_CONNECTION:
		{
			if ( CUSBManager::IsActive() == false )
			{
				CUSBManager::Activate();
			}
			else
			{
				CUSBManager::Deactivate();
			}
		}
		break;

	case OPTION_SKIN_SELECTION:
		{
			CSkinSelectMenu *	p_skin_select( new CSkinSelectMenu() );

			p_skin_select->Show();

			SAFE_DELETE( p_skin_select );
		}
		break;

	case OPTION_HIDE_CORRUPT_FILES:
		{
			CFileSystem::SetHideCorruptFiles( ( CFileSystem::HideCorruptFiles() == false ) );

			CFileAssistant::Get()->GetSrcList()->RefreshList( true );
			CFileAssistant::Get()->GetDstList()->RefreshList( true );

			p_window->Delete();
		}
		break;

	case OPTION_STOP_MUSIC:
		{
			CMusicFileHandler::Stop();

			p_window->Delete();
		}
		break;

	case OPTION_HIDE_VISUALISER:
		{
			CMusicFileHandler::ShowVisualiser( CMusicFileHandler::VisualiserVisible() == false );

			p_window->Delete();
		}
		break;

	case OPTION_TOGGLE_ECHO_FX:
		{
			CMusicFileHandler::ToggleEcho();

			p_window->Delete();
		}
		break;

	case OPTION_TOGGLE_SPATIAL_FX:
		{
			CMusicFileHandler::ToggleSpatialSound();

			p_window->Delete();
		}
		break;

	case OPTION_CREDITS:
		{
			RunCredits();
		}
		break;

	default:
		{
			ASSERT( 0, "Unknown option number!" );
		}
	}

	p_window->SetFocus( true );
}

//**********************************************************************************
//
//**********************************************************************************
void	COptionsMenu::SetFocus( bool focus )
{
	CWindow::SetFocus( focus );

	if ( focus == true )
	{
		CHUD::SetButtons( "Select", "Back", "", "" );
		CHUD::SetButton( CHUD::BUTTON_START, "Back" );
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	COptionsMenu::RunCredits()
{
	const CString	credits_exe( CFileSystem::GetCurrentFolder() + "/Data/Credits/Credits.elf" );

	if ( CELFFileHandler::Execute( credits_exe ) != true )
	{
		ASSERT( 0, "Failed to run Credits!" );
	}
}

//*******************************  END OF FILE  ************************************
