/***********************************************************************************

  Module :	CMusicPlugin.h

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

#ifndef CMUSICPLUGIN_H_
#define CMUSICPLUGIN_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CPRXManager.h"

//**********************************************************************************
//   Macros
//**********************************************************************************

//**********************************************************************************
//   Types
//**********************************************************************************

//**********************************************************************************
//   Constants
//**********************************************************************************

//**********************************************************************************
//   Class definitions
//**********************************************************************************
class CMusicPlugin : public CPRXPlugin
{
	public:

		CMusicPlugin( const CString & filename, const CString & extension );

		virtual const eType		GetType() const;

		const CString &			GetMenuText() const;

		virtual bool			Load();
		virtual void			Unload();

		bool					Play( const CString & filename ) const;
		void					Stop();
		void					Pause();

	protected:

		typedef void			( *fd_initFunc )( int );
		typedef int				( *fd_loadFunc )( char * );
		typedef int				( *fd_playFunc )( void );
		typedef void			( *fd_pauseFunc )( void );
		typedef int				( *fd_stopFunc )( void );
		typedef void			( *fd_endFunc )( void );

	protected:

		fd_initFunc				m_pInitFunc;
		fd_loadFunc				m_pLoadFunc;
		fd_playFunc				m_pPlayFunc;
		fd_pauseFunc			m_pPauseFunc;
		fd_stopFunc				m_pStopFunc;
		fd_endFunc				m_pEndFunc;
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CMUSICPLUGIN_H_ */
