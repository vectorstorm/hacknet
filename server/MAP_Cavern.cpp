#include <stdlib.h>
#include "MAP_Cavern.h"
#include "HN_Point.h"

#include <stdio.h>

#define RND(x)  (int)(rand() % (long)(x))
#define min(x,y) ( (x>y)?y:x )

//-----------------------------------------------------------------------------
//  Massive rip-off of some special level generation code in NetHack.
//
//  I've copied this code because I thought it was really cool.  It's
//  effectively random noise into a buffer, followed by several low-pass
//  filters, and produces very nice-looking natural cavern formations.
//  In NetHack it's not restrained to creating rock/floor combinations
//  as it is here, and I suspect it's used for swamps and other natural
//  terrains....  but I'm not sure.  Anyone have any comments on this?
//-----------------------------------------------------------------------------

mapCavern::mapCavern(uint8 width, uint8 height, uint8 depth):
	mapBase(width,height,depth)
{
}

mapCavern::~mapCavern()
{
}

int dir[16] = {
   -1, -1 /**/, -1, 0 /**/, -1, 1 /**/,
    0, -1 /**/,              0, 1 /**/,
    1, -1 /**/,  1, 0 /**/,  1, 1};
	    

void
mapCavern::Randomise()
{
	int limit = (m_width * m_height * 2) / 5;
	int count = 0;
	while ( count < limit )
	{
		int i, j;

		i = RND(m_width);
		j = RND(m_height);
		if ( WallAt(i,j) == WALL_Any )
		{
			WallAt(i,j) = WALL_None;
			count++;
		}
	}

#if 0
	for ( int i = 0; i < m_width * m_height; i++ )
	{
		int random = rand() % 4;
		hnFloor type = WALL_Any;
		
		switch( random )
		{
			case 0:
				type = WALL_Any;
				break;
			case 1:
				type = WALL_None;
				break;
			case 2:
				type = MATERIAL_Ice;
				break;
			case 3:
				type = MATERIAL_Lava;
				break;
		}

		m_tile[i] = type;
	}
#endif //0
}

void
mapCavern::HighPassOne()
{
	for ( int i = 0; i < m_width; i++ )
		for ( int j = 0; j < m_height; j++ )
		{
			int count = 0;
			for ( int dr = 0; dr < 8; dr++ )
				if ( WallAt(i + dir[dr*2],j + dir[(dr*2)+1] ) == WALL_None )
					count++;

			if ( count < 3 )
				WallAt(i,j) = WALL_Any;
			else if ( count > 4 )
				WallAt(i,j) = WALL_None;
		}
}

void
mapCavern::HighPassTwo()
{
	hnWallType *newGrid = new hnWallType[m_width * m_height];
	
	for ( int i = 0; i < m_width; i++ )
		for ( int j = 0; j < m_height; j++ )
		{
			int count = 0;
			for ( int dr = 0; dr < 8; dr++ )
				if ( WallAt(i+dir[dr*2],j+dir[(dr*2)+1]) == WALL_None )
					count++;

			if ( count == 5 )
				newGrid[i + (j*m_width)] = m_backgroundType.wall;
			else
				newGrid[i + (j*m_width)] = WallAt(i,j);
		}
	
	for ( int i = 0; i < m_width; i++ )
		for ( int j = 0; j < m_height; j++ )
			WallAt(i,j) = newGrid[i + (j*m_width)];

	delete newGrid;	
}

void
mapCavern::HighPassThree()
{
	hnWallType *newGrid = new hnWallType[m_width * m_height];
	
	for ( int i = 0; i < m_width; i++ )
		for ( int j = 0; j < m_height; j++ )
		{
			int count = 0;
			for ( int dr = 0; dr < 8; dr++ )
				if ( WallAt(i+dir[dr*2],j+dir[(dr*2)+1]) == WALL_None )
					count++;

			if ( count < 3 )
				newGrid[i + (j*m_width)] = m_backgroundType.wall;
			else
				newGrid[i + (j*m_width)] = WallAt(i,j);
		}
	
	for ( int i = 0; i < m_width; i++ )
		for ( int j = 0; j < m_height; j++ )
			WallAt(i,j) = newGrid[i + (j*m_width)];

	delete newGrid;	
}

void
mapCavern::Generate()
{
	for ( int i = 0; i < m_width * m_height; i++ )  //clear us to fully rock
	{
		m_tile[i].wall = WALL_Solid;
		m_tile[i].material = MATERIAL_Rock;
		m_tile[i].border = false;
	}
	
	Randomise();
	HighPassOne();
	HighPassTwo();
	HighPassThree();
	HighPassThree();
}
