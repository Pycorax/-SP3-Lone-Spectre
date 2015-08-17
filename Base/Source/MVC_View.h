#ifndef MVC_VIEW_H
#define MVC_VIEW_H

#include "MVC_Model.h"

//Include GLEW
#include <GL/glew.h>

#include "DepthFBO.h"
#include "MatrixStack.h"
#include <string>
#include <map>

#include "Map.h"
#include "MapLayer.h"

using std::string;
using std::map;

class MVC_View
{
	// Static Constants
	static const int MAX_LIGHTS = 8;		// Maximum number of lights in GL

	// Config File
	const string m_configSONFile;

	// Shaders
	string m_gPassVertexShaderFile;
	string m_gPassFragmentShaderFile;
	string m_mainPassVertexShaderFile;
	string m_mainPassFragmentShaderFile;
	
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT7_EXPONENT = U_NUMLIGHTS + 8/*Number of Lights*/ * (U_LIGHT0_EXPONENT - U_NUMLIGHTS),
		// -- Textures
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE_ENABLED1,
		U_COLOR_TEXTURE_ENABLED2,
		U_COLOR_TEXTURE_ENABLED3,
		U_COLOR_TEXTURE_ENABLED4,
		U_COLOR_TEXTURE_ENABLED5,
		U_COLOR_TEXTURE_ENABLED6,
		U_COLOR_TEXTURE_ENABLED7,
		U_COLOR_TEXTURE,
		U_COLOR_TEXTURE1,
		U_COLOR_TEXTURE2,
		U_COLOR_TEXTURE3,
		U_COLOR_TEXTURE4,
		U_COLOR_TEXTURE5,
		U_COLOR_TEXTURE6,
		U_COLOR_TEXTURE7,
		// -- Text
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		// -- Fog
		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLE,
		// -- Shadow
		U_LIGHT_DEPTH_MVP_GPASS,
		U_LIGHT_DEPTH_MVP,
		U_SHADOW_MAP,
		U_TOTAL,
	};

	enum RENDER_PASS_TYPE
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
		NUM_RENDER_PASS
	};

	// Handle to the Model
	MVC_Model* m_model;

	int m_viewWidth;
	int m_viewHeight;
	
	// GLEW
	unsigned m_vertexArrayID;
	unsigned m_programID;

	// Shader Uniform Parameters
	unsigned m_parameters[U_TOTAL];

	// GL Settings
	bool m_bCull;
	bool m_bWireframe;
	bool m_bLightEnabled;

	// MVP Transform MatrixStacks
	MS modelStack;
	MS viewStack;
	MS projectionStack;

	// Render Pass
	unsigned m_gPassShaderID;
	RENDER_PASS_TYPE m_renderPass;

	// Shadows
	DepthFBO* m_lightDepthFBO;
	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;

	public:
		MVC_View(MVC_Model* model, string configSONFile);
		~MVC_View();

		void Init(void);
		void Render(void);
		void Exit(void);

		void ToggleCulling(void);
		void ToggleWireframe(void);
		void ToggleLight(void);
		void ToggleLight(bool light);

		vector<Mesh*> LoadMeshes(string SONFile) const;
		map<string, unsigned> LoadTextures(string SONFile) const;

	private:
		void loadConfig(void);
		void InitGL(void);
		void InitShadersAndLights(void);
		void InitFog(void);

		/*
		 * Rendering - General
		 */ 
		// Meshes
		void Render2DMesh(Mesh * mesh, const bool enableLight, const float sizeX, const float sizeY, const float x = 0.0f, const float y = 0.0f, const float rotateZ = 0.0f, const float rotateY = 0.0f);
		void RenderMeshIn2D(Mesh *mesh, Vector3 size = Vector3(1.0f, 1.0f, 1.0f), float x = 0.0f, float y = 0.0f, float rot = 0.0f);
		void RenderMesh(Mesh *mesh, bool enableLight, bool enableFog = true);
		
		// Text
		void RenderText(Mesh* mesh, string text, Color color);
		void RenderTextOnScreen(Mesh* mesh, string text, Color color, float size, float x, float y);

		// Start Rendering to Screen Mode
		void SetHUD(const bool HUDmode);

		// Render Passes
		void renderPassGPass(void);
		void renderPassMain(void);
		void renderWorld(void);

		// Helper functions fo Render()
		void RenderGameObject3D(GameObject* go);
		void RenderGameObject3D(VisualObject* go);
		void RenderGameObject2D(GameObject* go);

		void renderLights(void);
		void renderObjects3D(void);
		void renderObjects2D(void);

		void framePrep(void);
};

#endif