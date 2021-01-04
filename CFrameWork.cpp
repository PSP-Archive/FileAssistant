/***********************************************************************************

  Module :	CFrameWork.cpp

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
#include "CGfx.h"
#include "CUSBManager.h"
#include "CInput.h"
#include "CFileSystem.h"
#include "CProcess.h"
#include "CFont.h"
#include "CSkinManager.h"
#include "CExecutableFileHandler.h"
#include "CFileHandler.h"
#include "CMusicFileHandler.h"
#include "CBackground.h"
#include "CRenderable.h"
#include "CWindow.h"
#include "CConfigFile.h"
#include "CPRXManager.h"
#include "CLanguage.h"
#include "CFTPClient.h"
#include "CMessageBox.h"
#include "CTextureManager.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************
asm( ".global __lib_stub_top" );
asm( ".global __lib_stub_bottom" );

static const CString	s_szPluginsPath( "Data/Plugins" );
static const CString 	s_szELF_Filename( "BOOTLOADER.ELF" );
static const CString 	s_szPRX_Filename( "ELF_LOADER.PRX" );
static const CString 	s_szUMD_Filename( "UMD_LOADER.PRX" );

#define BOOTLOADER_NAME "FileAssistant"

//**********************************************************************************
//   Static Prototypes
//**********************************************************************************
PSP_HEAP_SIZE_KB( 10000 );
PSP_MODULE_INFO( BOOTLOADER_NAME, 0x1000, 1, 1 );
PSP_MAIN_THREAD_ATTR( 0 );

//**********************************************************************************
//   Global Variables
//**********************************************************************************

//**********************************************************************************
//   Static Variables
//**********************************************************************************
u64						CFrameWork::s_Time( 0 );
u64						CFrameWork::s_LastTime( 0 );
u64						CFrameWork::s_TickResolution( 0 );

static u32				s_nVersion;
static CVarString		CVAR_USB_ON( "usb", "on" );

static const CString	FA_FILENAME( "FileAssistant.elf" );
static char				FA_initialPath[ MAXPATHLEN ];

static const u8			s_BombGfx[ 32 ][ 32 ] =
{
	{ 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0 },
	{ 0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0 },
	{ 0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0 },
	{ 0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0 },
	{ 0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0 },
	{ 0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0 },
	{ 0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0 },
	{ 0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0 },
	{ 0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0 },
	{ 0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0 },
};

//**********************************************************************************
//   Class Definition
//**********************************************************************************

//*************************************************************************************
//
//*************************************************************************************
static void	SetKernelPatch()
{
	//Patch PSP OS
	s_nVersion = *( ( u32 * )0x8805c510 );		// Save the real version number

	if ( CFrameWork::GetVersion() == CFrameWork::PSP_v1_5 )
	{
		// Firmware v1.5
		*((unsigned int *)0x8805c510)=0x00001021; // good PRX
		*((unsigned int *)0x8805c834)=0x00000000; // good VSH
		*((unsigned int *)0x8805c690)=0x00000000; // VSHByID
		*((unsigned int *)0x8805c3b8)=0x00000000; // modByID
		*((unsigned int *)0x8805c834)=0x00000000; // VSH
		*((unsigned int *)0x8805ca40)=0x00000000; // VSHPlain

		//that will write a j MyFunction to AddressOfFunction
		//u32 data;
		//data = (u32) MyFunction & 0xFFFFFFF;
		//data = 0x8000000 | (data >> 2);
		//_sw(data, AddressOfFunction);

		//Patch sceExitGame
		//*((unsigned int *)0x8806882c)=0x03e00008; // disable sceExitGame with jr $ra
		//*((unsigned int *)0x88068830)=0x00001021; // addu v0, zero, zero
#ifdef DEBUG
		printf( "\tKernel patched : \n\t\tPRX loadmodule,\n\t\tsceExitGame\n" );
		sceKernelDelayThread(1000000);
#endif
	}
	else
	{
#ifdef DEBUG
		printf( "Bad OS version, 1.5 required, no patch\n" );
		sceKernelDelayThread(1000000);
#endif
	}
}

//*************************************************************************************
//	Framework exit point
//*************************************************************************************
static int	ExitCallback( int arg1, int arg2, void * common )
{
	CFrameWork::Close();

	sceKernelExitGame();

	return 0;
}

//*************************************************************************************
//
//*************************************************************************************
static void	HomeButtonCallback()
{
	static bool	s_bShowing( false );

	if ( s_bShowing == false )
	{
		s_bShowing = true;

		CModalMessageBox	box( "Exit", "Do you want to\nexit FileAssistant?" );

		box.AddExitCode( CModalMessageBox::EXIT_CROSS, "Yes" );
		box.AddExitCode( CModalMessageBox::EXIT_CIRCLE, "No" );
		box.AddExitCode( CModalMessageBox::EXIT_HOME, "No" );

		if ( box.Show() == CModalMessageBox::EXIT_CROSS )
		{
			ExitCallback( 0, 0, NULL );
		}

		s_bShowing = false;
	}
}

//**********************************************************************************
//
//**********************************************************************************
static void	ExceptionHandler( PspDebugRegBlock * p_regs )
{
	bool	flash( false );
	bool	bombs( rand() & 1 );
	u32 *	pg_vramtopu32 = ( u32 * )( 0x04000000 + 0x40000000 );

	pspDebugScreenInit();

	if ( bombs == true )
	{
		pspDebugScreenSetBackColor( 0xffffffff );
		pspDebugScreenSetTextColor( 0xff000000 );
	}
	else
	{
		pspDebugScreenSetBackColor( 0x00000000 );
		pspDebugScreenSetTextColor( 0xff0000ff );
	}

	pspDebugScreenClear();

	TRACE( "\n\n\n\n\n\n\n\n\n");
	TRACE( "Exception Details:\n" );
	pspDebugDumpException( p_regs );
	TRACE( "\n\n\n\n\n\t\tPRESS X TO REBOOT FA++ !!!\n" );

	while ( CInput::IsButtonDown( CInput::CROSS ) == false )
	{
		CInput::Process();

		if ( bombs == true )
		{
			for ( u32 i = 0; i < 4; ++i )
			{
				for ( u32 y = 0; y < 32; ++y )
				{
					for ( u32 x = 0; x < 32; ++x )
					{
						if ( s_BombGfx[ y ][ x ] == 1 )
						{
							*( pg_vramtopu32 + ( y * 512 ) + x + ( i * 32 ) ) = 0x00000000;
						}
					}
				}
			}

			sceDisplayWaitVblankStart();
		}
		else
		{
			pspDebugScreenSetXY( 0, 0 );

			if ( flash == true )
			{
				pspDebugScreenSetTextColor( 0xff0000ff );
			}
			else
			{
				pspDebugScreenSetTextColor( 0xff000000 );
			}

			TRACE( "*******************************************************************\n");
			TRACE( "*                                                                 *\n");
			TRACE( "*                                                                 *\n");
			TRACE( "*                                                                 *\n");
			TRACE( "*******************************************************************\n");

			pspDebugScreenSetTextColor( 0xff0000ff );
			pspDebugScreenSetXY( 27, 2 );
			TRACE( "Guru Meditation");

			//
			//	Pause for some time
			//
			for ( u32 i = 0; ( i < 30 ) && ( CInput::IsButtonDown( CInput::CROSS ) == false ); ++i )
			{
				CInput::Process();
				sceDisplayWaitVblankStart();
			}
		}

		flash = ( flash == false );
	}

	TRACE( "REBOOTING!!!" );

	CELFFileHandler::Execute( FA_initialPath );
}

//*************************************************************************************
//	Initialise the framework sub systems
//*************************************************************************************
bool	CFrameWork::Open()
{
	//
	//	Record the start time
	//
	s_TickResolution = sceRtcGetTickResolution();

	sceRtcGetCurrentTick( &s_Time );

	s_LastTime = s_Time;

	//
	//	Patch the Kernel
	//
	SetKernelPatch();
//	pspSdkInstallNoDeviceCheckPatch();
	pspSdkInstallNoPlainModuleCheckPatch();

	//
	//	Initialise the debug output
	//
	pspDebugScreenInit();

	//
	//	Install my error handler
	//
	pspDebugInstallErrorHandler( ExceptionHandler );

	//
	//	Open the assert message module
	//
	CAssert::Open();

	//
	//	Set the clock speed
	//
//	scePowerSetBusClockFrequency( 166 );
//	scePowerSetCpuClockFrequency( 333 );

	// Get initial boot path
	char	initialPath[ MAXPATHLEN ];

	getcwd( initialPath, MAXPATHLEN );
	sprintf( FA_initialPath, "%s/%s", initialPath, FA_FILENAME.GetPtr() );

	//
	//	Initialise the random seed
	//
	srand( static_cast< u32 >( time( NULL ) ) );

	//
	//	Open the language database
	//
	if ( CLanguage::Open() == false )
	{
		ASSERT( 0, "Failed to initialise the language database" );

		return false;
	}

	//
	//	Open the process manager
	//
	CProcess::Open();

	//
	//	Initialise the USB manager
	//
	if ( CUSBManager::Open() == false )
	{
		ASSERT( 0, "USB Manager failed to open" );

		return false;
	}

	//
	//	Initialise the graphics sub system
	//
	if( CGfx::Open() == false )
	{
		ASSERT( 0, "GPU failed to initialise" );

		return false;
	}

	//
	//	Initialise the PRX manager
	//
	//CPRXManager::Open();

	//
	//	Open the render manager
	//
	CRenderable::Open();

	//
	//	Open the configuration file
	//
	CConfigFile::Open();

	//
	//	Open the skin manager
	//
	CSkinManager::Open();

	//
	//	Open the font system
	//
	CFont::Open();

	//
	//	Initialise the input controller
	//
	if ( CInput::Open() == false )
	{
		ASSERT( 0, "Input Manager failed to open" );

		return false;
	}

	//
	//	Open the file handler class
	//
	if ( CFileHandler::Open() == false )
	{
		ASSERT( 0, "File handler failed to open" );

		return false;
	}

	//
	//	Open the background renderer
	//
	CBackground::Open();

	//
	//	Open the window manager
	//
	CWindow::Open();

	//
	//	In debug we always want the USB connection to be on
	//
	if ( CVAR_USB_ON.Get() == "on" )
	{
		if ( CUSBManager::Activate() == false )
		{
			ASSERT( 0, "Failed to activate USB connection" );
		}
	}

	//
	//	Open the FTP client
	//
	CFTPClient::Open();

	return true;
}

//*************************************************************************************
//
//*************************************************************************************
void	CFrameWork::Close()
{
	//
	//	Close the FTP client
	//
	CFTPClient::Close();

	//
	//	Shut down the USB manager
	//
	CUSBManager::Close();

	//
	//	Close the codecs engines
	//
	CMusicFileHandler::Close();

	//
	//	Close the window manager
	//
	CWindow::Close();

	//
	//	Open the background renderer
	//
	CBackground::Close();

	//
	//	Close the file handler class
	//
	CFileHandler::Close();

	//
	//	Close the input controller
	//
	CInput::Close();

	//
	//	Close the skin manager
	//
	CSkinManager::Close();

	//
	//	Close the font system
	//
	CFont::Close();

	//
	//	Close the PRX manager
	//
	//CPRXManager::Close();

	//
	//	Close the render manager
	//
	CRenderable::Close();

	//
	//	Shut down the GPU
	//
	CGfx::Close();

	//
	//	Close the config file
	//
	CConfigFile::Close();

	//
	//	Close the process manager
	//
	CProcess::Close();

	//
	//	Fade out!
	//
	s32	fade_level( 255 );

	while ( fade_level > 0 )
	{
		CGfx::BeginRender();
		CGfx::DrawQuad( V2( 0.f, 0.f ), V2( CGfx::s_ScreenWidth, CGfx::s_ScreenHeight ), 0x28000000 );
		CGfx::EndRender();
		CGfx::SwapBuffers();

		fade_level -= 4;
	}

	//
	//	Open the assert message module
	//
	CAssert::Close();
}

//*************************************************************************************
//
//*************************************************************************************
bool	CFrameWork::Process()
{
	//
	//	Update the timers
	//
	s_LastTime = s_Time;
	sceRtcGetCurrentTick( &s_Time );

	//
	//	Update the input
	//
	CInput::Process();

	//
	//	Update the process list
	//
	CProcess::ProcessList();

	//
	//	Check for the home button
	//
	if ( CProcess::IsPaused() == false )
	{
		if ( CInput::IsButtonClicked( CInput::HOME ) == true )
		{
			HomeButtonCallback();
		}
		}

	//
	//	Render everything
	//
	CRenderable::Render();

	//
	//	If we're in debug, allow FA++ to be reloaded
	//
#ifdef _DEBUG

	if ( CInput::IsButtonClicked( CInput::SELECT ) == true )
	{
		CELFFileHandler::Execute( FA_initialPath );

		return false;
	}

#endif	// #ifdef _DEBUG


	return true;
}

//*************************************************************************************
//
//*************************************************************************************
void	CFrameWork::DBGMessage( const char * fmt, ... )
{
	pspDebugScreenPrintf( fmt );
}

//*************************************************************************************
//
//*************************************************************************************
u32		CFrameWork::GetTicks()
{
	return s_Time;
}

//*************************************************************************************
//
//*************************************************************************************
float	CFrameWork::GetElapsedTime()
{
	return ( static_cast< float >( s_Time - s_LastTime ) / s_TickResolution );
}

//*************************************************************************************
//
//*************************************************************************************
SceUID	CFrameWork::LoadModule( const CString & module, eModulePartition partition )
{
	SceUID	load_id( -1 );

	switch ( partition )
	{
	case KERNEL_PARTITION:
		{
			load_id = sceKernelLoadModule( module, 0, NULL );
		}
		break;

	case USER_PARTITION:
		{
			SceKernelLMOption	option;

			option.flags = 0;
			option.access = 1;
			option.position = 0;
			option.mpidtext = 2;
			option.mpiddata = 2;
			option.size = sizeof( option );

			load_id = sceKernelLoadModule( module, option.flags, &option );
		}
		break;
	}

	if ( load_id & 0x80000000 )
	{
		BREAK_POINT( "sceKernelLoadModule - %X", load_id );
	}

	return load_id;
}

//*************************************************************************************
//
//*************************************************************************************
bool	CFrameWork::LoadAndStartModule( const CString & module, eModulePartition partition )
{
	s32		status;
	SceUID	start_id( -1 );
	SceUID	load_id( LoadModule( module, partition ) );

	if ( ( load_id & 0x80000000 ) == 0 )
	{
		start_id = sceKernelStartModule( load_id, 0, NULL, &status, NULL );
	}

	if ( load_id != start_id )
	{
		BREAK_POINT( "sceKernelStartModule - %X, %X", start_id, status );

		return false;
	}

	return true;
}

//**********************************************************************************
//
//**********************************************************************************
bool	CFrameWork::RunExecutable( const CString & executable )
{

  	// Get current path
	char					cwd[ 1024 ];
	getcwd( cwd, 1024 );
	const CString			this_eboot( cwd );

	// Build PRX path
	CString					prxloader_path;
	prxloader_path.Printf( "%s/%s/%s", this_eboot.GetPtr(), s_szPluginsPath.GetPtr(), s_szPRX_Filename.GetPtr() );

	// Build ELF Loader path
	CString					elfloader_path;
	elfloader_path.Printf( "%s/%s/%s", this_eboot.GetPtr(), s_szPluginsPath.GetPtr(), s_szELF_Filename.GetPtr() );

	CString					args;
	// Provide 3 args to the ELF Kernel PRX Loader :
	// [0] : ELF PRXLoader path
	// [1] : ELF to run
	// [2] : ELF to reboot (that replaces sceKernelExitGame())
	args.Printf( "%s;%s;%s\0", prxloader_path.GetPtr(), executable.GetPtr(), FA_initialPath );


	SceKernelLoadExecParam	execParam;
	const u32				total_length( args.Length() + 1 );

	execParam.args = total_length;
	execParam.argp = const_cast< char * >( args.GetPtr() );
	execParam.key = NULL;
	execParam.size = sizeof( execParam ) + total_length;

	if ( sceKernelLoadExec( elfloader_path, &execParam ) < 0 )
	{
		return false;
	}

	return true;
}


//**********************************************************************************
//
//**********************************************************************************
bool	CFrameWork::RunUMD( const CString & executable )
{

   	// Get current path
	char					cwd[ 1024 ];
	getcwd( cwd, 1024 );
	const CString			this_eboot( cwd );

	// Build PRX path
	CString					prxloader_path;
	prxloader_path.Printf( "%s/%s/%s", this_eboot.GetPtr(), s_szPluginsPath.GetPtr(), s_szUMD_Filename.GetPtr() );

	// Build ELF Loader path
	CString					elfloader_path;
	elfloader_path.Printf( "%s/%s/%s", this_eboot.GetPtr(), s_szPluginsPath.GetPtr(), s_szELF_Filename.GetPtr() );

	CString					args;
	// Provide 3 args to the ELF Kernel PRX Loader :
	// [0] : ELF PRXLoader path
	// [1] : ELF to run
	// [2] : ELF to reboot (that replaces sceKernelExitGame())
	args.Printf( "%s;%s;%s\0", prxloader_path.GetPtr(), executable.GetPtr(), FA_initialPath );

	SceKernelLoadExecParam	execParam;
	const u32				total_length( args.Length() + 1 );

	execParam.args = total_length;
	execParam.argp = const_cast< char * >( args.GetPtr() );
	execParam.key = NULL;
	execParam.size = sizeof( execParam ) + total_length;

	if ( sceKernelLoadExec( elfloader_path, &execParam ) < 0 )
	{
		return false;
	}


	return true;
}

//**********************************************************************************
//
//**********************************************************************************
CFrameWork::eFirmwareVersion	CFrameWork::GetVersion()
{
	if ( s_nVersion == 0x0e017c43 )
	{
		return PSP_v1_5;
	}

	return PSP_v1_0;
}

//**********************************************************************************
//
//**********************************************************************************
SceModuleInfo *	CFrameWork::GetModuleInfo()
{
	return &module_info;
}

//*******************************  END OF FILE  ************************************
