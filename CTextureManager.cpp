/***********************************************************************************

  Module :	CTextureManager.cpp

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

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CTextureManager.h"
#include "CFileSystem.h"
#include "CFrameWork.h"
#include "CGfx.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************

//**********************************************************************************
//   Static Prototypes
//**********************************************************************************

//**********************************************************************************
//   Global Variables
//**********************************************************************************

//**********************************************************************************
//   Static Variables
//**********************************************************************************

//**********************************************************************************
//   Class Definition
//**********************************************************************************

//*************************************************************************************
//*************************************************************************************
//
//	CTexture manager implementation
//
//*************************************************************************************
//*************************************************************************************

//*************************************************************************************
//
//*************************************************************************************
CTexture *	CTextureManager::Create( const CString & filename, bool warn )
{
	CTexture *		p_texture( NULL );
	CFile * const	p_file( CFileSystem::Open( filename, "rb" ) );

	if ( p_file != NULL )
	{
		const CString	extension( p_file->GetExtension() );

		if ( extension == "png" || extension == "PNG" )
		{
			p_texture = ReadPNG( p_file );
		}
		else if ( extension == "tga" || extension == "TGA" )
		{
			p_texture = ReadTGA( p_file );
		}
		else if ( extension == "jpg" || extension == "JPG" )
		{
			p_texture = ReadJPG( p_file );
		}
		else if ( extension == "bmp" || extension == "BMP" )
		{
			p_texture = ReadBMP( p_file );
		}
		else
		{
			TRACE( "Unrecognised image extension %s\n", extension.GetPtr() );
			ASSERT( 0, "" );
		}

		CFileSystem::Close( p_file );
	}

	if ( p_texture == NULL )
	{
		if ( warn == true )
		{
			TRACE( "Failed to load image %s\n", filename.GetPtr() );
			ASSERT( 0, "" );
		}
	}

	return p_texture;
}


//*************************************************************************************
//*************************************************************************************
//
//	CTexture implementation
//
//*************************************************************************************
//*************************************************************************************

//*************************************************************************************
//
//*************************************************************************************
CTexture::CTexture()
:	m_nWidth( 0 )
,	m_nHeight( 0 )
,	m_nCanvasWidth( 0 )
,	m_nCanvasHeight( 0 )
,	m_pBuffer( NULL )
,	m_pOriginalAddress( NULL )
,	m_pVRAM( NULL )
{
}

//*************************************************************************************
//
//*************************************************************************************
CTexture::~CTexture()
{
	SAFE_RDELETE( m_pOriginalAddress );

	if ( m_pVRAM != NULL )
	{
		CGfx::FreeVRAM( m_nCanvasWidth * m_nHeight * 4 );
	}
}

//**********************************************************************************
//
//**********************************************************************************
bool	CTexture::Init( u32 width, u32 height )
{
	m_nWidth = width;
	m_nHeight = height;
	m_nCanvasWidth = 1;
	m_nCanvasHeight = 1;

	//
	//	Round the width and height up to a power of 2
	//
	while ( m_nCanvasWidth < width )
	{
		m_nCanvasWidth <<= 1;
	}

	while ( m_nCanvasHeight < height )
	{
		m_nCanvasHeight <<= 1;
	}

	//
	//	Align the memory to 16 bytes
	//
	m_pOriginalAddress = new u8[ ( 4 * m_nCanvasWidth * m_nCanvasHeight ) + 16 ];
	m_pBuffer = reinterpret_cast< u8 * >( ( ( reinterpret_cast< u32 >( m_pOriginalAddress ) + 16 ) & 0xfffffff0 ) );

	memset( m_pBuffer, 0xff, 4 * m_nCanvasWidth * m_nCanvasHeight );

	ASSERT( m_pOriginalAddress != NULL, "Out of memory!" );

	return ( m_pOriginalAddress != NULL );
}

//*************************************************************************************
//
//*************************************************************************************
void	CTexture::Save( const CString & file_name )
{
	CFile * const	p_file( CFileSystem::Open( file_name, "wb" ) );

	ASSERT( p_file != NULL, "Failed to open texture file" );

	if ( p_file->Write( m_pBuffer, 4 * m_nCanvasWidth * m_nCanvasHeight ) == false )
	{
		ASSERT( 0, "Failed to save texture" );
	}

	CFileSystem::Close( p_file );
}

//**********************************************************************************
//
//**********************************************************************************
void	CTexture::Upload()
{
//	Swizzle();

	sceKernelDcacheWritebackAll();

	m_pVRAM = CGfx::AllocateVRAM( m_nCanvasWidth * m_nHeight * 4 );

	sceGuStart( GU_DIRECT, CGfx::GetDrawList() );

	sceGuCopyImage( GU_PSM_8888, 0, 0, m_nWidth, m_nHeight, m_nCanvasWidth, m_pBuffer, 0, 0, m_nCanvasWidth, m_pVRAM );

	sceGuFinish();
	sceGuSync( 0, 0 );
}

//**********************************************************************************
//
//**********************************************************************************
void	swizzle_fast( u8* out, const u8* in, unsigned int width, unsigned int height )
{
	unsigned int blockx, blocky;
	unsigned int j;

	unsigned int width_blocks = (width / 16);
	unsigned int height_blocks = (height / 8);

	unsigned int src_pitch = (width-16)/4;
	unsigned int src_row = width * 8;

	const u8* ysrc = in;
	u32* dst = (u32*)out;

	for (blocky = 0; blocky < height_blocks; ++blocky)
	{
		const u8* xsrc = ysrc;
		for (blockx = 0; blockx < width_blocks; ++blockx)
		{
			const u32* src = (u32*)xsrc;
			for (j = 0; j < 8; ++j)
			{
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				src += src_pitch;
			}
			xsrc += 16;
		}
		ysrc += src_row;
	}
}

void	CTexture::Swizzle()
{
	//
	//	Create the swizzle buffer
	//
	const u32	width( m_nCanvasWidth * 4 );
	const u32	texture_size( width * m_nCanvasHeight );
	u8 *		p_buffer( new u8[ texture_size ] );

	swizzle_fast( p_buffer, m_pBuffer, m_nCanvasWidth, m_nCanvasHeight );

	memcpy( m_pBuffer, p_buffer, texture_size );

	SAFE_RDELETE( p_buffer );
}

//*******************************  END OF FILE  ************************************
