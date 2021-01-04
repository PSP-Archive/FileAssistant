/***********************************************************************************

  Module :	CSkinSelectMenu.h

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

#ifndef CSKINSELECTMENU_H_
#define CSKINSELECTMENU_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CWindow.h"

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
class CSkinSelectMenu : public CWindow
{
	public:

		CSkinSelectMenu();
		~CSkinSelectMenu();

		virtual void	SetSize( const V2 & size );
		virtual void	SetFocus( bool focus );
		virtual void	ProcessInput();

		bool			Show();

	protected:

		void			Quit();

		void			AddAllSkins();

		static void		SelectCallback( CWindowTableItem * p_item, u32 item_no );

	private:

		CWindowTable *	m_pSkinList;

		bool			m_bQuit;
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CSKINSELECTMENU_H_ */
