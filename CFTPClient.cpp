/***********************************************************************************

  Module :	CFTPClient.cpp

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
#include "CFTPClient.h"
#include "nlh.h"
#include "my_socket.h"
#include "CFrameWork.h"
#include "CConfigFile.h"
#include "CTextInput.h"
#include "CMessageBox.h"
#include "CRenderable.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************
#define FTP_LOG
//#define FTP_LOG		TRACE

//**********************************************************************************
//   Local Constants
//**********************************************************************************
#define TRANSFER_BUFFER_SIZE 	4096	// Buffer size for transfer
#define ERRTIMEOUT				15		// TIMEOUT in seconds
#define NOOPDELAY				60		// delay to wait in seconds (float) before sending NOOP command
#define USENOOP					1		// whether or not to send NOOP command

static CVarString	CVAR_SERVER_IP( "server_ip", "192.168.1.200" );
static CVarString	CVAR_SERVER_PORT( "server_port", "21" );
static CVarString	CVAR_SERVER_USER( "server_user", "psp" );
static CVarString	CVAR_SERVER_PASSWORD( "server_password", "psp" );

//**********************************************************************************
//   Types
//**********************************************************************************
struct MclientConnection
{
	SOCKET	comSocket;
	SOCKET	listenSocket;
	SOCKET	dataSocket;

	bool	usePASV;
	int		netconn;
	int		dataPort;
	int		data_portA;
	int		data_portB;

	char	localip[ 32 ];
	char	remoteip[ 4 ];
	int		remoteport;
	char	comBuffer[ 1024 ];
	char	dataBuffer[ 1024 ];

	char *	username;
	char *	password;
	char	currDir[ 256 ];
	char	remoteSystem[ 32 ];
};

//**********************************************************************************
//   Global Variables
//**********************************************************************************

//**********************************************************************************
//   Static Variables
//**********************************************************************************
static remoteDirent	s_Directory;

//**********************************************************************************
//**********************************************************************************
//
//   IFTPClient interface class definition
//
//**********************************************************************************
//**********************************************************************************
class IFTPClient : public CFTPClient
{
	public:

		enum eStatus
		{
			STATUS_NOTCONNECTED,
			STATUS_WAITFORUSER,
			STATUS_WAITFORPASS,
			STATUS_LOGGEDIN,
			STATUS_SENDPASV,
			STATUS_ENTERPASV,
			STATUS_SENDPORT,
			STATUS_PORTSUCCESS,
			STATUS_WAITFORSYST,
			STATUS_WAITFORPWD,
			STATUS_WAITFORRMD,
			STATUS_WAITFORMKD,
			STATUS_WAITFORCWD,
			STATUS_WAITFORDELE,
			STATUS_WAITFORNOOP,
			STATUS_NOOPSUCCESS,
			STATUS_215SUCCESS,
			STATUS_250SUCCESS,
			STATUS_257SUCCESS,
			STATUS_TRANSFERWAIT,
			STATUS_TRANSFERRING,
			STATUS_FAILED,
			STATUS_IDLE,
		};

		enum eApctlStatus
		{
			APCTL_IDLE,
			APCTL_INITIALIZING,
			APCTL_ASSOCIATING,
			APCTL_DHCPQUERY,
			APCTL_READY,
		};

		enum eErrorCodes
		{
			ERROR_UNKNOWN = 1,
			ERROR_TIMEOUT,
			ERROR_NOIP,
			ERROR_CREATECONNECTSOCKET,
			ERROR_BINDCONNECTSOCKET,
			ERROR_PORTFAILED,
			ERROR_CREATELISTENSOCKET,
			ERROR_BINDSOCKET,
			ERROR_LISTENSOCKET,
			ERROR_ACCEPTSOCKET,
			ERROR_CONNECTDATAPORT,
			ERROR_NODATACONNECTION,
			ERROR_TRANSFERFAILED,
			ERROR_CREATECOMSOCKET,
			ERROR_UNABLETOCONNECT,
			ERROR_NETDRIVERLOAD,
			ERROR_NLHINIT,
			ERROR_NOSTATICCONNECTION,
			ERROR_NETAPCTLCONNECT,
			ERROR_CREATINGTHREAD,
			ERROR_CREATINGTIMERTHREAD,
			ERROR_CWDFAILED,
			ERROR_DELEFAILED,
			ERROR_NOOPFAILED,
			ERROR_EXITED,
		};

	public:

		IFTPClient();
		virtual ~IFTPClient();

		virtual bool				Connect( u32 WLAN_connection_number );
		virtual bool				Disconnect();

		virtual const char *		GetRemoteDir();

		virtual remoteDirent *		LIST();
		virtual bool				DELE( const char * const file );
		virtual bool 				CWD( const char * const dir );
		virtual bool 				MKD( const char * const dir );
		virtual bool 				RMD( const char * const dir );
		virtual void				CHMOD( const char * const file, const char * const perm );
		virtual void				SetCurrentDirectory( const char * const command );
		virtual void				SetPASV( bool enabled );
		virtual bool				PASVEnabled() const;

		virtual bool				IsWLANEnabled() const;
		virtual bool				IsWLANPowerEnabled() const;

		virtual const CWLANList &	GetWLANList() const;

	protected:

		bool						EnumerateConnections();
		bool						CreateFTPThread( u32 WLAN_connection_number );

		bool						AttemptToConnect();

		int							ftpDispatch( char * command );
		void						ftpHandleResponses();
		bool						ftpConnect( char * ip, int port );
		bool						ftpLogin( char * user, char * pass );

		char *						ftpPWD();
		char *						ftpSYST();

		void						ftpABOR();
		int							ftpNOOP();
		bool						ftpExited();
		int							ftpTimeoutISNOT( eStatus waitstatus, int sectimeout );
		int							ftpTimeoutIS( eStatus waitstatus, int sectimeout );

		int 						RETR( char * localdir, char * file );
		int 						ftpRETR( char * localdir, char * file );

		int 						STOR( char * localdir, char * file );
		int 						ftpSTOR( char * localdir, char * file );

		bool						startDataConnect();
		bool						startDataAccept();
		int							openClientDataConnection();
		bool						closeClientDataConnection();

		void						sendclientDataLn( const char * const s );
		void						sendclientData( const char * const s );
		void						sendclientResponseLn( const char * const s );
		void						sendclientResponse( const char * const s );

		void						DisplayError( const CString & error );
		void						DisplayMessage( const CString & message );

		static int					mainThread( SceSize args, void * argp );
		static int					ftpTimerThread( SceSize args, void * argp );
		static int 					RETRThread( SceSize args, void * argp );
		static int 					STORThread( SceSize args, void * argp );

	protected:

		MclientConnection *			m_pConnection;

		CWLANList					m_WLANList;

		bool						m_bConnected;

		s32							m_nRETRSize;	// current downloaded size
		s32							m_nSTORSize;	// current uploaded size

	protected:

		static bool					s_bQuit;
		static eStatus				s_eStatus;
		static SceUID				s_MainThreadID;
		static SceUID				s_TimerThreadID;
		static bool					s_bWaitForConnect;
};

//**********************************************************************************
//	Static variables
//**********************************************************************************
bool				IFTPClient::s_bQuit( false );
IFTPClient::eStatus	IFTPClient::s_eStatus( STATUS_NOTCONNECTED );
SceUID				IFTPClient::s_MainThreadID( -1 );
SceUID				IFTPClient::s_TimerThreadID( -1 );
bool				IFTPClient::s_bWaitForConnect( false );

static IFTPClient *	s_pInstance( NULL );

//**********************************************************************************
//
//**********************************************************************************
IFTPClient::IFTPClient()
:	m_pConnection( new MclientConnection() )
,	m_bConnected( false )
,	m_nRETRSize( 0 )
,	m_nSTORSize( 0 )
{
	nlhLoadDrivers( CFrameWork::GetModuleInfo() );

	if ( nlhInit() != 0 )
	{
		DisplayError( "nlhInit FAILED!" );
	}

	EnumerateConnections();
}

//**********************************************************************************
//
//**********************************************************************************
IFTPClient::~IFTPClient()
{
	Disconnect();

	SAFE_DELETE( m_pConnection );
}

//**********************************************************************************
//
//**********************************************************************************
void	IFTPClient::DisplayError( const CString & error )
{
	CErrorMessage	error_msg( error );
}

//**********************************************************************************
//
//**********************************************************************************
void	IFTPClient::DisplayMessage( const CString & message )
{
	CMessageBox	msg_box;

	msg_box.SetTitle( "FTP" );
	msg_box.SetText( message );

	CRenderable::Render();
}

//**********************************************************************************
//
//**********************************************************************************
bool	IFTPClient::EnumerateConnections()
{
	netData	data;

	m_WLANList.clear();

	for ( u32 i = 1; i < 100; ++i )
	{
		sWLANSetting	settings;

		if ( sceUtilityCheckNetParam( i ) != 0 )
		{
			break;
		}

		sceUtilityGetNetParam( i, PSP_NETPARAM_NAME, &data );

		settings.m_szName = data.asString;

		sceUtilityGetNetParam( i, PSP_NETPARAM_SSID, &data );

		settings.m_szDetails += "SSID=";
		settings.m_szDetails += data.asString;

		sceUtilityGetNetParam( i, PSP_NETPARAM_IS_STATIC_IP, &data );

		if ( data.asUint != 0 )
		{
			CHECK_ERROR( sceUtilityGetNetParam( i, PSP_NETPARAM_IP, &data ) );

			settings.m_szDetails += " IPADDR=";
			settings.m_szDetails += data.asString;

			m_WLANList.push_back( settings );
		}	
	}

	return ( m_WLANList.empty() == false );
}

//**********************************************************************************
//
//**********************************************************************************
bool	IFTPClient::IsWLANEnabled() const
{
	return ( sceWlanGetSwitchState() != 0 );
}

//**********************************************************************************
//
//**********************************************************************************
bool	IFTPClient::IsWLANPowerEnabled() const
{
	return ( sceWlanDevIsPowerOn() != 0 );
}

//**********************************************************************************
//
//**********************************************************************************
const CWLANList &	IFTPClient::GetWLANList() const
{
	return m_WLANList;
}

//**********************************************************************************
//
//**********************************************************************************
bool	IFTPClient::Connect( u32 WLAN_connection_number )
{
	//
	//	Enter the IP address
	//
enter_ip:
	{
		CTextInput	ip_input( "Enter IP", CVAR_SERVER_IP.Get() );

		if ( ip_input.Show() == CTextInput::RC_OK )
		{
			CVAR_SERVER_IP.Set( ip_input.GetText() );
		}
		else
		{
			return false;
		}
	}

	//
	//	Enter the PORT
	//
enter_port:
	{
		CTextInput	port_input( "Enter PORT", CVAR_SERVER_PORT.Get() );

		if ( port_input.Show() == CTextInput::RC_OK )
		{
			CVAR_SERVER_PORT.Set( port_input.GetText() );
		}
		else
		{
			goto enter_ip;
		}
	}

	//
	//	Enter the USERname
	//
enter_user:
	{
		CTextInput	user_input( "Enter USER", CVAR_SERVER_USER.Get() );

		if ( user_input.Show() == CTextInput::RC_OK )
		{
			CVAR_SERVER_USER.Set( user_input.GetText() );
		}
		else
		{
			goto enter_port;
		}
	}

	//
	//	Enter the PASSWORD
	//
	{
		CTextInput	password_input( "Enter PASSWORD", CVAR_SERVER_PASSWORD.Get() );

		if ( password_input.Show() == CTextInput::RC_OK )
		{
			CVAR_SERVER_PASSWORD.Set( password_input.GetText() );

			password_input.SetVisible( false );
		}
		else
		{
			goto enter_user;
		}
	}

	//
	//	Enter PASV
	//
	{
		CModalMessageBox	box( "PASV", "Use PASV connection?" );

		box.AddExitCode( CModalMessageBox::EXIT_CROSS, "Yes" );
		box.AddExitCode( CModalMessageBox::EXIT_CIRCLE, "No" );

		SetPASV( ( box.Show() == CModalMessageBox::EXIT_CROSS ) );
	}

	//
	//	Save the new settings
	//
	CConfigFile::Save();

	//
	//	Try to connect
	//
	s_bQuit = false;

	DisplayMessage( "Connecting..." );

	if ( CreateFTPThread( WLAN_connection_number ) == true )
	{
		DisplayMessage( "Attempt to Connect..." );

		if ( AttemptToConnect() == true )
		{
			m_bConnected = true;

			return true;
		}
		else
		{
			DisplayError( "FAILED to connect!" );
		}
	}
	else
	{
		DisplayError( "Failed to create FTP thread" );
	}

	Disconnect();

	return false;
}

//**********************************************************************************
//
//**********************************************************************************
bool	IFTPClient::Disconnect()
{
	s_bQuit = true;

	s_bWaitForConnect = false;

	if ( s_eStatus > STATUS_NOTCONNECTED )
	{
		sendclientResponseLn( "QUIT" );

		closeClientDataConnection();
	}

	if ( sceNetApctlDisconnect() != 0 )
	{
		DisplayError( "sceNetApctlDisconnect failed" );

		return false;
	}

	if ( nlhTerm() != 0 )
	{
		DisplayError( "nlhTerm failed" );

		return false;
	}

	if ( s_MainThreadID != -1 )
	{
		CHECK_ERROR( sceKernelTerminateDeleteThread( s_MainThreadID ) );

		s_MainThreadID = -1;
	}

	if ( s_TimerThreadID != -1 )
	{
		CHECK_ERROR( sceKernelTerminateDeleteThread( s_TimerThreadID ) );

		s_TimerThreadID = -1;
	}

	m_bConnected = false;

	s_eStatus = STATUS_NOTCONNECTED;

	return true;
}

//**********************************************************************************
//
//**********************************************************************************
int	IFTPClient::mainThread( SceSize args, void * argp )
{
	FTP_LOG( "mainThread\n" );

	while ( s_bWaitForConnect == false && s_bQuit == false )
	{
		FTP_LOG( "..." );
		sceKernelDelayThread( 50 * 1000 );
	}

	FTP_LOG( "\nStart timer thread\n" );

	s_TimerThreadID = sceKernelCreateThread( "THREAD_FTP_TIMER", ftpTimerThread, 0x18, 0xFA0, 0, NULL );

	if ( s_TimerThreadID >= 0 )
	{
		sceKernelStartThread( s_TimerThreadID, 0, 0 );
	}
	else
	{
		// unable to create client thread
		DISPLAY_ERROR( s_TimerThreadID );

		FTP_LOG( "ERROR_CREATINGTIMERTHREAD\n" );

		s_TimerThreadID = -1;

		return 0;
	}

	s_pInstance->ftpHandleResponses();

	return 0;
}

//**********************************************************************************
//
//**********************************************************************************
bool	IFTPClient::CreateFTPThread( u32 WLAN_connection_number )
{
	int	state( 0 );

	if ( ftpExited() == true )
	{
		BREAK_POINT( "ftpExited" );
		return false;
	}

	// try first connection
	if ( sceNetApctlConnect( WLAN_connection_number ) != 0 )
	{
		// sceNetApctlConnect error
		BREAK_POINT( "sceNetApctlConnect" );
		return false;
	}

	// wait for ready state
	if ( sceNetApctlGetState( &state ) != 0 )
	{
		BREAK_POINT( "sceNetApctlGetState" );
		return false;
	}

	// TODO: timeout on ready state and return unable to start

	// wait for ready state unless exited
	while ( ftpExited() == false )
	{
		if ( sceNetApctlGetState( &state ) != 0 )
		{
			BREAK_POINT( "sceNetApctlGetState" );

			return false;
		}

		// 4 - got IP - usable
		if ( state == APCTL_READY )
		{
			FTP_LOG( "connected with static IP\n" );
			break;  // connected with static IP
		}

		// delay a bit
		sceKernelDelayThread( 500 * 1000 );
	}

	// make sure it was not exited
	if ( ftpExited() == true )
	{
		BREAK_POINT( "ftpExited 2" );
		return false;
	}

	// allocate connection info
	m_pConnection->dataSocket = 0;
	m_pConnection->usePASV = false;
	m_pConnection->data_portA = 16;
	m_pConnection->data_portB = 115;
	m_pConnection->netconn = WLAN_connection_number;

	memset( m_pConnection->comBuffer, 0, 1024 );
	memset( m_pConnection->dataBuffer, 0, 1024 );
	memset( m_pConnection->localip, 0, 4 );
	memset( m_pConnection->remoteip, 0, 4 );

	// begin ftp client thread
	FTP_LOG( "Starting main thread\n" );

	s_MainThreadID = sceKernelCreateThread( "THREAD_FTP_CLIENTLOOP", mainThread, 0x18, 0x10000, 0, NULL );

	if ( s_MainThreadID >= 0 )
	{
		sceKernelStartThread( s_MainThreadID, 0, 0 );
	}
	else
	{
		DISPLAY_ERROR( s_MainThreadID );

		s_MainThreadID = -1;

		BREAK_POINT( "unable to create client thread" );

		return false;
	}

	// success
	FTP_LOG( "CreateFTPThread success\n" );
	return true;
}

//**********************************************************************************
//
//**********************************************************************************
bool	IFTPClient::AttemptToConnect()
{
	// attempt to connect to server
	int	retries( 3 );
	int	retrycount( 0 );

	while ( retrycount < retries )
	{
		if ( ftpConnect( const_cast< char * >( CVAR_SERVER_IP.Get().GetPtr() ), atoi( CVAR_SERVER_PORT.Get() ) ) == true )
		{
			DisplayMessage( "Connected..." );

			if ( ftpLogin( const_cast< char * >( CVAR_SERVER_USER.Get().GetPtr() ), const_cast< char * >( CVAR_SERVER_PASSWORD.Get().GetPtr() ) ) == true )
			{
				DisplayMessage( "Logged in..." );

				// do some stuff on connect
				ftpSYST();
				ftpPWD();
			}

			return true;
		}
		else
		{
			DisplayMessage( CString().Printf( "Unable to Connect (#%d), retrying ...\n", retrycount ) );

			++retrycount;
		}
	}

	return false;
}

//**********************************************************************************
//
//**********************************************************************************
bool	IFTPClient::ftpConnect( char * ip, int port )
{
	sockaddr_in	addrTo;

	if ( sceNetApctlGetInfo( 8, m_pConnection->localip ) != 0 )
	{
		BREAK_POINT( "sceNetApctlGetInfo NO IP!" );

		return false;
	}

	// parse remote ip address from string
	char		serverip[ 25 ];
	char *		remoteip;
	const char	delimiters[] = ".";

	strcpy( serverip, ip );

	remoteip = strtok( serverip, delimiters );
	m_pConnection->remoteip[ 0 ] = atoi( remoteip );
	remoteip = strtok( NULL, delimiters );
	m_pConnection->remoteip[ 1 ] = atoi( remoteip );
	remoteip = strtok(NULL, delimiters);
	m_pConnection->remoteip[ 2 ] = atoi( remoteip );
	remoteip = strtok(NULL, delimiters);
	m_pConnection->remoteip[ 3 ] = atoi( remoteip );

	m_pConnection->remoteport = port;

	addrTo.sin_family = AF_INET;
	addrTo.sin_port = htons( port );
	addrTo.sin_addr[ 0 ] = m_pConnection->remoteip[ 0 ];
	addrTo.sin_addr[ 1 ] = m_pConnection->remoteip[ 1 ];
	addrTo.sin_addr[ 2 ] = m_pConnection->remoteip[ 2 ];
	addrTo.sin_addr[ 3 ] = m_pConnection->remoteip[ 3 ];

	//int err = sceNetInetConnect(sock, &addrTo, sizeof(addrTo));
	SOCKET	socket = nlhSimpleConnectWithTimeout( SOCK_STREAM /*TCP/IP*/, addrTo.sin_addr, port, 500 );

	if ( socket == INVALID_SOCKET )
	{
		FTP_LOG( "nlhSimpleConnectWithTimeout == INVALID_SOCKET" );

		return false;
	}
	else
	{
		// close existing comSocket
		FTP_LOG( "close existing comSocket\n" );

		sceNetInetClose( m_pConnection->comSocket );

		s_bWaitForConnect = true;
		m_pConnection->comSocket = socket;

		return true;
	}
}

