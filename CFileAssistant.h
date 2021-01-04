/***********************************************************************************

  Module :	CFileAssistant.h

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

#ifndef CFILEASSISTANT_H_
#define CFILEASSISTANT_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CProcess.h"
#include "CDirectoryList.h"

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
class CFileAssistant : public CProcess
{
	public:

		static void				Create();
		static void				Destroy();

		static CFileAssistant *	Get();

	public:

		virtual void			Process();
		virtual void			ProcessInput();

		CDirectoryList *		GetSrcList() const;
		CDirectoryList *		GetDstList() const;
		CDirectoryList *		GetFocusList() const;

		void					SetListFocus( bool focus );

	private:

		CFileAssistant();
		~CFileAssistant();

	private:

		CDirectoryList *		m_pSrcList;
		CDirectoryList *		m_pDstList;

		CDirectoryList *		m_pListFocus;

		float					m_fSrcScale;
		float					m_fDstScale;

		float					m_FadeInLevel;

		bool					m_bSrcInFocus;

	private:

		static CFileAssistant *	s_pInstance;
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CFILEASSISTANT_H_ */
