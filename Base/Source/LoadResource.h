#ifndef LOAD_RESOURCE_H
#define LOAD_RESOURCE_H

#include "Material.h"
#include "Light.h"
#include "Mesh.h"
#include <vector>
#include <map>
#include <string>
#include "GameObject2D.h"

using std::vector;
using std::map;
using std::string;

// Load Resource Function
map<string, Material>	LoadMaterial(string filename);
vector<Color>			LoadColor(string filename, vector<string> &colListMap);
vector<Light>			LoadLight(string filename);
vector<GameObject2D*>		LoadGameObject(string filename, const vector<Mesh*>& meshList);
vector<GameObject2D*>		LoadVisualObject(string filename, const vector<Mesh*>& meshList);
map<string, unsigned>	LoadSounds(string filename);

// Helper Functions
vector<float> StringToFloats(string str);
bool ReadTextualBool(string str);

#endif