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

  Module :	CCredits.cpp

  Description :

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 08 September 2005 71M

***********************************************************************************/

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "CCredits.h"
#include "CFrameWork.h"
#include "CCamera.h"
#include "CRenderable.h"
#include "CInput.h"
#include "CGfx.h"
#include "CSphere.h"
#include "CPlane.h"
#include "CFont.h"
#include "CFileSystem.h"
#include "CMusicFileHandler.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************
static const CString	MUSIC_FILE( "Data/Credits/Obliterator.mod" );
static const float		GRID_SIZE( 5.f );
static const u32		NUM_GRIDS( 20 );
static const float		FLOOR_SIZE( NUM_GRIDS * GRID_SIZE );
static const float		HALF_GRID_SIZE( 0.5f * GRID_SIZE );
static const u32		MAX_CUBES( 32 );
static const float		FLOOR_FRICTION( 0.2f );

//**********************************************************************************

static const ARGB		CUBE_COLOR( 39, 166, 225 );
static const ARGB		GRID_COLOR0( 180, 80, 80, 80 );
static const ARGB		GRID_COLOR1( 180, 40, 40, 40 );

static sVertexColor __attribute__( ( aligned( 16 ) ) )			s_Grid[ NUM_GRIDS * NUM_GRIDS * 6 ];
static sVertexColorNormal __attribute__( ( aligned( 16 ) ) )	s_Cube[ 12 * 3 ] =
{
	{CUBE_COLOR,V3(0,0,0), V3(-1,-1,1 )},	{CUBE_COLOR,V3(0,0,0), V3(1,-1, 1)},	{CUBE_COLOR,V3(0,0,0), V3(1, 1, 1)},
	{CUBE_COLOR,V3(0,0,0),V3(-1,-1, 1)},	{CUBE_COLOR,V3(0,0,0), V3(1, 1, 1)},	{CUBE_COLOR,V3(0,0,0),V3(-1, 1, 1)},

	{CUBE_COLOR,V3(0,0,0), V3(1, 1, 1)},	{CUBE_COLOR,V3(0,0,0), V3(1,-1, 1)},	{CUBE_COLOR,V3(0,0,0), V3(1,-1,-1)},
	{CUBE_COLOR,V3(0,0,0), V3(1, 1, 1)},	{CUBE_COLOR,V3(0,0,0), V3(1,-1,-1)},	{CUBE_COLOR,V3(0,0,0), V3(1, 1,-1)},

	{CUBE_COLOR,V3(0,0,0),V3(-1, 1, 1)},	{CUBE_COLOR,V3(0,0,0), V3(1, 1, 1)},	{CUBE_COLOR,V3(0,0,0), V3(1, 1,-1)},
	{CUBE_COLOR,V3(0,0,0),V3(-1, 1, 1)},	{CUBE_COLOR,V3(0,0,0), V3(1, 1,-1)},	{CUBE_COLOR,V3(0,0,0),V3(-1, 1,-1)},

	{CUBE_COLOR,V3(0,0,0), V3(1,-1,-1)},	{CUBE_COLOR,V3(0,0,0),V3(-1,-1,-1)},	{CUBE_COLOR,V3(0,0,0),V3(-1, 1,-1)},
	{CUBE_COLOR,V3(0,0,0), V3(1,-1,-1)},	{CUBE_COLOR,V3(0,0,0),V3(-1, 1,-1)},	{CUBE_COLOR,V3(0,0,0), V3(1, 1,-1)},

	{CUBE_COLOR,V3(0,0,0),V3(-1,-1,-1)},	{CUBE_COLOR,V3(0,0,0),V3(-1,-1, 1)},	{CUBE_COLOR,V3(0,0,0),V3(-1, 1, 1)},
	{CUBE_COLOR,V3(0,0,0),V3(-1,-1,-1)},	{CUBE_COLOR,V3(0,0,0),V3(-1, 1, 1)},	{CUBE_COLOR,V3(0,0,0),V3(-1, 1,-1)},

	{CUBE_COLOR,V3(0,0,0),V3(-1,-1,-1)},	{CUBE_COLOR,V3(0,0,0), V3(1,-1,-1)},	{CUBE_COLOR,V3(0,0,0), V3(1,-1, 1)},
	{CUBE_COLOR,V3(0,0,0),V3(-1,-1,-1)},	{CUBE_COLOR,V3(0,0,0), V3(1,-1, 1)},	{CUBE_COLOR,V3(0,0,0),V3(-1,-1, 1)},
};

