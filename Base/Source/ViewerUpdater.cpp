#include "ViewerUpdater.h"

void UpdateViewBox(Viewer* _viewer, TileMap* _tileMap)
{
	static const int INITIAL_WIDTH = 1;

	Vector2 viewerPos =_viewer->viewer_GetTilePos();
	Vector2 viewerDir = _viewer->viewer_GetDirection();

	for (int rangeY = 0; rangeY < _viewer->GetMaxRangeY(); ++rangeY)
	{
		// The horizontal range of this row
		int viewRowSide = rangeY * _viewer->GetViewSideGrowthX();
		int viewRowWidth = viewRowSide * 2 + INITIAL_WIDTH;

		for (int rangeX = -viewRowSide; rangeX < viewRowSide; ++rangeX)
		{
			int posX = viewerPos.x + rangeX;
			int posY = viewerPos.y + rangeY;

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
