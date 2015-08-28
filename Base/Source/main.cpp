#include "MVC_Model.h"
#include "MVC_Model_Spectre.h"
#include "MVC_View.h"
#include "MVC_Controller.h"

/*
 *	Visual Leak Detector
 *****************************
 * Do not uncomment unless VLD has been installed. This enabled VLD for memory leak checking.
 * 
 */ 
//#include <vld.h>

int main( void )
{
	MVC_Model* model = new MVC_Model_Spectre("SONs//ModelConfig_Spectre.son");
	MVC_View* view = new MVC_View(model, "SONs//ViewConfig.son");
	MVC_Controller* controller = MVC_Controller::GetInstance(model, view, "SONs//ControllerConfig.son");

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
