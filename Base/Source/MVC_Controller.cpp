#include "MVC_Controller.h"

#include "timer.h"
#include "Camera3.h"
#include "SONIO.h"
#include "MVC_Model_3D.h"

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

	void resize_callback(GLFWwindow* window, int w, int h)
	{
		glViewport(0, 0, w, h);
	}
}

const double MVC_Controller::INPUT_DELAY = 0.2;

MVC_Controller::MVC_Controller(MVC_Model * model, MVC_View * view, string configSONFile) : m_model(model), m_view(view), m_configSONFile(configSONFile)
{
}

MVC_Controller::~MVC_Controller()
{
}

void MVC_Controller::Init(int fps/* = 60*/)
{
	loadConfig();

	// Create a Window for our Application
	SetupWindow(m_window_title, m_window_width, m_window_height);
	
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

	SetFrameRate(fps);
	initInputKeys();
	initInputMouse();
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
			else if (attrib->name == "WindowWidth")
			{
				m_window_width = stoi(attrib->value);
			}
			else if (attrib->name == "WindowHeight")
			{
				m_window_height = stoi(attrib->value);
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
	glfwSetWindowSizeCallback(m_window, GLFW::resize_callback);

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

	// Game
	inputKey[MVC_Model::GAME_EXIT_KEY] = VK_ESCAPE;
	inputKey[MVC_Model::GAME_ACCEPT_KEY] = VK_RETURN;
	inputKey[MVC_Model::GAME_CANCEL_KEY] = VK_BACK;
	inputKey[MVC_Model::GAME_DEBUG_KEY] = VK_OEM_3; // `~

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
	for (size_t i = 0; i < MVC_Model::NUM_KEY_ACTION; ++i)
	{
		if (i == MVC_Model::INTERACT_ATTACK_1_KEY)
		{
			if (glfwGetMouseButton(m_window, 0) != 0)
			{
				m_model->ActivateKey(static_cast<MVC_Model::KEY_ACTION_TYPE>(i));
			}
			continue;
		}

		// TODO: Code in a way to let Model control if a button press requires a delay

		if (isKeyPressed(inputKey[i]))
		{
			m_model->ActivateKey(static_cast<MVC_Model::KEY_ACTION_TYPE>(i));
		}
	}
}
