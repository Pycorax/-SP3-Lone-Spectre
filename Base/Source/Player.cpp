#include "Player.h"

Player* Player::s_instances = NULL;
const float Player::S_PLAYER_MOVE_SPEED = 300.f;
const float Player::S_SPECTRE_DIVE_LIGHT_LIMIT = 2.f;
const float Player::S_SPECTRE_DIVE_COOLDOWN = 1.f;
const float Player::S_SPECTRE_JUMP_COOLDOWN = 1.f;
const int Player::S_MAX_JUMP_RANGE = 2; // Minimum 2

Player::Player(void)
{
}

void Player::Init(Mesh* _mesh)
{
	SetMesh(_mesh);
	/*m_moving = m_diving = m_jumping = m_inShadow = false;
	m_moveDist = 0.f;
	m_tileMoved = 0;
	m_diveTimer = m_jumpTimer = 0.f;
	m_currentState = PS_IDLE;*/
	resetMove();
	resetDive();
	resetJump();
}

Player::~Player(void)
{

}

Player* Player::GetInstance(int instance)
{
	if (s_instances == NULL)
	{
		s_instances = new Player[S_MAX_INSTANCES];
	}

	if (instance >= 0 && instance < S_MAX_INSTANCES)
	{
		return s_instances + instance;
	}
	else
	{
		return NULL;
	}
}

//adds sprite animation depending on the state specified
void Player::AddMesh(Mesh* _mesh, E_PLAYER_STATE playerState)
{
	m__saList[playerState] = _mesh;
}

Player::E_PLAYER_STATE Player::Interact(E_INTERACTION interact, TileMap* _map)
{
	Vector2 interactionDistance(m_lookDir * _map->GetTileSize());
	//TODO : Add in algorithm for determerning the type of action
	//		Host, Dive , Jump or Hex
	// if( lookDir == enemy back) -> host
	// if( lookDir == Item on map) ->dive
	// if( m_currentState == Dive || host && lookDir == enemyBack || item on map) -> jump then become dive ||host
	// if (lookDir == Camera ) -> Hex goto minigame
	bool shiftOrigin = false;
	static Vector2 s_newOrigin;
	if (GetLookDir().x < 0 || GetLookDir().y < 0) // Moving left or down
	{
		s_newOrigin = GetMapPos(); // No change in origin (Bottom or Left)
	}
	else // Moving right or up
	{
		s_newOrigin = GetMapPos() + (GetLookDir() * _map->GetTileSize()); // Change in origin (Top or Right)
		shiftOrigin = true;
	}
	Vector2 tilePos = Vector2(floor(s_newOrigin.x / _map->GetTileSize()) * _map->GetTileSize(), floor(s_newOrigin.y / _map->GetTileSize()) * _map->GetTileSize()) + m_lookDir;
	Tile* _tileInFrontOfPlayer = NULL;
	Tile::E_TILE_TYPE tileTypeInFrontOfPlayer = static_cast<Tile::E_TILE_TYPE>(NULL);
	if (tilePos.x < 0 || tilePos.x >= _map->GetMapSize().x || tilePos.y < 0 || tilePos.y >= _map->GetMapSize().y)
	{
		// skip
	}
	else
	{
		_tileInFrontOfPlayer = _map->GetTileAt(tilePos);
		tileTypeInFrontOfPlayer = _tileInFrontOfPlayer->GetType();
	}
	Tile* _tileOnPlayer = _map->GetTileAt(GetMapPos());
	Tile::E_TILE_TYPE tileTypeOnPlayer = _tileOnPlayer->GetType();

	// Spectral Dive
	if (interact == INTERACT_DIVE && m_diveTimer <= 0.f && !m_moving) // Light level is low enough and dive key is pressed, do Spectre Dive
	{
		if (_tileOnPlayer->GetLightLevel() <= S_SPECTRE_DIVE_LIGHT_LIMIT || (_tileInFrontOfPlayer && _tileInFrontOfPlayer->GetLightLevel() <= S_SPECTRE_DIVE_LIGHT_LIMIT && !_map->CheckCollision(tilePos)))
		{
			return PS_SPECTRAL_DIVE;
		}
	}

	// Spectral Jump
	if (interact == INTERACT_JUMP && m_inShadow && !m_moving && m_jumpTimer <= 0.f && (_tileInFrontOfPlayer && _tileInFrontOfPlayer->GetLightLevel() > S_SPECTRE_DIVE_LIGHT_LIMIT && !_map->CheckCollision(tilePos))) // Jump across lighted area
	{
		for (int tile = 1; tile < S_MAX_JUMP_RANGE; ++tile)
		{
			tilePos += (m_lookDir * _map->GetTileSize()) * tile;
			if (tilePos.x < 0 || tilePos.x >= _map->GetMapSize().x || tilePos.y < 0 || tilePos.y >= _map->GetMapSize().y || _map->CheckCollision(tilePos))
			{
				return PS_IDLE; // Hit the end of map or collided
			}
			else
			{
				if (_map->GetTileAt(tilePos)->GetLightLevel() <= S_SPECTRE_DIVE_LIGHT_LIMIT)
				{
					return PS_SPECTRAL_JUMP;
				}
			}
		}
		return PS_IDLE; // Unable to jump since no shadow within range
	}

	//If inside enemy's shadow (Not completed)
	if (interact == INTERACT_HOST) // Control enemy
	{
		return PS_SPECTRAL_HOST;
	}

	//If next to a camera to hack
	if ((tileTypeOnPlayer == Tile::TILE_OBJ_CAMERA_ON_1_1 
		||
		tileTypeOnPlayer == Tile::TILE_OBJ_CAMERA_ON_1_2
		||
		tileTypeOnPlayer == Tile::TILE_OBJ_CAMERA_ON_1_3
		||
		tileTypeOnPlayer == Tile::TILE_OBJ_CAMERA_ON_1_4
		||
		(tileTypeInFrontOfPlayer && (tileTypeInFrontOfPlayer == Tile::TILE_OBJ_CAMERA_ON_1_1 
		||
		tileTypeInFrontOfPlayer == Tile::TILE_OBJ_CAMERA_ON_1_2
		||
		tileTypeInFrontOfPlayer == Tile::TILE_OBJ_CAMERA_ON_1_3
		||
		tileTypeInFrontOfPlayer == Tile::TILE_OBJ_CAMERA_ON_1_4)))
		&&
		m_currentState != PS_SPECTRAL_HAX
		&&
		!m_moving && !m_diving) // Camera on or in front of player, not already in hax mode, not moving or diving in order to enter hax mode
	{
		return PS_SPECTRAL_HAX;
	}
	return PS_IDLE; // No interactions suit criteria
}

