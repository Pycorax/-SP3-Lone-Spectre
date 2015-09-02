#include "MVC_Model.h"

// For debug traces
#include <iostream>
#include "timer.h"

#include <sstream>
#include "LoadResource.h"
#include "SONIO.h"
#include "LoadTGA.h"
#include "Camera3.h"
#include "TerrainCamera.h"
#include "SpriteAnimation.h"

using std::ostringstream;

MVC_Model::MVC_Model(string configSONFile) 
	: m_configSONFile(configSONFile)
	, m_running(true)
	, m_lastInputDevice(ID_KB_MOUSE)
{
	loadConfig();

	Math::InitRNG();
}

MVC_Model::~MVC_Model()
{
}

string MVC_Model::GetMeshSONFile(void) const
{
	return m_meshSONFile;
}

string MVC_Model::GetTexSONFile(void) const
{
	return m_texSONFile;
}

void MVC_Model::AssignMeshes(vector<Mesh*>& _meshList)
{
	meshList = _meshList;
}

void MVC_Model::AssignTextures(map<string, unsigned>& _texList)
{
	texList = _texList;
}

void MVC_Model::GetViewRes(int& x, int& y)
{
	x = m_viewWidth;
	y = m_viewHeight;
}

void MVC_Model::UpdateViewRes(int width, int height)
{
	int oldWidth = m_viewWidth;
	int oldHeight = m_viewHeight;

	m_viewWidth = width;
	m_viewHeight = height;

	onResolutionChanged(oldWidth, oldHeight);
}

void MVC_Model::UpdateLastInputDevice(E_INPUT_DEVICE_TYPE inputType)
{
	m_lastInputDevice = inputType;
}

void MVC_Model::Init(void)
{
	// Initialize Light Config from File
	InitLights();

	// Init RNG
	Math::InitRNG();

	/*
	* Resource Initialization from File
	*/
	// Colors
	InitColors();
	// Materials
	InitMaterials();
	// Meshes -- Apply Textures and Materials to the Meshes. Actual MeshBuilding handled by MVC_View in Pre-Initialization
	InitMeshes();
	// GameObjects
	InitGameObjects();
	// VisualObjects
	// InitVisualObjects();		// TODO: Reimplement when 3D is required
	// SoundObjects
	InitSounds();
	// DefaultFont
	m_defaultFont = GetMeshResource(m_fontName);

	// Initialize Input
	resetKeysPressed();

	// Initializes Statistics
	m_fps = 0;
	m_fpsCount = new TextObject(m_defaultFont, "FPS: ");
	m_fpsCount->SetPos(Vector3(5.0f, 24.0f));
	m_fpsCount->SetScale(Vector3(3.0f));
}

void MVC_Model::Update(double dt)
{
	m_fps = static_cast<float>(1.f / dt);
	m_camera->Update(dt);

	// Empties the render lists for this update
	if (m_renderList3D.size() > 0)
	{
		throw std::runtime_error("RenderList3D leak detected!");
	}

	if (m_renderList2D.size() > 0)
	{
		throw std::runtime_error("RenderList2D leak detected!");
	}

	// Update Sprite Animations in the Mesh List
	for (std::vector<Mesh*>::iterator it = meshList.begin(); it != meshList.end(); ++it)
	{
		SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>((*it));
		if (sa != NULL)
		{
			sa->Update(dt);
		}
	}

	// Update FPS Counter
	static ostringstream oss;
	oss.str("");

	oss << "FPS: " << m_fps;

	m_fpsCount->SetText(oss.str());

	// Process input
	processKeyAction(dt);
	resetKeysPressed();
}

void MVC_Model::Exit(void)
{
	// Clear Mesh Resource
	while (meshList.size() > 0)
	{
		delete meshList.back();
		meshList.pop_back();
	}

	for (map<string, unsigned>::iterator it = texList.begin(); it != texList.end(); ++it)
	{
		ClearTGA(it->second);
	}

	if (m_camera)
	{
		delete m_camera;
		m_camera = NULL;
	}

	if (m_fpsCount)
	{
		delete m_fpsCount;
		m_fpsCount = NULL;
	}

	// You think m_defaultFont wasn't deleted? It was deleted in the meshList already
	m_defaultFont = NULL;
}

