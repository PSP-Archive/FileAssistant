/***********************************************************************************

  Module :	CMusicPlugin.cpp

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
#include "CMusicPlugin.h"
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
static CString			s_szFilePlaying( "" );
static const CString	s_szMenuText( "Play" );
static bool				s_bMusicIsPlaying( false );
static bool				s_bMusicIsPaused( false );

//**********************************************************************************
//   Class Definition
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
CMusicPlugin::CMusicPlugin( const CString & filename, const CString & extension )
:	CPRXPlugin( filename, extension )
,	m_pInitFunc( NULL )
,	m_pLoadFunc( NULL )
,	m_pPlayFunc( NULL )
,	m_pPauseFunc( NULL )
,	m_pStopFunc( NULL )
,	m_pEndFunc( NULL )
{
	s_szFilePlaying = "";
}

//**********************************************************************************
//
//**********************************************************************************
const CPRXPlugin::eType	CMusicPlugin::GetType() const
{
	return MUSIC_PLUGIN;
}

//**********************************************************************************
//
//**********************************************************************************
bool	CMusicPlugin::Load()
{
	if ( CPRXPlugin::Load() == true )
	{
		s32		fd;
		u32		arg[ 6 ] = { 6, 0, 0, 0, 0, 0 };
		u32 *	p_args( arg );

		if ( sceKernelStartModule( m_ModuleID, sizeof( arg ), &p_args, &fd, NULL ) >= 0 )
		{
			m_pInitFunc = ( fd_initFunc )*p_args++;
			m_pLoadFunc = ( fd_loadFunc )*p_args++;
			m_pPlayFunc = ( fd_playFunc )*p_args++;
			m_pPauseFunc = ( fd_pauseFunc )*p_args++;
			m_pStopFunc = ( fd_stopFunc )*p_args++;
			m_pEndFunc = ( fd_endFunc )*p_args++;

			return true;
		}
		else
		{
			ASSERT( 0, "Failed to start the module!" );
		}
	}
	else
	{
		ASSERT( 0, "Failed to load the module!" );
	}

	return false;
}

//**********************************************************************************
//
//**********************************************************************************
void	CMusicPlugin::Unload()
{
	CPRXPlugin::Unload();
}

//**********************************************************************************
//
//**********************************************************************************
bool	CMusicPlugin::Play( const CString & filename ) const
{
	m_pInitFunc( 0 );

	if ( m_pLoadFunc( const_cast< char * >( filename.GetPtr() ) ) )
	{
		s_bMusicIsPlaying = true;

		m_pPlayFunc();
	}
	else
	{
		s_bMusicIsPlaying = false;

		m_pStopFunc();
		m_pEndFunc();

		BREAK_POINT( "Cannot replay file..." );

		// Cannot replay file...
		return false;
	}

	// ok !
	s_bMusicIsPaused = false;

	return true;
}

//**********************************************************************************
//
//**********************************************************************************
void	CMusicPlugin::Stop()
{
	if ( s_bMusicIsPlaying == true )
	{
		m_pStopFunc();
		m_pEndFunc();

		s_szFilePlaying = "";
		s_bMusicIsPlaying = false;
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CMusicPlugin::Pause()
{
	if ( s_bMusicIsPlaying == true )
	{
		m_pPauseFunc();

		s_bMusicIsPaused = !s_bMusicIsPaused;
	}
}

//**********************************************************************************
//
//**********************************************************************************
const CString &	CMusicPlugin::GetMenuText() const
{
	return s_szMenuText;
}

//*******************************  END OF FILE  ************************************