//**********************************************************************************
//
//**********************************************************************************
bool	IFTPClient::ftpLogin( char * user, char * pass )
{
	//waiting for user request
	FTP_LOG( "ftpTimeoutISNOT\n" );

	if ( ftpTimeoutISNOT( STATUS_WAITFORUSER, ERRTIMEOUT ) < 0 )
	{
		s_eStatus = STATUS_IDLE;
		return false;
	}

	m_pConnection->username = user;
	m_pConnection->password = pass;

	// Send Username
	FTP_LOG( "Send Username\n" );
	sendclientResponse( "USER " );
	sendclientResponseLn( user );

	// wait for PASS request
	FTP_LOG( "ftpTimeoutISNOT\n" );
	if ( ftpTimeoutISNOT( STATUS_WAITFORPASS, ERRTIMEOUT ) < 0 )
	{
		s_eStatus = STATUS_IDLE;
		return false;
	}

	// Send Password
	FTP_LOG( "Send Password\n" );
	sendclientResponse("PASS ");
	sendclientResponseLn(pass);

	// wait for login confirmation
	FTP_LOG( "ftpTimeoutISNOT\n" );
	if ( ftpTimeoutISNOT( STATUS_LOGGEDIN, ERRTIMEOUT ) < 0 )
	{
		s_eStatus = STATUS_IDLE;
		return false;
	}

	s_eStatus = STATUS_IDLE;

	// success
	FTP_LOG( "ftpLogin SUCCESS\n" );

	return true;
}

