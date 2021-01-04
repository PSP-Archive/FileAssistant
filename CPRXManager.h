/***********************************************************************************

  Module :	CPRXManager.h

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

#ifndef CPRXMANAGER_H_
#define CPRXMANAGER_H_

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

//**********************************************************************************
//   Constants
//**********************************************************************************

//**********************************************************************************
//**********************************************************************************
//
//   Base PRX plugin class definition
//
//**********************************************************************************
//**********************************************************************************
class CPRXPlugin
{
	public:

		enum eType
		{
			MUSIC_PLUGIN,
			IMAGE_PLUGIN,
			INPUT_PLUGIN,
			EXTERNAL_PLUGIN,

			MAX_PLUGIN_TYPES
		};

		CPRXPlugin( const CString & filename, const CString & extension );
		virtual ~CPRXPlugin();

		virtual bool			Load();
		virtual void			Unload();

		virtual const eType		GetType() const = 0;
		const CString &			GetExtension() const;

		virtual const CString &	GetMenuText() const = 0;

	protected:

		SceUID					m_ModuleID;
		CString					m_szFilename;
		CString					m_szExtension;
};


//**********************************************************************************
//**********************************************************************************
//
//   PRX Manager class definition
//
//**********************************************************************************
//**********************************************************************************
class CPRXManager
{
	public:

		static void							Open();
		static void							Close();

		static void							RegisterPlugin( CPRXPlugin::eType type, const CString & name, const CString & filename, const CString & extension );

		static const CPRXPlugin *			FindPlugin( const CString & extension );

	private:

		typedef std::list< CPRXPlugin * >	CPRXList;

		static CPRXList						s_PRXList;
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CPRXMANAGER_H_ */
