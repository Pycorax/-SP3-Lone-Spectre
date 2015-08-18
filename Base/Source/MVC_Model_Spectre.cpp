#include "MVC_Model_Spectre.h"


MVC_Model_Spectre::MVC_Model_Spectre(string configSONFile) : MVC_Model(configSONFile), test(NULL)
{
}


MVC_Model_Spectre::~MVC_Model_Spectre(void)
{
}

void MVC_Model_Spectre::processKeyAction(double dt)
{

}

void MVC_Model_Spectre::Init()
{
	test = new TileMap(Vector2(32,25), Vector2(32,25), 32.f);
	test->LoadTileMap("TileMap//Level1.csv", meshList);
}

void MVC_Model_Spectre::Update(double dt)
{

}

void MVC_Model_Spectre::Exit()
{

}