//**********************************************************************************
//	
//**********************************************************************************
bool	IFTPClient::ftpExited()
{
	return s_bQuit;
}

//**********************************************************************************
//
//**********************************************************************************
void	IFTPClient::SetCurrentDirectory( const char * const command )
{
	int currLen = 0;
	int infoLen = 0;
	int isDir = 0;

	FTP_LOG( "SetCurrentDirectory( %s )\n", command );

	while ( infoLen < strlen( command ) )
	{
		if ( command[ infoLen ] == '"' )
		{
			if ( isDir == 1)
			{
				isDir = 0;
			}
			else
			{
				isDir = 1;
			}
		}
		else
		{
			if ( isDir == 1 )
			{
				m_pConnection->currDir[ currLen ] = command[ infoLen ];
				currLen++;
			}
		}

		infoLen++;
	}

	m_pConnection->currDir[ currLen ] = '\0';
}

//**********************************************************************************
//
//**********************************************************************************
int	IFTPClient::ftpTimeoutIS( eStatus waitstatus, int sectimeout )
{
	// waits until a specified s_eStatus is no longer true and returns -1 if it does not in specified timeout
	int	mscount = 0;

	while ( s_eStatus == waitstatus && ftpExited() == false )
	{
		sceKernelDelayThread( 50 * 1000 );

		mscount += 50;

		if ( mscount >= ( sectimeout * 1000 ) )
		{
			// timeout reached
			FTP_LOG( "ftpTimeoutIS TIMED OUT!\n" );

			s_eStatus = STATUS_IDLE;

			return ERROR_TIMEOUT;
		}
	}

	if ( s_bQuit == false )
	{
		return 0;
	}
	else
	{
		return ERROR_EXITED;
	}
}

//**********************************************************************************
//	
//**********************************************************************************
int	IFTPClient::ftpTimeoutISNOT( eStatus waitstatus, int sectimeout )
{
	// waits for a specific s_eStatus to occur and returns -1 if it does not in specified timeout
	int	mscount = 0;

	while ( s_eStatus != waitstatus && ftpExited() == false )
	{
		sceKernelDelayThread( 50 * 1000 );

		mscount += 50;

		if ( mscount >= ( sectimeout * 1000 ) )
		{
			// timeout reached
			FTP_LOG( "ftpTimeoutISNOT TIMED OUT!\n" );

			s_eStatus = STATUS_IDLE;

			return ERROR_TIMEOUT;
		}
	}

	if ( s_bQuit == false )
	{
		return 0;
	}
	else
	{
		return ERROR_EXITED;
	}
}