bool MVC_Model::IsRunning() const
{
	return m_running;
}

void MVC_Model::InitLights(void)
{
#ifdef _DEBUG
	StopWatch initTimer;
	initTimer.startTimer();

	std::cout << "Loading " << m_lightSONFile << "... ";
#endif

	m_lights = LoadLight(m_lightSONFile);

#ifdef _DEBUG
	std::cout << "Loaded! (" << initTimer.getElapsedTime() << "s)" << std::endl;
#endif
}

Color MVC_Model::GetBGCol(void) const
{
	return m_bgColor;
}

Camera3 * MVC_Model::GetCamera(void) const
{
	return m_camera;
}

vector<Light> MVC_Model::GetLights(void) const
{
	return m_lights;
}

Fog MVC_Model::GetFog(void) const
{
	return m_fog;
}

Vector3 MVC_Model::GetWorldSize() const
{
	return m_worldSize;
}

queue<GameObject2D*>* MVC_Model::Get3DRenderList(void)
{
	return &m_renderList3D;
}

queue<GameObject2D*>* MVC_Model::Get2DRenderList(void)
{
	return &m_renderList2D;
}

bool MVC_Model::IsMouseFree(void)
{
	return m_bFreeMouse;
}

void MVC_Model::ActivateKey(KEY_ACTION_TYPE keyAction)
{
	m_bKeyPressed[keyAction] = true;
}

void MVC_Model::UpdateMouseInfo(double x, double y)
{
	m_mousePosX = x;
	m_mousePosY = y;
}

void MVC_Model::UpdateXInput(float lX, float lY, float rX, float rY, float lT, float rT)
{
	m_leftThumbStick.Set(lX, lY);
	m_rightThumbStick.Set(rX, rY);
	m_leftTrigger = lT;
	m_rightTrigger = rT;
}

float MVC_Model::GetFrameRate(void)
{
	return m_fps;
}

