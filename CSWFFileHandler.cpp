/***********************************************************************************

  Module :	CSWFFileHandler.cpp

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
#include "CSWFFileHandler.h"
#include "CRenderable.h"
#include "CHUD.h"
#include "CInput.h"
#include "CFileSystem.h"
#include "CProcess.h"
#include "CFrameWork.h"
#include "CMessageBox.h"
#include "gameswf.h"
//#include "container.h"
#include "tu_file.h"
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
static float	s_SWFScale = 1.0f;
static bool		s_verbose = true;
static bool		s_background = true;

//**********************************************************************************
//   Global Variables
//**********************************************************************************

//**********************************************************************************
//   Static Variables
//**********************************************************************************
static const sFileExtensionInfo	s_FileExtensionInfo =
{
	"View",
	CSkinManager::SC_ICON_MOVIE
};

//**********************************************************************************
//   Class Definition
//**********************************************************************************

//**********************************************************************************
//	Process a log message
//**********************************************************************************
static void	MessageLog( const char * const p_message )
{
	if ( s_verbose == true )
	{
		BREAK_POINT( p_message );
	}
}

//**********************************************************************************
//	Error callback for handling gameswf messages.
//**********************************************************************************
static void	LogCallback( bool error, const char * const p_message )
{
	BREAK_POINT( p_message );
}

//**********************************************************************************
//	Callback function.  This opens files for the gameswf library.
//**********************************************************************************
static tu_file *	FileOpener( const char * const p_url )
{
	return new tu_file( p_url, "rb" );
}

//**********************************************************************************
//	For handling notification callbacks from ActionScript.
//**********************************************************************************
static void	FSCallback( gameswf::movie_interface * movie, const char * command, const char * args )
{
	MessageLog( "fs_callback: '" );
	MessageLog( command );
	MessageLog( "' '" );
	MessageLog( args );
	MessageLog( "'\n" );
}

//**********************************************************************************
//
//**********************************************************************************
bool	CSWFFileHandler::Execute( const CString & file )
{
//	BREAK_POINT( "SWF Execute : %s", file.GetPtr() );

	gameswf::register_file_opener_callback( FileOpener );
	gameswf::register_fscommand_callback( FSCallback );
	gameswf::register_log_callback( LogCallback );

	gameswf::sound_handler *	p_sound( gameswf::create_sound_handler_psp() );
	gameswf::render_handler *	p_render( gameswf::create_render_handler_psp() );

	gameswf::set_sound_handler( p_sound );
	gameswf::set_render_handler( p_render );

	// Get info about the width & height of the movie.
	int	movie_version( 0 ), movie_width( 0 ), movie_height( 0 );

	gameswf::get_movie_info( file, &movie_version, &movie_width, &movie_height, NULL, NULL );

	if ( movie_version == 0 )
	{
		CErrorMessage( CString().Printf( "Can't get info about '%s'", file.GetPtr() ) );

		return false;
	}

	const float	div_width( static_cast< float >( movie_width ) / CGfx::s_ScreenWidth );
	const float	div_height( static_cast< float >( movie_height ) / CGfx::s_ScreenHeight );

	if ( div_width > div_height )
	{
		s_SWFScale = 1.f / div_width;
	}
	else
	{
		s_SWFScale = 1.f / div_height;
	}

	const int	width = static_cast< int >( static_cast< float >( movie_width ) * s_SWFScale );
	const int	height = static_cast< int >( static_cast< float >( movie_height ) * s_SWFScale );

//	BREAK_POINT( "width = %d, height = %d, scale = %.2f", width, height, s_SWFScale );

	// Load the actual movie.
	gameswf::movie_definition * const	p_movie_definition( gameswf::create_movie( file ) );

	if ( p_movie_definition == NULL )
	{
		CErrorMessage( CString().Printf( "Can't create a movie from '%s'", file.GetPtr() ) );

		return false;
	}

	gameswf::movie_interface * const	p_movie_instance( p_movie_definition->create_instance() );

	if ( p_movie_instance == NULL )
	{
		CErrorMessage( "Can't create movie instance" );

		return false;
	}

	// Mouse state.
	int		mouse_x = 0;
	int		mouse_y = 0;
	int		mouse_buttons = 0;
	bool	paused = false;
	float	speed_scale = 1.0f;
	u64		last_ticks;
	int		frame_counter = 0;

	sceRtcGetCurrentTick( &last_ticks );

//	BREAK_POINT( "Loaded!" );

	while ( CInput::IsButtonDown( CInput::CIRCLE ) == false )
	{
		u64	ticks;

		sceRtcGetCurrentTick( &ticks );

		const int	delta_ticks( ticks - last_ticks );
		const float	delta_t( paused == true ? 0.f : delta_ticks / 1000.f );

		last_ticks = ticks;

		p_movie_instance->set_display_viewport( 0, 0, width, height );
		p_movie_instance->set_background_alpha( s_background ? 1.0f : 0.05f );
		p_movie_instance->notify_mouse_state( mouse_x, mouse_y, mouse_buttons );
		p_movie_instance->advance( delta_t * speed_scale );
		p_movie_instance->display();

		++frame_counter;

		CInput::Process();
	}

	if ( p_movie_definition != NULL )
	{
		p_movie_definition->drop_ref();
	}

	if ( p_movie_instance != NULL )
	{
		p_movie_instance->drop_ref();
	}

	delete p_sound;
	delete p_render;

	// Clean up gameswf as much as possible, so valgrind will help find actual leaks.
	gameswf::clear();

	return true;
}

//**********************************************************************************
//
//**********************************************************************************
const sFileExtensionInfo &	CSWFFileHandler::Information( const CString & file )
{
	return s_FileExtensionInfo;
}

//**********************************************************************************
//
//**********************************************************************************
float	CSWFFileHandler::GetScale()
{
	static float	SCALE( 0.05f );

	return s_SWFScale * SCALE;
}

//*******************************  END OF FILE  ************************************
