#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "SpriteAnimation.h"
#include "Vertex.h"
#include <vector>

using std::vector;

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	// 2D
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateCrossHair(const std::string &meshName, float colour_r=1.0f, float colour_g=1.0f, float colour_b=0.0f, float length=1.0f);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float length = 1.f, float width = 1.f);
	static Mesh* GenerateEllipse(const std::string &meshName, Color color, float heightRadius, float widthRadius, unsigned int numSlice = 36);
	static Mesh* GenerateCircle(const std::string &meshName, Color color, float radius = 1.0f, unsigned int numSlice = 36);
	static Mesh* GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float outerR = 1.f, float innerR = 0.f);
	static Mesh* GenerateMeshFromSheet(const std::string &meshName, unsigned numRow = 1, unsigned numCol = 1, unsigned frameRow = 0, unsigned frameCol = 0, float posX = 0.f, float posY = 0.f);

	static Mesh* Generate2DMesh(const std::string &meshName, Color color, int pos_x, int pos_y, int width, int height);

	// 3D
	static Mesh* GenerateCube(const std::string &meshName, Color color, float length = 1.f);
	static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 1.f);
	static Mesh* GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height);
	static Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);
	static Mesh* GenerateText(const std::string &meshName, unsigned row, unsigned col);
	static Mesh* GenerateSkyPlane(const std::string &meshName, Color color, int slices,float PlanetRadius, float AtmosphereRadius, float hTile, float vTile);
	static Mesh* GenerateTerrainLegacy(const std::string &meshName, const std::string &file_path, std::vector<unsigned char> &heightMap);
	static Mesh * GenerateTerrain(const std::string & meshName, std::vector<unsigned char>& heightMap);
	static SpriteAnimation* GenerateSpriteAnimation(const std::string &meshName, unsigned numRow, unsigned numCol);
	static SpriteAnimation* GenerateSpriteAnimation2D(const std::string &meshName, unsigned numRow, unsigned numCol, int posX = 0, int posY = 0 , int ScaleX = 0, int ScaleY = 0);
	
	// Minimap
	static Mesh* GenerateMinimap(const std::string &meshName, Color color, float length = 1.f);
	static Mesh* GenerateMinimapBorder(const std::string &meshName, Color color, float length = 1.f);
	static Mesh* GenerateMinimapAvatar(const std::string &meshName, Color color, float length = 1.f);
};

#endif