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
/***********************************************************************************

  Module :	CCredits.h

  Description :

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 08 September 2005 71M

***********************************************************************************/

#ifndef CCREDITS_H_
#define CCREDITS_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CProcess.h"

//**********************************************************************************
//   Macros
//**********************************************************************************

//**********************************************************************************
//   Types
//**********************************************************************************
class CCamera;

//**********************************************************************************
//   Constants
//**********************************************************************************

//**********************************************************************************
//   Class definitions
//**********************************************************************************
class CCredits : public CProcess
{
	public:

		static void				Show();

	public:

		static void				Render();

		virtual void			Process();
		virtual void			ProcessInput();

		bool					HasQuit() const;

	protected:

		void					RenderCredits();
		void					RenderInternal();

		void					UpdateCamera();
		void					PickNewCameraOrientation();

		void					ShakeCubes();

	protected:

		CCredits();
		~CCredits();

	private:

		static CCredits *		s_pInstance;

	private:

		CCamera *				m_pCamera;
		V3						m_CamAngle;

		float					m_CameraTimer;

		float					m_fCamFocus;
		V3						m_fCamOffset;
		float					m_fCamRotation;
		float					m_fWantedCamFocus;
		V3						m_fWantedCamOffset;

		bool					m_bQuit;
};

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* CCredits_H_ */