void MVC_Model::loadConfig()
{
	Branch cfg = SONIO::LoadSON(m_configSONFile);

	const string ROOT_NAME = "ModelConfigContainer";

	Vector3 camPos;
	Vector3 camTar;
	Vector3 camUp;
	bool camPersp = true;
	double camONear = 1.0f;
	double camOFar = 1000.0f;
	// Perspective-Specific
	float camFOV = 45;

	if (cfg.name == ROOT_NAME)
	{
		for (vector<Attribute>::iterator attrib = cfg.attributes.begin(); attrib != cfg.attributes.end(); ++attrib)
		{
			if (attrib->name == "BackgroundColor")
			{
				vector<float> col = StringToFloats(attrib->value);
				m_bgColor.Set(col[0], col[1], col[2]);
			}
			// Fog
			else if (attrib->name == "FogStart")
			{
				m_fog.start = stof(attrib->value);
			}
			else if (attrib->name == "FogEnd")
			{
				m_fog.end = stof(attrib->value);
			}
			else if (attrib->name == "FogStart")
			{
				m_fog.start = stof(attrib->value);
			}
			else if (attrib->name == "FogDensity")
			{
				m_fog.density = stof(attrib->value);
			}
			else if (attrib->name == "FogType")
			{
				m_fog.SetType(stoi(attrib->value));
			}
			else if (attrib->name == "FogColor")
			{
				vector<float> col = StringToFloats(attrib->value);
				m_fog.col.Set(col[0], col[1], col[2]);
			}
			else if (attrib->name == "FogEnabled")
			{
				m_fog.enabled = ReadTextualBool(attrib->value);
			}
			// Camera
			else if (attrib->name == "CameraType")
			{
				if (attrib->value == "Camera3")
				{
					m_camera = new Camera3;
				}
				else if (attrib->value == "TerrainCamera")
				{
					m_camera = new TerrainCamera;
				}
#ifdef _DEBUG
				else
				{
					std::cout << attrib->value << " is not a valid Camera Type!" << std::endl;
				}
#endif
			}
			else if (attrib->name == "CameraMode")
			{
				if (attrib->value == "Perspective")
				{
					camPersp = true;
				}
				else if (attrib->value == "Orthographic")
				{
					camPersp = false;
				}
#ifdef _DEBUG
				else
				{
					std::cout << attrib->value << " is not a valid Camera Mode!" << std::endl;
				}
#endif
			}
			else if (attrib->name == "CameraPosition")
			{
				vector<float> pos = StringToFloats(attrib->value);
				camPos.Set(pos[0], pos[1], pos[2]);
			}
			else if (attrib->name == "CameraTarget")
			{
				vector<float> tar = StringToFloats(attrib->value);
				camTar.Set(tar[0], tar[1], tar[2]);
			}
			else if (attrib->name == "CameraUp")
			{
				vector<float> up = StringToFloats(attrib->value);
				camUp.Set(up[0], up[1], up[2]);
			}
			else if (attrib->name == "CameraFOV")
			{
				camFOV = stof(attrib->value);
			}
			else if (attrib->name == "CameraNear")
			{
				camONear = stof(attrib->value);
			}
			else if (attrib->name == "CameraFar")
			{
				camOFar = stof(attrib->value);
			}
			// Resource
			else if (attrib->name == "MeshSONFile")
			{
				m_meshSONFile = attrib->value;
			}
			else if (attrib->name == "TextureSONFile")
			{
				m_texSONFile = attrib->value;
			}
			else if (attrib->name == "MaterialSONFile")
			{
				m_materialSONFile = attrib->value;
			}
			else if (attrib->name == "ColorSONFile")
			{
				m_colorSONFile = attrib->value;
			}
			else if (attrib->name == "GameObjectSONFile")
			{
				m_goSONFile = attrib->value;
			}
			else if (attrib->name == "VisualObjectSONFile")
			{
				m_voSONFile = attrib->value;
			}
			else if (attrib->name == "LightSONFile")
			{
				m_lightSONFile = attrib->value;
			}
			else if (attrib->name == "SoundSONFile")
			{
				m_soundSONFile = attrib->value;
			}
			// Fonts
			else if (attrib->name == "DefaultFontMesh")
			{
				m_fontName = attrib->value;
			}
			// World
			else if (attrib->name == "WorldSize")
			{
				vector<float> size = StringToFloats(attrib->value);
				m_worldSize.Set(size[0], size[1], size[2]);
			}
			// Game Resolution
			else if (attrib->name == "ViewWidth")
			{
				m_viewWidth = stoi(attrib->value);
			}
			else if (attrib->name == "ViewHeight")
			{
				m_viewHeight = stoi(attrib->value);
			}
			// Other
			else if (attrib->name == "MouseFree")
			{
				m_bFreeMouse = ReadTextualBool(attrib->value);
			}
		}

		m_camera->Init(camPos, camTar, camUp, camFOV);
		m_camera->SetPerspective(camPersp);
		m_camera->SetNear(camONear);
		m_camera->SetFar(camOFar);
	}
}

void MVC_Model::InitMaterials()
{
#ifdef _DEBUG
	StopWatch initTimer;
	initTimer.startTimer();

	std::cout << "Loading " << m_materialSONFile << "... ";
#endif

	matList = LoadMaterial(m_materialSONFile);
	// Push the default Material to the 
	Material temp;
	temp.name = "DefaultMaterial";
	matList.insert(std::pair<string, Material>(temp.name, temp));

#ifdef _DEBUG
	std::cout << "Loaded! (" << initTimer.getElapsedTime() << "s)" << std::endl;
#endif
}

void MVC_Model::InitColors()
{
#ifdef _DEBUG
	StopWatch initTimer;
	initTimer.startTimer();

	std::cout << "Loading " << m_colorSONFile << "... ";
#endif

	colList = LoadColor(m_colorSONFile, colListMap);
	// Push the default Color to the back
	colList.push_back(Color());
	colListMap.push_back("Generic Color");

#ifdef _DEBUG
	if (colList.size() != colListMap.size())
	{
		std::cout << "Error: " << m_colorSONFile << " has resulted in unequal amounts of Color and Color Names!" << std::endl;
	}
	else
	{
		std::cout << "Loaded! (" << initTimer.getElapsedTime() << "s)" << std::endl;
	}
#endif
}

