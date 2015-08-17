#ifndef MVC_MODEL_3D_H
#define MVC_MODEL_3D_H

#include "MVC_Model.h"

class MVC_Model_3D : public MVC_Model
{
	// Terrain Maps
	string m_terrainMeshName;
	Vector3 m_terrainScale;
	vector<unsigned char> m_heightMap;

	public:
		MVC_Model_3D(string configSONFile);
		virtual ~MVC_Model_3D();

		virtual void Init(void);
		vector<unsigned char> GetHeightMap(void);

		void SetHeightMap(vector<unsigned char>& heightMap);

	protected:
		virtual void loadConfig(void);
};

#endif