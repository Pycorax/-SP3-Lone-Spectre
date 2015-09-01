#ifndef MVC_MODEL_H
#define MVC_MODEL_H

// Include STLs
#include <vector>
#include <queue>
#include <map>

#include "Camera3.h"
#include "Light.h"
#include "Fog.h"
#include "GameObject2D.h"
#include "TextObject.h"
#include "InputDevice.h"

using std::vector;
using std::queue;
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
		INTERACT_SKILL_1_KEY,
		INTERACT_SKILL_2_KEY,
		// Game
		GAME_EXIT_KEY,
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
	// Model Lifetime
	bool m_running;

	// Config File
	const string m_configSONFile;

	// Graphics
	// -- Resolution
	int m_viewWidth;
	int m_viewHeight;
	// -- Others
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
	map<string, unsigned> soundList;
	Mesh* m_defaultFont;

	// Overworld
	// -- GameObjects (The interactive stuff that gets rendered)
	vector<GameObject2D*> goList;
	// -- WorldScale
	Vector3 m_worldSize;

	// Input
	E_INPUT_DEVICE_TYPE m_lastInputDevice;		// Stores the type of the last input device used
	// -- Keyboard and Mouse
	bool m_bFreeMouse;
	bool m_bKeyPressed[NUM_KEY_ACTION];
	int m_mousePosX;
	int m_mousePosY;
	// -- XInput Analog							
	Vector2 m_leftThumbStick;			// Stores the displacement from the original state. Ranges from -1.0 to 1.0
	Vector2 m_rightThumbStick;			// Stores the displacement from the original state. Ranges from -1.0 to 1.0
	float m_leftTrigger;				// Stores the displacement from the original state. Ranges from 0.0 to 1.0
	float m_rightTrigger;				// Stores the displacement from the original state. Ranges from 0.0 to 1.0

	// Statistics
	float m_fps;
	TextObject* m_fpsCount;	// Text object to hold FPS counter

	// Output to View
	queue<GameObject2D*> m_renderList3D;
	queue<GameObject2D*> m_renderList2D;
	//vector<GameObject2D*> m_renderList3DStatic; TODO: Reimplement when 3D is required

private:
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
	// -- External Project Loads
	string m_soundSONFile;

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

	// External Update Functions
	/*
		* These functions are to be called outside of the model to update the model about the environment
		*/
	void UpdateViewRes(int width, int height);
	void UpdateLastInputDevice(E_INPUT_DEVICE_TYPE inputType);


	virtual void Init(void);
	virtual void Update(double dt);
	virtual void Exit(void);

	// Model Lifetime
	bool IsRunning(void) const;

	// Graphics
	void InitLights(void);

	Color GetBGCol(void) const;
	Camera3* GetCamera(void) const;
	vector<Light> GetLights(void) const;
	Fog GetFog(void) const;
	Vector3 GetWorldSize(void) const;

	// -- Render Lists
	queue<GameObject2D*>* Get3DRenderList(void);
	queue<GameObject2D*>* Get2DRenderList(void);

	// Input
	bool IsMouseFree(void);
	void ActivateKey(KEY_ACTION_TYPE keyAction);
	void UpdateMouseInfo(double x, double y);
	void UpdateXInput(float lX, float lY, float rX, float rY, float lT, float rT);

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
	void InitSounds();
	//void InitVisualObjects();		 // TODO: Reimplement when 3D is required

	Mesh* GetMeshResource(string name);			// Returns a pointer to a copy of the resource
	int GetTextureResource(string name) const;
	int GetSoundResource(string name) const;

	// Input
	virtual void processKeyAction(double dt) = 0;
	void resetKeysPressed(void);

	// Resizing Code
	virtual void onResolutionChanged(int oldViewWidth, int oldViewHeight) = 0;
};

#endif