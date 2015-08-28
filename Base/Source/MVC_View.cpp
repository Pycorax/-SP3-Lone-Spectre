#include "MVC_View.h"

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "GameObject2D.h"
#include "Utility.h"
#include "SONIO.h"
#include "LoadResource.h"
#include "timer.h"

// Models
#include "MVC_Model_3D.h"

// OpenGL Specific
#include "shader.hpp"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

// Sound Engine
#include "SoundEngine.h"

MVC_View::MVC_View(MVC_Model* model, string configSONFile) : m_configSONFile(configSONFile)
{
	m_model = model;
}


MVC_View::~MVC_View()
{
}

void MVC_View::Init()
{
	// Load Config File
	loadConfig();

	// Initialize SoundSystem
	SoundEngine::StartSoundEngine();

	// Initialize OpenGL
	InitGL();

	m_lightDepthFBO = new DepthFBO;

	InitShadersAndLights();
	InitFog();

	// Get the Resolution from the View
	m_model->GetViewRes(m_viewWidth, m_viewHeight);

	// Load GL Specific Resources to the Model
#ifdef _DEBUG
	StopWatch initTimer;
	initTimer.startTimer();

	std::cout << "Loading " << m_model->GetMeshSONFile() << "... ";
#endif

	// MVC_Model Pre-Initialization for Meshes
	m_model->AssignMeshes(LoadMeshes(m_model->GetMeshSONFile()));

#ifdef _DEBUG
	std::cout << "Loaded! (" << initTimer.getElapsedTime() << "s)" << std::endl;
#endif


#ifdef _DEBUG
	std::cout << "Loading " << m_model->GetTexSONFile() << "... ";
#endif
	// MVC_Model Pre-Initialization for Textures
	m_model->AssignTextures(LoadTextures(m_model->GetTexSONFile()));

#ifdef _DEBUG
	std::cout << "Loaded! (" << initTimer.getElapsedTime() << "s)" << std::endl;
#endif

#ifdef _DEBUG
	std::cout << "Loading " << m_model->GetTexSONFile() << "... ";
#endif
	// MVC_Model Pre-Initialization for Textures
	m_model->AssignTextures(LoadTextures(m_model->GetTexSONFile()));

#ifdef _DEBUG
	std::cout << "Loaded! (" << initTimer.getElapsedTime() << "s)" << std::endl;
#endif
}

void MVC_View::Render(void)
{
	//************************************ PRE-RENDER-PASS ************************************//
	renderPassGPass();
	//************************************ MAIN-RENDER-PASS ************************************//
	renderPassMain();
}

void MVC_View::Exit(void)
{
	// End SoundEngine
	SoundEngine::StopSoundEngine();

	// End OpenGL
	delete m_lightDepthFBO;

	glDeleteProgram(m_programID);
	glDeleteProgram(m_gPassShaderID);
}

void MVC_View::ToggleCulling(void)
{
	if (m_bCull)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
	}

	m_bCull = !m_bCull;
}