//**********************************************************************************
//	Processes the commands retrieved from the server
//**********************************************************************************
int	IFTPClient::ftpDispatch( char * command )
{
	int	ret = 0;

	FTP_LOG( "ftpDispatch( %s )\n", command );

	if ( strlen( command ) > 0 )
	{
		if ( strStartsWith( command, "150 " ) || strStartsWith( command, "150" ) )
		{
			// Data connection accepted
			s_eStatus = STATUS_TRANSFERRING;
			ret = 150;
		}
		else if ( strStartsWith( command, "200 " ) || strStartsWith( command, "200" ) )
		{
			// Noop - Port command successful
			if ( strStartsWith( command, "200 Port" ) )
			{
				s_eStatus = STATUS_PORTSUCCESS;
			}
			else
			{
				if ( s_eStatus == STATUS_WAITFORNOOP )
				{
					s_eStatus = STATUS_NOOPSUCCESS;
				}
				else
				{
					s_eStatus = STATUS_IDLE;
				}
			}
			ret = 200;
		}
		else if ( strStartsWith( command, "211 " ) || strStartsWith( command, "211" ) )
		{
			// returns extensions supported
			// sends 211- to start, 211 End. to end
			s_eStatus = STATUS_IDLE;
			ret = 211;
		}
		else if ( strStartsWith( command, "215 " ) || strStartsWith( command, "215" ) )
		{
			// return from SYST

			// save SYST info
			int	currLen = 0, infoLen = 4;

			while ( infoLen < strlen( command ) )
			{
				m_pConnection->remoteSystem[ currLen ] = command[ infoLen ];
				currLen++;
				infoLen++;
			}

			m_pConnection->remoteSystem[ currLen ] = '\0';

			s_eStatus = STATUS_215SUCCESS;
			ret = 215;
		}
		else if ( strStartsWith( command, "220 " ) || strStartsWith( command, "220" ) )
		{
			// server ready
			s_eStatus = STATUS_WAITFORUSER;
			ret = 220;
		}
		else if ( strStartsWith( command, "221 " ) || strStartsWith( command, "221" ) )
		{
			// server disconnected
			s_eStatus = STATUS_NOTCONNECTED;
		}
		else if ( strStartsWith( command, "226 " ) || strStartsWith( command, "226" ) )
		{
			// transfer finished (OK  or  Aborted)
			closeClientDataConnection();
			s_eStatus = STATUS_IDLE;
			ret = 226;
		}
		else if ( strStartsWith( command, "227 " ) || strStartsWith( command, "227" ) )
		{
			// Received PASV info
			//227 Entering Passive Mode (192,168,0,103,79,185)
			int		params[ 6 ];
			char	decimByte[ 4 ];
			char *	pDecimByte = decimByte;
			char *	pParams = skipWS( &command[ 27 ] );
			int		state=0; int err=0; int nbParams=0;

			do
			{
				if ( state == 0 && *pParams >= '0' && *pParams <= '9' )
				{
					// if this is a # from 0-9
					state = 1;
					pParams--;
				}
				else if ( state == 1 && *pParams >= '0' && *pParams <= '9' )
				{
					// if we have found a number already and this is a # from 0-9
					if ( pDecimByte - decimByte <= 2 )
					{
						// set the next # in the char array
						*( pDecimByte++ ) = *pParams;
					}
					else
					{
						err = 1;
					}
				}
				else if ( state == 1 && ( *pParams == ',' || *pParams == ')' || *pParams == 0 ) && nbParams < 6 )
				{
					*pDecimByte = 0;

					if ( strlen( decimByte ) == 0 )
					{
						err = 1;
					}
					else
					{
						int		param = 0;
						char *	tmp = decimByte + strlen( decimByte ) - 1;
						int		pow = 1;

						while ( tmp >= decimByte && err == 0 )
						{
							if ( *tmp >= '0' && *tmp <= '9' )
							{
								param += ( ( *tmp ) - 48 ) * pow;
								pow = pow * 10;
							}
							else
							{
								err = 1;
							}

							tmp--;
						}

						if ( err == 0 )
						{
							params[ nbParams++ ] = param;
							pDecimByte = decimByte;
						}
					}
				}
				else
				{
					err = 1;
				}
			} while ( *( pParams++ ) != 0 && err == 0 );

			// save data port info for later use
			m_pConnection->data_portA = (unsigned char) params[ 4 ];
			m_pConnection->data_portB = (unsigned char) params[ 5 ];

			// Entering PASV mode
			s_eStatus = STATUS_ENTERPASV;
			ret = 227;
		}
		else if ( strStartsWith( command, "230 " ) || strStartsWith( command, "230" ) )
		{
			//login successful
			s_eStatus = STATUS_LOGGEDIN;
			ret = 230;
		}
		else if ( strStartsWith( command, "250 " ) || strStartsWith( command, "250" ) )
		{
			// comand success (CWD), command success (RMDIR), command success (DELE)
			// 250 CWD command successful. "/" is current directory.
			// 250 "/a": directory removed.
			// 250 File "filename" deleted.

			// save current directory if CWD was passed
			if ( s_eStatus == STATUS_WAITFORCWD )
			{
				SetCurrentDirectory( command );
			}

			s_eStatus = STATUS_250SUCCESS;
			ret = 250;
		}
		else if ( strStartsWith( command, "257 " ) || strStartsWith( command, "257" ) )
		{
			//return of PWD, return of MKDIR SUCCESS
			//257 "/" is current directory.
			//257 "/a": directory created.

			// save current directory if PWD was passed
			if ( s_eStatus == STATUS_WAITFORPWD )
			{
				SetCurrentDirectory( command );
			}

			s_eStatus = STATUS_257SUCCESS;
			ret = 257;
		}
		else if ( strStartsWith( command, "331 " ) || strStartsWith( command, "331" ) )
		{
			// waiting for password
			s_eStatus = STATUS_WAITFORPASS;
			ret = 331;
		}
		else if ( strStartsWith( command, "421 " ) || strStartsWith( command, "421" ) )
		{
			// disconnected
			FTP_LOG( "Server closed connection.\n" );
			ret = 421;
			Disconnect();
		}
		else if ( strStartsWith( command, "425 " ) || strStartsWith( command, "425" ) )
		{
			// unable to open data connection
			s_eStatus = STATUS_FAILED;
			ret = 425;
		}
		else if ( strStartsWith( command, "426 " ) || strStartsWith( command, "426" ) )
		{
			// RETR aborted
			s_eStatus = STATUS_IDLE;
			ret = 426;
		}
		else if ( strStartsWith( command, "450 " ) || strStartsWith( command, "450" ) )
		{
			// 450: Unable to Delete file
			s_eStatus = STATUS_IDLE;
			ret = 450;
		}
		else if ( strStartsWith( command, "500 " ) || strStartsWith( command, "500" ) )
		{
			// failed port?
			s_eStatus = STATUS_IDLE;
			ret = 500;
		}
		else if ( strStartsWith( command, "550 " ) || strStartsWith( command, "550" ) )
		{
			// 550: Cannot STOR. No permission. File not found on server, unable to delete, unable to create, general failed code
			s_eStatus = STATUS_FAILED;
			ret = 550;
		}
		else
		{
			// Unimplemented
			FTP_LOG( "~~ %s\n", command );
			s_eStatus = STATUS_IDLE;
			return 999;
		}
	}
	else
	{
		s_eStatus = STATUS_IDLE;
		ret = ERROR_UNKNOWN;
	}

	return ret;
}

//**********************************************************************************
//
//**********************************************************************************
static int	NOOPtime = 0;

int IFTPClient::ftpTimerThread( SceSize args, void * argp )
{
	FTP_LOG( "ftpTimerThread ENTER\n" );

	while ( s_bQuit == false )
	{
		// check every 1 sec
		sceKernelDelayThread( 1000 * 1000 );

		// if noop is enabled, track noop time
		if ( USENOOP == 1 )
		{
			NOOPtime += 1;

			if ( NOOPtime >= NOOPDELAY && s_eStatus == STATUS_IDLE )
			{
				// send noop
				FTP_LOG( "Sending NOOP\n" );
				s_pInstance->ftpNOOP();
			}
		}
	}

	FTP_LOG( "ftpTimerThread EXIT\n" );

	return 0;
}