void MVC_Model::InitMeshes()
{
	// Retrieve data as a Branch object from the SONFile
	Branch meshes = SONIO::LoadSON(m_meshSONFile);

	const string ROOT_NAME = "MeshContainer";

	string attribName = "";
	string attribVal = "";

	// For every Mesh
	for (size_t i = 0; i < meshes.childBranches.size(); ++i)
	{
		bool found = false;

		// Find the Mesh in meshList equal to this Mesh in the SONFile
		for (vector<Mesh*>::iterator it = meshList.begin(); it != meshList.end(); ++it)
		{
			if (meshes.childBranches[i].name == (*it)->name)
			{
				int texCount = 0;

				// Find the attribute and add it to the Mesh Attributes
				for (size_t j = 0; j < meshes.childBranches[i].attributes.size(); ++j)
				{
					attribName = meshes.childBranches[i].attributes[j].name;
					attribVal = meshes.childBranches[i].attributes[j].value;

					if (attribName == "Texture")
					{
						// Find Texture from the textList and assign the Texture
						map<string, unsigned>::const_iterator it2 = texList.find(attribVal);
						if (it2 != texList.end())
						{
							if (texCount < Mesh::MAX_TEXTURES)
							{
								(*it)->textureArray[texCount] = it2->second;
								++texCount;
							}
						}
					}
					else if (attribName == "Material")
					{
						// Find Material from the matList and assign the Material
						map<string, Material>::const_iterator it2 = matList.find(attribVal);
						if (it2 != matList.end())
						{
							(*it)->material = it2->second;
						}
					}
				}

				break;
			}
		}		
	}

}

void MVC_Model::InitGameObjects()
{
#ifdef _DEBUG
	StopWatch initTimer;
	initTimer.startTimer();

	std::cout << "Loading " << m_goSONFile << "... ";
#endif

	goList = LoadGameObject(m_goSONFile, meshList);

#ifdef _DEBUG
	std::cout << "Loaded! (" << initTimer.getElapsedTime() << "s)" << std::endl;
#endif
}

void MVC_Model::InitSounds()
{
#ifdef _DEBUG
	StopWatch initTimer;
	initTimer.startTimer();

	std::cout << "Loading " << m_soundSONFile << "... ";
#endif

	soundList = LoadSounds(m_soundSONFile);

#ifdef _DEBUG
	std::cout << "Loaded! (" << initTimer.getElapsedTime() << "s)" << std::endl;
#endif
}

//TODO: Reimplement when 3D is required
/*
void MVC_Model::InitVisualObjects()
{
#ifdef _DEBUG
	StopWatch initTimer;
	initTimer.startTimer();

	std::cout << "Loading " << m_voSONFile << "... ";
#endif

	//m_renderList3DStatic = LoadVisualObject(m_voSONFile, meshList);

#ifdef _DEBUG
	std::cout << "Loaded! (" << initTimer.getElapsedTime() << "s)" << std::endl;
#endif
}
*/

Mesh * MVC_Model::GetMeshResource(string name)
{
	for (vector<Mesh*>::iterator it = meshList.begin(); it != meshList.end(); ++it)
	{
		if ((*it)->name == name)
		{
			return *it;
		}
	}

	// Error message if no mesh is found
	std::cout << "No mesh named " << name << " was found! Are you sure it is defined in the SON file?" << std::endl;

	// Returns NULL if no mesh with that name is found
	return NULL;
}

int MVC_Model::GetTextureResource(string name) const
{
	// Search for texture with the specified name
	// Find Texture from the textList and return the Texture ID
	map<string, unsigned>::const_iterator it = texList.find(name);
	if (it != texList.end())
	{
		return it->second;
	}
	else
	{
		// Error message if no mesh is found
		std::cout << "No Texture named " << name << " was found! Are you sure it is defined in the SON file?" << std::endl;

		// Returns no texture if none is found
		return -1;
	}
}

int MVC_Model::GetSoundResource(string name) const
{
	// Search for sound with the specified name
	// Find Sound from the soundList and assign the sound ID
	map<string, unsigned>::const_iterator it = soundList.find(name);
	if (it != soundList.end())
	{
		return it->second;
	}
	else
	{
		// Error message if no mesh is found
		std::cout << "No Sound named " << name << " was found! Are you sure it is defined in the SON file?" << std::endl;

		// Returns no texture if none is found
		return -1;
	}
}

void MVC_Model::resetKeysPressed(void)
{
	for (size_t i = 0; i < NUM_KEY_ACTION; ++i)
	{
		m_bKeyPressed[i] = false;
	}
}