void MVC_View::ToggleWireframe(void)
{
	if (m_bWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	m_bWireframe = !m_bWireframe;
}

void MVC_View::ToggleLight(void)
{
	m_bLightEnabled = !m_bLightEnabled;
}

void MVC_View::ToggleLight(bool light)
{
	m_bLightEnabled = light;
}

vector<Mesh*> MVC_View::LoadMeshes(string SONFile) const
{
	vector<Mesh*> meshList;

	// Retrieve data as a Branch object from the SONFile
	Branch meshes = SONIO::LoadSON(SONFile);

	const string ROOT_NAME = "MeshContainer";

	// Terrain
	bool terrainGenerated = false;

	string attribName = "";
	string attribVal = "";
	vector<float> colour;

	if (meshes.name == ROOT_NAME)
	{
		// For every Mesh
		for (size_t i = 0; i < meshes.childBranches.size(); ++i)
		{
			#pragma region InitVars
			/*
			* Initialize Variables that will be Used for Mesh Initialization
			*/
			string meshName = "Unnamed Mesh";
			Color meshCol;

			// Float Variables
			enum MESH_FLOAT_VARS
			{
				F_LENGTH,
				F_WIDTH,
				F_HEIGHT,
				F_RADIUS,
				F_INNER_RADIUS,
				F_OUTER_RADIUS,
				F_SLICES,
				F_STACK,
				F_PLANET_RADIUS,
				F_ATMOS_RADIUS,
				F_HORI_TILE,
				F_VERT_TILE,
				F_ANIM_TIME,
				NUM_FLOAT_VARS
			};
			float meshFloats[NUM_FLOAT_VARS];
			// Initialize meshFloats
			for (size_t i = 0; i < NUM_FLOAT_VARS; ++i)
			{
				meshFloats[i] = 0.0f;
			}
			// Set Up meshFloats Names
			string meshFloatNames[NUM_FLOAT_VARS] =
			{
				"Length",
				"Width",
				"Height",
				"Radius",
				"InnerRadius",
				"OuterRadius",
				"Slices",
				"Stack",
				"PlanetRadius",
				"AtmosphereRadius",
				"HorizontalTile",
				"VerticalTile",
				"SpriteAnimationTime"
			};

			int meshSlices = 4;
			int meshStacks = 4;
			int meshAnimRow = 1;
			int meshAnimCol = 1;
			int meshAnimRepeat = 0;

			// 2DMesh
			int meshPosX = 0;
			int meshPosY = 0;
			int meshWidth = 0;
			int meshHeight = 0;
			int meshFrameRow = 0;
			int meshFrameCol = 0;
			int meshScaleX = 1;
			int meshScaleY = 1;

			string meshFilePath = "";
			string meshType = "";

			/*
			* End of Variables that will be Used for Mesh Initialization
			*/
			#pragma endregion InitVars

			// Set the name of this mesh
			meshName = meshes.childBranches[i].name;

			// Find the attribute and add it to the Mesh Attributes
			for (size_t j = 0; j < meshes.childBranches[i].attributes.size(); ++j)
			{
				attribName = meshes.childBranches[i].attributes[j].name;
				attribVal = meshes.childBranches[i].attributes[j].value;
				// Extract the values out
				if (attribName == "Type")
				{
					meshType = attribVal;
				}
				else if (attribName == "Color")
				{
					colour = StringToFloats(attribVal);
					meshCol.Set(colour[0], colour[1], colour[2]);
				}
				else if (attribName == "Slices")
				{
					meshSlices = stoi(attribVal);
				}
				else if (attribName == "Stacks")
				{
					meshStacks = stoi(attribVal);
				}
				else if (attribName == "FilePath")
				{
					meshFilePath = attribVal;
				}
				else if (attribName == "SpriteAnimationColumn")
				{
					meshAnimCol = stoi(attribVal);
				}
				else if (attribName == "SpriteAnimationRow")
				{
					meshAnimRow = stoi(attribVal);
				}
				else if (attribName == "SpriteAnimationRepeat")
				{
					meshAnimRepeat = stoi(attribVal);
				}
				else if (attribName == "PosX")
				{
					meshPosX = stoi(attribVal);
				}
				else if (attribName == "PosY")
				{
					meshPosY = stoi(attribVal);
				}
				else if (attribName == "FrameRow")
				{
					meshFrameRow = stoi(attribVal);
				}
				else if (attribName == "FrameCol")
				{
					meshFrameCol = stoi(attribVal);
				}
				else if(attribName == "ScaleX")
				{
					meshScaleX = stoi(attribVal);
				}
				else if(attribName == "ScaleY")
				{
					meshScaleY = stoi(attribVal);
				}
				else // Floats
				{
					for (size_t k = 0; k < NUM_FLOAT_VARS; ++k)
					{
						if (attribName == meshFloatNames[k])
						{
							meshFloats[k] = stof(attribVal);
							break;
						}
					}
				}
			}

			// Generate the mesh from the meshType obtained
			Mesh* mesh = NULL;

			if (meshType == "Quad")
			{
				if (meshFloats[F_WIDTH] < Math::EPSILON)
				{
					meshFloats[F_WIDTH] = meshFloats[F_LENGTH];
				}

				mesh = MeshBuilder::GenerateQuad(meshName, meshCol, meshFloats[F_LENGTH], meshFloats[F_WIDTH]);
			}
			else if (meshType == "2D")
			{
				mesh = MeshBuilder::Generate2DMesh(meshName, meshCol, meshPosX, meshPosY, static_cast<int>(meshFloats[F_WIDTH]), static_cast<int>(meshFloats[F_HEIGHT]));
			}
			else if (meshType == "Cube")
			{
				mesh = MeshBuilder::GenerateCube(meshName, meshCol, meshFloats[F_LENGTH]);
			}
			else if (meshType == "Ring")
			{
				mesh = MeshBuilder::GenerateRing(meshName, meshCol, meshSlices, meshFloats[F_OUTER_RADIUS], meshFloats[F_INNER_RADIUS]);
			}
			else if (meshType == "Sphere")
			{
				mesh = MeshBuilder::GenerateSphere(meshName, meshCol, meshStacks, meshSlices, meshFloats[F_RADIUS]);
			}
			else if (meshType == "Cone")
			{
				mesh = MeshBuilder::GenerateCone(meshName, meshCol, meshSlices, meshFloats[F_RADIUS], meshFloats[F_HEIGHT]);
			}
			else if (meshType == "OBJ")
			{
				mesh = MeshBuilder::GenerateOBJ(meshName, meshFilePath);
			}
			else if (meshType == "Axes")
			{
				mesh = MeshBuilder::GenerateAxes(meshName, meshFloats[F_LENGTH], meshFloats[F_LENGTH], meshFloats[F_LENGTH]);
			}
			else if (meshType == "SpriteAnimation")
			{
				mesh = MeshBuilder::GenerateSpriteAnimation(meshName, meshAnimRow, meshAnimCol);

				SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(mesh);
				if (sa)
				{
					sa->m_anim = new Animation();
					sa->m_anim->Set(0, meshAnimCol - 1, meshAnimRepeat, meshFloats[F_ANIM_TIME]);
				}
			}
			else if (meshType == "SpriteAnimation2D")
			{
				mesh = MeshBuilder::GenerateSpriteAnimation2D(meshName, meshAnimRow, meshAnimCol, meshPosX, meshPosY, meshScaleX, meshScaleY);

				/*SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(mesh);
				if (sa)
				{
					sa->m_anim = new Animation();
					sa->m_anim->Set(0, meshAnimCol - 1, meshAnimRepeat, meshFloats[F_ANIM_TIME]);
				}*/
			}
			else if (meshType == "SkyPlane")
			{
				mesh = MeshBuilder::GenerateSkyPlane(meshName, meshCol, meshSlices, meshFloats[F_PLANET_RADIUS], meshFloats[F_ATMOS_RADIUS], meshFloats[F_HORI_TILE], meshFloats[F_VERT_TILE]);
			}
			else if (meshType == "Font")
			{
				mesh = MeshBuilder::GenerateText(meshName, 16, 16);
			}
			else if (meshType == "Terrain" && !terrainGenerated)
			{
				// Load Terrain
				MVC_Model_3D* model3d = dynamic_cast<MVC_Model_3D*>(m_model);

				if (model3d != NULL)
				{
					mesh = MeshBuilder::GenerateTerrain("Terrain", model3d->GetHeightMap());
					terrainGenerated = true;
				}
			}
			else if (meshType == "MeshSheet")
			{
				mesh = MeshBuilder::GenerateMeshFromSheet(meshName, meshAnimRow, meshAnimCol, meshFrameRow, meshFrameCol, meshPosX, meshPosY);
			}

			if (mesh != NULL)
			{
				meshList.push_back(mesh);
			}	
		}
	}

	return meshList;
}

map<string, unsigned> MVC_View::LoadTextures(string SONFile) const
{
	map<string, unsigned>texList;

	// Retrieve data as a Branch object from the SONFile
	Branch textures = SONIO::LoadSON(SONFile);

	const string ROOT_NAME = "TextureContainer";

	if (textures.name == ROOT_NAME)
	{
		// For every Texture
		for (size_t i = 0; i < textures.childBranches.size(); ++i)
		{
			unsigned textureID;
			// Add the name of this texture to the list
			string texName = textures.childBranches[i].name;
			// Find the attribute and add it to this Material object
			for (size_t j = 0; j < textures.childBranches[i].attributes.size(); ++j)
			{
				// Extract the values out
				if (textures.childBranches[i].attributes[j].name == "FilePath")
				{
					string textureFilePath = textures.childBranches[i].attributes[j].value;
					textureID = LoadTGA(textureFilePath.c_str());
				}
			}

			texList.insert(std::pair<string, unsigned>(texName, textureID));
		}
	}

	return texList;
}

void MVC_View::loadConfig()
{
	Branch cfg = SONIO::LoadSON(m_configSONFile);

	const string ROOT_NAME = "ViewConfigContainer";

	if (cfg.name == ROOT_NAME)
	{
		for (vector<Attribute>::iterator attrib = cfg.attributes.begin(); attrib != cfg.attributes.end(); ++attrib)
		{
			if (attrib->name == "GPassVertexShaderFile")
			{
				m_gPassVertexShaderFile = attrib->value;
			}
			else if (attrib->name == "GPassFragmentShaderFile")
			{
				m_gPassFragmentShaderFile = attrib->value;
			}
			else if (attrib->name == "MainPassVertexShaderFile")
			{
				m_mainPassVertexShaderFile = attrib->value;
			}
			else if (attrib->name == "MainPassFragmentShaderFile")
			{
				m_mainPassFragmentShaderFile = attrib->value;
			}
		}
	}
}

void MVC_View::InitGL()
{
	// Background
	Color bgCol = m_model->GetBGCol();
	glClearColor(bgCol.r, bgCol.g, bgCol.b, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Face Culling
	glEnable(GL_CULL_FACE);
	m_bCull = true;

	// Filled/Wireframe Mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_bWireframe = false;

	// Enable Lighting
	ToggleLight(true);

	// Enable Alpha Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
}

void MVC_View::InitShadersAndLights()
{
	const int NUM_LIGHT_PARAMS = U_LIGHT0_EXPONENT - U_NUMLIGHTS;

	//Load vertex and fragment shaders
	m_programID = LoadShaders(m_mainPassVertexShaderFile.c_str(), m_mainPassFragmentShaderFile.c_str());
	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
	
	m_gPassShaderID = LoadShaders(m_gPassVertexShaderFile.c_str(), m_gPassFragmentShaderFile.c_str());
	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");
	m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");

	// Use our shader
	glUseProgram(m_programID);

	#pragma region Loop to get a handle for all our "colorTexture" uniforms

	// Individual Texture Params
	const int NUM_TEX_PARAMS = 2;
	string texCode[NUM_TEX_PARAMS] =
	{
		"colorTextureEnabled[0]",
		"colorTexture[0]"
	};

	for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)		// For every texture
	{
		if (i != 0)
		{
			// Loop to update array index of strings
			for (size_t param = 0; param < NUM_TEX_PARAMS; ++param)		// For every param for each texture
			{
				texCode[param][texCode[param].length() - 2] = static_cast<char>(i + 48) /*convert into char*/;
			}
		}

		m_parameters[U_COLOR_TEXTURE_ENABLED + i] = glGetUniformLocation(m_programID, texCode[0].c_str());
		m_parameters[U_COLOR_TEXTURE + i] = glGetUniformLocation(m_programID, texCode[1].c_str());
	}

	#pragma endregion

	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

	// Global Light Params
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	#pragma region Loop to get a handle for all our light related uniforms

	// Individual Light Params
	string code[NUM_LIGHT_PARAMS] =
	{
		"lights[0].position_cameraspace",
		"lights[0].color",
		"lights[0].power",
		"lights[0].kC",
		"lights[0].kL",
		"lights[0].kQ",
		"lights[0].type",
		"lights[0].spotDirection",
		"lights[0].cosCutoff",
		"lights[0].cosInner",
		"lights[0].exponent"
	};

	const short INDEX_POS = 7;

	for (int i = 0; i < MAX_LIGHTS; ++i)		// For every light
	{
		if (i != 0)
		{
			// Loop to update array index of strings
			for (size_t param = 0; param < NUM_LIGHT_PARAMS; ++param)		// For every param for each light
			{
				code[param][INDEX_POS] = static_cast<char>(i + 48) /*convert into char*/;
			}
		}

		m_parameters[U_LIGHT0_POSITION + i * NUM_LIGHT_PARAMS] = glGetUniformLocation(m_programID, code[0].c_str());
		m_parameters[U_LIGHT0_COLOR + i * NUM_LIGHT_PARAMS] = glGetUniformLocation(m_programID, code[1].c_str());
		m_parameters[U_LIGHT0_POWER + i * NUM_LIGHT_PARAMS] = glGetUniformLocation(m_programID, code[2].c_str());
		m_parameters[U_LIGHT0_KC + i * NUM_LIGHT_PARAMS] = glGetUniformLocation(m_programID, code[3].c_str());
		m_parameters[U_LIGHT0_KL + i * NUM_LIGHT_PARAMS] = glGetUniformLocation(m_programID, code[4].c_str());
		m_parameters[U_LIGHT0_KQ + i * NUM_LIGHT_PARAMS] = glGetUniformLocation(m_programID, code[5].c_str());
		m_parameters[U_LIGHT0_TYPE + i * NUM_LIGHT_PARAMS] = glGetUniformLocation(m_programID, code[6].c_str());
		m_parameters[U_LIGHT0_SPOTDIRECTION + i * NUM_LIGHT_PARAMS] = glGetUniformLocation(m_programID, code[7].c_str());
		m_parameters[U_LIGHT0_COSCUTOFF + i * NUM_LIGHT_PARAMS] = glGetUniformLocation(m_programID, code[8].c_str());
		m_parameters[U_LIGHT0_COSINNER + i * NUM_LIGHT_PARAMS] = glGetUniformLocation(m_programID, code[9].c_str());
		m_parameters[U_LIGHT0_EXPONENT + i * NUM_LIGHT_PARAMS] = glGetUniformLocation(m_programID, code[10].c_str());
	}

	glUniform1i(m_parameters[U_NUMLIGHTS], MAX_LIGHTS);

	Light defaultLight;

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		glUniform1i(m_parameters[U_LIGHT0_TYPE + NUM_LIGHT_PARAMS * i], defaultLight.type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR + NUM_LIGHT_PARAMS * i], 1, &defaultLight.color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER + NUM_LIGHT_PARAMS * i], defaultLight.power);
		glUniform1f(m_parameters[U_LIGHT0_KC + NUM_LIGHT_PARAMS * i], defaultLight.kC);
		glUniform1f(m_parameters[U_LIGHT0_KL + NUM_LIGHT_PARAMS * i], defaultLight.kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ + NUM_LIGHT_PARAMS * i], defaultLight.kQ);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF + NUM_LIGHT_PARAMS * i], defaultLight.cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER + NUM_LIGHT_PARAMS * i], defaultLight.cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT + NUM_LIGHT_PARAMS * i], defaultLight.exponent);
	}

	#pragma endregion

	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	// Initialize Light Depth FBO
	m_lightDepthFBO->Init(1024, 1024);
}

