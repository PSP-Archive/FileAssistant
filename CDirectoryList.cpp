/***********************************************************************************

  Module :	CDirectoryList.cpp

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
#include "CDirectoryList.h"
#include "CFrameWork.h"
#include "CWindowTable.h"
#include "CTextureManager.h"
#include "CFont.h"
#include "CFileSystem.h"
#include "CInput.h"
#include "CFileOptions.h"
#include "CGfx.h"
#include "CHUD.h"
#include "CInformationDialog.h"
#include "CFileAssistant.h"
#include "CFTPClient.h"

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
const CString	CDirectoryList::s_szDriveNames[ MAX_DRIVES ][ MAX_DRIVE_INFO ] =
{
	{	"NULL",			"ALL_DRIVES"			},
	{	"flash0:/",		"PSP BIOS 0 (ReadOnly)"	},
	{	"flash1:/",		"PSP BIOS 1 (ReadOnly)"	},
	{	"ms0:/",		"Memory Stick"			},
	{	"disc0:/",		"UMD"					},
};

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
//**********************************************************************************
//
//	Directory list item implementation
//
//**********************************************************************************
//**********************************************************************************
class CDirectoryListItem : public CWindowTableItem
{
	public:

		CDirectoryListItem( const sDirEntry & file_info, CDirectoryList::eDrive drive = CDirectoryList::MAX_DRIVES );

		virtual V2				Render( V2 pos, bool highlight, float scroll_offset );

		virtual V2				GetSize() const;

		const sDirEntry *		GetFileInfo() const;

		bool					Mark( bool mark );
		bool					IsMarked() const;

		u32						GetWLANConnectionNumber() const;
		void					SetWLANConnectionNumber( u32 wlan_number );

		static int				Compare( const void * arg1, const void * arg2 );

	protected:

		sDirEntry				m_FileInfo;
		CSkinTexture *			m_pIcon;
		bool					m_bMarked;
		u32						m_nWLANConnection;
		CDirectoryList::eDrive	m_eDrive;
};

//**********************************************************************************
//
//**********************************************************************************
CDirectoryListItem::CDirectoryListItem( const sDirEntry & file_info, CDirectoryList::eDrive drive )
:	m_FileInfo( file_info )
,	m_pIcon( NULL )
,	m_bMarked( false )
,	m_nWLANConnection( 0 )
,	m_eDrive( drive )
{
	const sFileHandlerInfo * const	p_info( CFileHandler::FindHandler( file_info.m_szFileName ) );

	if ( p_info == NULL )
	{
		if ( file_info.IsDrive() == true )
		{
			m_pIcon = CSkinManager::GetComponent( CSkinManager::SC_ICON_DRIVE );
		}
		else if ( file_info.IsDirectory() == true )
		{
			m_pIcon = CSkinManager::GetComponent( CSkinManager::SC_ICON_FOLDER );
		}
		else if ( file_info.IsBackButton() == true )
		{
			m_pIcon = CSkinManager::GetComponent( CSkinManager::SC_ICON_BACK );
		}
		else if ( file_info.IsWLAN() == true )
		{
			m_pIcon = CSkinManager::GetComponent( CSkinManager::SC_ICON_WLAN );
		}
		else
		{
			m_pIcon = CSkinManager::GetComponent( CSkinManager::SC_ICON_FILE );
		}
	}
	else
	{
		m_pIcon = CSkinManager::GetComponent( p_info->m_InformationCallback( file_info.m_szFileName ).m_Icon );
	}
}

//**********************************************************************************
//
//**********************************************************************************
V2	CDirectoryListItem::Render( V2 ret_pos, bool highlight, float scroll_offset )
{
	V2			pos( ret_pos );
	float		y_step( 0.f );
	ARGB		color( m_pParent->GetColor() );
	float		icon_width( 0.f );
	float		icon_height( 0.f );
	const float	scale( GetParent()->GetScale() );
	const V2	text_size( CFont::GetDefaultFont()->GetStringSize( m_FileInfo.m_szFileName ) * scale );
	CString		file_size_text( "" );

	//
	//	Get the size of the file as a string
	//
	if ( m_FileInfo.IsFile() == true )
	{
		const CString	full_filename( CFileAssistant::Get()->GetFocusList()->GetCurrentPath() + m_FileInfo.m_szFileName );

		file_size_text = CFileSystem::GetSizeString( m_FileInfo.m_Stats.st_size );
	}
	else if ( m_FileInfo.IsDrive() == true )
	{
		const s32	free_space( CFileSystem::GetFreeSpace( CDirectoryList::GetDriveInfo( m_eDrive, CDirectoryList::DRIVE_ID ) ) );

		if ( free_space >= 0 )
		{
			file_size_text = "Free Space ";
			file_size_text += CString().Printf( "%.0fmb", static_cast< float >( free_space ) / ( 1024 * 1024 ) );
		}
	}
	else if ( m_FileInfo.IsWLAN() == true )
	{
		if ( CFTPClient::Get()->IsWLANEnabled() == false )
		{
			color.a /= 2;
			color.r /= 2;
			color.g /= 2;
			color.b /= 2;
		}
	}

	//
	//	Draw the icon
	//
	if ( m_pIcon->GetTexture() != NULL )
	{
		icon_width = m_pIcon->GetTexture()->m_nWidth * 0.5f * scale;
		icon_height = m_pIcon->GetTexture()->m_nHeight * 0.5f * scale;

		CGfx::DrawQuad( m_pIcon->GetTexture(), pos, V2( icon_width, icon_height ), color );

		pos.x += icon_width + ( 2.f * scale );
		y_step = icon_height + ( 1.f * scale );
	}

	if ( text_size.y > y_step )
	{
		y_step = text_size.y;
	}

	pos.y += 0.5f * ( y_step - text_size.y );

	//
	//	Draw the file marker
	//
	const V2	file_size_text_size( CFont::GetDefaultFont()->GetStringSize( file_size_text ) * scale );
	const V2	clip_region( m_pParent->GetSize().x - icon_width - file_size_text_size.x - ( 6.f * scale ), y_step );

	if ( m_bMarked == true )
	{
		ARGB	file_mark_color;

		if ( m_pParent->HasFocus() == true )
		{
			file_mark_color = CSkinManager::GetColor( "directory_list", "file_mark_color_on", 0x80404060 );
		}
		else
		{
			file_mark_color = CSkinManager::GetColor( "directory_list", "file_mark_color_off", 0x80404060 );
		}

		if ( text_size.x > clip_region.x )
		{
			CGfx::DrawQuad( V2( pos.x, ret_pos.y ), V2( clip_region.x, y_step ), file_mark_color );
		}
		else
		{
			CGfx::DrawQuad( V2( pos.x, ret_pos.y ), V2( text_size.x, y_step ), file_mark_color );
		}
	}

	//
	//	Draw the file size text
	//
	CFont::GetDefaultFont()->Print( file_size_text, V2( ret_pos.x + m_pParent->GetSize().x - file_size_text_size.x, pos.y ), color, scale );

	//
	//	Draw the clipped filename
	//
	CGfx::SetClipRegion( pos, clip_region );

	CFont::GetDefaultFont()->Print( m_FileInfo.m_szFileName, pos + V2( scroll_offset, 0.f ), color, scale );

	if ( scroll_offset != 0.f )
	{
		CFont::GetDefaultFont()->Print( m_FileInfo.m_szFileName, pos + V2( GetSize().x + scroll_offset, 0.f ), color, scale );
	}

	CGfx::DisableClipRegions();

	ret_pos.y += y_step + ( 1.f * scale );

	return ret_pos;
}

//**********************************************************************************
//
//**********************************************************************************
V2	CDirectoryListItem::GetSize() const
{
	V2	size( CFont::GetDefaultFont()->GetStringSize( m_FileInfo.m_szFileName ) );

	if ( m_pIcon->GetTexture() != NULL )
	{
		size.x += m_pIcon->GetTexture()->m_nWidth * 0.5f;

		if ( m_pIcon->GetTexture()->m_nHeight * 0.5f > size.y )
		{
			size.y = m_pIcon->GetTexture()->m_nHeight * 0.5f;
		}
	}

	return ( size * GetParent()->GetScale() );
}

//**********************************************************************************
//
//**********************************************************************************
const sDirEntry *	CDirectoryListItem::GetFileInfo() const
{
	return &m_FileInfo;
}

//**********************************************************************************
//
//**********************************************************************************
int	CDirectoryListItem::Compare( const void * arg1, const void * arg2 )
{
	const sDirEntry * const	p_l_file_info( ( *( CDirectoryListItem ** )( arg1 ) )->GetFileInfo() );
	const sDirEntry * const	p_r_file_info( ( *( CDirectoryListItem ** )( arg2 ) )->GetFileInfo() );

	if ( p_l_file_info->IsBackButton() == true )
	{
		return -1;
	}

	if ( p_r_file_info->IsBackButton() == true )
	{
		return +1;
	}

	if ( p_l_file_info->IsDirectory() == true && p_r_file_info->IsDirectory() == false )
	{
		return -1;
	}

	if ( p_l_file_info->IsDirectory() == false && p_r_file_info->IsDirectory() == true )
	{
		return +1;
	}

	if ( p_l_file_info->m_szFileName > p_r_file_info->m_szFileName )
	{
		return +1;
	}
	else if ( p_l_file_info->m_szFileName < p_r_file_info->m_szFileName )
	{
		return -1;
	}

	return 0;
}

//**********************************************************************************
//
//**********************************************************************************
bool	CDirectoryListItem::Mark( bool marked )
{
	if ( m_FileInfo.IsFile() == true || m_FileInfo.IsDirectory() == true )
	{
		m_bMarked = marked;

		return true;
	}

	return false;
}

//**********************************************************************************
//
//**********************************************************************************
bool	CDirectoryListItem::IsMarked() const
{
	return m_bMarked;
}

//**********************************************************************************
//
//**********************************************************************************
void	CDirectoryListItem::SetWLANConnectionNumber( u32 wlan_number )
{
	m_nWLANConnection = wlan_number;
}

//**********************************************************************************
//
//**********************************************************************************
u32	CDirectoryListItem::GetWLANConnectionNumber() const
{
	return m_nWLANConnection;
}


//**********************************************************************************
//**********************************************************************************
//
//	Directory list item implementation
//
//**********************************************************************************
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
CDirectoryList::CDirectoryList()
:	m_pTable( new CWindowTable( false ) )
,	m_szDirName( "" )
,	m_bFTPList( false )
{

	m_pTable->SetPos( V2( WINDOW_BORDER, WINDOW_BORDER ) );
	m_pTable->SetSize( V2( 0.f, 0.f ) );

	AddItem( m_pTable );
}

//**********************************************************************************
//
//**********************************************************************************
CDirectoryList::~CDirectoryList()
{
	SAFE_DELETE( m_pTable );
}

//**********************************************************************************
//
//**********************************************************************************
void	CDirectoryList::SetSize( const V2 & size )
{
	CWindow::SetSize( size );

	m_pTable->SetSize( V2( size.x - ( 2.f * WINDOW_BORDER ), size.y - ( 2.f * WINDOW_BORDER ) ) );
}

//**********************************************************************************
//
//**********************************************************************************
void	CDirectoryList::SetDrive( eDrive drive )
{
	if ( m_bFTPList == true )
	{
		//
		//	If we're moving back to the drive list, then stop any FTP connections
		//
		m_bFTPList = false;

		CFTPClient::Get()->Disconnect();
	}

	m_pTable->ClearItems();

	if ( drive == ALL_DRIVES )
	{
		sDirEntry	dir_entry;

		m_szDirName = s_szDriveNames[ ALL_DRIVES ][ DRIVE_NAME ];

		for ( u32 i = DRIVE_FLASH0; i < MAX_DRIVES; ++i )
		{
			//
			//	Check for a UMD disc
			//
			if ( i == DRIVE_UMD )
			{
				if ( CFileSystem::IsUMDInserted() == false )
				{
					break;
				}
			}

			dir_entry.m_szFileName = s_szDriveNames[ i ][ DRIVE_NAME ];
			dir_entry.m_Stats.st_mode = AF_DRIVE;

			CDirectoryListItem * const	p_item( new CDirectoryListItem( dir_entry, static_cast< eDrive >( i ) ) );

			p_item->SetCallback( SelectionCallback );

			m_pTable->AddItem( p_item );
		}

		//
		//	Add any WLAN connections
		//
		const CWLANList &	wlan_list( CFTPClient::Get()->GetWLANList() );

		for ( u32 i = 0; i < wlan_list.size(); ++i )
		{
			dir_entry.m_szFileName = "WLAN (";
			dir_entry.m_szFileName += wlan_list[ i ].m_szName;
			dir_entry.m_szFileName += ")";
			dir_entry.m_Stats.st_mode = AF_WLAN;

			CDirectoryListItem * const	p_item( new CDirectoryListItem( dir_entry ) );

			p_item->SetCallback( FTPSelectionCallback );
			p_item->SetWLANConnectionNumber( i + 1 );

			m_pTable->AddItem( p_item );
		}

		//
		//	Sort all the drives
		//
		m_pTable->Sort( CDirectoryListItem::Compare );

		SetTitle( "My PSP" );
	}
	else
	{
		SetPath( s_szDriveNames[ drive ][ DRIVE_ID ] );
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CDirectoryList::SetPath( const CString & path )
{
	sDirEntry	dir_entry;

//	BREAK_POINT( "SetPath( %s )", path.GetPtr() );

	m_szDirName = path;

	if ( m_bFTPList == true )
	{
		if ( CFTPClient::Get()->CWD( m_szDirName ) == false )
		{
			ASSERT( 0, "CWD Failed!" );
		}
	}

	RefreshList( false );

	SetTitle( m_szDirName );
}

//**********************************************************************************
//
//**********************************************************************************
void	CDirectoryList::RefreshList( bool maintain_selection )
{
	sDirEntry			dir_entry;
	FIND_FILE_HANDLE	handle;

	m_pTable->ClearItems( maintain_selection );

	dir_entry.m_szFileName = "..";
	dir_entry.m_Stats.st_mode = AF_BACK_BUTTON;

	CDirectoryListItem * const	p_item( new CDirectoryListItem( dir_entry ) );

	p_item->SetCallback( SelectionCallback );

	m_pTable->AddItem( p_item );


	//
	//	Fill FTP directory list
	//
	if ( m_bFTPList == true )
	{
		CFTPClient * const		p_ftp( CFTPClient::Get() );
		remoteDirent * const	p_list( p_ftp->LIST() );

		if ( p_list != NULL )
		{
			for ( s32 i = 0; i < p_list->totalCount; i++ )
			{
				if ( FIO_SO_ISDIR( p_list->files[ i ].st_attr ) || FIO_SO_ISREG( p_list->files[ i ].st_attr ) )
				{
					dir_entry.m_szFileName = p_list->files[ i ].d_name;

					if ( FIO_SO_ISDIR( p_list->files[ i ].st_attr ) )
					{
						dir_entry.m_Stats.st_mode = AF_DIRECTORY;
					}
					else if ( FIO_SO_ISREG( p_list->files[ i ].st_attr ) )
					{
						dir_entry.m_Stats.st_mode = AF_ARCHIVE;
					}

					dir_entry.m_Stats.st_attr = p_list->files[ i ].st_attr;
					dir_entry.m_Stats.st_size = atoi( p_list->files[ i ].st_size );

					if ( CFileSystem::HideCorruptFiles() == false || ( CFileSystem::HideCorruptFiles() == true && dir_entry.m_szFileName.Find( "%" ) == NULL ) )
					{
						CDirectoryListItem * const	p_item( new CDirectoryListItem( dir_entry ) );

						p_item->SetCallback( SelectionCallback );

						m_pTable->AddItem( p_item );
					}
				}
			}
		}
		else
		{
			CErrorMessage( "LIST Failed!" );
		}
	}
	else
	{
		if ( CFileSystem::FindFirstFile( m_szDirName, handle ) == true )
		{
			while ( CFileSystem::FindNextFile( dir_entry, handle ) == true )
			{
				if ( dir_entry.m_szFileName != "." && dir_entry.m_szFileName != ".." )
				{
					if ( CFileSystem::HideCorruptFiles() == false || ( CFileSystem::HideCorruptFiles() == true && dir_entry.m_szFileName.Find( "%" ) == NULL ) )
					{
						CDirectoryListItem * const	p_item( new CDirectoryListItem( dir_entry ) );

						p_item->SetCallback( SelectionCallback );

						m_pTable->AddItem( p_item );
					}
				}
			}

			CFileSystem::FindCloseFile( handle );
		}
	}

	m_pTable->Sort( CDirectoryListItem::Compare );

	ClearSelectedFiles();
}

//**********************************************************************************
//
//**********************************************************************************
void	CDirectoryList::SetFocus( bool focus )
{
	m_pTable->SetFocus( focus );

	CWindow::SetFocus( focus );
}

//**********************************************************************************
//
//**********************************************************************************
void	CDirectoryList::Process()
{
	CWindow::Process();

	//
	//	Automatically refresh the directory list
	//
	if ( IsDriveList() == false && m_bFTPList == false )
	{
		if ( AnyItemsSelected() == false )
		{
			++m_nRefreshTimer;

			if ( m_nRefreshTimer == 100 )
			{
				m_nRefreshTimer = 0;

				RefreshList( true );
			}
		}
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CDirectoryList::ProcessInput()
{
	if ( HasFocus() == true )
	{
		CDirectoryListItem * const	p_item( static_cast< CDirectoryListItem * >( m_pTable->GetSelectedItem() ) );

		CHUD::SetButton( CHUD::BUTTON_START, "Options" );
		CHUD::SetButtons( "Select", "Back", "Mark", "" );

		//
		//	Information button
		//
		if ( p_item != NULL )
		{
			if ( p_item->GetFileInfo()->IsFile() == true || p_item->GetFileInfo()->IsDirectory() == true )
			{
				CHUD::SetButtons( "Select", "Back", "Mark", "Info" );

				if ( CInput::IsButtonClicked( CInput::TRIANGLE ) == true )
				{
					CInformationDialog *	p_info_dialog( new CInformationDialog( p_item->GetFileInfo() ) );

					SetFocus( false );

					p_info_dialog->Show();

					SetFocus( true );

					SAFE_DELETE( p_info_dialog );
				}
			}
		}

		//
		//	Back button
		//
		if ( CInput::IsButtonClicked( CInput::CIRCLE ) == true )
		{
			const CString	previous_dir( GetPreviousDir() );

			if ( previous_dir.IsEmpty() == true )
			{
				SetDrive( ALL_DRIVES );
			}
			else
			{
				SetPath( previous_dir );
			}
		}

		//
		//	Mark item
		//
		if ( CInput::IsButtonRepeat( CInput::SQUARE, 8 ) == true )
		{
			if ( p_item != NULL )
			{
				if ( p_item->Mark( ( p_item->IsMarked() == false ) ) == true )
				{
					m_pTable->ScrollDown();
				}
			}
		}
	}

	CWindow::ProcessInput();
}

//**********************************************************************************
//
//**********************************************************************************
void	CDirectoryList::Selection( CDirectoryListItem * const p_item )
{
	const sDirEntry * const	p_info( p_item->GetFileInfo() );

	if ( p_item->IsMarked() == false )
	{
		if ( p_info->IsDrive() == true )
		{
			CString	new_dir( p_info->m_szFileName );

			if ( new_dir == s_szDriveNames[ DRIVE_UMD ][ DRIVE_NAME ] )
			{
				if ( CFileSystem::ActivateUMD() == false )
				{
					ASSERT( 0, "Failed to activate UMD\n" );
				}
			}

			for ( u32 i = DRIVE_FLASH0; i < MAX_DRIVES; ++i )
			{
				if ( s_szDriveNames[ i ][ DRIVE_NAME ] == p_info->m_szFileName )
				{
					SetPath( s_szDriveNames[ i ][ DRIVE_ID ] );
				}
			}

			return;
		}
		else if ( p_info->IsDirectory() == true )
		{
			CString	new_dir( m_szDirName );

			new_dir += p_info->m_szFileName;
			new_dir += "/";

			SetPath( new_dir );

			return;
		}
		else if ( p_info->IsBackButton() == true )
		{
			const CString	previous_dir( GetPreviousDir() );

			if ( previous_dir.IsEmpty() == true )
			{
				SetDrive( ALL_DRIVES );
			}
			else
			{
				SetPath( previous_dir );
			}

			return;
		}
	}

	//
	//	Show the file options menu
	//
	CFileList	file_list;

	GetSelectedFiles( file_list, true );

	if ( file_list.empty() == false )
	{
		CFileOptions * const	p_file_options( new CFileOptions( file_list ) );

		p_file_options->SetFocus( true );
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CDirectoryList::SelectionCallback( CWindowTableItem * const p_table_item, u32 item_no )
{
	CDirectoryListItem * const	p_item( static_cast< CDirectoryListItem * >( p_table_item ) );

	if ( p_item != NULL )
	{
		CDirectoryList * const	p_list( static_cast< CDirectoryList * >( p_item->GetParent()->GetParent() ) );

		p_list->Selection( p_item );
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CDirectoryList::FTPSelectionCallback( CWindowTableItem * const p_table_item, u32 item_no )
{
	CDirectoryListItem * const	p_item( static_cast< CDirectoryListItem * >( p_table_item ) );

	if ( p_item != NULL )
	{
		CFTPClient * const			p_ftp( CFTPClient::Get() );

		if ( p_ftp->IsWLANEnabled() == false )
		{
			CErrorMessage( "WLAN is disabled!\nPlease re-enable and try again" );
		}
		else
		{
			CDirectoryList * const	p_list( static_cast< CDirectoryList * >( p_item->GetParent()->GetParent() ) );

			if ( p_ftp->Connect( p_item->GetWLANConnectionNumber() ) == true )
			{
				p_list->m_bFTPList = true;
				p_list->SetPath( "/" );
			}
			else
			{
				CErrorMessage( "Failed to connect!" );
			}
		}
	}
}

//**********************************************************************************
//
//**********************************************************************************
const CString &	CDirectoryList::GetDriveInfo( eDrive drive, eDriveInfo info )
{
	return s_szDriveNames[ drive ][ info ];
}

//**********************************************************************************
//
//**********************************************************************************
const CString &	CDirectoryList::GetCurrentPath() const
{
	return m_szDirName;
}

//**********************************************************************************
//
//**********************************************************************************
CString	CDirectoryList::GetFullPathFileName( const CString & file_name )
{
	CString	path;

	path += m_szDirName;
	path += file_name;

	return path;
}

//**********************************************************************************
//
//**********************************************************************************
bool	CDirectoryList::AnyItemsSelected()
{
	CTableItemList &	item_list( m_pTable->GetItemList() );

	for ( CTableItemList::iterator it = item_list.begin(); it != item_list.end(); ++it )
	{
		CDirectoryListItem * const	p_item( static_cast< CDirectoryListItem * >( ( * it ) ) );

		if ( p_item->IsMarked() == true )
		{
			return true;
		}
	}

	return false;
}

//**********************************************************************************
//
//**********************************************************************************
void	CDirectoryList::GetSelectedFiles( CFileList & list, bool full_path )
{
	CTableItemList &		item_list( m_pTable->GetItemList() );
	CDirectoryListItem *	p_selected_item( static_cast< CDirectoryListItem * >( m_pTable->GetSelectedItem() ) );

	list.clear();

	for ( CTableItemList::iterator it = item_list.begin(); it != item_list.end(); ++it )
	{
		CDirectoryListItem * const	p_item( static_cast< CDirectoryListItem * >( ( * it ) ) );

		if ( p_item->IsMarked() == true )
		{
			if ( p_item == p_selected_item )
			{
				p_selected_item = NULL;
			}

			list.push_back( *p_item->GetFileInfo() );

			if ( full_path == true )
			{
				list.back().m_szFileName = GetFullPathFileName( list.back().m_szFileName );
			}
		}
	}

	if ( p_selected_item != NULL )
	{
		if ( p_selected_item->GetFileInfo()->IsFile() == true )
		{
			list.push_back( *p_selected_item->GetFileInfo() );

			if ( full_path == true )
			{
				list.back().m_szFileName = GetFullPathFileName( list.back().m_szFileName );
			}
		}
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CDirectoryList::ClearSelectedFiles()
{
	CTableItemList &	item_list( m_pTable->GetItemList() );

	for ( CTableItemList::iterator it = item_list.begin(); it != item_list.end(); ++it )
	{
		CDirectoryListItem * const	p_item( static_cast< CDirectoryListItem * >( ( * it ) ) );

		p_item->Mark( false );
	}
}

//**********************************************************************************
//
//**********************************************************************************
bool	CDirectoryList::ReadOnly() const
{
	if ( m_szDirName.Find( s_szDriveNames[ ALL_DRIVES ][ DRIVE_ID ] ) != NULL )
	{
		return true;
	}

	if ( m_szDirName.Find( s_szDriveNames[ ALL_DRIVES ][ DRIVE_NAME ] ) != NULL )
	{
		return true;
	}

	if ( m_szDirName.Find( s_szDriveNames[ DRIVE_UMD ][ DRIVE_ID ] ) != NULL )
	{
		return true;
	}

	if ( m_szDirName.Find( s_szDriveNames[ DRIVE_FLASH0 ][ DRIVE_ID ] ) != NULL )
	{
		return true;
	}

	if ( m_szDirName.Find( s_szDriveNames[ DRIVE_FLASH1 ][ DRIVE_ID ] ) != NULL )
	{
		return true;
	}

	return false;
}

//**********************************************************************************
//	Strips off the trailing slashes until it reachs the root drive
//**********************************************************************************
CString	CDirectoryList::GetPreviousDir()
{
	if ( m_bFTPList == true )
	{
		CString	dir( m_szDirName );
		u32		dir_length( dir.Length() - 2 );

		while ( dir_length > 0 && dir[ dir_length ] != '/' )
		{
			--dir_length;
		}

		if ( dir_length != 0 )
		{
			char *	p_slash( const_cast< char * >( dir.GetPtr() ) );

			p_slash[ dir_length + 1 ] = '\0';
		}

//		BREAK_POINT( "dir = %s, m_szDirName = %s", dir.GetPtr(), m_szDirName.GetPtr() );

		if ( dir != m_szDirName )
		{
			return dir;
		}
		else
		{
			return CString( "" );
		}
	}
	else
	{
		CString	dir( m_szDirName );
		u32		dir_length( dir.Length() - 2 );

		while ( dir_length > 0 && dir[ dir_length ] != '/' )
		{
			--dir_length;
		}

		if ( dir_length != 0 )
		{
			char *	p_slash( const_cast< char * >( dir.GetPtr() ) );

			p_slash[ dir_length + 1 ] = '\0';
		}

		if ( dir != m_szDirName )
		{
			return dir;
		}
		else
		{
			return CString( "" );
		}
	}
}

//**********************************************************************************
//
//**********************************************************************************
bool	CDirectoryList::IsDriveList() const
{
	if ( m_szDirName == s_szDriveNames[ ALL_DRIVES ][ DRIVE_ID ] || m_szDirName == s_szDriveNames[ ALL_DRIVES ][ DRIVE_NAME ] )
	{
		return true;
	}

	return false;
}

//**********************************************************************************
//	
//**********************************************************************************
bool	CDirectoryList::IsFTPList() const
{
	return m_bFTPList;
}

//*******************************  END OF FILE  ************************************
