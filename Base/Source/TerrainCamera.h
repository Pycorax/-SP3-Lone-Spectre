#ifndef TERRAIN_CAMERA_H
#define TERRAIN_CAMERA_H

#include "Camera3.h"

class TerrainCamera : public Camera3
{
	// Terrain Collision
	vector<unsigned char> m_heightMap;
	Vector3 m_terrainScale;

	public:
		TerrainCamera();
		~TerrainCamera();

		virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up, const vector<unsigned char> &heightMap, const Vector3 &terrainScale);
		virtual void Update(double dt);

		virtual void UpdateJump(const double dt);

		void InitTerrain(vector<unsigned char> heightMap, Vector3 terrainScale);
};

#endif;