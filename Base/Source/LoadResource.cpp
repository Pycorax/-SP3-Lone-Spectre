#include "LoadResource.h"
#include "SONIO.h"
#include "LoadTGA.h"
#include "MeshBuilder.h"
#include "SoundEngine.h"

map<string, Material> LoadMaterial(string filename)
{
	map<string, Material> matList;

	// Retrieve data as a Branch object from the SONFile
	Branch materials = SONIO::LoadSON(filename);

	const string ROOT_NAME = "MaterialContainer";

	if (materials.name == ROOT_NAME)
	{
		// For every Material
		for (size_t i = 0; i < materials.childBranches.size(); ++i)
		{
			Material temp;
			temp.name = materials.childBranches[i].name;
			// Find the attribute and add it to this Material object
			for (size_t j = 0; j < materials.childBranches[i].attributes.size(); ++j)
			{
				Component attribv3;
				string attribName = materials.childBranches[i].attributes[j].name;
				vector<float> values = StringToFloats(materials.childBranches[i].attributes[j].value);
				// Extract the values out
				if (attribName == "Ambient" ||
					attribName == "Diffuse" ||
					attribName == "Specular")
				{
					attribv3.Set(values[0], values [1], values[2]);
				}
				else if (attribName == "Shininess")
				{
					temp.kShininess = stof(materials.childBranches[i].attributes[j].value);
				}

				if (attribName == "Ambient")
				{
					temp.kAmbient = attribv3;
				}
				else if (attribName == "Diffuse")
				{
					temp.kDiffuse = attribv3;
				}
				else if (attribName == "Specular")
				{
					temp.kSpecular = attribv3;
				}
			}
			
			matList.insert(std::pair<string, Material>(temp.name, temp));
		}
	}

	return matList;
}

vector<Color> LoadColor(string filename, vector<string> &colListMap)
{
	vector<Color> colList;

	// Empties the vector so that it will be inline with the colours loaded in from the file
	colListMap.empty();

	// Retrieve data as a Branch object from the SONFile
	Branch colors = SONIO::LoadSON(filename);

	const string ROOT_NAME = "ColorContainer";

	if (colors.name == ROOT_NAME)
	{
		// For every Color
		for (size_t i = 0; i < colors.childBranches.size(); ++i)
		{
			Color temp;
			colListMap.push_back(colors.childBranches[i].name);
			// Find the attribute and add it to this Material object
			for (size_t j = 0; j < colors.childBranches[i].attributes.size(); ++j)
			{
				vector<float> values = StringToFloats(colors.childBranches[i].attributes[j].value);
				// Extract the values out
				if (colors.childBranches[i].attributes[j].name == "RGB")
				{
					temp.Set(values[0], values[1], values[2]);
				}
			}

			colList.push_back(temp);
		}
	}

	return colList;
}