//**********************************************************************************
//	Waits for data to arrive from the server and process it
//**********************************************************************************
void IFTPClient::ftpHandleResponses()
{
beginhandle:
	{
		// send - receive data
		char readBuffer[1024];
		char lineBuffer[1024];
		int lineLen=0;
		int errLoop = 0;

		SOCKET sock = m_pConnection->comSocket;

		// wait until connection has been started again
		while ( s_bWaitForConnect == 0 && s_bQuit == 0)
		{
			sceKernelDelayThread(50*1000);
		}


		while(errLoop >= 0 && s_bQuit == 0)
		{
			// waiting for data

			int nb = sceNetInetRecv(sock, (u8*)readBuffer, 1024, 0);

			if (nb <= 0)
			{
				// nothing to recv
				break;
			}

			int i=0;
			while (i<nb)
			{
				if (readBuffer[i]!='\r')
				{
					lineBuffer[lineLen++]=readBuffer[i];
					if (readBuffer[i]=='\n' || lineLen==1024)
					{
						lineBuffer[--lineLen]=0;

						char* response=skipWS(lineBuffer);
						trimEndingWS(response);
						FTP_LOG( "%s\n", response );

						// if noop is enabled, reset since we received a response
						if ( USENOOP == 1 )
						{
							NOOPtime = 0;
						}

						if ((errLoop = ftpDispatch(response)) < 0)
						{
							// unknown response error (not implemented in ftpDispatch)
							break;
						}
						lineLen=0;
					}
				}
				i++;
			}
		}

		// connection closed, attempting to restart
		if ( s_bQuit == false )
		{
			goto beginhandle;
		}
	}
}

//**********************************************************************************
//	Sends a Response to the Server
//**********************************************************************************
void	IFTPClient::sendclientResponse( const char * const s )
{
	FTP_LOG( "sendclientResponse = %s\n", s );

	strcat( m_pConnection->comBuffer, s );

	if ( endsWith( m_pConnection->comBuffer, "\n" ) )
	{
		sceNetInetSend( m_pConnection->comSocket, m_pConnection->comBuffer, strlen( m_pConnection->comBuffer ), 0 );

		strcpy( m_pConnection->comBuffer, "" );
	}
}

//**********************************************************************************
//	Sends a Response with Line Break to the Server
//**********************************************************************************
void	IFTPClient::sendclientResponseLn( const char * const s )
{
	FTP_LOG( "sendclientResponseLn = %s\n", s );

	strcat( m_pConnection->comBuffer, s );
	strcat( m_pConnection->comBuffer, "\r\n" );

	sceNetInetSend( m_pConnection->comSocket, m_pConnection->comBuffer, strlen( m_pConnection->comBuffer ), 0 );

	strcpy( m_pConnection->comBuffer, "" );
}

//**********************************************************************************
//	Sends Data to the Server
//**********************************************************************************
void	IFTPClient::sendclientData( const char * const s )
{
	FTP_LOG( "sendclientData = %s\n", s );

	strcat( m_pConnection->dataBuffer, s );

	if ( endsWith( m_pConnection->dataBuffer, "\n" ) )
	{
		sceNetInetSend( m_pConnection->dataSocket, m_pConnection->dataBuffer, strlen( m_pConnection->dataBuffer ), 0 );

		strcpy( m_pConnection->dataBuffer, "" );
	}
}

//**********************************************************************************
//	Sends Data with Line Break to the Server
//**********************************************************************************
void	IFTPClient::sendclientDataLn( const char * const s )
{
	FTP_LOG( "sendclientDataLn = %s\n", s );

	strcat( m_pConnection->dataBuffer, s );
	strcat( m_pConnection->dataBuffer, "\r\n" );

	sceNetInetSend( m_pConnection->dataSocket, m_pConnection->dataBuffer, strlen( m_pConnection->dataBuffer ), 0 );

	strcpy( m_pConnection->dataBuffer, "" );
}

//**********************************************************************************
//	Closes any open sockets
//**********************************************************************************
bool	IFTPClient::closeClientDataConnection()
{
	return ( sceNetInetClose( m_pConnection->dataSocket ) == 0 );
}

//**********************************************************************************
//	Begins Opening the Data Connection
//**********************************************************************************
int	IFTPClient::openClientDataConnection()
{
	u32 err;

	// close any previous data connection
	closeClientDataConnection();

	// Check if We Are Using PASV Mode
	if ( m_pConnection->usePASV == true ) {
		// Using PASV Mode

		// send PASV command
		s_eStatus = STATUS_SENDPASV;
		sendclientResponseLn("PASV");

		// wait for return/failure of pasv command
		int ret = ftpTimeoutISNOT(STATUS_ENTERPASV, ERRTIMEOUT);
		if (ret < 0) {
			// timed out
			s_eStatus = STATUS_IDLE;
			return ERROR_TIMEOUT;
		}

		if ( s_eStatus == STATUS_ENTERPASV ) {
			// PASV info (ip/port) was received and parsed

			// ******* Connect

			// Create Connect Socket
			struct sockaddr_in addrConnect;
			memset(&addrConnect, 0, sizeof(struct sockaddr_in));

			SOCKET sockConnect = sceNetInetSocket(AF_INET, SOCK_STREAM, 0);
			if (sockConnect & 0x80000000) {
				return ERROR_CREATECONNECTSOCKET;
			}

			// Setup Connection Info
			addrConnect.sin_family = AF_INET;

			// get the port to use (portA (static number) * 256 + portB (incrementing number)
			m_pConnection->dataPort = (m_pConnection->data_portA * 256) + m_pConnection->data_portB;

			// set port to use
			addrConnect.sin_port = htons(m_pConnection->dataPort);

			// ip info not needed
			addrConnect.sin_addr[0] = 0;
			addrConnect.sin_addr[1] = 0;
			addrConnect.sin_addr[2] = 0;
			addrConnect.sin_addr[3] = 0;

			// Bind the Connect Socket
			err = sceNetInetBind(sockConnect, &addrConnect, sizeof(addrConnect));
			if (err != 0) {
				s_eStatus = STATUS_IDLE;
				return ERROR_BINDCONNECTSOCKET;
			}

			// Save the Connect Socket for Later
			m_pConnection->dataSocket = sockConnect;


			// Finished, Return to IDLE State
			s_eStatus = STATUS_IDLE;
		} else {
			// unknown s_eStatus
			s_eStatus = STATUS_IDLE;
			return ERROR_UNKNOWN;
		}

	} else {
		// Using PORT Mode


		// increment port for next data connection (won't effect PASV)
		m_pConnection->data_portB++;


		// get local ip info to pass with PORT command
		char ipinfo[32];
		strncpy(ipinfo, m_pConnection->localip, 31);
		strReplaceChar(ipinfo, '.', ',');

		// Set Status to Sending Port
		s_eStatus = STATUS_SENDPORT;

		// get port info to pass with PORT command
		static char portinfo[7];
		sprintf(portinfo, "%d,%d", m_pConnection->data_portA, m_pConnection->data_portB);

		// Send Port Info to remote
		sendclientResponse("PORT ");
		sendclientResponse(ipinfo);
		sendclientResponse(",");
		sendclientResponseLn(portinfo);

		// wait for return/failure of port command
		int ret = ftpTimeoutIS(STATUS_SENDPORT, ERRTIMEOUT);
		if (ret < 0) {
			return ERROR_TIMEOUT;
		}

		// port command returned success/failure
		if ( s_eStatus == STATUS_PORTSUCCESS ) {
			// PORT Successful
		} else {
			// PORT Failed, Return OpenDataConnection Failed
			s_eStatus = STATUS_IDLE;
			return ERROR_PORTFAILED;
		}

		// Create Listen Socket
		struct sockaddr_in addrListen;
		SOCKET sockListen = sceNetInetSocket(AF_INET, SOCK_STREAM, 0);
		if (sockListen & 0x80000000) {
			s_eStatus = STATUS_IDLE;
			return ERROR_CREATELISTENSOCKET;
		}

		// Setup Connection Info
		addrListen.sin_family = AF_INET;

		// get the port to use (portA (static number) * 256 + portB (incrementing number)
		m_pConnection->dataPort = (m_pConnection->data_portA * 256) + m_pConnection->data_portB;

		// set port to use
		addrListen.sin_port = htons(m_pConnection->dataPort);

		// ip info not needed
		addrListen.sin_addr[0] = 0;
		addrListen.sin_addr[1] = 0;
		addrListen.sin_addr[2] = 0;
		addrListen.sin_addr[3] = 0;

		// Bind the Listen Socket
		err = sceNetInetBind(sockListen, &addrListen, sizeof(addrListen));
		if (err != 0) {
			s_eStatus = STATUS_IDLE;
			return ERROR_BINDSOCKET;
		}

		// Listen to the Listen Socket
		err = sceNetInetListen(sockListen, 1);
		if (err != 0) {
			s_eStatus = STATUS_IDLE;
			return ERROR_LISTENSOCKET;
		}

		// Save the Listen Socket for Later
		m_pConnection->listenSocket = sockListen;

		// Finished, Return to IDLE State
		s_eStatus = STATUS_IDLE;

	}

	// data connection listening

	// Now that the Local Data Connection is ready, and once you have
	// sent a request for data (i.e RETR, LIST), all that needs to be done is
	// 1: call startDataAccept to begin Accepting connections
	// 2: call startDataConnect to connect to Server
	// 3: begin Send/Recv Calls

	// success
	return 0;
}


