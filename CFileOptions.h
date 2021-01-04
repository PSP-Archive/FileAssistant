/***********************************************************************************

  Module :	CFileOptions.h

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

#ifndef CFILEOPTIONS_H_
#define CFILEOPTIONS_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CWindow.h"
#include "CFileHandler.h"
#include "CFileSystem.h"

//**********************************************************************************
//   Macros
//**********************************************************************************

//**********************************************************************************
//   Types
//**********************************************************************************
class CPRXPlugin;
class CWindowTable;
class CWindowTableItem;

//**********************************************************************************
//   Constants
//**********************************************************************************

//**********************************************************************************
//   Class definitions
//**********************************************************************************
class CFileOptions : public CWindow
{
	public:

		enum eOption
		{
			OPTION_NULL,
			OPTION_EXECUTE,
			OPTION_COPY,
			OPTION_DELETE,
			OPTION_MOVE,
			OPTION_RENAME,

			MAX_OPTIONS
		};

	public:

		CFileOptions( const CFileList & file_list );
		~CFileOptions();

		virtual void				ProcessInput();

	protected:

		void						AddOptions();

		void						ExecuteFile();

		static void					OptionSelectedCallback( CWindowTableItem * p_item, u32 item_no );

		static void					CopyCallback();
		static void					MoveCallback();
		static void					DeleteCallback();
		static void					RenameCallback();

	protected:

		CWindowTable *				m_pOptions;
		CFileList					m_FileList;
		CString						m_szFirstFile;
		const sFileHandlerInfo *	m_pHandleInfo;
		const CPRXPlugin *			m_pPlugin;

		static const CString		s_szOptionNames[ MAX_OPTIONS ];
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CFILEOPTIONS_H_ */