void MVC_View::InitFog(void)
{
	Fog fog = m_model->GetFog();

	// Get handles for
	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	m_parameters[U_FOG_ENABLE] = glGetUniformLocation(m_programID, "fogParam.enabled");

	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fog.col.r);
	glUniform1f(m_parameters[U_FOG_START], fog.start);
	glUniform1f(m_parameters[U_FOG_END], fog.end);
	glUniform1f(m_parameters[U_FOG_DENSITY], fog.density);
	glUniform1i(m_parameters[U_FOG_TYPE], fog.type);
	glUniform1i(m_parameters[U_FOG_ENABLE], fog.enabled);
}

void MVC_View::SetViewRes(void)
{
	// Update the model with the new resolution
	m_model->GetViewRes(m_viewWidth, m_viewHeight);

	// Change the resolution
	glViewport(0, 0, m_viewWidth, m_viewHeight);
}

void MVC_View::Render2DMesh(Mesh * mesh, const bool enableLight, const float sizeX, const float sizeY, const float x, const float y, const float rotateZ, const float rotateY, const float rotateX ,  const int spriteID)
{
	if (mesh == NULL)
	{
		return;
	}

	Mtx44 ortho;
	ortho.SetToOrtho(0, m_viewWidth, 0, m_viewHeight, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix(); 
			viewStack.LoadIdentity();  
			modelStack.PushMatrix();   
				modelStack.LoadIdentity();  
				modelStack.Translate(x, y, 0); 

				if (rotateY)
				{
					glDisable(GL_CULL_FACE);
					modelStack.Rotate(rotateY, 0, 1, 0);
				}

				if (rotateX)
				{
					glDisable(GL_CULL_FACE);
					modelStack.Rotate(rotateX, 1, 0, 0);
				}

				if (rotateZ)
				{
					modelStack.Rotate(rotateZ, 0, 0, 1);
				}

				modelStack.Scale(sizeX, sizeY, 1.0f);  

				
					    
				Mtx44 MVP, modelView, modelView_inverse_transpose;  
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();   

				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);   
				
				for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
				{
					if (mesh->textureArray[i] > 0)
					{
						glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
						glActiveTexture(GL_TEXTURE0 + i);
						glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
						glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
					}
					else
					{
						glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
					}
				}

				if (spriteID >= 0)
				{
					mesh->Render(static_cast<unsigned>(spriteID) * 6, 6);
				}
				else
				{
					mesh->Render();
				}

				if ((rotateY || rotateX)&& m_bCull)
				{
					glEnable(GL_CULL_FACE);
				}

				glBindTexture(GL_TEXTURE_2D, 0);       
	
			modelStack.PopMatrix(); 
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void MVC_View::RenderMeshIn2D(Mesh * mesh, Vector3 size, float x, float y, float rot)
{
	if (mesh == NULL)
	{
		return;
	}

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(-m_viewWidth * 0.1, m_viewWidth * 0.1, -m_viewHeight * 0.1, m_viewHeight * 0.1, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size.x, size.y, size.z);
	modelStack.Rotate(rot, 0.0f, 0.0f, 1.0f);
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}

	mesh->Render();

	glBindTexture(GL_TEXTURE_2D, 0);

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void MVC_View::RenderMesh(Mesh * mesh, bool enableLight, bool enableFog)
{
	if (mesh == NULL)
	{
		return;
	}

	Mtx44 MVP, modelView, modelView_inverse_transpose;
	if (m_renderPass == RENDER_PASS_PRE)
	{
		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1, GL_FALSE, &lightDepthMVP.a[0]);

		mesh->Render();

		return;
	} 

	MVP = projectionStack.Top() * viewStack.Top() *modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if (enableLight && m_bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		// Shadows
		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1, GL_FALSE, &lightDepthMVP.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}

	mesh->Render();
}