//**********************************************************************************
//	Begins Accepting a Connection from Remote
//	NOTE: Make Sure to Open a Data Connection First
//**********************************************************************************
bool	IFTPClient::startDataAccept()
{
	// only needed if we are using PORT, not PASV
	if ( m_pConnection->usePASV == false )
	{
		// Create Accept Socket
		sockaddr	addrAccept;
		int			cbAddrAccept = sizeof( addrAccept );
		SOCKET		sockClient = sceNetInetAccept( m_pConnection->listenSocket, &addrAccept, &cbAddrAccept );

		if ( sockClient & 0x80000000 )
		{
			return false;
		}

		m_pConnection->dataSocket = sockClient;
		// data socket accepted and set
	}

	//success
	return true;
}

//**********************************************************************************
//	
//**********************************************************************************
bool	IFTPClient::startDataConnect()
{
	u32			err;
	sockaddr_in	addrPort;

	memset( &addrPort, 0, sizeof( sockaddr_in ) );

	addrPort.sin_size = sizeof( sockaddr_in );
	addrPort.sin_family = AF_INET;
	addrPort.sin_port = htons( m_pConnection->dataPort );

	// ip info not needed for PORT
	if ( m_pConnection->usePASV == false )
	{
		addrPort.sin_addr[ 0 ] = 0;
		addrPort.sin_addr[ 1 ] = 0;
		addrPort.sin_addr[ 2 ] = 0;
		addrPort.sin_addr[ 3 ] = 0;
	}
	else
	{
		addrPort.sin_addr[ 0 ] = m_pConnection->remoteip[ 0 ];
		addrPort.sin_addr[ 1 ] = m_pConnection->remoteip[ 1 ];
		addrPort.sin_addr[ 2 ] = m_pConnection->remoteip[ 2 ];
		addrPort.sin_addr[ 3 ] = m_pConnection->remoteip[ 3 ];
	}

	// connecting data port to remote
	if ( sceNetInetConnect( m_pConnection->dataSocket, &addrPort, sizeof( sockaddr_in ) ) )
	{
		return ERROR_CONNECTDATAPORT;
	}
	else
	{
		// data port connected to remote
		return 0;
	}
}

//**********************************************************************************
//	
//**********************************************************************************
void	IFTPClient::SetPASV( bool enabled )
{
//	FTP_LOG( "SetPASV\n" );
	m_pConnection->usePASV = enabled;
}

//**********************************************************************************
//	
//**********************************************************************************
bool	IFTPClient::PASVEnabled() const
{
	return m_pConnection->usePASV;
}

//**********************************************************************************
//	
//**********************************************************************************
int	IFTPClient::ftpNOOP()
{
	sendclientResponseLn( "NOOP" );

	s_eStatus = STATUS_WAITFORNOOP;

	if ( ftpTimeoutISNOT( STATUS_NOOPSUCCESS, ERRTIMEOUT ) < 0 )
	{
		s_eStatus = STATUS_IDLE;

		return ERROR_NOOPFAILED;
	}
	else
	{
		s_eStatus = STATUS_IDLE;

		return 0;
	}
}

//**********************************************************************************
//	
//**********************************************************************************
void	IFTPClient::CHMOD( const char * const file, const char * const perm )
{
	sendclientResponse( "SITE CHMOD " );
	sendclientResponse( perm );
	sendclientResponse( " " );
	sendclientResponseLn( file );

	// TODO: return success/failure (always returns success??)
	// 200 success
}

//**********************************************************************************
//	
//**********************************************************************************
char *	IFTPClient::ftpSYST()
{
	sendclientResponseLn( "SYST" );

	s_eStatus = STATUS_WAITFORSYST;

	if ( ftpTimeoutISNOT( STATUS_215SUCCESS, ERRTIMEOUT ) < 0 )
	{
		s_eStatus = STATUS_IDLE;

		return NULL;
	}
	else
	{
		s_eStatus = STATUS_IDLE;

		return m_pConnection->remoteSystem;
	}
}

//**********************************************************************************
//	
//**********************************************************************************
bool	IFTPClient::RMD( const char * const dir )
{
	sendclientResponse( "RMD " );
	sendclientResponseLn( dir );

	s_eStatus = STATUS_WAITFORRMD;

	if ( ftpTimeoutIS( STATUS_WAITFORRMD, ERRTIMEOUT ) < 0 )
	{
		s_eStatus = STATUS_IDLE;

		return false;
	}

	if ( s_eStatus == STATUS_250SUCCESS )
	{
		s_eStatus = STATUS_IDLE;

		return true;
	}
	else
	{
		s_eStatus = STATUS_IDLE;

		return false;
	}
}

//**********************************************************************************
//	
//**********************************************************************************
bool	IFTPClient::MKD( const char * const dir )
{
	sendclientResponse( "MKD " );
	sendclientResponseLn( dir );

	s_eStatus = STATUS_WAITFORMKD;

	if ( ftpTimeoutIS( STATUS_WAITFORMKD, ERRTIMEOUT ) < 0 )
	{
		s_eStatus = STATUS_IDLE;

		return false;
	}

	if ( s_eStatus == STATUS_257SUCCESS )
	{
		s_eStatus = STATUS_IDLE;

		return true;
	}
	else
	{
		s_eStatus = STATUS_IDLE;

		return false;
	}
}

//**********************************************************************************
//	
//**********************************************************************************
bool	IFTPClient::DELE( const char * const file )
{
	sendclientResponse( "DELE " );
	sendclientResponseLn( file );

	s_eStatus = STATUS_WAITFORDELE;

	if ( ftpTimeoutIS( STATUS_WAITFORDELE, ERRTIMEOUT ) < 0 )
	{
		s_eStatus = STATUS_IDLE;

		return false;
	}

	if ( s_eStatus == STATUS_250SUCCESS )
	{
		s_eStatus = STATUS_IDLE;

		return true;
	}
	else
	{
		s_eStatus = STATUS_IDLE;

		return false;
	}
}

//**********************************************************************************
//	
//**********************************************************************************
bool	IFTPClient::CWD( const char * const dir )
{
	sendclientResponse( "CWD " );
	sendclientResponseLn( dir );

	s_eStatus = STATUS_WAITFORCWD;

	if ( ftpTimeoutIS( STATUS_WAITFORCWD, ERRTIMEOUT ) < 0 )
	{
		s_eStatus = STATUS_IDLE;

		return false;
	}

	if ( s_eStatus == STATUS_250SUCCESS )
	{
		s_eStatus = STATUS_IDLE;

		return true;
	}
	else
	{
		s_eStatus = STATUS_IDLE;

		return false;
	}
}

//**********************************************************************************
//	
//**********************************************************************************
char *	IFTPClient::ftpPWD()
{
	sendclientResponseLn("PWD");

	s_eStatus = STATUS_WAITFORPWD;
	s_eStatus = STATUS_IDLE;

	if ( ftpTimeoutISNOT( STATUS_257SUCCESS, ERRTIMEOUT ) < 0 )
	{
		return NULL;
	}
	else
	{
		return m_pConnection->currDir;
	}
}

//**********************************************************************************
//	
//**********************************************************************************
void IFTPClient::ftpABOR()
{
	s_eStatus = STATUS_FAILED;

	sendclientResponseLn( "ABOR" );
	// TODO: return success/failure
}

//**********************************************************************************
//	
//**********************************************************************************
const char *	IFTPClient::GetRemoteDir()
{
	return m_pConnection->currDir;
}

