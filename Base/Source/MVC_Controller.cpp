#include "MVC_Controller.h"

#include "timer.h"
#include "Camera3.h"
#include "SONIO.h"
#include "MVC_Model_3D.h"
#include <Xinput.h>
#include "InputDevice.h"

namespace GLFW
{
	//Define an error callback
	static void error_callback(int error, const char* description)
	{
		fputs(description, stderr);
		_fgetchar();
	}

	//Define the key input callback
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	MVC_Controller* _controller = MVC_Controller::GetInstance();
	_controller->m_model->UpdateViewRes(w, h);
	_controller->m_view->SetViewRes();
}

MVC_Controller* MVC_Controller::s_instance = NULL;
const double MVC_Controller::INPUT_DELAY = 0.2;

MVC_Controller::MVC_Controller(MVC_Model * model, MVC_View * view, string configSONFile) 
	: m_model(model)
	, m_view(view)
	, m_configSONFile(configSONFile)
	, m_window_width(1)
	, m_window_height(1)
{
}

MVC_Controller* MVC_Controller::GetInstance(MVC_Model * model, MVC_View * view, string configSONFile)
{
	if (s_instance == NULL)
	{
		s_instance = new MVC_Controller(model, view, configSONFile);
	}
	
	return s_instance;
}

MVC_Controller* MVC_Controller::GetInstance(void)
{
	return s_instance;
}

MVC_Controller::~MVC_Controller()
{
}

void MVC_Controller::Init(int fps/* = 60*/)
{
	loadConfig();

	// Create a Window for our Application
	SetupWindow(m_window_title, m_window_width, m_window_height);
	// Hide this window while loading
	hideWindow();
	
	// Initialize MVC_View
	m_view->Init();

#ifdef _DEBUG
	StopWatch initTimer;
	initTimer.startTimer();

	std::cout << "Loading " << m_model->GetMeshSONFile() << "... ";
#endif

	// MVC_Model Pre-Initialization for Meshes
	m_model->AssignMeshes(m_view->LoadMeshes(m_model->GetMeshSONFile()));

#ifdef _DEBUG
	std::cout << "Loaded! (" << initTimer.getElapsedTime() << "s)" << std::endl;
#endif


#ifdef _DEBUG
	std::cout << "Loading " << m_model->GetTexSONFile() << "... ";
#endif
	// MVC_Model Pre-Initialization for Textures
	m_model->AssignTextures(m_view->LoadTextures(m_model->GetTexSONFile()));

#ifdef _DEBUG
	std::cout << "Loaded! (" << initTimer.getElapsedTime() << "s)" << std::endl;
#endif

	// Initialize MVC_Model
	m_model->Init();

	// Get Resolution from MVC_Model
	int width, height;
	m_model->GetViewRes(width, height);
	resizeWindow(width, height);

	SetFrameRate(fps);
	initInputKeys();
	initInputMouse();
	initInputXInput();

	// Unhide window now that loading is doen
	showWindow();
}