void Player::Update(double dt, TileMap* _map)
{
	Character::Update();

	if (m_diveTimer > 0.f) // Cooldown
	{
		m_diveTimer -= dt;
	}
	if (m_jumpTimer > 0.f) // Cooldown
	{
		m_jumpTimer -= dt;
	}

	if (m_currentState != PS_SPECTRAL_HAX)
	{
		if (m_diving)
		{
			dive(dt, _map);
		}
		if (m_jumping)
		{
			jump(dt, _map);
		}
		if (m_moving)
		{
			move(dt, _map);
		}
	}

	// TODO: Update spriteanimation here
	//updates idle side - changes side according to last position
	/*if(m_lookDir == Vector3(0,1) ) // if facing up
	{
		m_currentState = PS_IDLE_UP;
	}
	else if(m_lookDir == Vector3(-1,0)  )//if facing left
	{
		m_currentState = PS_IDLE_LEFT;
	}
	else if(m_lookDir == Vector3(1,0) ) // if facing right
	{
		m_currentState = PS_IDLE_RIGHT;
	}
	else if(m_lookDir == Vector3(0,-1) ) //if facing down
	{
		m_currentState = PS_IDLE_DOWN;
	}*/
}

void Player::UpdateHost(double dt)
{
	static_cast <Enemy* >(m__host);
	//update host here
}

void Player::Clear()
{
	if (s_instances)
	{
		delete[] s_instances;
		s_instances = NULL;
	}
}

void Player::SetState(Player::E_PLAYER_STATE currentState)
{
	this->m_currentState = currentState;
}

Player::E_PLAYER_STATE Player::GetState(void) const
{
	return this->m_currentState;
}