//**********************************************************************************
//	
//**********************************************************************************
remoteDirent *	IFTPClient::LIST()
{
	// Wait Till Other Events are Finished
	if ( ftpTimeoutISNOT( STATUS_IDLE, ERRTIMEOUT ) < 0 )
	{
		FTP_LOG( "LIST: Blocked by other commands\n" );

		return NULL;
	}

	// Attempt to Open a Data Connection
	if ( openClientDataConnection() < 0 )
	{
		FTP_LOG( "LIST: Unable to open local data connection\n" );

		return NULL;
	}
	else
	{
		// Succesfully Opened Local Data Connection
		// Send Request for Remote File Listing
		sendclientResponseLn( "LIST" );

		// Wait for Data connection accepted for Transfer to Start
		if ( m_pConnection->usePASV == false )
		{
			s_eStatus = STATUS_TRANSFERWAIT;

			if ( ftpTimeoutIS( STATUS_TRANSFERWAIT, ERRTIMEOUT ) < 0 )
			{
				// timed out waiting
				FTP_LOG( "LIST: timed out waiting\n" );

				// close data connection
				closeClientDataConnection();

				s_eStatus = STATUS_IDLE;

				return NULL;
			}
		}
		else
		{
			// PASV mode
			s_eStatus = STATUS_TRANSFERWAIT;
		}

		// Wait For Connection Request from Remote
		startDataAccept();
		// Connect to Remote
		startDataConnect();

		if ( s_eStatus == STATUS_FAILED )
		{
			s_eStatus = STATUS_IDLE;

			return NULL;
		}
		else
		{
			// Begin Receiving Data
			int		c = 0;
			int		i = 0;
			int		currFile = 0;
			int		currInfo = 0;
			int		infoLen = 0;
			int		spaceCount = 0;
			u8		buf[ TRANSFER_BUFFER_SIZE ];

			memset( buf, 0, TRANSFER_BUFFER_SIZE );

			while ( ( c = sceNetInetRecv( m_pConnection->dataSocket, ( u8 * )buf, TRANSFER_BUFFER_SIZE, 0 ) ) > 0 && s_bQuit == false )
			{
			}

			for ( u32 i = 0; i < TRANSFER_BUFFER_SIZE; ++i )
			{
				FTP_LOG( "%X", buf[ i ] );
			}
			FTP_LOG( "\n" );

			if ( s_bQuit == true )
			{
				return NULL;
			}

			memset( &s_Directory, 0, sizeof( remoteDirent ) );

			s_Directory.totalCount = 0;
			s_Directory.dirCount = 0;
			s_Directory.linkCount = 0;
			s_Directory.fileCount = 0;

			while ( i < sizeof( buf ) )
			{
				// INFO ISN'T SEPERATED BY TABS??? checking for spaces instead
				if ( buf[ i ] == '\n' )
				{
					// end the file info
					s_Directory.files[ currFile ].d_name[ infoLen - 1 ] = '\0';

					// keep track of total files/dir
					s_Directory.totalCount++;

					if ( FIO_SO_ISDIR( s_Directory.files[ currFile ].st_attr ) )
					{
						s_Directory.dirCount++;
					}
					else if ( FIO_SO_ISLNK( s_Directory.files[ currFile ].st_attr ) )
					{
						s_Directory.linkCount++;
					}
					else if ( FIO_SO_ISREG( s_Directory.files[ currFile ].st_attr ) )
					{
						s_Directory.fileCount++;
					}

					// clear info for next file
					currFile++;
					currInfo = 0;
					infoLen = 0;
					spaceCount = 0;
				}
				else
				{
					// split line info
					if ( buf[i] == ' ' )
					{
						//drw-rw-rw-   1 user     group           0 Jun 28 19:42 Apps
						// currInfo: (0) permissions (1) owner info  (2) group info  (3) size  (4) date  (5) filename
						if ( infoLen > 0 )
						{
							// this makes sure that we have started getting the next info in the line (there is at least 1 character in the info
							if ( currInfo == 0 )
							{
								// no spaces in permissions, so we reached the end
								s_Directory.files[currFile].perms[infoLen] = '\0';
								if ( s_Directory.files[currFile].perms[0] == 'd' ) {
									s_Directory.files[currFile].st_attr = FIO_SO_IFDIR;
								} else if ( s_Directory.files[currFile].perms[0] == 'l' ) {
									s_Directory.files[currFile].st_attr = FIO_SO_IFLNK;
								} else if ( s_Directory.files[currFile].perms[0] == '-' ) {
									s_Directory.files[currFile].st_attr = FIO_SO_IFREG;
								}

								// start next info
								currInfo++;
								infoLen = 0;
								spaceCount = 0;
							} else if ( currInfo == 1) {
								if ( spaceCount == 1 ) {
									// reached the end of the owner info, only 1 space is in this info
									s_Directory.files[currFile].owner[infoLen] = '\0';
									// start next info
									currInfo++;
									infoLen = 0;
									spaceCount = 0;
								} else {
									// the space is part of the info, just add it
									s_Directory.files[currFile].owner[infoLen] = buf[i];
									infoLen++;
									spaceCount++;
								}
							} else if ( currInfo == 2 ) {
								// no spaces in group, so we reached the end
								s_Directory.files[currFile].group[infoLen] = '\0';
								// start next info
								currInfo++;
								infoLen = 0;
								spaceCount = 0;
							} else if ( currInfo == 3 ) {
								// no spaces in size, so we reached the end
								s_Directory.files[currFile].st_size[infoLen] = '\0';
								// start next info
								currInfo++;
								infoLen = 0;
								spaceCount = 0;
							} else if ( currInfo == 4) {
								if ( spaceCount == 2 ) {
									// reached the end of the date info, only 3 spaces in this info
									s_Directory.files[currFile].st_ctime[infoLen] = '\0';
									// start next info
									currInfo++;
									infoLen = 0;
									spaceCount = 0;
								} else {
									// the space is part of the info, just add it
									s_Directory.files[currFile].st_ctime[infoLen] = buf[i];
									infoLen++;
									spaceCount++;
								}
							} else if ( currInfo == 5 ) {
								//unknown spaces in the filename,  the space is part of the info, just add it
								s_Directory.files[currFile].d_name[infoLen] = buf[i];
								infoLen++;
								spaceCount = 0;
							}
						}
					} else {
						// this is a regular character, just add it to the current info

						if ( currInfo == 0 ) {
							s_Directory.files[currFile].perms[infoLen] = buf[i];
						} else if ( currInfo == 1 ) {
							s_Directory.files[currFile].owner[infoLen] = buf[i];
						} else if ( currInfo == 2 ) {
							s_Directory.files[currFile].group[infoLen] = buf[i];
						} else if ( currInfo == 3 ) {
							s_Directory.files[currFile].st_size[infoLen] = buf[i];
						} else if ( currInfo == 4 ) {
							s_Directory.files[currFile].st_ctime[infoLen] = buf[i];
						} else if ( currInfo == 5 ) {
							s_Directory.files[currFile].d_name[infoLen] = buf[i];
						}

						infoLen++;

					}
					//
				}
				i++;
			}

			// close data connection
			closeClientDataConnection();

			s_eStatus = STATUS_IDLE;

			// success
			return &s_Directory;
		}
	}

	return NULL;
}

