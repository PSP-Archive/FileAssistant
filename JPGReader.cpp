/***********************************************************************************

  Module :	JPGReader.cpp

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
#include "CFrameWork.h"
#include "CTextureManager.h"
#include "CFileSystem.h"

extern "C"
{
#include <jpeglib.h>
}

#include <setjmp.h>

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
struct my_error_mgr
{
	struct jpeg_error_mgr	pub;
	jmp_buf					setjmp_buffer;
};

typedef my_error_mgr *	my_error_ptr;

//**********************************************************************************
//   Class Definition
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
void	my_error_exit (j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr) cinfo->err;

	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message) (cinfo);

	ASSERT( 0, "JPG ERROR!!\n" );

	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}

//*************************************************************************************
//
//*************************************************************************************
CTexture *	CTextureManager::ReadJPG( CFile * const p_file )
{
	CTexture *				p_texture( NULL );
	jpeg_decompress_struct	cinfo;
	JSAMPARRAY				buffer;
	my_error_mgr			jerr;

	cinfo.err = jpeg_std_error( &jerr.pub );
	jerr.pub.error_exit = my_error_exit;

	if ( setjmp( jerr.setjmp_buffer ) != 0 )
	{
		jpeg_destroy_decompress( &cinfo );

		return NULL;
	}

	jpeg_create_decompress( &cinfo );

	jpeg_stdio_src( &cinfo, *p_file );

	jpeg_read_header( &cinfo, TRUE );

	jpeg_calc_output_dimensions( &cinfo );

	buffer = ( *cinfo.mem->alloc_sarray )( ( j_common_ptr )&cinfo, JPOOL_IMAGE, cinfo.output_width * cinfo.output_components, 1 );

	jpeg_start_decompress( &cinfo );

	if ( cinfo.out_color_components >= 3 )
	{
		p_texture = new CTexture();

		if ( p_texture != NULL )
		{
			if ( p_texture->Init( cinfo.output_width, cinfo.output_height ) == false )
			{
				SAFE_DELETE( p_texture );
			}
			else
			{
				u8 *	p_dest( p_texture->m_pBuffer );

				while ( cinfo.output_scanline < cinfo.output_height )
				{
					const JSAMPLE *		p_src( *buffer );
					const JDIMENSION	num_scanlines( jpeg_read_scanlines( &cinfo, buffer, 1 ) );

					ASSERT( num_scanlines == 1, "FAILED to read jpeg scanline\n" );

					for ( u32 x = 0; x < cinfo.output_width; ++x )
					{
						for ( s32 c = 0; c < cinfo.out_color_components; ++c )
						{
							p_dest[ ( x << 2 ) + c ] = *p_src++;
						}

						if ( cinfo.out_color_components == 3 )
						{
							p_dest[ ( x << 2 ) + 3 ] = 0xff;
						}
					}

					p_dest += ( p_texture->m_nCanvasWidth << 2 );
				}
			}
		}
	}

	jpeg_finish_decompress( &cinfo );

	jpeg_destroy_decompress( &cinfo );

	return p_texture;
}


//*******************************  END OF FILE  ************************************