void Player::move(double dt, TileMap* _map)
{
	static Vector2 s_newOrigin;
	bool shiftOrigin = false;
	float tileSize = _map->GetTileSize();
	if (m_lookDir.x < 0 || m_lookDir.y < 0) // Moving left or down
	{
		s_newOrigin = GetMapPos(); // No change in origin (Bottom or Left)
	}
	else // Moving right or up
	{
		s_newOrigin = GetMapPos() + (m_lookDir * tileSize); // Change in origin (Top or Right)
		shiftOrigin = true;
	}
	Vector2 newPos = s_newOrigin + (m_lookDir * S_PLAYER_MOVE_SPEED * dt); // New position if move
	if (_map->CheckCollision(newPos) || (m_moveDist + (S_PLAYER_MOVE_SPEED * dt)) >= tileSize || ((m_inShadow && !m_jumping) && _map->GetTileAt(newPos)->GetLightLevel() > S_SPECTRE_DIVE_LIGHT_LIMIT)) // Collided or moving more than 1 tile
	{
		Vector2 tilePos;
		if (shiftOrigin)
		{
			s_newOrigin -= m_lookDir * 0.5; // Shift origin to half the distance between new and original origin
		}
		if (m_lookDir.x == 0) // Moving along y axis, snap y axis and ignore x axis
		{
			tilePos = Vector2(s_newOrigin.x, floor(s_newOrigin.y / tileSize) * tileSize); // Position to snap to by axis
			if (tilePos.y >= _map->GetMapSize().y)
			{
				tilePos.y -= _map->GetTileSize();
			}
			else if (tilePos.y < 0)
			{
				tilePos.y = 0;
			}
		}
		else if (GetLookDir().y == 0) // Moving along x axis, snap x axis and ignore y axis
		{
			tilePos = Vector2(floor(s_newOrigin.x / tileSize) * tileSize, s_newOrigin.y); // Position to snap to by axis
			if (tilePos.x >= _map->GetMapSize().x)
			{
				tilePos.x -= _map->GetTileSize();
			}
			else if (tilePos.x < 0)
			{
				tilePos.x = 0;
			}
		}
		_map->AddToScrollOffset(tilePos - GetMapPos()); // Change scroll offset
		SetMapPosition(tilePos, _map->GetScrollOffset()); // Snap player to wall
		resetMove();
	}
	else
	{
		if (shiftOrigin) // Moving right or up
		{
			newPos -= GetLookDir() * _map->GetTileSize();
		}
		_map->AddToScrollOffset(GetLookDir() * S_PLAYER_MOVE_SPEED * dt);
		SetMapPosition(newPos, _map->GetScrollOffset()); // Remove tile size that was added previously
		m_moveDist += S_PLAYER_MOVE_SPEED * dt;
	}
}

void Player::resetMove()
{
	m_moving = false;
	m_moveDist = 0.f;
	m_currentState = PS_IDLE;
}

void Player::resetDive()
{
	m_diving = false;
	m_currentState = PS_IDLE;
	m_inShadow = !m_inShadow;
	m_diveTimer = S_SPECTRE_DIVE_COOLDOWN;
}

void Player::resetJump()
{
	m_jumping = false;
	m_currentState = PS_IDLE;
	m_jumpTimer = S_SPECTRE_JUMP_COOLDOWN;
	m_tileMoved = 0;
}

void Player::SetMove(Vector2 dir)
{
	if (!m_moving)
	{
		if (m_currentState != PS_SPECTRAL_HAX) // Move when not in hax mode
		{
			SetLookDir(dir);
			m_moving = true;
			m_currentState = PS_WALK;
		}
		else // Disable all movement when in hax mode
		{
			m_moving = false;
		}
	}
}

void Player::SetDive()
{
	if (m_currentState != PS_SPECTRAL_HAX) // Dive when not in hax mode
	{
		m_diving = true;
		m_currentState = PS_SPECTRAL_DIVE;
	}
	else // Disable diving when in hax mode
	{
		m_diving = false;
	}
}

void Player::SetJump()
{
	if (m_currentState != PS_SPECTRAL_HAX) // Jump when not in hax mode
	{
		m_jumping = true;
		m_currentState = PS_SPECTRAL_JUMP;
		m_tileMoved = 0;
	}
	else // Disable jumping when in hax mode
	{
		m_jumping = false;
	}
}

void Player::dive(double dt, TileMap* _map)
{
	if (m_inShadow) // Already in shadow, jump out
	{
		// When animation ended, change m_inShadow to false
		resetDive();
	}
	else // Jump into shadow
	{
		if (_map->GetTileAt(GetMapPos())->GetLightLevel() <= S_SPECTRE_DIVE_LIGHT_LIMIT) // Dive in current tile
		{
			if (m_currentState != PS_SPECTRAL_DIVE)
			{
				m_currentState = PS_SPECTRAL_DIVE;
				// TODO: Change spriteanimation to diving animation
			}
			resetDive();
			// When animation ended, change m_inShadow to true
		}
		else // Dive in the tile in front of player
		{
			if (m_currentState != PS_WALK)
			{
				SetMove(m_lookDir); // Set to move player to proper tile
			}
		}
	}
}

void Player::jump(double dt, TileMap* _map)
{
	if (m_tileMoved == 0) // Move once
	{
		SetMove(m_lookDir);
		++m_tileMoved;
	}
	else if (!m_moving)
	{
		if (_map->GetTileAt(GetMapPos())->GetLightLevel() <= S_SPECTRE_DIVE_LIGHT_LIMIT) // In shadow
		{
			resetJump();
		}
		else // Not in shadow
		{
			SetMove(m_lookDir);
			++m_tileMoved;
		}
		if (m_tileMoved >= S_MAX_JUMP_RANGE) // Hit max, reset
		{
			resetJump();
		}
	}
}

void Player::SetInShadow(bool inShadow)
{
	this->m_inShadow = inShadow;
}

bool Player::GetInShadow()
{
	return m_inShadow;
}