vector<Light> LoadLight(string filename)
{
	// Lights to return when processed
	vector<Light> lightList;

	// Retrieve data as a Branch object from the SONFile
	Branch lights = SONIO::LoadSON(filename);

	const string ROOT_NAME = "LightContainer";
	const unsigned short MAX_LIGHTS = 8;

	if (lights.name == ROOT_NAME)
	{
		// For every Material
		for (size_t i = 0; i < lights.childBranches.size() && i < MAX_LIGHTS; ++i)
		{
			Light temp;
			// Find the attribute and add it to this Material object
			for (size_t j = 0; j < lights.childBranches[i].attributes.size(); ++j)
			{
				// Extract attribute
				string attribName = lights.childBranches[i].attributes[j].name;
				string attribVal = lights.childBranches[i].attributes[j].value;

				// Extract the values out
				vector<float> values;
				if (attribName == "Position" || attribName == "Color" || attribName == "SpotDirection")
				{
					values = StringToFloats(attribVal);
				}
				
				// Handle each value
				if (attribName == "Type")
				{
					if (attribVal == "LIGHT_POINT")
					{
						temp.type = Light::LIGHT_POINT;
					}
					else if (attribVal == "LIGHT_SPOT")
					{
						temp.type = Light::LIGHT_SPOT;
					}
					else // attribVal == "LIGHT_DIRECTIONAL"
					{
						temp.type = Light::LIGHT_DIRECTIONAL;
					}
				}
				else if (attribName == "Position")
				{
					Position pos;
					pos.Set(values[0], values[1], values[2]);
					temp.position = pos;
				}
				else if (attribName == "Color")
				{
					Color col;
					col.Set(values[0], values[1], values[2]);
					temp.color = col;
				}
				else if (attribName == "Power")
				{
					temp.power = stof(attribVal);
				}
				else if (attribName == "ConstantAttenuation")
				{
					temp.kC = stof(attribVal);
				}
				else if (attribName == "LinearAttenuation")
				{
					temp.kL = stof(attribVal);
				}
				else if (attribName == "Quadratic Attenuation")
				{
					temp.kQ = stof(attribVal);
				}
				else if (attribName == "CosineCutOff")
				{
					temp.cosCutoff = stof(attribVal);
					temp.cosCutoff = cos(Math::DegreeToRadian(temp.cosCutoff));
				}
				else if (attribName == "CosineInner")
				{
					temp.cosInner = stof(attribVal);
					temp.cosInner = cos(Math::DegreeToRadian(temp.cosInner));
				}
				else if (attribName == "Exponent")
				{
					temp.exponent = stof(attribVal);
				}
				else if (attribName == "SpotDirection")
				{
					Vector3 dir;
					dir.Set(values[0], values[1], values[2]);
					temp.spotDirection = dir;
				}
			}

			lightList.push_back(temp);
		}
	}

	return lightList;
}

vector<GameObject2D*> LoadGameObject(string filename, const vector<Mesh*>& meshList)
{
	vector<GameObject2D*> goList;

	// Retrieve data as a Branch object from the SONFile
	Branch gObjects = SONIO::LoadSON(filename);

	const string ROOT_NAME = "GObjectContainer";

	if (gObjects.name == ROOT_NAME)
	{
		// For every GameObject2D
		for (size_t i = 0; i < gObjects.childBranches.size(); ++i)
		{
			#pragma region InitVars

				bool	tmpBool;
				vector<float> values;
				GameObject2D* go = NULL;
				Mesh* goMesh = NULL;
				Vector3 goPos;
				Vector3 goRot;
				Vector3 goScale;
				Vector3 goBoundExt;
				Vector3 goBoundOffset;
				bool goCollidable = false;
				bool goLighted = true;
				bool goFogged = false;
				bool goBillboard = false;

			#pragma endregion

			// Find the attribute and add it to this GameObject2D
			for (size_t j = 0; j < gObjects.childBranches[i].attributes.size(); ++j)
			{
				// Extract the values out
				if (gObjects.childBranches[i].attributes[j].name == "Mesh")
				{
					// Look for and obtain the Mesh
					for (vector<Mesh*>::const_iterator it = meshList.begin(); it != meshList.end(); ++it)
					{
						if ((*it)->name == gObjects.childBranches[i].attributes[j].value)
						{
							// Set the Mesh
							goMesh = *it;
							break;
						}
					}
				}
				else if (gObjects.childBranches[i].attributes[j].name == "Translation")
				{
					// Get Vector3 Values
					values = StringToFloats(gObjects.childBranches[i].attributes[j].value);
					goPos.Set(values[0], values[1], values[2]);

				}
				else if (gObjects.childBranches[i].attributes[j].name == "Rotation")
				{
					// Get Vector3 Values
					values = StringToFloats(gObjects.childBranches[i].attributes[j].value);
					goRot.Set(values[0], values[1], values[2]);
				}
				else if (gObjects.childBranches[i].attributes[j].name == "Scale")
				{
					// Get Vector3 Values
					values = StringToFloats(gObjects.childBranches[i].attributes[j].value);
					goScale.Set(values[0], values[1], values[2]);
				}
				else if (gObjects.childBranches[i].attributes[j].name == "BoundExtension")
				{
					// Get Vector3 Values
					values = StringToFloats(gObjects.childBranches[i].attributes[j].value);
					goBoundExt.Set(values[0], values[1], values[2]);
				}
				else if (gObjects.childBranches[i].attributes[j].name == "BoundOffset")
				{
					// Get Vector3 Values
					values = StringToFloats(gObjects.childBranches[i].attributes[j].value);
					goBoundOffset.Set(values[0], values[1], values[2]);
				}
				else if (gObjects.childBranches[i].attributes[j].name == "Collidable")
				{
					tmpBool = ReadTextualBool(gObjects.childBranches[i].attributes[j].value);

					goCollidable = tmpBool;
				}
				else if (gObjects.childBranches[i].attributes[j].name == "Lighted")
				{
					tmpBool = ReadTextualBool(gObjects.childBranches[i].attributes[j].value);

					goLighted = tmpBool;
				}
				else if (gObjects.childBranches[i].attributes[j].name == "Fogged")
				{
					tmpBool = ReadTextualBool(gObjects.childBranches[i].attributes[j].value);

					goFogged = tmpBool;
				}
				else if (gObjects.childBranches[i].attributes[j].name == "Billboard")
				{
					tmpBool = ReadTextualBool(gObjects.childBranches[i].attributes[j].value);

					goBillboard = tmpBool;
				}
			}

			go = new GameObject2D;
			go->SetMesh(goMesh);
			go->SetRot(goRot);
			go->SetPos(goPos);
			go->SetScale(goScale);
			// TODO: GameObject3D
			//go->SetLighted(goLighted);
			//go->SetFogged(goFogged);
			//go->SetBillboard(goBillboard);

			goList.push_back(go);
		}
	}

	return goList;
}

