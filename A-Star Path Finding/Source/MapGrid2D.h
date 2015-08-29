/******************************************************************************/
/*!
\file	MapGrid2D.h
\author Tng Kah Wei
\brief
Abstract class to define a type of 2D world map that uses a tile based grid 
structure.

This abstract class is meant to provide an interface between the AINodeGrid
and other tile based grid structures.
*/
/******************************************************************************/
#ifndef MAP_GRID_2D
#define MAP_GRID_2D

class MapGrid2D
{
public:
	MapGrid2D(void);
	virtual ~MapGrid2D(void);

	/*
	*	Abstract Functions
	*	These functions need to be defined in child classes and serves as the interface between child classes and this class.
	*/
	// Function to allow AINodeGrid to find out if the tile at the position provided can be passed through
	virtual bool mapGrid2D_TileIsBlocked(int xPos, int yPos) const = 0;
	// Function to allow AINodeGrid to obtain the tile size of the grid map via reference
	virtual void mapGrid2D_GetMapSize(unsigned& width, unsigned& height) const = 0;
};

#endif
