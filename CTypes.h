/***********************************************************************************

  Module :	CTypes.h

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

#ifndef CTYPES_H_
#define CTYPES_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#ifndef WIN32

#include <pspsdk.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <psptypes.h>
#include <pspsysmem.h>
#include <psploadexec.h>
#include <psprtc.h>
#include <pspgu.h>
#include <pspdisplay.h>
#include <pspaudio.h>
#include <pspaudiolibplus.h>
#include <psphprm.h>
#include <pspiofilemgr.h>
#include <pspumd.h>
#include <psppower.h>
#include <pspwlan.h>
#include <psputility.h>
#include <sys/unistd.h>

#else

typedef signed long				s32;
typedef unsigned long			u32;
typedef signed short			s16;
typedef unsigned short			u16;
typedef char					s8;
typedef unsigned char			u8;
typedef float					f32;
typedef unsigned long long		u64;
typedef long long				s64;

#endif

#include <list>
#include <vector>
#include "CMath.h"
#include "CVector.h"
#include "CMatrix.h"
#include "CAssert.h"

//**********************************************************************************
//   Macros
//**********************************************************************************
// Compile-time QASSERT.  Thanks to Jon Jagger
// (http://www.jaggersoft.com) for this trick.
#define compiler_assert( x )	switch( 0 )	{ case 0: case x:; }

#define UNUSED( x )				( x ) = ( x )

#define SAFE_DELETE( x )		\
{								\
	delete x;					\
	x = NULL;					\
}

#define SAFE_RDELETE( x )		\
{								\
	delete [] x;				\
	x = NULL;					\
}

#define MAKE_ARGB( a, r, g, b )	( ( a << 24 ) | ( b << 16 ) | ( g << 8 ) | ( r << 0 ) )

//**********************************************************************************
//   Types
//**********************************************************************************
struct ARGB
{
	ARGB() : color( 0x00000000 )	{}
	ARGB( u32 col ) : color( col )	{}
	ARGB( u8 r, u8 g, u8 b ) : color( MAKE_ARGB( 255, r, g, b ) )		{}
	ARGB( u8 a, u8 r, u8 g, u8 b ) : color( MAKE_ARGB( a, r, g, b ) )	{}

	union
	{
		u32	color;

		struct
		{
			u8	r, g, b, a;//a, b, g, r;
		};
	};
};

inline int	fchop( float f ) { return (int) f; }		// replace w/ inline asm if desired
inline int	frnd(float f) { return fchop(f + 0.5f); }	// replace with inline asm if desired

//**********************************************************************************
//   Constants
//**********************************************************************************
#define _TU_LITTLE_ENDIAN_	1

//**********************************************************************************
//   Class definitions
//**********************************************************************************

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************


#endif /* CTYPES_H_ */
