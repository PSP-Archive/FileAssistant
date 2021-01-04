/***********************************************************************************

  Module :	COptionsMenu.h

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

#ifndef COPTIONSMENU_H_
#define COPTIONSMENU_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CWindow.h"
#include "CFileHandler.h"

//**********************************************************************************
//   Macros
//**********************************************************************************

//**********************************************************************************
//   Types
//**********************************************************************************
class CWindowTable;
class CWindowTableItem;

//**********************************************************************************
//   Constants
//**********************************************************************************

//**********************************************************************************
//   Class definitions
//**********************************************************************************
class COptionsMenu : public CWindow
{
	public:

		enum eOption
		{
			OPTION_MAKE_DIRECTORY,
			OPTION_USB_CONNECTION,
			OPTION_SKIN_SELECTION,
			OPTION_HIDE_CORRUPT_FILES,
			OPTION_STOP_MUSIC,
			OPTION_CREDITS,
			OPTION_HIDE_VISUALISER,
			OPTION_TOGGLE_ECHO_FX,
			OPTION_TOGGLE_SPATIAL_FX,

			MAX_OPTIONS
		};

	public:

		COptionsMenu();
		~COptionsMenu();

		virtual void				Delete();
		virtual void				Process();
		virtual void				ProcessInput();
		virtual void				SetFocus( bool focus );

	protected:

		void						AddOptions();

		static void					RunCredits();

		static void					OptionSelectedCallback( CWindowTableItem * p_item, u32 item_no );

	protected:

		CWindowTable *				m_pOptions;

		static const CString		s_szOptionNames[ MAX_OPTIONS ];
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* COPTIONSMENU_H_ */
