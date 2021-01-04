/***********************************************************************************

  Module :	CFTPClient.h

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

#ifndef CFTPCLIENT_H_
#define CFTPCLIENT_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CString.h"

//**********************************************************************************
//   Macros
//**********************************************************************************

//**********************************************************************************
//   Types
//**********************************************************************************
#define MAX_FILES 				256		// MAX files in a list

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


struct sWLANSetting
{
	CString	m_szName;
	CString	m_szDetails;
};

typedef std::vector< sWLANSetting >	CWLANList;

//**********************************************************************************
//   Constants
//**********************************************************************************

//**********************************************************************************
//   Class definitions
//**********************************************************************************
class CFTPClient
{
	public:

		static void					Open();
		static void					Close();

		static CFTPClient *			Get();

	public:

		virtual bool				Connect( u32 WLAN_connection_number ) = 0;
		virtual bool				Disconnect() = 0;

		virtual const char *		GetRemoteDir() = 0;
		virtual remoteDirent *		LIST() = 0;
		virtual bool				DELE( const char * const file ) = 0;
		virtual bool				CWD( const char * const p_dir ) = 0;
		virtual bool				MKD( const char * const dir ) = 0;
		virtual bool				RMD( const char * const dir ) = 0;
		virtual void				CHMOD( const char * const file, const char * const perm ) = 0;
		virtual void				SetCurrentDirectory( const char * const command ) = 0;
		virtual void				SetPASV( bool enabled ) = 0;
		virtual bool				PASVEnabled() const = 0;

		virtual bool				IsWLANEnabled() const = 0;
		virtual bool				IsWLANPowerEnabled() const = 0;

		virtual const CWLANList &	GetWLANList() const = 0;

	protected:

		virtual ~CFTPClient()		{}
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CFTPCLIENT_H_ */
