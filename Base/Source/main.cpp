#include "MVC_Model.h"
#include "MVC_Model_Spectre.h"
#include "MVC_View.h"
#include "MVC_Controller.h"

int main( void )
{
	//MVC_Model* model = new MVC_Model_Shadow("SONs//Shadow_Test//ModelConfig_Shadow.son");
	//MVC_Model* model = new MVC_Model_Zombie("SONs//ModelConfig_Zombie.son");
	MVC_Model* model = new MVC_Model_Spectre("SONs//ModelConfig_Spectre.son");
	MVC_View* view = new MVC_View(model, "SONs//ViewConfig.son");
	MVC_Controller* controller = new MVC_Controller(model, view, "SONs//ControllerConfig.son");

	controller->Init();
	controller->Run();
	controller->Exit();

	if (model != NULL)
	{
		delete model;
	}

	if (view != NULL)
	{
		delete view;
	}

	if (controller != NULL)
	{
		delete controller;
	}
}
