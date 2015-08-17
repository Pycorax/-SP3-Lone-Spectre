#include "TerrainCamera.h"

#include "LoadHmap.h"

TerrainCamera::TerrainCamera()
{
}


TerrainCamera::~TerrainCamera()
{
}

void TerrainCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up, const vector<unsigned char> &heightMap, const Vector3 &terrainScale)
{
	Camera3::Init(pos, target, up);

	// Terrain
	InitTerrain(heightMap, terrainScale);
}

void TerrainCamera::Update(double dt)
{
	Camera3::Update(dt);

	/*
	* Terrain
	*/
	if (sCameraType == LAND_CAM && m_camMoveMode != JUMP_MOVE)
	{
		// Set camera height according to heightmap
		float terrainHeight = ReadHeightMap(m_heightMap, position.x / m_terrainScale.x, position.z / m_terrainScale.z) * m_terrainScale.y + CAMERA_HEIGHT[m_camMoveMode];
		float dtMove = terrainHeight - position.y;
		position.y = terrainHeight;
		target.y += dtMove;
	}
}

void TerrainCamera::UpdateJump(const double dt)
{
	if (m_camMoveMode == JUMP_MOVE)
	{
		// Factor in gravity
		m_fJumpVel += GRAVITY * static_cast<float>(dt);
		// Update the camera and target position
		position.y += m_fJumpVel * static_cast<float>(dt);
		target.y += m_fJumpVel * static_cast<float>(dt);

		// Check camera height according to heightmap
		float terrainHeight = ReadHeightMap(m_heightMap, position.x / m_terrainScale.x, position.z / m_terrainScale.z) * m_terrainScale.y + CAMERA_HEIGHT[m_camMoveMode];

		// Check if the camera has reached the ground
		if (position.y <= terrainHeight)
		{
			float dtMove = terrainHeight - position.y;
			position.y = terrainHeight;
			target.y += dtMove;
			m_fJumpVel = 0.0f;
			m_camMoveMode = DEFAULT_MOVE;
		}
	}
}

void TerrainCamera::InitTerrain(vector<unsigned char> heightMap, Vector3 terrainScale)
{
	m_heightMap = heightMap;
	m_terrainScale = terrainScale;
}