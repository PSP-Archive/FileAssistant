/***********************************************************************************

  Module :	CTextInput.h

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

#ifndef CTEXTINPUT_H_
#define CTEXTINPUT_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CWindow.h"
#include "CKeyboard.h"

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
class CTextInput : public CWindow, public CKeyboardListener
{
	public:

		enum eReturnCode
		{
			RC_OK,
			RC_CANCEL,
		};

		CTextInput( const CString & title, const CString & text );
		~CTextInput();

		virtual void	Render();

		eReturnCode		Show();

		const CString &	GetText() const;

		virtual void	Message( CKeyboardListener::eMessage message, const void * const p_data );

	private:

		CString			m_szText;

		CKeyboard *		m_pKeyboard;
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CTEXTINPUT_H_ */
