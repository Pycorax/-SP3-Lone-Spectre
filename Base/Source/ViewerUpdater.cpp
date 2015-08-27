#include "ViewerUpdater.h"

void CreateViewBox(Viewer* _viewer, TileMap* _tileMap)
{
	static const int INITIAL_WIDTH = 2;

	Vector2 viewerPos =_viewer->viewer_GetTilePos();
	Vector2 viewerDir = _viewer->viewer_GetDirection();

	// Loop through front to back
	for (int rangeY = 1; rangeY <= _viewer->GetMaxRangeY(); ++rangeY)
	{
		// The horizontal range of this row
		int viewRowSide = rangeY * _viewer->GetViewSideGrowthX();
		int viewRowWidth = viewRowSide * 2 + INITIAL_WIDTH;

		// Loop through left to right
		for (int rangeX = -viewRowSide; rangeX <= viewRowSide; ++rangeX)
		{
			int posX = viewerPos.x;
			int posY = viewerPos.y;

			if (viewerDir == Direction::DIRECTIONS[Direction::DIR_UP])			// Down
			{
				posX += rangeX;
				posY += rangeY;
			}
			else if (viewerDir == Direction::DIRECTIONS[Direction::DIR_DOWN])		// Up
			{
				posX += rangeX;
				posY -= rangeY;
			}
			else if (viewerDir == Direction::DIRECTIONS[Direction::DIR_LEFT])		// Left
			{
				posX -= rangeY;
				posY += rangeX;
			}
			else if (viewerDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])		// Right
			{
				posX += rangeY;
				posY += rangeX;
			}

			// The tile that is being looked at
			Tile* _tile = _tileMap->GetTileAt(posX, posY);

			// Prevent any invalid tiles from being added
			if (_tile != NULL && !_tileMap->IsObstructed(viewerPos, Vector2(posX, posY), vector<Vector2>()))
			{
				_tile->AddViewer(_viewer);
				// Store this tile into the viewer for clearing later
				_viewer->AddViewedTileIndex(Vector2(posX, posY));
			}
		}
	}
}

void ClearViewBox(Viewer* _viewer, TileMap* _tileMap)
{
	vector<Vector2> lastViewedPos = _viewer->GetViewedTileIndex();
	
	for (vector<Vector2>::iterator vec2Iter = lastViewedPos.begin(); vec2Iter != lastViewedPos.end(); ++vec2Iter)
	{
		Vector2 tilePos = *vec2Iter;

		// The tile that is being looked at
		Tile* _tile = _tileMap->GetTileAt(tilePos.x, tilePos.y);

		// Prevent any invalid tiles from being added
		if (_tile != NULL)
		{
			_tile->RemoveViewer(_viewer);
		}
	}

	_viewer->ClearViewedTileIndex();
}