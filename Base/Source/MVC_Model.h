#ifndef MVC_MODEL_H
#define MVC_MODEL_H

// Include STLs
#include <vector>
#include <map>

#include "Camera3.h"
#include "Light.h"
#include "Fog.h"
#include "GameObject2D.h"
#include "TextObject.h"

using std::vector;
using std::map;

class MVC_Model
{
	public:
		enum KEY_ACTION_TYPE
		{
			// Movement
			MOVE_FORWARD_KEY,
			MOVE_BACKWARD_KEY,
			MOVE_LEFT_KEY,
			MOVE_RIGHT_KEY,
			MOVE_JUMP_KEY,
			MOVE_CROUCH_KEY,
			MOVE_PRONE_KEY,
			MOVE_SPRINT_KEY,
			// Look
			LOOK_UP_KEY,
			LOOK_DOWN_KEY,
			LOOK_LEFT_KEY,
			LOOK_RIGHT_KEY,
			// Interact
			INTERACT_ATTACK_1_KEY,
			INTERACT_ATTACK_2_KEY,
			INTERACT_RELOAD_KEY,
			INTERACT_PREV_KEY,
			INTERACT_NEXT_KEY,
			INTERACT_GENERIC_KEY,
			// Game
			GAME_PAUSE_KEY,
			GAME_ACCEPT_KEY,
			GAME_CANCEL_KEY,
			GAME_DEBUG_KEY,
			// Camera
			CAMERA_FP_KEY,
			CAMERA_FP_NOCLIP_KEY,
			// Misc
			MISC_0_KEY,
			MISC_1_KEY,
			MISC_2_KEY,
			MISC_3_KEY,
			MISC_4_KEY,
			MISC_5_KEY,
			MISC_6_KEY,
			MISC_7_KEY,
			MISC_8_KEY,
			MISC_9_KEY,

			NUM_KEY_ACTION
		};

	protected:
		// Config File
		const string m_configSONFile;

		// Resource SON Files
		// -- View Must Load
		string m_meshSONFile;
		string m_texSONFile;
		// -- Model Must Load
		string m_materialSONFile;
		string m_colorSONFile;
		string m_goSONFile;
		string m_voSONFile;
		string m_lightSONFile;
		string m_fontName;

		// Graphics
		int m_viewWidth;
		int m_viewHeight;

		Color m_bgColor;
		Camera3* m_camera;
		vector<Light> m_lights;
		Fog m_fog;

		// Resources
		map<string, Material> matList;
		vector<string> colListMap;
		vector<Color> colList;
		map<string, unsigned> texList;
		vector<Mesh*> meshList;
		Mesh* m_defaultFont;

		// Overworld
		// -- GameObjects (The interactive stuff that gets rendered)
		vector<GameObject2D*> goList;
		// -- WorldScale
		Vector3 m_worldSize;

		// Input
		bool m_bFreeMouse;
		bool m_bKeyPressed[NUM_KEY_ACTION];
		int m_mousePosX;
		int m_mousePosY;

		// Statistics
		float m_fps;
		TextObject* m_fpsCount;	// Text object to hold FPS counter

		// Output to View
		vector<GameObject2D*> m_renderList3D;
		vector<GameObject2D*> m_renderList2D;
		vector<GameObject2D*> m_renderList3DStatic;

	public:
		MVC_Model(string configSONFile);
		virtual ~MVC_Model();

		// Pre-Initialization Functions
		/*
		 * These functions are to be called in the Controller before MVC_Model::Init() 
		 * so that MVC_View can handle loading of these GraphicsPlatform-Specific resources
		 */
		string GetMeshSONFile(void) const;
		string GetTexSONFile(void) const;
		void AssignMeshes(vector<Mesh*>& _meshList);
		void AssignTextures(map<string, unsigned>& _texList);

		// Post-Initialization-Initialization Functions
		/*
		 * These functions are to be called during the program initialization to send critical information from other components to the model
		 */
		void GetViewRes(int& x, int& y);

		virtual void Init(void);
		virtual void Update(double dt);
		virtual void Exit(void);

		// Graphics
		void InitLights(void);

		Color GetBGCol(void) const;
		Camera3* GetCamera(void) const;
		vector<Light> GetLights(void) const;
		Fog GetFog(void) const;
		Vector3 GetWorldSize(void) const;

		// -- Render Lists
		const vector<GameObject2D*>& Get3DRenderList(void) const;
		const vector<GameObject2D*>& Get2DRenderList(void) const;

		// Input
		bool IsMouseFree(void);
		void ActivateKey(KEY_ACTION_TYPE keyAction);
		void UpdateMouseInfo(double x, double y);

		// Statistics
		float GetFrameRate(void);

	protected:
		// Load Config
		virtual void loadConfig();

		// Resources
		void InitMaterials();
		void InitColors();
		void InitMeshes();
		void InitGameObjects();
		void InitVisualObjects();

		Mesh* GetMeshResource(string name);			// Returns a pointer to a copy of the resource
		unsigned GetTextureResource(string name) const;

		// Input
		virtual void processKeyAction(double dt) = 0;

		void resetKeysPressed(void);
};

#endif