void MVC_View::RenderText(Mesh* mesh, string text, Color color)
{
	if (!mesh || mesh->textureArray[0] <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void MVC_View::RenderTextOnScreen(Mesh* mesh, string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureArray[0] <= 0)
		return;

	viewStack.PushMatrix();
		viewStack.LoadIdentity();
		modelStack.PushMatrix();

			modelStack.LoadIdentity();
			modelStack.Translate(x, y, 0);
			modelStack.Scale(size, size, size);
			
			glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
			glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
			glUniform1i(m_parameters[U_LIGHTENABLED], 0);
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

			for (unsigned i = 0; i < text.length(); ++i)
			{
				Mtx44 characterSpacing;
				characterSpacing.SetToTranslation(i * 0.6f + 0.5f, 0.5f, 0);
				Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

				mesh->Render((unsigned)text[i] * 6, 6);
			}

			glBindTexture(GL_TEXTURE_2D, 0);
			glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

		modelStack.PopMatrix();
	viewStack.PopMatrix();
}

void MVC_View::SetHUD(const bool HUDmode)
{
	if (HUDmode)
	{
		glDisable(GL_DEPTH_TEST);
		Mtx44 ortho;
		ortho.SetToOrtho(0, m_viewWidth * 0.1, 0, m_viewHeight * 0.1, -10, 10);
		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
	}
	else
	{
		projectionStack.PopMatrix();
		glEnable(GL_DEPTH_TEST);
	}
}

void MVC_View::renderPassGPass()
{
	m_renderPass = RENDER_PASS_PRE;
	m_lightDepthFBO->BindForWriting();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Switch to GPass Shader
	glUseProgram(m_gPassShaderID);

	// These matrices should change when light position or direction changes
	vector<Light> lights = m_model->GetLights();

	if (lights.size() > 0)
	{
		Light light = lights.front();

		if (light.type == Light::LIGHT_DIRECTIONAL)
		{
			m_lightDepthProj.SetToOrtho(-2000, 2000, -2000, 2000, -2000, 2000);
		}
		else
		{
			m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20.0);
		}

		m_lightDepthView.SetToLookAt(
			light.position.x, light.position.y, light.position.z,
			0, 0, 0,
			0, 1, 0
			);
	}

	renderWorld();
}

