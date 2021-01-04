/***********************************************************************************

  Module :	CKeyboard.h

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

#ifndef CKEYBOARD_H_
#define CKEYBOARD_H_

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

//**********************************************************************************
//   Constants
//**********************************************************************************

//**********************************************************************************
//**********************************************************************************
//
//   Keyboard listener class definition
//
//**********************************************************************************
//**********************************************************************************
class CKeyboardListener
{
	public:

		enum eMessage
		{
			MSG_CHARACTER,
			MSG_CURSOR,
			MSG_DELETE,
		};

		virtual ~CKeyboardListener()	{}

		virtual void	Message( eMessage message, const void * const p_data ) = 0;
};

//**********************************************************************************
//**********************************************************************************
//
//   Keyboard class definition
//
//**********************************************************************************
//**********************************************************************************
class CKeyboard : public CWindow
{
	public:

		enum
		{
			NUM_ROWS = 4
		};

	public:

		CKeyboard( CKeyboardListener * const p_listener );
		virtual ~CKeyboard()	{}

		virtual void		Render();
		virtual void		ProcessInput();

		char				GetCurrentKey() const;

	private:

		CKeyboardListener *	m_pListener;

		const CString *		m_pCurrentLayout;

		float				m_fWidestRow;
		float				m_fRowWidth[ NUM_ROWS ];

		s32					m_nRow;
		s32					m_nColumn;

		SceUID				m_IRDA;
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CKEYBOARD_H_ */