vector<GameObject2D*>	 LoadVisualObject(string filename, const vector<Mesh*>& meshList)
{
	vector<GameObject2D*> voList;

	// Retrieve data as a Branch object from the SONFile
	Branch gObjects = SONIO::LoadSON(filename);

	const string ROOT_NAME = "VObjectContainer";

	if (gObjects.name == ROOT_NAME)
	{
		// For every VisualObject
		for (size_t i = 0; i < gObjects.childBranches.size(); ++i)
		{
			#pragma region InitVars

			bool	tmpBool;
			vector<float> values;
			GameObject2D* vo = NULL;
			Mesh* goMesh = NULL;
			Vector3 goPos;
			Vector3 goRot;
			Vector3 goScale;
			Transform voDeltaTransform;
			bool goLighted = true;
			bool goFogged = false;
			bool goBillboard = false;
			int voRepeat = 0;

			#pragma endregion

			// Find the attribute and add it to this GameObject2D
			for (size_t j = 0; j < gObjects.childBranches[i].attributes.size(); ++j)
			{
				// Extract the values out
				if (gObjects.childBranches[i].attributes[j].name == "Mesh")
				{
					// Look for and obtain the Mesh
					for (vector<Mesh*>::const_iterator it = meshList.begin(); it != meshList.end(); ++it)
					{
						if ((*it)->name == gObjects.childBranches[i].attributes[j].value)
						{
							// Set the Mesh
							goMesh = *it;
							break;
						}
					}
				}
				else if (gObjects.childBranches[i].attributes[j].name == "Translation")
				{
					// Get Vector3 Values
					values = StringToFloats(gObjects.childBranches[i].attributes[j].value);
					goPos.Set(values[0], values[1], values[2]);

				}
				else if (gObjects.childBranches[i].attributes[j].name == "Rotation")
				{
					// Get Vector3 Values
					values = StringToFloats(gObjects.childBranches[i].attributes[j].value);
					goRot.Set(values[0], values[1], values[2]);
				}
				else if (gObjects.childBranches[i].attributes[j].name == "Scale")
				{
					// Get Vector3 Values
					values = StringToFloats(gObjects.childBranches[i].attributes[j].value);
					goScale.Set(values[0], values[1], values[2]);
				}
				else if (gObjects.childBranches[i].attributes[j].name == "DeltaTranslation")
				{
					// Get Vector3 Values
					values = StringToFloats(gObjects.childBranches[i].attributes[j].value);
					voDeltaTransform.Translation = Vector3(values[0], values[1], values[2]);

				}
				else if (gObjects.childBranches[i].attributes[j].name == "DeltaRotation")
				{
					// Get Vector3 Values
					values = StringToFloats(gObjects.childBranches[i].attributes[j].value);
					voDeltaTransform.Rotation = Vector3(values[0], values[1], values[2]);
				}
				else if (gObjects.childBranches[i].attributes[j].name == "DeltaScale")
				{
					// Get Vector3 Values
					values = StringToFloats(gObjects.childBranches[i].attributes[j].value);
					voDeltaTransform.Scale = Vector3(values[0], values[1], values[2]);
				}
				else if (gObjects.childBranches[i].attributes[j].name == "Lighted")
				{
					tmpBool = ReadTextualBool(gObjects.childBranches[i].attributes[j].value);

					goLighted = tmpBool;
				}
				else if (gObjects.childBranches[i].attributes[j].name == "Fogged")
				{
					tmpBool = ReadTextualBool(gObjects.childBranches[i].attributes[j].value);

					goFogged = tmpBool;
				}
				else if (gObjects.childBranches[i].attributes[j].name == "Billboard")
				{
					tmpBool = ReadTextualBool(gObjects.childBranches[i].attributes[j].value);

					goBillboard = tmpBool;
				}
				else if (gObjects.childBranches[i].attributes[j].name == "Repeat")
				{
					voRepeat = stoi(gObjects.childBranches[i].attributes[j].value);
				}
			}

			for (int obj = 0; obj < voRepeat; ++obj)
			{
				vo = new GameObject2D;
				vo->SetMesh(goMesh);
				vo->SetPos(goPos + (obj * voDeltaTransform.Translation));
				vo->SetRot(goRot + (obj * voDeltaTransform.Rotation));
				vo->SetScale(goScale + (obj * voDeltaTransform.Scale));
				//vo->SetLighted(goLighted);
				//vo->SetFogged(goFogged);
				//vo->SetBillboard(goBillboard);
				
				voList.push_back(vo);
			}
		}
	}

	return voList;
}