void MVC_View::renderPassMain()
{
	m_renderPass = RENDER_PASS_MAIN;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_viewWidth, m_viewHeight);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Switch to Main Shader
	glUseProgram(m_programID);

	// Pass light depth texture
	m_lightDepthFBO->BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);
	// glActiveTexture(GL_TEXTURE0);

	// Prepare this frame for rendering
	framePrep();

	renderLights();
	renderWorld();
	renderObjects2D();

	/*
	Light light = m_model->GetLights()[0];
	static Mesh* mesh = MeshBuilder::GenerateSphere("sphere", Color(), 12, 12, 1.0f);

	modelStack.PushMatrix();
	modelStack.Translate(light.position.x, light.position.y, light.position.z);
	RenderMesh(mesh, false, false);
	modelStack.PopMatrix();
	*/
}

void MVC_View::renderWorld()
{
	renderObjects3D();
}

void MVC_View::RenderGameObject3D(GameObject2D* go)
{
	Transform transform = go->GetTransform();

	TextObject* to = dynamic_cast<TextObject*>(go);

	modelStack.PushMatrix();
	modelStack.Translate(transform.Translation.x, transform.Translation.y, transform.Translation.z);

	/* TODO: GameObject3D
	if (go->IsBillboard())
	{
		Camera3* cam = m_model->GetCamera();
		Vector3 deltaPos = cam->GetPosition() - transform.Translation;

		float angle = atan2(deltaPos.x, deltaPos.z);
		angle = Math::RadianToDegree(angle);

		modelStack.Rotate(angle, 0.0f, 1.0f, 0.0f);
	}
	*/
	
	modelStack.Rotate(transform.Rotation.x, 1.0f, 0.0f, 0.0f);
	modelStack.Rotate(transform.Rotation.y, 0.0f, 1.0f, 0.0f);
	modelStack.Rotate(transform.Rotation.z, 0.0f, 0.0f, 1.0f);
	modelStack.Scale(transform.Scale.x, transform.Scale.y, transform.Scale.z);

	if (to != NULL)		// TextObject
	{
		RenderText(to->GetMesh(), to->GetText(), to->GetColor());
	}
	else				// GameObject2D
	{
		RenderMesh(go->GetMesh(), false, false);
	}
	/*
	else				// TODO: GameObject3D
	{
		RenderMesh(go->GetMesh(), go->IsLighted(), go->IsFogged() && m_model->GetFog().enabled);
	}
	*/

	modelStack.PopMatrix();
}

