#ifndef VIEWER_UPDATER_H
#define VIEWER_UPDATER_H

#include "TileMap.h"
#include "Viewer.h"

void CreateViewBox(Viewer* _viewer, TileMap* _tileMap);
void ClearViewBox(Viewer* _viewer, TileMap* _tileMap);

#endif