void MVC_Controller::Run(void)
{
	StopWatch timer;
	timer.startTimer();

	while (!glfwWindowShouldClose(m_window) && m_model->IsRunning())
	{
		double dt = timer.getElapsedTime();

		inputUpdate(dt);
		m_model->Update(dt);
		m_view->Render();

		checkForResResize();

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();

		timer.waitUntil(m_frametime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
	}
}

void MVC_Controller::Exit(void)
{
	m_model->Exit();
	m_view->Exit();

	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

void MVC_Controller::SetFrameRate(int fps)
{
	m_frametime = 1000.0f / static_cast<float>(fps);
}

void MVC_Controller::loadConfig(void)
{
	Branch cfg = SONIO::LoadSON(m_configSONFile);

	const string ROOT_NAME = "ControllerConfigContainer";

	if (cfg.name == ROOT_NAME)
	{
		for (vector<Attribute>::iterator attrib = cfg.attributes.begin(); attrib != cfg.attributes.end(); ++attrib)
		{
			if (attrib->name == "WindowTitle")
			{
				m_window_title = attrib->value;
			}
			else if (attrib->name == "ViewRefreshRate")
			{
				SetFrameRate(stoi(attrib->value));
			}
			else if (attrib->name == "InputRefreshRate")
			{
				
			}
		}
	}
}

void MVC_Controller::SetupWindow(string windowTitle, int windowWidth, int windowHeight)
{
	m_window_width = windowWidth;
	m_window_height = windowHeight;

	//Set the error callback
	glfwSetErrorCallback(GLFW::error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, windowTitle.c_str(), NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
							 //Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
}

void MVC_Controller::resizeWindow(int windowWidth, int windowHeight)
{
	m_window_width = windowWidth;
	m_window_height = windowHeight;
	glfwSetWindowSize(m_window, m_window_width, m_window_height);
}

void MVC_Controller::minimizeWindow(void)
{
	glfwIconifyWindow(m_window);
}

void MVC_Controller::restoreWindow(void)
{
	glfwRestoreWindow(m_window);
}

void MVC_Controller::hideWindow(void)
{
	glfwHideWindow(m_window);
}

void MVC_Controller::showWindow(void)
{
	glfwShowWindow(m_window);
}

void MVC_Controller::checkForResResize(void)
{
	int width, height;
	m_model->GetViewRes(width, height);

	if (width != m_window_width || height != m_window_height)
	{
		resizeWindow(width, height);
	}
}

void MVC_Controller::initInputMouse(void)
{
	mouse_last_x = mouse_last_y = mouse_current_x = mouse_current_y = mouse_diff_x = mouse_diff_y = 0.0;

	camera_yaw = camera_pitch = camera_spin = 0.0;
}

void MVC_Controller::initInputKeys(void)
{
	// Set the keys that are to be pressed
	inputDebugKey[VIEW_TOGGLE_CULL_KEY] = VK_F1;
	inputDebugKey[VIEW_TOGGLE_WIREFRAME_KEY] = VK_F2;
	inputDebugKey[VIEW_TOGGLE_LIGHT_KEY] = VK_F3;

	// Movement
	inputKey[MVC_Model::MOVE_FORWARD_KEY] = 'W';
	inputKey[MVC_Model::MOVE_BACKWARD_KEY] = 'S';
	inputKey[MVC_Model::MOVE_LEFT_KEY] = 'A';
	inputKey[MVC_Model::MOVE_RIGHT_KEY] = 'D';
	inputKey[MVC_Model::MOVE_JUMP_KEY] = ' ';
	inputKey[MVC_Model::MOVE_CROUCH_KEY] = 'C';
	inputKey[MVC_Model::MOVE_PRONE_KEY] = VK_CONTROL;
	inputKey[MVC_Model::MOVE_SPRINT_KEY] = VK_SHIFT;

	// Look
	inputKey[MVC_Model::LOOK_UP_KEY] = VK_UP;
	inputKey[MVC_Model::LOOK_DOWN_KEY] = VK_DOWN;
	inputKey[MVC_Model::LOOK_LEFT_KEY] = VK_LEFT;
	inputKey[MVC_Model::LOOK_RIGHT_KEY] = VK_RIGHT;
	
	// Interact
	inputKey[MVC_Model::INTERACT_ATTACK_1_KEY] = VK_LBUTTON;
	inputKey[MVC_Model::INTERACT_ATTACK_2_KEY] = VK_RBUTTON;
	inputKey[MVC_Model::INTERACT_RELOAD_KEY] = 'R';
	inputKey[MVC_Model::INTERACT_PREV_KEY] = 'Q';
	inputKey[MVC_Model::INTERACT_NEXT_KEY] = 'E';
	inputKey[MVC_Model::INTERACT_GENERIC_KEY] = 'F';
	inputKey[MVC_Model::INTERACT_SKILL_1_KEY] = 'J';
	inputKey[MVC_Model::INTERACT_SKILL_2_KEY] = 'K';

	// Game
	inputKey[MVC_Model::GAME_EXIT_KEY] = VK_ESCAPE;
	inputKey[MVC_Model::GAME_ACCEPT_KEY] = VK_RETURN;
	inputKey[MVC_Model::GAME_CANCEL_KEY] = VK_BACK;
	inputKey[MVC_Model::GAME_DEBUG_KEY] = VK_OEM_3;

	// Camera
	inputKey[MVC_Model::CAMERA_FP_KEY] = VK_F5;
	inputKey[MVC_Model::CAMERA_FP_NOCLIP_KEY] = VK_F6;

	// Misc
	inputKey[MVC_Model::MISC_0_KEY] = '0';
	inputKey[MVC_Model::MISC_1_KEY] = '1';
	inputKey[MVC_Model::MISC_2_KEY] = '2';
	inputKey[MVC_Model::MISC_3_KEY] = '3';
	inputKey[MVC_Model::MISC_4_KEY] = '4';
	inputKey[MVC_Model::MISC_5_KEY] = '5';
	inputKey[MVC_Model::MISC_6_KEY] = '6';
	inputKey[MVC_Model::MISC_7_KEY] = '7';
	inputKey[MVC_Model::MISC_8_KEY] = '8';
	inputKey[MVC_Model::MISC_9_KEY] = '9';
}

void MVC_Controller::initInputDelayController(void)
{
	for (size_t i = 0; i < MAX_KEYS; ++i)
	{
		m_inputDelayController[i].startTimer();
	}
}

void MVC_Controller::initInputXInput(void)
{
	// Movement
	inputXInputKey[MVC_Model::MOVE_FORWARD_KEY] = XVK_LEFT_THUMB_UP;
	inputXInputKey[MVC_Model::MOVE_BACKWARD_KEY] = XVK_LEFT_THUMB_DOWN;
	inputXInputKey[MVC_Model::MOVE_LEFT_KEY] = XVK_LEFT_THUMB_LEFT;
	inputXInputKey[MVC_Model::MOVE_RIGHT_KEY] = XVK_LEFT_THUMB_RIGHT;
	inputXInputKey[MVC_Model::MOVE_JUMP_KEY] = XVK_BUTTON_A;
	inputXInputKey[MVC_Model::MOVE_CROUCH_KEY] = XVK_BUTTON_B;
	inputXInputKey[MVC_Model::MOVE_PRONE_KEY] = XVK_BUTTON_B;
	inputXInputKey[MVC_Model::MOVE_SPRINT_KEY] = XVK_BUTTON_LEFT_THUMB;

	// Look
	inputXInputKey[MVC_Model::LOOK_UP_KEY] = XVK_RIGHT_THUMB_UP;
	inputXInputKey[MVC_Model::LOOK_DOWN_KEY] = XVK_RIGHT_THUMB_DOWN;
	inputXInputKey[MVC_Model::LOOK_LEFT_KEY] = XVK_RIGHT_THUMB_LEFT;
	inputXInputKey[MVC_Model::LOOK_RIGHT_KEY] = XVK_RIGHT_THUMB_RIGHT;

	// Interact
	inputXInputKey[MVC_Model::INTERACT_ATTACK_1_KEY] = XVK_LEFT_TRIGGER;
	inputXInputKey[MVC_Model::INTERACT_ATTACK_2_KEY] = XVK_RIGHT_TRIGGER;
	inputXInputKey[MVC_Model::INTERACT_RELOAD_KEY] = XVK_BUTTON_Y;
	inputXInputKey[MVC_Model::INTERACT_PREV_KEY] = XVK_LEFT_SHOULDER;
	inputXInputKey[MVC_Model::INTERACT_NEXT_KEY] = XVK_RIGHT_SHOULDER;
	inputXInputKey[MVC_Model::INTERACT_GENERIC_KEY] = XVK_BUTTON_X;
	inputXInputKey[MVC_Model::INTERACT_SKILL_1_KEY] = XVK_BUTTON_B;
	inputXInputKey[MVC_Model::INTERACT_SKILL_2_KEY] = XVK_BUTTON_Y;

	// Game
	inputXInputKey[MVC_Model::GAME_EXIT_KEY] = XVK_BUTTON_BACK;
	inputXInputKey[MVC_Model::GAME_ACCEPT_KEY] = XVK_BUTTON_A;
	inputXInputKey[MVC_Model::GAME_CANCEL_KEY] = XVK_BUTTON_B;
	inputXInputKey[MVC_Model::GAME_DEBUG_KEY] = XVK_BUTTON_START;

	// Camera
	inputXInputKey[MVC_Model::CAMERA_FP_KEY] = XVK_DPAD_UP;
	inputXInputKey[MVC_Model::CAMERA_FP_NOCLIP_KEY] = XVK_DPAD_DOWN;

	// Misc
	inputXInputKey[MVC_Model::MISC_0_KEY] = NUM_XINPUT_VIRTUAL_KEYS;
	inputXInputKey[MVC_Model::MISC_1_KEY] = NUM_XINPUT_VIRTUAL_KEYS;
	inputXInputKey[MVC_Model::MISC_2_KEY] = NUM_XINPUT_VIRTUAL_KEYS;
	inputXInputKey[MVC_Model::MISC_3_KEY] = NUM_XINPUT_VIRTUAL_KEYS;
	inputXInputKey[MVC_Model::MISC_4_KEY] = NUM_XINPUT_VIRTUAL_KEYS;
	inputXInputKey[MVC_Model::MISC_5_KEY] = NUM_XINPUT_VIRTUAL_KEYS;
	inputXInputKey[MVC_Model::MISC_6_KEY] = NUM_XINPUT_VIRTUAL_KEYS;
	inputXInputKey[MVC_Model::MISC_7_KEY] = NUM_XINPUT_VIRTUAL_KEYS;
	inputXInputKey[MVC_Model::MISC_8_KEY] = NUM_XINPUT_VIRTUAL_KEYS;
	inputXInputKey[MVC_Model::MISC_9_KEY] = NUM_XINPUT_VIRTUAL_KEYS;
}

bool MVC_Controller::isKeyPressed(unsigned short key, bool delay)
{
	if (delay && !m_inputDelayController[key].hasTimePassed(INPUT_DELAY))
	{
		return false;
	}
	else
	{
		return ((GetAsyncKeyState(key) & 0x8001) != 0);
	}
}

void MVC_Controller::inputUpdate(double dt)
{
	inputMouseUpdate();

	inputViewUpdate();
	inputKeyUpdate();
	inputXInputUpdate();

	Camera3* cam3 = dynamic_cast<Camera3*>(m_model->GetCamera());

	if (camera_pitch > Math::EPSILON || camera_pitch < -Math::EPSILON)
	{
		cam3->Pitch(dt * camera_pitch);
	}

	if (camera_yaw > Math::EPSILON || camera_yaw < -Math::EPSILON)
	{
		cam3->Yaw(dt * camera_yaw);
	}
}

void MVC_Controller::inputMouseUpdate(void)
{
	// Get the current cursor position
	glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);

	// Set the mouse mode
	if (m_model->IsMouseFree())
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Calculate the difference in positions
		mouse_diff_x = mouse_current_x - mouse_last_x;
		mouse_diff_y = mouse_current_y - mouse_last_y;

		//Calculate the yaw and pitch
		camera_yaw = static_cast<float>(mouse_diff_x) * 0.0174555555555556f;// * 3.142f / 180.0f;
		camera_pitch = -(static_cast<float>(mouse_diff_y) * 0.0174555555555556f);// 3.142f / 180.0f );

																				 // Do a wraparound if the mouse cursor has gone out of the deadzone
																				 // -- For Horizontal
		if ((mouse_current_x < m_window_deadzone) || (mouse_current_x > m_window_width - m_window_deadzone))
		{
			mouse_current_x = m_window_width >> 1;			// Cheaply divide by 2
			glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
		}
		// -- For Vertical
		if ((mouse_current_y < m_window_deadzone) || (mouse_current_y > m_window_height - m_window_deadzone))
		{
			mouse_current_y = m_window_height >> 1;			// Cheaply divide by 2
			glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
		}

		// Store the current position as the last position
		mouse_last_x = mouse_current_x;
		mouse_last_y = mouse_current_y;
	}

	// Send Mouse Input Position to Model
	m_model->UpdateMouseInfo(mouse_current_x, mouse_current_y);
}

void MVC_Controller::inputViewUpdate(void)
{
	if (isKeyPressed(inputDebugKey[VIEW_TOGGLE_CULL_KEY], true))
	{
		m_view->ToggleCulling();
	}
	if (isKeyPressed(inputDebugKey[VIEW_TOGGLE_WIREFRAME_KEY], true))
	{
		m_view->ToggleWireframe();
	}
	if (isKeyPressed(inputDebugKey[VIEW_TOGGLE_LIGHT_KEY], true))
	{
		m_view->ToggleLight();
	}
}

void MVC_Controller::inputKeyUpdate(void)
{
	bool buttonPressed = false;

	for (size_t i = 0; i < MVC_Model::NUM_KEY_ACTION; ++i)
	{
		if (i == MVC_Model::INTERACT_ATTACK_1_KEY)
		{
			if (glfwGetMouseButton(m_window, 0) != 0)
			{
				m_model->ActivateKey(static_cast<MVC_Model::KEY_ACTION_TYPE>(i));
				buttonPressed = true;
			}
			continue;
		}

		// TODO: Code in a way to let Model control if a button press requires a delay

		if (isKeyPressed(inputKey[i]))
		{
			m_model->ActivateKey(static_cast<MVC_Model::KEY_ACTION_TYPE>(i));
			buttonPressed = true;
		}
	}

	if (buttonPressed)
	{
		m_model->UpdateLastInputDevice(ID_KB_MOUSE);
	}
}

bool MVC_Controller::inputXInputUpdateKeyPressed(void)
{
	XINPUT_STATE gamepadState;
	ZeroMemory(&gamepadState, sizeof(XINPUT_STATE));

	// Get gamepad state from XInput and store the error in error
	DWORD error = XInputGetState(0, &gamepadState);

	// Reset the analog inputs for this update
	lX = lY = rX = rY = lT = rT = 0.0f;

	// If a gamepad is connected
	if (error == ERROR_SUCCESS)
	{
		/*
		* Analog Inputs
		*/
		// Left Thumb Stick
		// -- Checking for dead zone
		// ---- X-Axis
		if (
			gamepadState.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
			||
			gamepadState.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
			)
		{
			lX = gamepadState.Gamepad.sThumbLX / SHRT_MAX;
		}
		// ---- Y-Axis
		if (
			gamepadState.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
			||
			gamepadState.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
			)
		{
			lY = gamepadState.Gamepad.sThumbLY / SHRT_MAX;
		}

		// Right Thumb Stick
		// -- Checking for dead zone
		// ---- X-Axis
		if (
			gamepadState.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
			||
			gamepadState.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
			)
		{
			rX = gamepadState.Gamepad.sThumbRX / SHRT_MAX;
		}
		// ---- Y-Axis
		if (
			gamepadState.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
			||
			gamepadState.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
			)
		{
			rY = gamepadState.Gamepad.sThumbRY / SHRT_MAX;
		}

		// Left Trigger
		// -- Checking for dead zone
		if (gamepadState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			lT = gamepadState.Gamepad.bLeftTrigger / CHAR_MAX;
		}

		// Right Trigger
		// -- Checking for dead zone
		if (gamepadState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			rT = gamepadState.Gamepad.bRightTrigger / CHAR_MAX;
		}

		static const short BUTTON_MAP[NUM_XINPUT_VIRTUAL_KEYS] =
		{
			XINPUT_GAMEPAD_A,
			XINPUT_GAMEPAD_B,
			XINPUT_GAMEPAD_X,
			XINPUT_GAMEPAD_Y,
			XINPUT_GAMEPAD_DPAD_UP,
			XINPUT_GAMEPAD_DPAD_DOWN,
			XINPUT_GAMEPAD_DPAD_LEFT,
			XINPUT_GAMEPAD_DPAD_RIGHT,
			XINPUT_GAMEPAD_LEFT_SHOULDER,
			XINPUT_GAMEPAD_LEFT_SHOULDER,
			XINPUT_GAMEPAD_LEFT_THUMB,
			XINPUT_GAMEPAD_RIGHT_THUMB,
			XINPUT_GAMEPAD_START,
			XINPUT_GAMEPAD_BACK
			// Anything below this is analog
		};

		for (size_t button = 0; button < XVK_ANALOG_MARKER; ++button)
		{
			if (gamepadState.Gamepad.wButtons & BUTTON_MAP[button])
			{
				xInputKeyPressed[button] = true;
			}
			else
			{
				xInputKeyPressed[button] = false;
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool MVC_Controller::isXInputKeyPressed(E_XINPUT_VIRTUAL_KEY key)
{
	// Don't check for disabled keys
	if (key == NUM_XINPUT_VIRTUAL_KEYS || key == XVK_ANALOG_MARKER)
	{
		return false;
	}

	if (key < XVK_ANALOG_MARKER)		// Not Analog
	{
		return xInputKeyPressed[key];
	}
	else		// Analog
	{
		switch (key)
		{
			case XVK_LEFT_THUMB_UP:
			{
				return lY > 0.0f;
				break;
			}
				
			case XVK_LEFT_THUMB_DOWN:
			{
				return lY < 0.0f;
				break;
			}
			case XVK_LEFT_THUMB_LEFT:
			{
				return lX < 0.0f;
				break;
			}
			case XVK_LEFT_THUMB_RIGHT:
			{
				return lX > 0.0f;
				break;
			}
			case XVK_RIGHT_THUMB_UP:
			{
				return rY > 0.0f;
				break;
			}
			case XVK_RIGHT_THUMB_DOWN:
			{
				return rY < 0.0f;
				break;
			}
			case XVK_RIGHT_THUMB_LEFT:
			{
				return rX < 0.0f;
				break;
			}
			case XVK_RIGHT_THUMB_RIGHT:
			{
				return rX > 0.0f;
				break;
			}
			case XVK_LEFT_TRIGGER:
			{
				return lT > 0.0f;
				break;
			}
			case XVK_RIGHT_TRIGGER:
			{
				return rT > 0.0f;
				break;
			}
		}
	}
}

void MVC_Controller::inputXInputUpdate(void)
{
	bool buttonPressed = false;

	// Check if the controller is plugged in
	if (inputXInputUpdateKeyPressed())
	{
		/*
		* Buttons
		*/
		for (size_t button = 0; button < MVC_Model::NUM_KEY_ACTION; ++button)
		{
			if (isXInputKeyPressed(inputXInputKey[button]))
			{
				m_model->ActivateKey(static_cast<MVC_Model::KEY_ACTION_TYPE>(button));
				buttonPressed = true;
			}
		}
	}

	// Update the analog input in the model
	m_model->UpdateXInput(lX, lY, rX, rY, lT, rT);

	if (buttonPressed)
	{
		m_model->UpdateLastInputDevice(ID_XINPUT);
	}
}
