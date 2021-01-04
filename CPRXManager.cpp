/***********************************************************************************

  Module :	CPRXManager.cpp

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
#include "CPRXManager.h"
#include "CFileSystem.h"
#include "CFrameWork.h"
#include "TinyXML.h"
#include "CMusicPlugin.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************
static const CString	s_szPRXPath( "Data/Plugins/" );
static const CString	s_szConfigFile( "Data/Config.xml" );
static const CString	s_szPluginTypes[ CPRXPlugin::MAX_PLUGIN_TYPES ] =
{
	"music",		// MUSIC_PLUGIN,
	"image",		// IMAGE_PLUGIN,
	"input",		// INPUT_PLUGIN,
	"external",		// EXTERNAL_PLUGIN,
};

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
//**********************************************************************************
//
//   Base PRX plugin class definition
//
//**********************************************************************************
//**********************************************************************************
CPRXPlugin::CPRXPlugin( const CString & filename, const CString & extension )
:	m_ModuleID( -1 )
,	m_szFilename( filename )
,	m_szExtension( extension )
{
}

//**********************************************************************************
//
//**********************************************************************************
CPRXPlugin::~CPRXPlugin()
{
}

//**********************************************************************************
//
//**********************************************************************************
bool	CPRXPlugin::Load()
{
	const CString	full_file( s_szPRXPath + m_szFilename );
	const CString	full_path( CFileSystem::MakeFullPath( full_file ) );

	m_ModuleID = CFrameWork::LoadModule( full_path, CFrameWork::KERNEL_PARTITION );

	if ( m_ModuleID < 0 )
	{
		BREAK_POINT( "Failed to load the module!" );

		return false;
	}

	return true;
}

//**********************************************************************************
//
//**********************************************************************************
void	CPRXPlugin::Unload()
{
	if ( m_ModuleID > 0 )
	{
		int	status;

		if ( sceKernelStopModule( m_ModuleID, 0, NULL, &status, NULL ) >= 0 )
		{
			sceKernelUnloadModule( m_ModuleID );
		}
	}
}

//**********************************************************************************
//
//**********************************************************************************
const CString &	CPRXPlugin::GetExtension() const
{
	return m_szExtension;
}


//**********************************************************************************
//**********************************************************************************
//
//   PRX Manager class implementation
//
//**********************************************************************************
//**********************************************************************************
CPRXManager::CPRXList	CPRXManager::s_PRXList;

//**********************************************************************************
//
//**********************************************************************************
void	CPRXManager::Open()
{
	s_PRXList.clear();

	TiXmlDocument *	p_document( new TiXmlDocument() );

	ASSERT( p_document != NULL, "Failed to create the XML document" );

	//
	//	Parse the plugin configuration
	//
	if ( p_document != NULL )
	{
		if ( p_document->LoadFile( s_szConfigFile ) == true )
		{
			TiXmlHandle	doc_handle( p_document );
			TiXmlHandle	plugins_handle( doc_handle.FirstChild( "FileAssistant" ).FirstChild( "plugins" ) );

			//
			//	Iterate through the different categories of plugin
			//
			for ( u32 i = 0; i < CPRXPlugin::MAX_PLUGIN_TYPES; ++i )
			{
				u32					child( 0 );
				CPRXPlugin::eType	plugin_type( static_cast< CPRXPlugin::eType >( i ) );
				TiXmlHandle			plugin_handle( plugins_handle.FirstChild( s_szPluginTypes[ i ] ) );
				TiXmlElement *		p_plugin( plugin_handle.Child( "plugin", child ).Element() );

				while ( p_plugin != NULL )
				{
					if ( p_plugin->Attribute( "name" ) != NULL )
					{
						if ( p_plugin->Attribute( "prx" ) != NULL )
						{
							if ( p_plugin->Attribute( "extension" ) != NULL )
							{
								CPRXManager::RegisterPlugin( plugin_type, p_plugin->Attribute( "name" ), p_plugin->Attribute( "prx" ), p_plugin->Attribute( "extension" ) );
							}
						}
					}

					++child;
					p_plugin = plugin_handle.Child( "plugin", child ).Element();
				}
			}
		}
		else
		{
			TRACE( "%s\n", p_document->ErrorDesc() );

			ASSERT( 0, "Failed to load config file!" );
		}

		SAFE_DELETE( p_document );
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CPRXManager::Close()
{
	while ( s_PRXList.empty() == false )
	{
		CPRXPlugin *	p_plugin( s_PRXList.back() );

		s_PRXList.pop_back();

		p_plugin->Unload();

		SAFE_DELETE( p_plugin );
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CPRXManager::RegisterPlugin( CPRXPlugin::eType type, const CString & name, const CString & filename, const CString & extension )
{
	CPRXPlugin *	p_plugin( NULL );

	switch ( type )
	{
	case CPRXPlugin::MUSIC_PLUGIN:
		{
			p_plugin = new CMusicPlugin( filename, extension );
		}
		break;

	case CPRXPlugin::IMAGE_PLUGIN:
		{
//			p_plugin = new CPRXPlugin( filename, extension );
		}
		break;

	case CPRXPlugin::INPUT_PLUGIN:
		{
//			p_plugin = new CPRXPlugin( filename, extension );
		}
		break;

	case CPRXPlugin::EXTERNAL_PLUGIN:
		{
//			p_plugin = new CPRXPlugin( filename, extension );
		}
		break;

	default:
		{
		}
		break;
	}

	if ( p_plugin != NULL )
	{
		if ( p_plugin->Load() == true )
		{
			s_PRXList.push_back( p_plugin );
		}
		else
		{
			SAFE_DELETE( p_plugin );
		}
	}
}

//**********************************************************************************
//
//**********************************************************************************
const CPRXPlugin *	CPRXManager::FindPlugin( const CString & extension )
{
	const CString	plugin_extension( CFileSystem::GetFileExtension( extension ) );

	for ( CPRXList::iterator it = s_PRXList.begin(); it != s_PRXList.end(); ++it )
	{
		const CPRXPlugin * const	p_plugin( *it );

		if ( plugin_extension.IEquals( p_plugin->GetExtension() ) == true )
		{
			return p_plugin;
		}
	}

	return NULL;
}


//*******************************  END OF FILE  ************************************