void MVC_View::RenderGameObject2D(GameObject2D* go)
{
	Transform transform = go->GetTransform();

	TextObject* to = dynamic_cast<TextObject*>(go);


	if (to != NULL)		// TextObject
	{
		RenderTextOnScreen(to->GetMesh(), to->GetText(), to->GetColor(), transform.Scale.x, transform.Translation.x, transform.Translation.y);
	}
	else				// GameObject2D
	{
		//RenderMeshIn2D(go->GetMesh(), transform.Scale, transform.Translation.x, transform.Translation.y, transform.Rotation.z);
		Render2DMesh(go->GetMesh(), false, transform.Scale.x, transform.Scale.y, transform.Translation.x, transform.Translation.y, transform.Rotation.z, transform.Rotation.y, transform.Rotation.x);
	}
}

void MVC_View::renderLights(void)
{
	const int NUM_LIGHT_PARAMS = U_LIGHT0_EXPONENT - U_NUMLIGHTS;
	
	// Get Light Info from Model
	vector<Light> lights = m_model->GetLights();

	// Update the light parameters
	for (size_t i = 0; i < MAX_LIGHTS && i < lights.size(); ++i)
	{
		glUniform1i(m_parameters[U_LIGHT0_TYPE + NUM_LIGHT_PARAMS * i], lights[i].type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR + NUM_LIGHT_PARAMS * i], 1, &lights[i].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER + NUM_LIGHT_PARAMS * i], lights[i].power);
		glUniform1f(m_parameters[U_LIGHT0_KC + NUM_LIGHT_PARAMS * i], lights[i].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL + NUM_LIGHT_PARAMS * i], lights[i].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ + NUM_LIGHT_PARAMS * i], lights[i].kQ);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF + NUM_LIGHT_PARAMS * i], lights[i].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER + NUM_LIGHT_PARAMS * i], lights[i].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT + NUM_LIGHT_PARAMS * i], lights[i].exponent);
	}

	// Render the Lights
	for (size_t i = 0; i < MAX_LIGHTS && i < lights.size(); ++i)
	{
		if (lights[i].power < Math::EPSILON)
		{
			continue;
		}

		if (lights[i].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(lights[i].position.x, lights[i].position.y, lights[i].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + i * NUM_LIGHT_PARAMS], 1, &lightDirection_cameraspace.x);
		}
		else if (lights[i].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[i].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + i * NUM_LIGHT_PARAMS], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * lights[i].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION + i * NUM_LIGHT_PARAMS], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[i].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + i * NUM_LIGHT_PARAMS], 1, &lightPosition_cameraspace.x);
		}
	}
}

