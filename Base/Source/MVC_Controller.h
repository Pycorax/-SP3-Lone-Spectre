#ifndef MVC_CONTROLLER_H
#define MVC_CONTROLLER_H

#include "MVC_Model.h"
#include "MVC_View.h"
#include "timer.h"

//Include GLFW
#include <GLFW/glfw3.h>

class MVC_Controller
{
	enum E_XINPUT_VIRTUAL_KEY
	{
		XVK_BUTTON_A,
		XVK_BUTTON_B,
		XVK_BUTTON_X,
		XVK_BUTTON_Y,
		XVK_DPAD_UP,
		XVK_DPAD_DOWN,
		XVK_DPAD_LEFT,
		XVK_DPAD_RIGHT,
		XVK_LEFT_SHOULDER,
		XVK_RIGHT_SHOULDER,
		XVK_BUTTON_LEFT_THUMB,
		XVK_BUTTON_RIGHT_THUMB,
		XVK_BUTTON_START,
		XVK_BUTTON_BACK,
		// End of Analog
		XVK_ANALOG_MARKER,
		// Start of Analog
		XVK_LEFT_THUMB_UP,
		XVK_LEFT_THUMB_DOWN,
		XVK_LEFT_THUMB_LEFT,
		XVK_LEFT_THUMB_RIGHT,
		XVK_RIGHT_THUMB_UP,
		XVK_RIGHT_THUMB_DOWN,
		XVK_RIGHT_THUMB_LEFT,
		XVK_RIGHT_THUMB_RIGHT,
		XVK_LEFT_TRIGGER,
		XVK_RIGHT_TRIGGER,
		// For indicating no key as well
		NUM_XINPUT_VIRTUAL_KEYS
	};

	static MVC_Controller* s_instance;

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

	/*
	 * Input::KB + Mouse
	 */
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
	
	
	/*
	 * Input::XInput Controllers
	 */
	E_XINPUT_VIRTUAL_KEY inputXInputKey[MVC_Model::NUM_KEY_ACTION];		// The button virtual key codes for the controller buttons
	bool xInputKeyPressed[NUM_XINPUT_VIRTUAL_KEYS];						// Updated every frame first, will determine if a button is pressed
	float lX, lY, rX, rY, lT, rT;										// Stores the analog values from the analog XInput controls

	// Camera target control variables
	double camera_yaw, camera_pitch, camera_spin;

	public:
		static MVC_Controller* GetInstance(MVC_Model* model, MVC_View* view, string configSONFile);
		static MVC_Controller* GetInstance(void);
		~MVC_Controller();

		void Init(int fps = 60);
		void Run(void);
		void Exit(void);

		void SetFrameRate(int fps);

		friend void resize_callback(GLFWwindow* window, int w, int h);

	private:
		MVC_Controller(MVC_Model* model, MVC_View* view, string configSONFile);

		// Load Config File
		void loadConfig(void);

		// Window
		void SetupWindow(string windowTitle, int windowWidth, int windowHeight);
		void resizeWindow(int windowWidth, int windowHeight);
		void minimizeWindow(void);
		void restoreWindow(void);
		void hideWindow(void);
		void showWindow(void);
		// -- Windows Update from Model
		void checkForResResize(void);

		/*
		 * Input Init
		 */
		// -- Mouse
		void initInputMouse(void);
		// -- Keyboard
		void initInputKeys(void);
		void initInputDelayController(void);
		// -- XInput
		void initInputXInput(void);

		/*
		 * Input Update
		 */
		void inputUpdate(double dt);
		// -- KB + Mouse
		bool isKeyPressed(unsigned short key, bool delay = false);
		void inputMouseUpdate(void);
		void inputViewUpdate(void);
		void inputKeyUpdate(void);
		// -- XInput Controllers
		bool inputXInputUpdateKeyPressed(void);					// Will update all the keys fro isXInputKeyPressed to check
		bool isXInputKeyPressed(E_XINPUT_VIRTUAL_KEY key);		// Will control the updates
		void inputXInputUpdate(void);							// Processes XInput control data to MVC_Model
};

#endif