map<string, unsigned> LoadSounds(string filename)
{
	map<string, unsigned> soundList;

	// Retrieve data as a Branch object from the SONFile
	Branch sounds = SONIO::LoadSON(filename);

	const string ROOT_NAME = "SoundContainer";

	if (sounds.name == ROOT_NAME)
	{
		// For every Sound
		for (size_t i = 0; i < sounds.childBranches.size(); ++i)
		{
			// Find the attribute and add it to this Material object
			for (size_t j = 0; j < sounds.childBranches[i].attributes.size(); ++j)
			{
				Attribute attrib = sounds.childBranches[i].attributes[j];

				// Extract the values out
				if (attrib.name == "FilePath")
				{
					soundList.insert(std::pair<string, unsigned>(sounds.childBranches[i].name, SoundEngine::AddSoundSource(attrib.value)));
				}
			}
		}
	}

	return soundList;
}

vector<float> StringToFloats(string str)
{
	const char DELIMITER = ',';
	vector<float> result;
	short startPos = 0;

	for (size_t i = 0; i < str.length(); ++i)
	{
		// If hit delimiter or reached end of the string
		if (str[i] == DELIMITER || i == str.length() - 1)
		{
			float part = stof(str.substr(startPos,  i - startPos));

			startPos = i;

			result.push_back(part);
		}

		if (str[i] == DELIMITER)
		{
			// Skip the delimiter
			++startPos;
		}
	}

	return result;
}

bool ReadTextualBool(string str)
{
	return
		(
			str == "true" ||
			str == "1"
		);
}