void MVC_View::renderObjects3D(void)
{
	// Get objects to render from model
	queue<GameObject2D*>* renderList3D = m_model->Get3DRenderList();

	// Render objects from the model
	while (renderList3D->size() > 0)
	{
		RenderGameObject3D(renderList3D->front());
		renderList3D->pop();
	}
}

void MVC_View::renderObjects2D(void)
{
	SetHUD(true);

	// Get objects to render from model
	queue<GameObject2D*>* renderList2D = m_model->Get2DRenderList();

	while (renderList2D->size() > 0)
	{
		RenderGameObject2D(renderList2D->front());
		renderList2D->pop();
	}

	SetHUD(false);
}

void MVC_View::framePrep(void)
{
	// Camera matrix
	Camera* camera = m_model->GetCamera();
	Vector3 camPos = camera->GetPosition();
	Vector3 camTar = camera->GetTarget();
	Vector3 camUp = camera->GetUp();

	// Clear the screen to render this frame
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Vector3 worldSize = m_model->GetWorldSize();
	Mtx44 projection;

	if (camera->IsPerspective())
	{
		projection.SetToPerspective(camera->GetFOV(), static_cast<float>(m_viewWidth) / static_cast<float>(m_viewHeight), camera->GetNear(), camera->GetFar());
	}
	else
	{
		projection.SetToOrtho(0, worldSize.x, 0, worldSize.y * static_cast<double>(m_viewWidth) / static_cast<double>(m_viewHeight), camera->GetNear(), camera->GetFar());
	}

	projectionStack.LoadMatrix(projection);
	
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camPos.x, camPos.y, camPos.z,
		camTar.x, camTar.y, camTar.z,
		camUp.x, camUp.y, camUp.z
		);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
}