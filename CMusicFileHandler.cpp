/***********************************************************************************

  Module :	CMusicFileHandler.cpp

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
#include "CMusicFileHandler.h"
#include "Codec.h"

#include "AHX/AHX.h"
#include "MOD/MODPlayer.h"
#include "MP3/MP3Player.h"
#include "OGG/OGGPlayer.h"
#include "YMPlay/Player.h"
#include "sc68/sc68Player.h"
#include "sid/SIDPlayer.h"
#include "sap/SAPplayer.h"
#include "nsf/NSFplayer.h"

#include "CFrameWork.h"
#include "CFileSystem.h"
#include "CMessageBox.h"
#include "CRenderable.h"
#include "CHUD.h"
#include "CInput.h"
#include "CGfx.h"
#include "CTextureManager.h"
#include "CBackground.h"
#include "CFont.h"
#include "CSkinManager.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************
static const s32	NUM_POWER_BUFS( 32 );
static const s32	RENDER_TARGET_WIDTH( 256 );
static const s32	RENDER_TARGET_HEIGHT( 128 );

static const s32	brtbl[ 256 ]=
{
	0,128, 64,192, 32,160, 96,224, 16,144, 80,208, 48,176,112,240,  8,136, 72,200, 40,168,104,232, 24,152, 88,216, 56,184,120,248,
	4,132, 68,196, 36,164,100,228, 20,148, 84,212, 52,180,116,244, 12,140, 76,204, 44,172,108,236, 28,156, 92,220, 60,188,124,252,
	2,130, 66,194, 34,162, 98,226, 18,146, 82,210, 50,178,114,242, 10,138, 74,202, 42,170,106,234, 26,154, 90,218, 58,186,122,250,
	6,134, 70,198, 38,166,102,230, 22,150, 86,214, 54,182,118,246, 14,142, 78,206, 46,174,110,238, 30,158, 94,222, 62,190,126,254,
	1,129, 65,193, 33,161, 97,225, 17,145, 81,209, 49,177,113,241,  9,137, 73,201, 41,169,105,233, 25,153, 89,217, 57,185,121,249,
	5,133, 69,197, 37,165,101,229, 21,149, 85,213, 53,181,117,245, 13,141, 77,205, 45,173,109,237, 29,157, 93,221, 61,189,125,253,
	3,131, 67,195, 35,163, 99,227, 19,147, 83,211, 51,179,115,243, 11,139, 75,203, 43,171,107,235, 27,155, 91,219, 59,187,123,251,
	7,135, 71,199, 39,167,103,231, 23,151, 87,215, 55,183,119,247, 15,143, 79,207, 47,175,111,239, 31,159, 95,223, 63,191,127,255,
};

static const s32	mm1[ 8 ][ 128 ] =
{
	{	16384,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, },
	{	16384,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, },
	{	16384, 11585,     0,-11585,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, },
	{	16384, 15136, 11585,  6269,     0, -6269,-11585,-15136,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, },
	{	16384, 16069, 15136, 13622, 11585,  9102,  6269,  3196,     0, -3196, -6269, -9102,-11585,-13622,-15136,-16069,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, },
	{	16384, 16305, 16069, 15678, 15136, 14449, 13622, 12665, 11585, 10393,  9102,  7723,  6269,  4756,  3196,  1605,     0, -1605, -3196, -4756, -6269, -7723, -9102,-10393,-11585,-12665,-13622,-14449,-15136,-15678,-16069,-16305,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, },
	{	16384, 16364, 16305, 16206, 16069, 15892, 15678, 15426, 15136, 14810, 14449, 14053, 13622, 13159, 12665, 12139, 11585, 11002, 10393,  9759,  9102,  8423,  7723,  7005,  6269,  5519,  4756,  3980,  3196,  2404,  1605,   803,     0,  -803, -1605, -2404, -3196, -3980, -4756, -5519, -6269, -7005, -7723, -8423, -9102, -9759,-10393,-11002,-11585,-12139,-12665,-13159,-13622,-14053,-14449,-14810,-15136,-15426,-15678,-15892,-16069,-16206,-16305,-16364,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, },
	{	16384, 16379, 16364, 16339, 16305, 16260, 16206, 16142, 16069, 15985, 15892, 15790, 15678, 15557, 15426, 15286, 15136, 14978, 14810, 14634, 14449, 14255, 14053, 13842, 13622, 13395, 13159, 12916, 12665, 12406, 12139, 11866, 11585, 11297, 11002, 10701, 10393, 10079,  9759,  9434,  9102,  8765,  8423,  8075,  7723,  7366,  7005,  6639,  6269,  5896,  5519,  5139,  4756,  4369,  3980,  3589,  3196,  2801,  2404,  2005,  1605,  1205,   803,   402,     0,  -402,  -803, -1205, -1605, -2005, -2404, -2801, -3196, -3589, -3980, -4369, -4756, -5139, -5519, -5896, -6269, -6639, -7005, -7366, -7723, -8075, -8423, -8765, -9102, -9434, -9759,-10079,-10393,-10701,-11002,-11297,-11585,-11866,-12139,-12406,-12665,-12916,-13159,-13395,-13622,-13842,-14053,-14255,-14449,-14634,-14810,-14978,-15136,-15286,-15426,-15557,-15678,-15790,-15892,-15985,-16069,-16142,-16206,-16260,-16305,-16339,-16364,-16379, },
};

static const s32	mm2[ 8 ][ 128 ] =
{
	{	0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, },
	{	0,-16384,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, },
	{	0,-11585,-16384,-11585,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, },
	{	0, -6269,-11585,-15136,-16383,-15136,-11585, -6269,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, },
	{	0, -3196, -6269, -9102,-11585,-13622,-15136,-16069,-16383,-16069,-15136,-13622,-11585, -9102, -6269, -3196,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, },
	{	0, -1605, -3196, -4756, -6269, -7723, -9102,-10393,-11585,-12665,-13622,-14449,-15136,-15678,-16069,-16305,-16384,-16305,-16069,-15678,-15136,-14449,-13622,-12665,-11585,-10393, -9102, -7723, -6269, -4756, -3196, -1605,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, },
	{	0,  -803, -1605, -2404, -3196, -3980, -4756, -5519, -6269, -7005, -7723, -8423, -9102, -9759,-10393,-11002,-11585,-12139,-12665,-13159,-13622,-14053,-14449,-14810,-15136,-15426,-15678,-15892,-16069,-16206,-16305,-16364,-16384,-16364,-16305,-16206,-16069,-15892,-15678,-15426,-15136,-14810,-14449,-14053,-13622,-13159,-12665,-12139,-11585,-11002,-10393, -9759, -9102, -8423, -7723, -7005, -6269, -5519, -4756, -3980, -3196, -2404, -1605,  -803,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, },
	{	0,  -402,  -803, -1205, -1605, -2005, -2404, -2801, -3196, -3589, -3980, -4369, -4756, -5139, -5519, -5896, -6269, -6639, -7005, -7366, -7723, -8075, -8423, -8765, -9102, -9434, -9759,-10079,-10393,-10701,-11002,-11297,-11585,-11866,-12139,-12406,-12665,-12916,-13159,-13395,-13622,-13842,-14053,-14255,-14449,-14634,-14810,-14978,-15136,-15286,-15426,-15557,-15678,-15790,-15892,-15985,-16069,-16142,-16206,-16260,-16305,-16339,-16364,-16379,-16384,-16379,-16364,-16339,-16305,-16260,-16206,-16142,-16069,-15985,-15892,-15790,-15678,-15557,-15426,-15286,-15136,-14978,-14810,-14634,-14449,-14255,-14053,-13842,-13622,-13395,-13159,-12916,-12665,-12406,-12139,-11866,-11585,-11297,-11002,-10701,-10393,-10079, -9759, -9434, -9102, -8765, -8423, -8075, -7723, -7366, -7005, -6639, -6269, -5896, -5519, -5139, -4756, -4369, -3980, -3589, -3196, -2801, -2404, -2005, -1605, -1205,  -803,  -402, },
};

//**********************************************************************************
//   Static Prototypes
//**********************************************************************************

//**********************************************************************************
//   Global Variables
//**********************************************************************************

//**********************************************************************************
//   Static Variables
//**********************************************************************************
static sFileExtensionInfo	s_FileExtensionInfo =
{
	"Play",
	CSkinManager::SC_ICON_AUDIO
};

static codecStubs			s_Stubs[ 100 ];
static codecStubs *			s_pDecoder( NULL );
static u32					s_CodecNum( 0 );
static bool					s_bMusicIsPlaying( false );
static bool					s_bMusicIsPaused( false );
static CString				s_szFilePlaying;
static CTexture *			s_pBuffer[ 2 ] = { NULL, NULL };
static u32					s_nPowerFrame( 0 );
static s16					s_PowerBuf[ NUM_POWER_BUFS ][ 256 ];
static u32					s_CurrentPowerBuf( 0 );
static u32					s_nLastPowerBuf[ 2 ] = { 0, 0 };
static float				s_VisualiserRotation( 0.f );
static bool					s_bShowVisualiser( true );

//**********************************************************************************
//   Class Definition
//**********************************************************************************

//**********************************************************************************
//
//**********************************************************************************
void	CMusicFileHandler::Open()
{
	//get codecStubs
	int	stubnum( 0 );

	//CODEC_INITSTUBS
	OGGsetStubs( &s_Stubs[ stubnum++ ] );
	MODsetStubs( &s_Stubs[ stubnum++ ] );
	AHXsetStubs( &s_Stubs[ stubnum++ ] );
	MP3setStubs( &s_Stubs[ stubnum++ ] );
	YMPLAYsetStubs( &s_Stubs[ stubnum++ ] );
	SC68setStubs( &s_Stubs[ stubnum++ ] );
	SIDsetStubs( &s_Stubs[ stubnum++ ] );
	SAPsetStubs( &s_Stubs[ stubnum++ ] );
	NSFsetStubs( &s_Stubs[ stubnum++ ] );

	s_CodecNum = stubnum;

	s_szFilePlaying = "";

	pspAudioInit();

	//
	//	Clear the power buffers
	//
	for ( s32 i = 0; i < NUM_POWER_BUFS; ++i )
	{
		memset( s_PowerBuf[ i ], 0, 256 );
	}

	CRenderable::Register( CRenderable::RO_BACKGROUND, CMusicFileHandler::Render );
}

//**********************************************************************************
//
//**********************************************************************************
void	CMusicFileHandler::Close()
{
	Stop();

	// Shutdown engines
	for ( u32 codec = 0; codec <= s_CodecNum; ++codec )
	{
		s_pDecoder = &s_Stubs[ codec ];

		if ( s_pDecoder != NULL )
		{
			//s_pDecoder->shutdown();
		}
	}

	pspAudioEnd();

	CRenderable::UnRegister( CRenderable::RO_WINDOWS, CMusicFileHandler::Render );
}

//**********************************************************************************
//
//**********************************************************************************
const sFileExtensionInfo &	CMusicFileHandler::Information( const CString & file )
{
	s_FileExtensionInfo.m_szExecutionName = "Play";

	return s_FileExtensionInfo;
}

//**********************************************************************************
//
//**********************************************************************************
bool	CMusicFileHandler::Execute( const CString & filename )
{
	s_VisualiserRotation = 0.f;

	Stop();

	if ( filename != s_szFilePlaying )
	{
		switch ( Play( filename ) )
		{
		case NO_CODEC:
			{
				CErrorMessage( "No codec found!" );
			}
			break;

		case REPLAY_ERROR:
			{
				CErrorMessage( "Music file is invalid!" );
			}
			break;

		default:
			{
			}
			break;
		}
	}

	return true;
}

//**********************************************************************************
//
//**********************************************************************************
CMusicFileHandler::eError	CMusicFileHandler::Play( const CString & filename )
{
	CString	extension( CFileSystem::GetFileExtension( filename ) );

	extension.ToLower();

	// Determine codec of the file
	s_pDecoder = NULL;

	for ( u32 codec = 0; codec <= s_CodecNum; ++codec )
	{
		const char *	p_extension( &( s_Stubs[ codec ].extension[ 0 ] ) );

		while ( *p_extension != '\0' )
		{
			CString	codec_extension( p_extension );

			codec_extension.ToLower();

			if ( codec_extension == extension || codec_extension.Find( extension ) != NULL)
			{
				s_pDecoder = &s_Stubs[ codec ];

				break;
			}

			p_extension += 4;
		}
	}

	if ( s_pDecoder == NULL )
	{
		return NO_CODEC;
	}

	s_pDecoder->init( 0 );

	if ( s_pDecoder->load( const_cast< char * >( filename.GetPtr() ) ) )
	{
		s_bMusicIsPlaying = true;

		s_pDecoder->play();
	}
	else
	{
		s_bMusicIsPlaying = false;

		s_pDecoder->stop();
		s_pDecoder->end();

		// Cannot replay file...
		return REPLAY_ERROR;
	}

	// ok !
	s_bMusicIsPaused = false;

	s_szFilePlaying = filename;

	// Create the visualiser texture
	CGfx::FreeAllVRAM();
	CSkinManager::UploadFontTexture();

	for ( u32 i = 0; i < 2; ++i )
	{
		s_pBuffer[ i ] = new CTexture();

		if ( s_pBuffer[ i ] != NULL )
		{
			if ( s_pBuffer[ i ]->Init( RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT ) == true )
			{
				s_pBuffer[ i ]->Upload();
			}
			else
			{
				SAFE_DELETE( s_pBuffer[ i ] );
			}
		}
	}

	return SUCCESS;
}

//**********************************************************************************
//
//**********************************************************************************
void	CMusicFileHandler::Stop()
{
	if ( s_bMusicIsPlaying == true )
	{
		if ( s_pDecoder != NULL )
		{
			s_pDecoder->stop();
			s_pDecoder->end();
		}

		s_szFilePlaying = "";

		s_bMusicIsPlaying = false;

		SAFE_DELETE( s_pBuffer[ 0 ] );
		SAFE_DELETE( s_pBuffer[ 1 ] );

		CGfx::FreeAllVRAM();
		CSkinManager::UploadTextures();
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CMusicFileHandler::Pause()
{
	if ( s_bMusicIsPlaying == true )
	{
		if ( s_pDecoder != NULL )
		{
			s_pDecoder->pause();
		}

		s_bMusicIsPaused = !s_bMusicIsPaused;
	}
}

//**********************************************************************************
//
//**********************************************************************************
bool	CMusicFileHandler::IsPlaying()
{
	return s_bMusicIsPlaying;
}

//**********************************************************************************
//
//**********************************************************************************
CString	CMusicFileHandler::GetPlayingTime()
{
	if ( s_pDecoder != NULL )
	{
		char	time[ 200 ];

		s_pDecoder->time( time );

		return time;
	}
	else
	{
		return "00:00:00";
	}
}

//**********************************************************************************
//
//**********************************************************************************
CString	CMusicFileHandler::GetInfo()
{
	CString	infotext;

	if ( s_pDecoder != NULL )
	{
		if ( s_pDecoder->tuneInfo != NULL )
		{
			Generic_TuneInfo	info;

			s_pDecoder->tuneInfo( &info );

			infotext.Printf( " %s - %s - %s (%s) -", s_szFilePlaying.GetPtr(), info.author, info.title, info.time );

			return infotext;
		}
		else
		{
			infotext.Printf( "Playing : %s - ", s_szFilePlaying.GetPtr() );

			return infotext;
		}
	}

	return "";
}

//**********************************************************************************
//
//**********************************************************************************
const CString &		CMusicFileHandler::GetFilePlaying()
{
	return s_szFilePlaying;
}

//**********************************************************************************
//	
//**********************************************************************************
void	PowerCalc( s16 * in )
{
//	s16 *	pp( s_PowerBuf[ s_CurrentPowerBuf ] );

//	pspAudioGetFreq( in, pp );

	static s16	powersrc[ 256 ];
	static s16	powersrc2[ 256 ];

	for ( s32 i = 0; i < 256 - 16; i++ )
	{
		powersrc[ i ] = powersrc[ i + 16 ];
	}

	for ( s32 i = 0; i < 16; i++ )
	{
		powersrc[ 256 - 16 + i ] = ( ( s32 )in[ i * 32 ] + ( s32 )in[ i * 32 + 8 ] + ( s32 )in[ i * 32 + 16 ] + ( s32 )in[ i * 32 + 24 ] ) / 4;
	}

	for ( s32 i = 0; i < 256; i++ )
	{
		powersrc2[ i ] = powersrc[ i ];
	}

	for ( s32 i = 0; i < 32; i++ )
	{
		powersrc2[ i ] = ( ( s32 )powersrc2[ i ] ) * i / 32;
		powersrc2[ 255 - i ] = ( ( s32 )powersrc2[ 255 - i ] ) * i / 32;
	}

	s32	ix[ 256 ], iy[ 256 ];

	//shuffle
	for ( s32 i = 0; i < 256; i++ )
	{
		ix[ i ] = ( s32 )( powersrc2[ brtbl[ i ] ] >> 1 );
		iy[ i ] = 0;
	}

	//fft main
	s32	j;
	s32	l2 = 1;

	for ( s32 l = 0; l < 8; l++ )
	{
		const s32	l1( l2 );

		l2 <<= 1;

		for ( j = 0; j < l1; j++ )
		{
			const s32	m1( mm1[ l ][ j ] / 64 );
			const s32	m2( mm2[ l ][ j ] / 64 );

			for ( s32 i = j; i < 256; i += l2 )
			{
				const s32	i1 = i + l1;
				const s32	tx( ( m1 * ix[ i1 ] - m2 * iy[ i1 ] ) / 256 );
				const s32	ty( ( m1 * iy[ i1 ] + m2 * ix[ i1 ] ) / 256 );

				ix[ i1 ] = ix[ i ] - tx; 
				iy[ i1 ] = iy[ i ] - ty;
				ix[ i ] += tx;
				iy[ i ] += ty;
			}
		}
	}

	//scale & normalize
	s16 *	pp( s_PowerBuf[ s_CurrentPowerBuf ] );

	for ( u32 i = 0; i < 128; i++ )
	{
		//tx,ty : re,im  forward fft scaling=256, fixed point +-16384
		const s32	tx( ix[ i ] / 256 );
		const s32	ty( iy[ i ] / 256 );
		s32			pw( SQRTI( tx * tx + ty * ty ) );
		//normalize it 
		pw = pw * ( j + 8 ) / 512;

		*( pp++ ) = pw;
	}

	s_CurrentPowerBuf = ( ( s_CurrentPowerBuf + 1 ) & ( NUM_POWER_BUFS - 1 ) );

	return;
}

//**********************************************************************************
//	
//**********************************************************************************
static u32		s_nCurrentColor( 0 );
static float	s_fColorTimer( 0.f );

static const ARGB	s_Colors[] =
{
	ARGB( 255, 0, 0 ),
	ARGB( 255, 255, 0 ),
	ARGB( 0, 255, 0 ),
	ARGB( 0, 255, 255 ),
	ARGB( 0, 0, 255 ),
	ARGB( 255, 0, 255 ),
};

static const u32	s_nNumColors( sizeof( s_Colors ) / sizeof( s_Colors[ 0 ] ) );


void	CMusicFileHandler::Render()
{
	if ( s_bMusicIsPlaying == true && s_bShowVisualiser == true )
	{
		if ( s_pBuffer[ 0 ] != NULL && s_pBuffer[ 1 ] != NULL )
		{
			s32			dots[ 128 ];
			const u32	pbtmp( s_CurrentPowerBuf );
			const u32	last_pb( s_nLastPowerBuf[ s_nPowerFrame ] );
			const u32	pbc( ( pbtmp - last_pb ) & ( NUM_POWER_BUFS - 1 ) );

			if ( pbc > 0 )
			{
				for ( u32 j = 0; j < 128; j++ )
				{
					s32	powsum( 0 );

					for ( u32 i = 0; i < pbc; i++ )
					{
						s32	pow( s_PowerBuf[ ( last_pb + i ) & ( NUM_POWER_BUFS - 1 ) ][ j ] );

						powsum += pow;
					}

					powsum /= pbc;

					dots[ j ] = powsum;
				}

				s_nLastPowerBuf[ s_nPowerFrame ] = pbtmp;

				//
				//	Draw the dots and splurge it all over the screen :)
				//
				u8 *			p_back_buffer( CGfx::GetBackBuffer() );
				u8 * const		p_rt_edram_00( s_pBuffer[ s_nPowerFrame ]->m_pVRAM );
				u8 * const		p_rt_00( ( u8 * )( ( u32 )( p_rt_edram_00 ) - ( u32 )( sceGeEdramGetAddr() ) ) );
				const V2		origin( RENDER_TARGET_WIDTH / 2, ( RENDER_TARGET_HEIGHT / 2 ) );
				CTexture *		p_render_target_tex( s_pBuffer[ ( s_nPowerFrame + 1 ) & 1 ] );

				// Setup render target viewport
				sceGuDrawBufferList( GU_PSM_8888, p_rt_00, RENDER_TARGET_WIDTH );
				sceGuOffset( 2048 - ( RENDER_TARGET_WIDTH / 2 ), 2048 - ( RENDER_TARGET_HEIGHT / 2 ) );
				sceGuViewport( 2048, 2048, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT );
				sceGuScissor( 0, 0, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT );

				// Render the previous render target to the current render target
				sVertexTexturedColor *	p_vert;
				ARGB					color( 0xfefefefe );
				const float				scale( 1.03f );
				const float				rotation( sinf( DEGS_TO_RADS( s_VisualiserRotation ) ) * 0.02f );

				const V2	ul( origin + ( Rotate( V2( -RENDER_TARGET_WIDTH / 2, -RENDER_TARGET_HEIGHT / 2 ), rotation ) * scale ) );
				const V2	ur( origin + ( Rotate( V2( +RENDER_TARGET_WIDTH / 2, -RENDER_TARGET_HEIGHT / 2 ), rotation ) * scale ) );
				const V2	bl( origin + ( Rotate( V2( -RENDER_TARGET_WIDTH / 2, +RENDER_TARGET_HEIGHT / 2 ), rotation ) * scale ) );
				const V2	br( origin + ( Rotate( V2( +RENDER_TARGET_WIDTH / 2, +RENDER_TARGET_HEIGHT / 2 ), rotation ) * scale ) );

				CGfx::GetPolyList( 4, &p_vert );

				p_vert[ 0 ].uv.x = 0.f;
				p_vert[ 0 ].uv.y = 0.f;
				p_vert[ 0 ].color = color;
				p_vert[ 0 ].pos.x = ul.x;
				p_vert[ 0 ].pos.y = ul.y;
				p_vert[ 0 ].pos.z = 0.f;

				p_vert[ 1 ].uv.x = p_render_target_tex->m_nWidth;
				p_vert[ 1 ].uv.y = 0.f;
				p_vert[ 1 ].color = color;
				p_vert[ 1 ].pos.x = ur.x;
				p_vert[ 1 ].pos.y = ur.y;
				p_vert[ 1 ].pos.z = 0.f;

				p_vert[ 2 ].uv.x = 0.f;
				p_vert[ 2 ].uv.y = p_render_target_tex->m_nHeight;
				p_vert[ 2 ].color = color;
				p_vert[ 2 ].pos.x = bl.x;
				p_vert[ 2 ].pos.y = bl.y;
				p_vert[ 2 ].pos.z = 0.f;

				p_vert[ 3 ].uv.x = p_render_target_tex->m_nWidth;
				p_vert[ 3 ].uv.y = p_render_target_tex->m_nHeight;
				p_vert[ 3 ].color = color;
				p_vert[ 3 ].pos.x = br.x;
				p_vert[ 3 ].pos.y = br.y;
				p_vert[ 3 ].pos.z = 0.f;

				CGfx::DrawPoly2D( GU_TRIANGLE_STRIP, p_render_target_tex, p_vert, 4 );

				// Draw the sound waves as a continous line
				sVertexColor *	p_line_list0;
				sVertexColor *	p_line_list1;

				CGfx::GetPolyList( 128, &p_line_list0 );
				CGfx::GetPolyList( 128, &p_line_list1 );

				ARGB		line_color0, line_color1;
				const ARGB	col0( s_Colors[ s_nCurrentColor ] );
				const ARGB	col1( s_Colors[ s_nCurrentColor == s_nNumColors - 1 ? 0 : s_nCurrentColor + 1 ] );

				line_color0.a = col0.a + static_cast< u8 >( ( s_fColorTimer * ( static_cast< float >( col1.a - col0.a ) ) ) );
				line_color0.r = col0.r + static_cast< u8 >( ( s_fColorTimer * ( static_cast< float >( col1.r - col0.r ) ) ) );
				line_color0.g = col0.g + static_cast< u8 >( ( s_fColorTimer * ( static_cast< float >( col1.g - col0.g ) ) ) );
				line_color0.b = col0.b + static_cast< u8 >( ( s_fColorTimer * ( static_cast< float >( col1.b - col0.b ) ) ) );
				line_color1 = ARGB( line_color0.r >> 1, line_color0.g >> 1, line_color0.b >> 1 );

				p_line_list0[ 0 ].color = line_color0;
				p_line_list0[ 0 ].pos = V3( origin.x - 32, origin.y, 0.f );
				p_line_list1[ 0 ].color = line_color1;
				p_line_list1[ 0 ].pos = V3( origin.x - 32, origin.y, 0.f );

				for ( u32 j = 1; j < 128; j++ )
				{
					p_line_list0[ j ].pos = V3( origin.x + ( j * 0.5f ) - 32, origin.y - ( dots[ j ] / 6 ), 0.f );
					p_line_list1[ j ].pos = V3( origin.x + ( j * 0.5f ) - 32, origin.y + ( dots[ j ] / 6 ), 0.f );

					if ( j & 1 )
					{
						p_line_list0[ j ].color = line_color1;
						p_line_list1[ j ].color = line_color0;
					}
					else
					{
						p_line_list0[ j ].color = line_color0;
						p_line_list1[ j ].color = line_color1;
					}
				}

				CGfx::DrawPoly2D( GU_LINE_STRIP, p_line_list0, 128 );
				CGfx::DrawPoly2D( GU_LINE_STRIP, p_line_list1, 128 );

				// Restore normal viewport and draw buffer
				sceGuDrawBufferList( GU_PSM_8888, p_back_buffer, 512 );
				sceGuOffset( 2048 - ( CGfx::s_ScreenWidth / 2 ), 2048 - ( CGfx::s_ScreenHeight / 2 ) );
				sceGuViewport( 2048, 2048, CGfx::s_ScreenWidth, CGfx::s_ScreenHeight );
				sceGuScissor( 0, 0, CGfx::s_ScreenWidth, CGfx::s_ScreenHeight );

				// Draw the render target on to the screen
				CGfx::DrawQuad( s_pBuffer[ s_nPowerFrame ], V2( 0.f, 0.f ), V2( CGfx::s_ScreenWidth, CGfx::s_ScreenHeight ), 0xffffffff );

				s_VisualiserRotation += 20.f * CFrameWork::GetElapsedTime();

				if ( s_VisualiserRotation >= 360.f )
				{
					s_VisualiserRotation -= 360.f;
				}

				s_nPowerFrame = ( s_nPowerFrame ? 0 : 1 );

				// Update the color timer
				s_fColorTimer += 0.1f * CFrameWork::GetElapsedTime();

				if ( s_fColorTimer >= 1.f )
				{
					++s_nCurrentColor;

					s_fColorTimer = 0.f;

					if ( s_nCurrentColor >= s_nNumColors )
					{
						s_nCurrentColor = 0;
					}
				}
			}
		}
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CMusicFileHandler::ToggleEcho()
{
	if ( pspAudioGetCurrentEffect() == FX_ECHO )
	{
		pspAudioSetEcho( FX_DISABLED );
	}
	else
	{
		pspAudioSetEcho( FX_ENABLED );
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CMusicFileHandler::ToggleSpatialSound()
{
	if ( pspAudioGetCurrentEffect() == FX_SPATIAL_SOUND )
	{
		pspAudioSetSpatialSound( FX_DISABLED );
	}
	else
	{
		pspAudioSetSpatialSound( FX_ENABLED );
	}
}

//**********************************************************************************
//	
//**********************************************************************************
void	CMusicFileHandler::ProcessInput()
{
	if ( s_bMusicIsPlaying == true )
	{
		if ( CFileHandler::MultiSelection() == true )
		{
			if ( s_pDecoder != NULL )
			{
				if ( CInput::IsButtonClicked( CInput::LTRIGGER ) == true )
				{
					Execute( CFileHandler::GetPrevFile() );
				}

				if ( CInput::IsButtonClicked( CInput::RTRIGGER ) == true || s_pDecoder->eos() != 0 )
				{
					Execute( CFileHandler::GetNextFile() );
				}
			}
		}
	}
}

//**********************************************************************************
//	
//**********************************************************************************
void	CMusicFileHandler::ShowVisualiser( bool show )
{
	s_bShowVisualiser = show;

	if ( show == false )
	{
		CGfx::FreeAllVRAM();
		CSkinManager::UploadTextures();
	}
}

//**********************************************************************************
//	
//**********************************************************************************
bool	CMusicFileHandler::VisualiserVisible()
{
	return s_bShowVisualiser;
}

//*******************************  END OF FILE  ************************************
