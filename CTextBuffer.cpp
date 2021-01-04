/***********************************************************************************

  Module :	CTextBuffer.cpp

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
#include "CTextBuffer.h"
#include "CFileSystem.h"

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

//**********************************************************************************
//	
//**********************************************************************************
CTextBuffer::CTextBuffer()
:	m_nMaxWidth( 0 )
,	m_pBuffer( NULL )
{
}

//**********************************************************************************
//	
//**********************************************************************************
CTextBuffer::~CTextBuffer()
{
	SAFE_DELETE( m_pBuffer );
}

//**********************************************************************************
//
//**********************************************************************************
bool	CTextBuffer::ProcessTextFile( const CString & filename )
{
	CFile * const	p_file( CFileSystem::Open( filename, "rt" ) );

	if ( p_file != NULL )
	{
		const u32	file_len( p_file->GetLength() );

		m_pBuffer = new char[ file_len ];

		if ( m_pBuffer != NULL )
		{
			if ( p_file->Read( m_pBuffer, file_len ) == true )
			{
				m_LineList.push_back( &m_pBuffer[ 0 ] );

				for ( u32 i = 1; i < file_len; ++i )
				{
					if ( m_pBuffer[ i - 1 ] == '\n' )
					{
						m_pBuffer[ i - 1 ] = '\0';

						m_LineList.push_back( &m_pBuffer[ i ] );
					}
				}

				m_nMaxWidth = 0;

				for ( u32 i = 0; i < m_LineList.size(); ++i )
				{
					if ( strlen( m_LineList[ i ] ) > m_nMaxWidth )
					{
						m_nMaxWidth = strlen( m_LineList[ i ] );
					}
				}

				CFileSystem::Close( p_file );

				return true;
			}
		}

		CFileSystem::Close( p_file );
	}

	return false;
}

//**********************************************************************************
//
//**********************************************************************************
const char *	CTextBuffer::GetLine( u32 index ) const
{
	ASSERT( index < GetMaxHeight(), "Index out of range!" );

	return m_LineList[ index ];
}

//**********************************************************************************
//
//**********************************************************************************
u32	CTextBuffer::GetMaxWidth() const
{
	return m_nMaxWidth;
}

//**********************************************************************************
//
//**********************************************************************************
u32	CTextBuffer::GetMaxHeight() const
{
	return m_LineList.size();
}

//*******************************  END OF FILE  ************************************
