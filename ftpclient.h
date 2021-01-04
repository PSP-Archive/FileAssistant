/*
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
*/
#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include "my_socket.h"

#define TRANSFER_BUFFER_SIZE 	4096	// Buffer size for transfer
#define MAX_FILES 				256		// MAX files in a list
#define ERRTIMEOUT				15		// TIMEOUT in seconds

// TODO seperate states for local and remote to prevent unknown status change while waiting for a specific status

// Various Error Codes
#define ERROR_UNKNOWN				-1
#define ERROR_TIMEOUT				-100
#define ERROR_NOIP					-101
#define ERROR_CREATECONNECTSOCKET	-102
#define ERROR_BINDCONNECTSOCKET		-103
#define ERROR_PORTFAILED			-104
#define ERROR_CREATELISTENSOCKET	-105
#define ERROR_BINDSOCKET			-106
#define ERROR_LISTENSOCKET			-107
#define ERROR_ACCEPTSOCKET			-108
#define ERROR_CONNECTDATAPORT		-109
#define ERROR_NODATACONNECTION		-110
#define ERROR_TRANSFERFAILED		-111
#define ERROR_CREATECOMSOCKET		-112
#define ERROR_UNABLETOCONNECT		-113
#define ERROR_NETDRIVERLOAD			-114
#define ERROR_NLHINIT				-115
#define ERROR_NOSTATICCONNECTION	-116
#define ERROR_NETAPCTLCONNECT		-117
#define ERROR_CREATINGTHREAD		-118
#define ERROR_CWDFAILED				-119
#define ERROR_DELEFAILED			-120
#define ERROR_EXITED				-200

// Various APCTL Status
#define APCTL_IDLE				0
#define APCTL_INITIALIZING		1
#define APCTL_ASSOCIATING		2
#define APCTL_DHCPQUERY			3
#define APCTL_READY				4

// Various States the Connection May Be In
#define STATUS_NOTCONNECTED 	0
#define STATUS_WAITFORUSER		1
#define STATUS_WAITFORPASS		2
#define STATUS_LOGGEDIN			3
#define STATUS_SENDPASV			4
#define STATUS_ENTERPASV		5
#define STATUS_SENDPORT			6
#define STATUS_PORTSUCCESS		7
#define STATUS_WAITFORSYST		8
#define STATUS_WAITFORPWD		9
#define STATUS_WAITFORRMD		10
#define STATUS_WAITFORMKD		11
#define STATUS_WAITFORCWD		12
#define STATUS_WAITFORDELE		13
#define STATUS_215SUCCESS		14
#define STATUS_250SUCCESS		15
#define STATUS_257SUCCESS		16
#define STATUS_TRANSFERWAIT		17
#define STATUS_TRANSFERRING 	18
#define STATUS_FAILED			19
#define STATUS_IDLE				20

// connection used
typedef struct MclientConnection {
	SOCKET comSocket;
	SOCKET listenSocket;
	SOCKET dataSocket;

	int usePASV;
	int netconn;
	int dataPort;
	int data_portA;
	int data_portB;

	char localip[32];
	char remoteip[4];
	int  remoteport;
	char comBuffer[1024];
	char dataBuffer[1024];

	char* username;
	char* password;
	char currDir[256];
	char remoteSystem[32];

} MclientConnection;

// file structure for remote files
typedef struct remoteFileent {
	char perms[10];
	char owner[10];
	char group[10];
	unsigned int st_attr;	// file type (DIR, LINK, FILE
	char st_size[12];		// size of file
	char d_name[256];		// name of file
	char st_ctime[12];		// creation time
} remoteFileent;

// directory structure for a directory listing
typedef struct remoteDirent {
	int totalCount;
	int dirCount;
	int fileCount;
	int linkCount;
	struct remoteFileent files[MAX_FILES];
} remoteDirent;

// helpful net functions
int ftpGetWLANSwitch(void);					// returns if WLAN switch is on or off
int ftpGetWLANPower(void);					// returns if WLAN Power save is on?
int ftpGetEtherAddr(char *etherAddr);		// gets ethernet address, pass a pointer to a buffer of chars at least length 19
char* ftpGetRemoteIP(void);					// returns the remote ip address
int ftpGetRemotePort(void);					// returns the remote port
int ftpApctlStatus(void);					// returns the current status of apctl (networking)

// implementation functions
int startFTP(SceModuleInfo *modInfoPtr);	// starts the ftp thread loop and waits for a ftpConnect() call
int ftpConnect(char* ip, int port);			// connects to remote address
int ftpLogin(char* user, char* pass);		// called once connected, to login
void ftpDisconnect(void);					// disconnects from remote if connected
void quitFTP(void);							// quits the ftp thread loop

int ftpPASVEnabled(void);					// returns if pasv is enabled
char* ftpGetRemoteDir(void);				// returns the current remote directory (without asking the server)
long int ftpCurrentRETR(void);				// returns downloaded size of current download
long int ftpCurrentSTOR(void);				// returns downloaded size of current upload

int ftpStatus(void);						// returns the current status of the client
int ftpExited(void);						// returns if the client has exited the program
int ftpConnected(void);						// returns if currently connected  ( 1 = Connected, 0 = Not Connected)
int ftpLoggedIn(void);						// returns 1 if currently logged in   ( 1 = Logged In, 0 = Not Logged In)

void ftpSetPASV(int enabled);				// enable/disable PASV

// ftp commands
int ftpRETR(char* location, char* file);	// download a file
int ftpSTOR(char* localdir, char* file);	// uploads a file
int ftpDELE(char* file);					// deletes a file
int ftpCWD(char* dir);						// change directory
int ftpMKD(char* dir);						// makes a directory
int ftpRMD(char* dir);						// removes a directory

void ftpABOR(void);							// sends abort command
void ftpCHMOD(char* file, char* perm);		// sets file permissions of a remote file

char* ftpSYST(void);						// returns system type of remote
char* ftpPWD(void);							// returns current directory

remoteDirent* ftpLIST(void);				// returns a list of files
// long int ftpSIZE(char* file);						// gets the size of a remote file

#endif
