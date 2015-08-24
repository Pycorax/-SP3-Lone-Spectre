#include "ViewerUpdater.h"

void UpdateViewBox(Viewer* _viewer, TileMap* _tileMap)
{
	static const int INITIAL_WIDTH = 1;

	Vector2 viewerPos =_viewer->viewer_GetTilePos();
	Vector2 viewerDir = _viewer->viewer_GetDirection();

	// Loop through front to back
	for (int rangeY = 1; rangeY <= _viewer->GetMaxRangeY(); ++rangeY)
	{
		// The horizontal range of this row
		int viewRowSide = rangeY * _viewer->GetViewSideGrowthX();
		int viewRowWidth = viewRowSide * 2 + INITIAL_WIDTH;

		// Loop through left to right
		for (int rangeX = -viewRowSide; rangeX < viewRowSide; ++rangeX)
		{
			int posX = viewerPos.x;
			int posY = viewerPos.y;

			if (viewerDir == Vector2(0.0f, 1.0f))			// Down
			{
				posX += rangeX;
				posY += rangeY;
			}
			else if (viewerDir == Vector2(0.0f, -1.0f))		// Up
			{
				posX += rangeX;
				posY -= rangeY;
			}
			else if (viewerDir == Vector2(-1.0f, 0.0f))		// Left
			{
				posX -= rangeY;
				posY += rangeX;
			}
			else if (viewerDir == Vector2(1.0f, 0.0f))		// Right
			{
				posX += rangeY;
				posY += rangeX;
			}

			// The tile that is being looked at
			Tile* _tile = _tileMap->GetTileAt(posX, posY);

			// Prevent any invalid tiles from being added
			if (_tile != NULL)
			{
				_tile->AddViewer(_viewer);
			}
		}
	}
}
