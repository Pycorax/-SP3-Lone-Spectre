#include "MVC_Model_3D.h"

#include "SONIO.h"
#include "LoadHmap.h"
#include "TerrainCamera.h"

MVC_Model_3D::MVC_Model_3D(string configSONFile) : MVC_Model(configSONFile)
{
	loadConfig();
}


MVC_Model_3D::~MVC_Model_3D()
{
}

void MVC_Model_3D::Init()
{
	MVC_Model::Init();

	// TODO: GameObject3D
	/* 
	if (m_terrainMeshName != "")
	{
		// Find the terrain mesh
		Mesh* terrainMesh = GetMeshResource(m_terrainMeshName);

		// Add the terrain mesh as a VisualObject
		GameObject2D* vo = new GameObject2D(terrainMesh);
		vo->SetScale(m_terrainScale);
		vo->SetFogged(true);
		vo->SetLighted(true);
		m_renderList3DStatic.insert(m_renderList3DStatic.begin(), vo);

		// Initialize Camera if is Terrain Camera
		TerrainCamera* tCam = dynamic_cast<TerrainCamera*>(m_camera);
		if (tCam != NULL)
		{
			tCam->InitTerrain(m_heightMap, m_terrainScale);
		}
	}
	*/
}

vector<unsigned char> MVC_Model_3D::GetHeightMap()
{
	return m_heightMap;
}

void MVC_Model_3D::SetHeightMap(vector<unsigned char>& heightMap)
{
	m_heightMap = heightMap;
}

void MVC_Model_3D::loadConfig()
{
	Branch cfg = SONIO::LoadSON(m_configSONFile);

	const string ROOT_NAME = "ModelConfigContainer";

	if (cfg.name == ROOT_NAME)
	{
		for (vector<Attribute>::iterator attrib = cfg.attributes.begin(); attrib != cfg.attributes.end(); ++attrib)
		{
			if (attrib->name == "HeightMapFile")
			{
				LoadHeightMap(attrib->value.c_str(), m_heightMap);
			}
			else if (attrib->name == "TerrainScale")
			{
				m_terrainScale = Vector3::StringToVector3(attrib->value);
			}
			else if (attrib->name == "TerrainMesh")
			{
				m_terrainMeshName = attrib->value;
			}
		}
	}
}