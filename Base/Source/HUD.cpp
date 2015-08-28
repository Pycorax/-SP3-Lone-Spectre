#include "HUD.h"


HUD::HUD(void)
{
}


HUD::~HUD(void)
{
	Clear();
}

void HUD::Init(Mesh* _mesh, Vector2 pos, Vector2 scale)
{
	SetMesh(_mesh);
	SetScale(scale);
	SetPos(pos);
}

void HUD::Update(double dt)
{
	
}

void HUD::Clear()
{
	
}