int IFTPClient::RETR(char* localdir, char* file) {
	int ret;

	ret = ftpTimeoutISNOT(STATUS_IDLE, ERRTIMEOUT);
	if ( ret < 0 ) return -1;


	// TODO: implement actual TYPE command
	sendclientResponseLn("TYPE I");

	if ( openClientDataConnection() < 0 ) {
		// unable to open data connection
		s_eStatus = STATUS_IDLE;
		return ERROR_NODATACONNECTION;
	} else {
		// succesfully opened data connection

		// setup file info
		char filePath[256];
		strcpy(filePath, localdir);
		strcat(filePath, file);

		// send request for file
		sendclientResponse("RETR ");
		sendclientResponseLn(file);

		// setup buffer
		u8* buff[TRANSFER_BUFFER_SIZE];

		// Wait for Data connection accepted for Transfer to Start
		if (m_pConnection->usePASV == false)
		{
			s_eStatus = STATUS_TRANSFERWAIT;
			ret = ftpTimeoutIS(STATUS_TRANSFERWAIT, ERRTIMEOUT);
			if ( ret < 0 ) {
				// timed out waiting

				// close data connection
				closeClientDataConnection();

				s_eStatus = STATUS_IDLE;
				return ERROR_TIMEOUT;
			}
		} else {
			// PASV mode doesn't wait for a request, it connects
			s_eStatus = STATUS_TRANSFERWAIT;

		}

		// begin server data accept/connect
		startDataAccept();
		startDataConnect();

		if ( s_eStatus >= STATUS_FAILED ) {
			// file not found on server probably
			// close data connection
			closeClientDataConnection();
			s_eStatus = STATUS_IDLE;
			return ERROR_TRANSFERFAILED;
		} else {

			// open file for storing
			int fdFile = sceIoOpen(filePath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);

			//TODO: call SIZE for tracking progress

			// retrieve file
			int c=0;
			int dbytes = 0;
			while ((c=sceNetInetRecv(m_pConnection->dataSocket, (u8*)buff, TRANSFER_BUFFER_SIZE, 0))>0  && s_bQuit == 0 ) {
				// store the amount of bytes that has downloaded;
				m_nRETRSize+=c;

				// redraw transfer every once and a while
				dbytes++;
				if ( dbytes >= 10 ) {
//					debugFooter();
					dbytes = 0;
				}

				sceIoWrite(fdFile, buff, c);
			}

			// close file
			sceIoClose(fdFile);

			// reset size of retr file download to 0
			m_nRETRSize = 0;

			// close data connection
			closeClientDataConnection();

			s_eStatus = STATUS_IDLE;

			// redraw footer
//			debugFooter();

			if ( s_bQuit == 1 ) {
				// quit while RETR was going
				return ERROR_EXITED;
			} else {
				// RETR successful
				return 0;
			}
		}
	}
}


int IFTPClient::RETRThread(SceSize args, void *argp)
{
	FTP_LOG( "begin RETR Thread\n" );

	// parse args
	const char delimiters[] = ",";
	char* localdir;
	char* file;
	localdir = strtok((char*)argp, delimiters);
	file = strtok(NULL, delimiters);

	// code goes here
	s_pInstance->RETR(localdir, file);

	FTP_LOG( "RETR Thread ended, s_eStatus=%d\n", s_eStatus );
	s_eStatus = STATUS_IDLE;
	return 0;

}

int IFTPClient::ftpRETR(char* localdir, char* file) {
	// create args
	char argslist[513];
	strcpy(argslist, localdir);
	strcat(argslist, ",");
	strcat(argslist, file);

	// move non transfer code out to here

	// begin RETR Thread
	int tmp=sceKernelCreateThread("THREAD_FTP_RETR", RETRThread, 0x18, 0x10000, 0, NULL);
	if(tmp >= 0) {
		sceKernelStartThread(tmp, sizeof(argslist), argslist);
	} else {
		// unable to create client thread
		DISPLAY_ERROR( tmp );
		return ERROR_CREATINGTHREAD;
	}

	return 0;
}



// Uploads a File to the Remote System
int IFTPClient::STOR(char* localdir, char* file) {
	int ret;	// return value

	// wait till other events are finished
	ret = ftpTimeoutISNOT(STATUS_IDLE, ERRTIMEOUT);
	if ( ret < 0 ) return ERROR_TIMEOUT;

	// Send TYPE command for File Type (needed?)
	sendclientResponseLn("TYPE I");

	// TODO: call SIZE to check for existing file, for resuming uploads/percent complete

	// Attempt to Open a Data Connection
	if ( openClientDataConnection() < 0 ) {
		// Unable to Open a Local Data Connection
		s_eStatus = STATUS_IDLE;
		return ERROR_NODATACONNECTION;
	} else {
		// Succesfully Opened Local Data Connection


		// Setup File path and name
		char filePath[256];
		strcpy(filePath, localdir);
		strcat(filePath, file);

		// Check if this is a file or not
		SceIoStat fileStats;
		sceIoGetstat(filePath, &fileStats);

		if (FIO_SO_ISREG(fileStats.st_attr)) {
			// this is a file

			// TODO:  directories download

			// Send Request to Retrieve File
			sendclientResponse("STOR ");
			sendclientResponseLn(file);

			// Wait for Data connection accepted for Transfer to Start
			if (m_pConnection->usePASV == false)
			{
				s_eStatus = STATUS_TRANSFERWAIT;
				ret = ftpTimeoutIS(STATUS_TRANSFERWAIT, ERRTIMEOUT);
				if ( ret < 0 ) {
					// timed out waiting

					// close data connection
					closeClientDataConnection();

					s_eStatus = STATUS_IDLE;
					return ERROR_TIMEOUT;
				}
			} else {
				// PASV mode doesn't wait for a request, it connects
				s_eStatus = STATUS_TRANSFERWAIT;
			}

				// Wait For Connection Request from Remote
				startDataAccept();
				// Connect to Remote
				startDataConnect();

			// Make sure that our request didn't fail
			if ( s_eStatus == STATUS_FAILED ) {
				// Transfer failed, no permission

				// close data connection
				closeClientDataConnection();

				s_eStatus = STATUS_IDLE;
				return ERROR_TRANSFERFAILED;
			} else {

				// TODO: setup multiple transfers to send recursive folders/files

				// open file for sending
				int fdFile = sceIoOpen(filePath, PSP_O_RDONLY, 0777);

				// Start Retrieving the File
				char* buf[TRANSFER_BUFFER_SIZE];
				int c=0;
				int dbytes = 0;
				while ((c=sceIoRead(fdFile, buf, TRANSFER_BUFFER_SIZE))>0 && s_bQuit == 0) {
					m_nSTORSize+=c;
					// redraw transfer every once and a while
					dbytes++;
					if ( dbytes >= 10 ) {
//						debugFooter();
						dbytes = 0;
					}
					sceNetInetSend(m_pConnection->dataSocket, buf, c , 0);
				}

				sceIoClose(fdFile);

				// reset size of stor file uploaded to 0
				m_nSTORSize = 0;

				// close data connection
				closeClientDataConnection();

				// Return Status to IDLE
				s_eStatus = STATUS_IDLE;

				if (s_bQuit == 1 ) {
					return ERROR_EXITED;
				}

			}

		} else {
			//error storing..  this is a Directory? ?? no permission file? file missing?
			s_eStatus = STATUS_IDLE;
			return ERROR_TRANSFERFAILED;
		}

		// success
		return 0;
	}
}

int IFTPClient::STORThread(SceSize args, void *argp)
{
	FTP_LOG( "begin STOR Thread\n" );

	// parse args
	const char delimiters[] = ",";
	char* localdir;
	char* file;
	localdir = strtok((char*)argp, delimiters);
	file = strtok(NULL, delimiters);

	// code goes here
	s_pInstance->STOR(localdir, file);

	FTP_LOG( "STOR Thread ended, s_eStatus=%d\n", s_eStatus );
	s_eStatus = STATUS_IDLE;
	return 0;

}

int IFTPClient::ftpSTOR(char* localdir, char* file) {
	// create args
	char argslist[513];
	strcpy(argslist, localdir);
	strcat(argslist, ",");
	strcat(argslist, file);

	// move non transfer code out to here

	// begin RETR Thread
	int tmp=sceKernelCreateThread("THREAD_FTP_STOR", STORThread, 0x18, 0x10000, 0, NULL);
	if(tmp >= 0) {
		sceKernelStartThread(tmp, sizeof(argslist), argslist);
	} else {
		// unable to create client thread
		DISPLAY_ERROR( tmp );
		return ERROR_CREATINGTHREAD;
	}

	return 0;
}





//**********************************************************************************
//**********************************************************************************
//
//   CFTPClient class definition
//
//**********************************************************************************
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
void	CFTPClient::Open()
{
	ASSERT( s_pInstance == NULL, "FTP client already open!" );

	s_pInstance = new IFTPClient();
}

//**********************************************************************************
//
//**********************************************************************************
void	CFTPClient::Close()
{
	ASSERT( s_pInstance != NULL, "FTP client isn't open!" );

	SAFE_DELETE( s_pInstance );
}

//**********************************************************************************
//
//**********************************************************************************
CFTPClient *	CFTPClient::Get()
{
	return s_pInstance;
}

//*******************************  END OF FILE  ************************************