//**********************************************************************************

struct sCredit
{
	float	m_fTime;
	V2		m_Pos;
	CString	m_szText;
};

static const sCredit	s_Credits[] =
{
	{
		4.f,
		V2( 0.f, 0.f ),
		"",
	},
	{
		5.f,
		V2( 64.f, 64.f ),
		"The Credits!",
	},
	{
		5.f,
		V2( 200.f, 170.f ),
		"Original Code:\n        Lin J.R.",
	},
	{
		5.f,
		V2( 100.f, 64.f ),
		"Coding:\n          71M",
	},
	{
		5.f,
		V2( 180.f, 160.f ),
		"Coding:\n        Shazz",
	},
	{
		5.f,
		V2( 90.f, 120.f ),
		"FTP Client:\n           LiQuiD8",
	},
	{
		5.f,
		V2( 250.f, 64.f ),
		"Original Logo:\n        Pochi",
	},
	{
		7.f,
		V2( 64.f, 100.f ),
		"Skins:\n        DDR Lord\n           Omnirage\n               Goodstewie",
	},
	{
		5.f,
		V2( 50.f, 200.f ),
		"Thanks to:\n        Adresd (PSPMC)",
	},
	{
		5.f,
		V2( 50.f, 200.f ),
		"Thanks to:\n      Lee Thomason (Tiny XML)",
	},
	{
		5.f,
		V2( 50.f, 200.f ),
		"Thanks to:\n       Tyranid for much help!\n",
	},
	{
		5.f,
		V2( 50.f, 200.f ),
		"Thanks to:\n        Everyone at PS2Dev!\n",
	},
	{
		10.f,
		V2( 50.f, 200.f ),
		"Thanks to:\n      All our friends and family!",
	},
	{
		1000.f,
		V2( 50.f, 200.f ),
		"Merci! :)",
	},
};

static const u32	s_nNumCredits( sizeof( s_Credits ) / sizeof( sCredit ) );

//**********************************************************************************
//   Static Prototypes
//**********************************************************************************

//**********************************************************************************
//   Global Variables
//**********************************************************************************

//**********************************************************************************
//   Static Variables
//**********************************************************************************
static CPlane		s_Ground;
static u32			s_CurrentCredit( 1 );
static float		s_fCreditTimer( 0.f );
static float		s_FadeTimer( 0.f );
static float		s_FadeInLevel( 0.f );

//**********************************************************************************
//**********************************************************************************
//
//	A very simple collision sphere class that stores distances to neighbouring spheres
//
//**********************************************************************************
//**********************************************************************************
class CCollisionSphere : public CSphere
{
	public:

		bool	Collide( CCollisionSphere & sphere );
		void	AddNeighbour( CCollisionSphere * const p_sphere );

	public:

		struct sNeighbour
		{
			float				m_fDistance;
			CCollisionSphere *	m_pNeighbour;
		};

		typedef std::vector< sNeighbour * >	CNeighbours;

		V3				m_Velocity;
		CNeighbours		m_Neighbours;
};

//**********************************************************************************
//
//**********************************************************************************
void	CCollisionSphere::AddNeighbour( CCollisionSphere * const p_sphere )
{
	sNeighbour * const	p_neighbour( new sNeighbour() );

	p_neighbour->m_fDistance = 0.f;
	p_neighbour->m_pNeighbour = p_sphere;

	m_Neighbours.push_back( p_neighbour );
}

//**********************************************************************************
//
//**********************************************************************************
bool	CCollisionSphere::Collide( CCollisionSphere & sphere )
{
	const V3	diff( m_Centre - sphere.m_Centre );
	const float	distance_sq( diff.LengthSq() );
	const float	fake_radius( 1.f * ( sphere.m_fRadius + m_fRadius ) );

	if ( distance_sq <= SQUARE( fake_radius ) )
	{
//		m_Velocity.y *= FLOOR_FRICTION;
		m_Centre -= diff.Normal() * ( SQRTF( distance_sq ) - fake_radius );

		return true;
	}

	return false;
}


//**********************************************************************************
//**********************************************************************************
//
//	Some rubbish physics code :)
//
//**********************************************************************************
//**********************************************************************************
class CSphereCollection
{
	public:

		typedef std::vector< CCollisionSphere * >	CSphereList;

		void				Render();
		void				Process();

		CCollisionSphere *	AddSphere( const CSphere & sphere );
		void				CalculateDistances();

		void				ResolveDistance( CCollisionSphere * const p_parent, CCollisionSphere * const p_neighbour, float distance );
		void				MoveSphere( CCollisionSphere * const p_move_sphere, const V3 & offset );

	public:

		CSphereList			m_SphereList;
};

//**********************************************************************************
//
//**********************************************************************************
CCollisionSphere *	CSphereCollection::AddSphere( const CSphere & sphere )
{
	CCollisionSphere * const	p_sphere( new CCollisionSphere() );

	p_sphere->m_Velocity = gZeroVector;
	p_sphere->SetCentre( sphere.GetCentre() );
	p_sphere->SetRadius( sphere.GetRadius() );

	for ( CSphereList::iterator it = m_SphereList.begin(); it != m_SphereList.end(); ++it )
	{
		p_sphere->AddNeighbour( *it );
		( *it )->AddNeighbour( p_sphere );
	}

	m_SphereList.push_back( p_sphere );

	return p_sphere;
}

//**********************************************************************************
//
//**********************************************************************************
void	CSphereCollection::CalculateDistances()
{
	for ( CSphereList::iterator it = m_SphereList.begin(); it != m_SphereList.end(); ++it )
	{
		CCollisionSphere * const	p_sphere( *it );

		for ( CCollisionSphere::CNeighbours::iterator nit = p_sphere->m_Neighbours.begin(); nit != p_sphere->m_Neighbours.end(); ++nit )
		{
			CCollisionSphere::sNeighbour * const	p_neighbour( *nit );

			p_neighbour->m_fDistance = ( p_neighbour->m_pNeighbour->GetCentre() - p_sphere->GetCentre() ).LengthSq();
		}
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CSphereCollection::Process()
{
	for ( CSphereList::iterator it = m_SphereList.begin(); it != m_SphereList.end(); ++it )
	{
		CCollisionSphere * const	p_sphere( *it );

		p_sphere->m_Velocity += V3( 0.f, -0.9f, 0.f ) * CFrameWork::GetElapsedTime();

		MoveSphere( p_sphere, p_sphere->m_Velocity );

		for ( CCollisionSphere::CNeighbours::iterator nit = p_sphere->m_Neighbours.begin(); nit != p_sphere->m_Neighbours.end(); ++nit )
		{
			ResolveDistance( p_sphere, ( *nit )->m_pNeighbour, ( *nit )->m_fDistance );
		}
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CSphereCollection::ResolveDistance( CCollisionSphere * const p_parent, CCollisionSphere * const p_neighbour, float distance )
{
	const V3	diff( ( p_neighbour->m_Centre - p_parent->m_Centre ).Normal() );
	const V3	wanted_pos( p_parent->m_Centre + ( diff * distance ) );
	const V3	move_delta( wanted_pos - p_neighbour->m_Centre );

	if ( move_delta.LengthSq() > SQUARE( EPSILON ) )
	{
		p_neighbour->m_Centre = wanted_pos;
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CSphereCollection::MoveSphere( CCollisionSphere * const p_move_sphere, const V3 & offset )
{
	*p_move_sphere += offset;

	const float	sphere_x( p_move_sphere->m_Centre.x - p_move_sphere->m_fRadius );
	const float	sphere_y( p_move_sphere->m_Centre.y - p_move_sphere->m_fRadius );
	const float	sphere_z( p_move_sphere->m_Centre.z - p_move_sphere->m_fRadius );

	if ( s_Ground.DistanceToPoint( p_move_sphere->m_Centre ) < p_move_sphere->m_fRadius )
	{
		p_move_sphere->m_Velocity.x = p_move_sphere->m_Velocity.x * FLOOR_FRICTION;
		p_move_sphere->m_Velocity.y = -p_move_sphere->m_Velocity.y * 0.4f;//FLOOR_FRICTION;
		p_move_sphere->m_Velocity.z = p_move_sphere->m_Velocity.z * FLOOR_FRICTION;
		p_move_sphere->m_Centre.y = p_move_sphere->m_fRadius;
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CSphereCollection::Render()
{
	for ( CSphereList::iterator it = m_SphereList.begin(); it != m_SphereList.end(); ++it )
	{
		const CCollisionSphere * const	p_sphere( *it );

		p_sphere->Render( 0xffffffff );
	}
}


//**********************************************************************************
//**********************************************************************************
//
//	Phake Fysics class implementation
//
//**********************************************************************************
//**********************************************************************************
typedef std::list< CSphereCollection * >	CCubeList;

//**********************************************************************************
//	Static Variables
//**********************************************************************************
static CCubeList	s_CubeList;
CCredits *			CCredits::s_pInstance( NULL );

//**********************************************************************************
//
//**********************************************************************************
void	CCredits::Show()
{
	s_pInstance = new CCredits();

	while ( s_pInstance->HasQuit() == false )
	{
		CFrameWork::Process();
	}

	SAFE_DELETE( s_pInstance );
}

//**********************************************************************************
//
//**********************************************************************************
CCredits::CCredits()
:	m_pCamera( new CCamera() )
,	m_CamAngle( 0.f, 180.f, 0.f )
,	m_CameraTimer( 0.f )
,	m_fCamFocus( 0.f )
,	m_fCamOffset( gZeroVector )
,	m_fCamRotation( 0.f )
,	m_fWantedCamFocus( 0.f )
,	m_fWantedCamOffset( gZeroVector )
,	m_bQuit( false )
{
	CFont::Open();

	CMusicFileHandler::Execute( "Data/Obliterator.mod" );

	PickNewCameraOrientation();

	m_fCamFocus = m_fWantedCamFocus;
	m_fCamOffset = m_fWantedCamOffset;

	m_pCamera->SetPos( V3( 0.f, 20.f, 60.f ) );

	CRenderable::Register( CRenderable::RO_BACKGROUND, Render );

	//
	//	Create the collision planes
	//
	s_Ground.m_Normal = gUpVector;
	s_Ground.m_D = 0.f;

	//
	//	Create the floor
	//
	V3				pos( 0.f, 0.f, 0.f );
	const V3		origin( V3( NUM_GRIDS * GRID_SIZE * 0.5f, 0.f, NUM_GRIDS * GRID_SIZE * 0.5f ) );
	sVertexColor *	p_tris( s_Grid );
	const float		max_dist( 4.f * NUM_GRIDS * GRID_SIZE * 0.5f );

	for ( u32 z = 0; z < NUM_GRIDS; ++z )
	{
		pos.x = 0.f;

		for ( u32 x = 0; x < NUM_GRIDS; ++x )
		{
			const ARGB	color( ( ( x + z ) & 1 ) ? GRID_COLOR0 : GRID_COLOR1 );
			const V3	p0( V3( pos.x - HALF_GRID_SIZE, 0.f, pos.z - HALF_GRID_SIZE ) - origin );
			const V3	p1( V3( pos.x - HALF_GRID_SIZE, 0.f, pos.z + HALF_GRID_SIZE ) - origin );
			const V3	p2( V3( pos.x + HALF_GRID_SIZE, 0.f, pos.z - HALF_GRID_SIZE ) - origin );
			const V3	p3( V3( pos.x + HALF_GRID_SIZE, 0.f, pos.z + HALF_GRID_SIZE ) - origin );
			float		dist0( MAX( 0.f, SQUARE( 1.f - ( p0.Length() / max_dist ) ) ) );
			float		dist1( MAX( 0.f, SQUARE( 1.f - ( p1.Length() / max_dist ) ) ) );
			float		dist2( MAX( 0.f, SQUARE( 1.f - ( p2.Length() / max_dist ) ) ) );
			float		dist3( MAX( 0.f, SQUARE( 1.f - ( p3.Length() / max_dist ) ) ) );

			p_tris->pos = p0;
			p_tris->color = ARGB( static_cast< u8 >( color.a * dist0 ), static_cast< u8 >( color.r * dist0 ), static_cast< u8 >( color.g * dist0 ), static_cast< u8 >( color.b * dist0 ) );
			++p_tris;

			p_tris->pos = p1;
			p_tris->color = ARGB( static_cast< u8 >( color.a * dist1 ), static_cast< u8 >( color.r * dist1 ), static_cast< u8 >( color.g * dist1 ), static_cast< u8 >( color.b * dist1 ) );
			++p_tris;

			p_tris->pos = p2;
			p_tris->color = ARGB( static_cast< u8 >( color.a * dist2 ), static_cast< u8 >( color.r * dist2 ), static_cast< u8 >( color.g * dist2 ), static_cast< u8 >( color.b * dist2 ) );
			++p_tris;

			p_tris->pos = p1;
			p_tris->color = ARGB( static_cast< u8 >( color.a * dist1 ), static_cast< u8 >( color.r * dist1 ), static_cast< u8 >( color.g * dist1 ), static_cast< u8 >( color.b * dist1 ) );
			++p_tris;

			p_tris->pos = p3;
			p_tris->color = ARGB( static_cast< u8 >( color.a * dist3 ), static_cast< u8 >( color.r * dist3 ), static_cast< u8 >( color.g * dist3 ), static_cast< u8 >( color.b * dist3 ) );
			++p_tris;

			p_tris->pos = p2;
			p_tris->color = ARGB( static_cast< u8 >( color.a * dist2 ), static_cast< u8 >( color.r * dist2 ), static_cast< u8 >( color.g * dist2 ), static_cast< u8 >( color.b * dist2 ) );
			++p_tris;

			pos.x += GRID_SIZE;
		}

		pos.z += GRID_SIZE;
	}

	//
	//	Generate cube normals
	//
	for ( u32 i = 0; i < 12; ++i )
	{
		const V3 &	p0( s_Cube[ ( i * 3 ) + 0 ].pos );
		const V3 &	p1( s_Cube[ ( i * 3 ) + 1 ].pos );
		const V3 &	p2( s_Cube[ ( i * 3 ) + 2 ].pos );
		V3			normal;

		normal.x = ( p1.y - p0.y ) * ( p0.z - p2.z ) - ( p1.z - p0.z ) * ( p0.y - p2.y );
		normal.y = ( p1.z - p0.z ) * ( p0.x - p2.x ) - ( p1.x - p0.x ) * ( p0.z - p2.z );
		normal.z = ( p1.x - p0.x ) * ( p0.y - p2.y ) - ( p1.y - p0.y ) * ( p0.x - p2.x );

		s_Cube[ ( i * 3 ) + 0 ].normal = normal.Normal();
		s_Cube[ ( i * 3 ) + 1 ].normal = normal.Normal();
		s_Cube[ ( i * 3 ) + 2 ].normal = normal.Normal();
	}
}

//**********************************************************************************
//
//**********************************************************************************
CCredits::~CCredits()
{
	CMusicFileHandler::Stop();

	CRenderable::UnRegister( CRenderable::RO_BACKGROUND, Render );

	while ( s_CubeList.empty() == false )
	{
		CSphereCollection *	p_cube( s_CubeList.front() );

		SAFE_DELETE( p_cube );

		s_CubeList.pop_front();
	}

	SAFE_DELETE( m_pCamera );
}

//**********************************************************************************
//
//**********************************************************************************
void	CCredits::Process()
{
	static float		timer( 0.f );
	static const float	SPAWN_TIME( 0.2f );

	//
	//	Fade the screen in
	//
	s_FadeInLevel += CFrameWork::GetElapsedTime() * 100.f;

	if ( s_FadeInLevel > 255.f )
	{
		CGfx::SetFadeLevel( 255 );
	}
	else
	{
		CGfx::SetFadeLevel( static_cast< u8 >( s_FadeInLevel ) );
	}

	//
	//	Spawn a new cube
	//
	timer += CFrameWork::GetElapsedTime();

	if ( timer >= SPAWN_TIME )
	{
		timer -= SPAWN_TIME;

		CSphereCollection *	p_collection;
		float				rx( static_cast< float >( rand() % 100 ) / 100.f );
		float				rz( static_cast< float >( rand() % 100 ) / 100.f );
		const V3			p0( -0.5f + rx, 20.f, 0.f + rz );
		const V3			p1( +0.5f + rx, 20.f, 0.f + rz );

		if ( s_CubeList.size() < MAX_CUBES )
		{
			p_collection = new CSphereCollection();

			p_collection->AddSphere( CSphere( p0, 1.f ) );
			p_collection->AddSphere( CSphere( p1, 1.f ) );
			p_collection->CalculateDistances();
		}
		else
		{
			p_collection = s_CubeList.back();

			p_collection->m_SphereList[ 0 ]->SetCentre( p0 );
			p_collection->m_SphereList[ 1 ]->SetCentre( p1 );

			p_collection->CalculateDistances();

			s_CubeList.pop_back();
		}

		rx *= 0.1f;
		rz *= 0.1f;

		p_collection->m_SphereList[ 0 ]->m_Velocity = V3( rx, +0.f, +rz );
		p_collection->m_SphereList[ 1 ]->m_Velocity = V3( -rz, +0.f, -rx );

		s_CubeList.push_front( p_collection );
	}


	//
	//	Collide cubes against each other
	//
	for ( CCubeList::iterator it = s_CubeList.begin(); it != s_CubeList.end(); ++it )
	{
		( *it )->Process();

		CCollisionSphere * const	p_solid( ( *it )->m_SphereList[ 0 ] );

		for ( CCubeList::iterator it2 = s_CubeList.begin(); it2 != s_CubeList.end(); ++it2 )
		{
			CCollisionSphere * const	p_sphere( ( *it2 )->m_SphereList[ 0 ] );

			if ( p_sphere != p_solid )
			{
				if ( p_sphere->Collide( *p_solid ) == true )
				{
					( *it2 )->m_SphereList[ 1 ]->Collide( *p_solid );
				}
			}
		}
	}

	UpdateCamera();
}

//**********************************************************************************
//
//**********************************************************************************
void	CCredits::ProcessInput()
{
	//
	//	Manual camera controls
	//
/*	static const float	CAM_MOVE_SPEED( 20.f );
	static const float	CAM_ROTATE_SPEED( 40.f );

	V3					cam_pos( m_pCamera->GetPos() );
	const float			delta( CFrameWork::GetElapsedTime() );
	V3					at( RotateX( gAtVector, DEGS_TO_RADS( m_CamAngle.x ) ) );

	at = RotateY( at, DEGS_TO_RADS( m_CamAngle.y ) );

	const V3			right( gUpVector.UnitCross( at ) );
	const V3			up( at.UnitCross( right ) );

	if ( CInput::IsButtonDown( CInput::UP ) == true )			cam_pos += at * delta * CAM_MOVE_SPEED;
	if ( CInput::IsButtonDown( CInput::DOWN ) == true )			cam_pos -= at * delta * CAM_MOVE_SPEED;

	if ( CInput::IsButtonDown( CInput::LEFT ) == true )			cam_pos += right * delta * CAM_MOVE_SPEED;
	if ( CInput::IsButtonDown( CInput::RIGHT ) == true )		cam_pos -= right * delta * CAM_MOVE_SPEED;

	if ( CInput::IsButtonDown( CInput::TRIANGLE ) == true )		cam_pos += up * delta * CAM_MOVE_SPEED;
	if ( CInput::IsButtonDown( CInput::SQUARE ) == true )		cam_pos -= up * delta * CAM_MOVE_SPEED;

	if ( CInput::IsButtonDown( CInput::LTRIGGER ) == true )		m_CamAngle.y -= delta * CAM_ROTATE_SPEED;
	if ( CInput::IsButtonDown( CInput::RTRIGGER ) == true )		m_CamAngle.y += delta * CAM_ROTATE_SPEED;

	if ( CInput::IsButtonDown( CInput::CROSS ) == true )		m_CamAngle.x -= delta * CAM_ROTATE_SPEED;
	if ( CInput::IsButtonDown( CInput::CIRCLE ) == true )		m_CamAngle.x += delta * CAM_ROTATE_SPEED;

	m_pCamera->SetPos( cam_pos );
	m_pCamera->SetFocus( cam_pos + at );
*/

	if ( CInput::IsButtonClicked( CInput::START ) == true )
	{
		ShakeCubes();
	}

	if ( CInput::IsButtonClicked( CInput::CROSS ) == true || CInput::IsButtonClicked( CInput::CIRCLE ) == true )
	{
		m_bQuit = true;
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CCredits::ShakeCubes()
{
	for ( CCubeList::iterator it = s_CubeList.begin(); it != s_CubeList.end(); ++it )
	{
		for ( u32 i = 0; i < 2; ++i )
		{
			const float	rx( static_cast< float >( ( rand() % 50 ) - 25 ) / 100.f );
			const float	ry( static_cast< float >( rand() % 50 ) / 100.f );
			const float	rz( static_cast< float >( ( rand() % 50 ) - 25 ) / 100.f );

			( *it )->m_SphereList[ i ]->m_Velocity += V3( rx, ry, rz );
		}
	}
}

//**********************************************************************************
//
//**********************************************************************************
void	CCredits::Render()
{
	sceGuEnable( GU_LIGHTING );
	sceGuEnable( GU_LIGHT0 );

	sceGuColor( 0xffffff );
	sceGuSpecular( 12.0f );
	sceGuAmbient( 0xff808080 );

	ScePspFVector3 dir = { 0.5f, -0.5f, 0.5f };
	sceGuLight( 0, GU_DIRECTIONAL, GU_DIFFUSE_AND_SPECULAR, &dir );
	sceGuLightColor( 0, GU_DIFFUSE, 0xffffffff );
	sceGuLightColor( 0, GU_SPECULAR, 0xffffffff );
	sceGuLightAtt( 0, 1.0f, 0.0f, 0.0f );

	s_pInstance->RenderInternal();
}

//**********************************************************************************
//
//**********************************************************************************
void	CCredits::RenderInternal()
{
	m_pCamera->SetView();

	//
	//	Calculate cube matrices
	//
	std::vector< M44 >	cube_mtx_list;
	M44					scale_mtx( gIdentity );

	scale_mtx.Scale( 1.f, -1.f, 1.f );

	for ( CCubeList::iterator it = s_CubeList.begin(); it != s_CubeList.end(); ++it )
	{
		M44							mtx;
		CCollisionSphere * const	p_s0( ( *it )->m_SphereList[ 0 ] );
		CCollisionSphere * const	p_s1( ( *it )->m_SphereList[ 1 ] );
		const V3					centre( ( p_s0->GetCentre() + p_s1->GetCentre() ) * 0.5f );
		const V3					at( ( p_s0->GetCentre() - p_s1->GetCentre() ).Normal() );
		const V3					right( gUpVector.UnitCross( at ) );
		const V3					up( at.UnitCross( right ) );

		mtx.SetAt( at );
		mtx.SetUp( up );
		mtx.SetRight( right );
		mtx.SetPos( centre );

		cube_mtx_list.push_back( mtx );
	}

	//
	//	Render mirror with stencil enabled
	//
	sceGuEnable( GU_CULL_FACE );
	sceGuFrontFace( GU_CCW );
	sceGuEnable( GU_STENCIL_TEST );
	sceGuDepthMask( GU_TRUE );
	sceGuStencilFunc( GU_ALWAYS, 1, 1 );
	sceGuStencilOp( GU_KEEP, GU_KEEP, GU_REPLACE );

	sceGuSetMatrix( GU_MODEL, gIdentity );
	CGfx::DrawPoly3D( GU_TRIANGLES, s_Grid, NUM_GRIDS * NUM_GRIDS * 6 );

	//
	//	Render the cubes upside down
	//
	sceGuDepthMask( GU_FALSE );
	sceGuFrontFace( GU_CW );
	sceGuStencilFunc( GU_EQUAL, 1, 1 );
	sceGuStencilOp( GU_KEEP, GU_KEEP, GU_KEEP );

	for ( u32 i = 0; i < s_CubeList.size(); ++i )
	{
		M44			mtx( cube_mtx_list[ i ] );

		mtx.Multiply( scale_mtx );

		sceGuSetMatrix( GU_MODEL, mtx );

		CGfx::DrawPoly3D( GU_TRIANGLES, s_Cube, 12 * 3 );
	}


	//
	//	Render the alpha blended floor
	//
	sceGuFrontFace( GU_CCW );
	sceGuDisable( GU_STENCIL_TEST );
	sceGuBlendFunc( GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0 );

	sceGuSetMatrix( GU_MODEL, gIdentity );
	CGfx::DrawPoly3D( GU_TRIANGLES, s_Grid, NUM_GRIDS * NUM_GRIDS * 6 );

	//
	//	Draw the normal cubes
	//
	for ( u32 i = 0; i < s_CubeList.size(); ++i )
	{
		sceGuSetMatrix( GU_MODEL, cube_mtx_list[ i ] );

		CGfx::DrawPoly3D( GU_TRIANGLES, s_Cube, 12 * 3 );
	}

	sceGuBlendFunc( GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0 );


	//
	//	Render credits text
	//
	RenderCredits();
}

//**********************************************************************************
//
//**********************************************************************************
void	CCredits::UpdateCamera()
{
	static const float	CAMERA_FOCUS_SPEED( 0.5f );
	static const float	CAMERA_OFFSET_SPEED( 0.3f );
	static const float	CAMERA_ROTATION_SPEED( 20.f );
	static const float	CAMERA_CHANGE_TIME( 10.f );

	//
	//	Spin the camera around the origin
	//
	const float			elapsed_time( CFrameWork::GetElapsedTime() );

	m_fCamRotation += CAMERA_ROTATION_SPEED * elapsed_time;

	m_fCamFocus += ( m_fWantedCamFocus - m_fCamFocus ) * CAMERA_FOCUS_SPEED * elapsed_time;
	m_fCamOffset += ( m_fWantedCamOffset - m_fCamOffset ) * CAMERA_OFFSET_SPEED * elapsed_time;

	const V3	cam_pos( RotateY( m_fCamOffset, DEGS_TO_RADS( m_fCamRotation ) ) );
	const V3	cam_focus( 0.f, m_fCamFocus, 0.f );

	m_pCamera->SetPos( cam_pos );
	m_pCamera->SetFocus( cam_focus );

	m_CameraTimer += elapsed_time;

	//
	//	Choose a new camera position
	//
	if ( m_CameraTimer > CAMERA_CHANGE_TIME )
	{
		PickNewCameraOrientation();

		m_CameraTimer -= CAMERA_CHANGE_TIME;
	}
}

//**********************************************************************************
//	Pick random points around the origin
//**********************************************************************************
void	CCredits::PickNewCameraOrientation()
{
	m_fWantedCamFocus = static_cast< float >( rand() % 300 ) / 100.f;
	m_fWantedCamOffset.x = 10.f + ( static_cast< float >( rand() % 2000 ) / 100.f );
	m_fWantedCamOffset.y = 4.f + ( static_cast< float >( rand() % 2000 ) / 100.f );
}

//**********************************************************************************
//
//**********************************************************************************
void	CCredits::RenderCredits()
{
	CFont * const			p_font( CFont::GetDefaultFont() );
	const sCredit * const	p_credit( &s_Credits[ s_CurrentCredit ] );
	const sCredit * const	p_prev_credit( &s_Credits[ s_CurrentCredit - 1 ] );
	const ARGB				color( static_cast< u32 >( 255.f * s_FadeTimer ), 255, 255, 255 );
	const ARGB				shadow_color( static_cast< u32 >( 128.f * s_FadeTimer ), 0, 0, 0 );
	const ARGB				prev_color( static_cast< u32 >( 255.f * ( 1.f - s_FadeTimer ) ), 255, 255, 255 );
	const ARGB				prev_shadow_color( static_cast< u32 >( 128.f * ( 1.f - s_FadeTimer ) ), 0, 0, 0 );
	const V2				shadow_offset( 4.f, 4.f );

	sceGuClear( GU_DEPTH_BUFFER_BIT | GU_STENCIL_BUFFER_BIT );

	if ( prev_color.a > 0 )
	{
		p_font->Print( p_prev_credit->m_szText, p_prev_credit->m_Pos + shadow_offset, prev_shadow_color, 1.f, false );
		p_font->Print( p_prev_credit->m_szText, p_prev_credit->m_Pos, prev_color, 1.f, false );
	}

	if ( color.a > 0 )
	{
		p_font->Print( p_credit->m_szText, p_credit->m_Pos + shadow_offset, shadow_color, 1.f, false );
		p_font->Print( p_credit->m_szText, p_credit->m_Pos, color, 1.f, false );
	}

	//
	//	Do we want the next credit yet?
	//
	s_fCreditTimer += CFrameWork::GetElapsedTime();

	if ( s_fCreditTimer > p_credit->m_fTime )
	{
		s_FadeTimer = 0.f;
		s_fCreditTimer -= p_credit->m_fTime;

		++s_CurrentCredit;

		if ( s_CurrentCredit == s_nNumCredits )
		{
			s_CurrentCredit = 1;
		}

		ShakeCubes();
	}

	s_FadeTimer += CFrameWork::GetElapsedTime();

	if ( s_FadeTimer > 1.f )
	{
		s_FadeTimer = 1.f;
	}
}

//**********************************************************************************
//
//**********************************************************************************
bool	CCredits::HasQuit() const
{
	return m_bQuit;
}

//*******************************  END OF FILE  ************************************
