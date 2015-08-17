#ifndef MVC_CONTROLLER_H
#define MVC_CONTROLLER_H

#include "MVC_Model.h"
#include "MVC_View.h"
#include "timer.h"

//Include GLFW
#include <GLFW/glfw3.h>

class MVC_Controller
{
	// Config File
	const string m_configSONFile;

	// MVC
	MVC_Model* m_model;
	MVC_View* m_view;

	// FPS
	float m_frametime;

	// Window
	GLFWwindow* m_window;
	string m_window_title;
	int m_window_width;
	int m_window_height;
	const static int m_window_deadzone = 100;

	// Keyboard Input
	enum INPUT_DEBUG_KEY_TYPE
	{
		VIEW_TOGGLE_CULL_KEY,
		VIEW_TOGGLE_WIREFRAME_KEY,
		VIEW_TOGGLE_LIGHT_KEY,
		NUM_DEBUG_KEY
	};
	unsigned char inputDebugKey[NUM_DEBUG_KEY];
	unsigned char inputKey[MVC_Model::NUM_KEY_ACTION];	// The button virtual key code for the button press
	static const double INPUT_DELAY;
	static const short MAX_KEYS = UCHAR_MAX;
	StopWatch m_inputDelayController[MAX_KEYS];

	// Mouse Input
	double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	// Camera target control variables
	double camera_yaw, camera_pitch, camera_spin;

	public:
		MVC_Controller(MVC_Model* model, MVC_View* view, string configSONFile);
		~MVC_Controller();

		void Init(int fps = 60);
		void Run(void);
		void Exit(void);

		void SetFrameRate(int fps);

	private:
		// Load Config File
		void loadConfig(void);

		// Window
		void SetupWindow(string windowTitle, int windowWidth, int windowHeight);

		// Input - Mouse
		void initInputMouse(void);

		// Input - Keys
		void initInputKeys(void);
		void initInputDelayController(void);
		bool isKeyPressed(unsigned short key, bool delay = false);

		// Input Update
		void inputUpdate(double dt);
		void inputMouseUpdate(void);
		void inputViewUpdate(void);
		void inputKeyUpdate(void);
};

#endif