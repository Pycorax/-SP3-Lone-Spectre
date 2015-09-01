#include "Player.h"

Player* Player::s_instances = NULL;
const float Player::S_PLAYER_MOVE_SPEED = 300.f;
const float Player::S_PLAYER_JUMP_SPEED = 1200.f;
const float Player::S_SPECTRE_DIVE_LIGHT_LIMIT = 5.f;
const float Player::S_SPECTRE_DIVE_COOLDOWN = 1.f;
const float Player::S_SPECTRE_JUMP_COOLDOWN = 1.f;
const int Player::S_MAX_JUMP_RANGE = 7; // Minimum 2

Player::Player(void)
{
}

void Player::Init(Mesh* _mesh)
{
	SetMesh(_mesh);
	m_currentSpeed = S_PLAYER_MOVE_SPEED;
	resetMove();
	resetDive();
	resetJump();
	resetHost();
	m_currentState = Player::PS_IDLE_DOWN;
	m_spectreMode = false;
	m__host = NULL;
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
void Player::AddAnimation(Animation* _anim, E_PLAYER_STATE playerState)
{
	m__animationList[playerState] = _anim;
}

Player::E_PLAYER_STATE Player::Interact(E_INTERACTION interact, TileMap* _map)
{
	Vector2 interactionDistance(m_lookDir * _map->GetTileSize());
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
		// skip if not within map
	}
	else
	{
		_tileInFrontOfPlayer = _map->GetTileAt(tilePos);
		tileTypeInFrontOfPlayer = _tileInFrontOfPlayer->GetType();
	}
	Tile* _tileOnPlayer = _map->GetTileAt(GetMapPos());
	Tile::E_TILE_TYPE tileTypeOnPlayer = _tileOnPlayer->GetType();

	// Spectral Dive
	if (interact == INTERACT_DIVE && m_diveTimer <= 0.f) // Light level is low enough and dive key is pressed, do Spectre Dive
	{
		if (_tileOnPlayer->GetLightLevel() <= S_SPECTRE_DIVE_LIGHT_LIMIT || (_tileInFrontOfPlayer && _tileInFrontOfPlayer->GetLightLevel() <= S_SPECTRE_DIVE_LIGHT_LIMIT && !_map->CheckCollision(tilePos)))
		{
			return PS_SPECTRAL_DIVE;
		}
	}

	// Spectral Jump
	if (interact == INTERACT_JUMP && m_inShadow && m_jumpTimer <= 0.f && (_tileInFrontOfPlayer && _tileInFrontOfPlayer->GetLightLevel() > S_SPECTRE_DIVE_LIGHT_LIMIT && !_map->CheckCollision(tilePos))) // Jump across lighted area
	{
		for (int tile = 1; tile < S_MAX_JUMP_RANGE; ++tile)
		{
			tilePos += (m_lookDir * _map->GetTileSize());
			if (tilePos.x < 0 || tilePos.x >= _map->GetMapSize().x || tilePos.y < 0 || tilePos.y >= _map->GetMapSize().y || _map->CheckCollision(tilePos))
			{
				if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_UP])
				{
					return PS_IDLE_UP; // Hit the end of map or collided
				}
				else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_DOWN])
				{
					return PS_IDLE_DOWN; //Hit the end of map or collided
				}
				else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_LEFT])
				{
					return PS_IDLE_LEFT; //Hit the end of map or collided
				}
				else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
				{
					return PS_IDLE_RIGHT; //Hit the end of map or collided
				}
				
			}
			else
			{
				if (_map->GetTileAt(tilePos)->GetLightLevel() <= S_SPECTRE_DIVE_LIGHT_LIMIT)
				{
					return PS_SPECTRAL_JUMP;
				}
			}
		}
		//updates idle based on look dir
		if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_UP])
		{
			return PS_IDLE_UP;
		}
		else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_DOWN])
		{
			return PS_IDLE_DOWN;
		}
		else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_LEFT])
		{
			return PS_IDLE_LEFT;
		}
		else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
		{
			return PS_IDLE_RIGHT; 
		} // Unable to jump since no shadow within range
	}

	//If inside enemy's shadow
	if (interact == INTERACT_HOST && !m_moving) // Control enemy
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

	if (interact == INTERACT_ESCAPE && tileTypeOnPlayer == Tile::TILE_EXTRACTION)
	{
		return PS_SPECTRAL_ESCAPE;
	}

	//If assassinating someone
	//if (m_currentState != PS_SPECTRAL_ASSASSINATE && !m_moving && !m_diving)
	//{
	//	return PS_SPECTRAL_ASSASSINATE;
	//}

	/*if(interact == INTERACT_HOSTAGE && tileTypeOnPlayer == Tile::TILE_EXTRACTION && m_currentState != PS_SPECTRAL_HOSTAGE && !m_moving && !m_diving)
	{
		return PS_SPECTRAL_HOSTAGE;
	}*/

	//If next to a document to collect ** NOTE: currently only check if infront is light **s
	if (interact == INTERACT_COLLECT && (tileTypeOnPlayer == Tile::TILE_DOCUMENT || tileTypeInFrontOfPlayer == Tile::TILE_DOCUMENT) && m_currentState != PS_SPECTRAL_COLLECT && !m_moving && !m_diving)
	{
		return PS_SPECTRAL_COLLECT;
	}

	//If next to a bomb to defuse
	if (interact == INTERACT_DEFUSE && (tileTypeOnPlayer == Tile::TILE_BOMB || tileTypeInFrontOfPlayer == Tile::TILE_BOMB) && m_currentState != PS_SPECTRAL_DEFUSE && !m_moving && !m_diving)
	{
		return PS_SPECTRAL_DEFUSE;
	}

	//If need to plant a bomb
	if (interact == INTERACT_SETBOMB && (tileTypeOnPlayer == Tile::TILE_SETBOMBAREA || tileTypeInFrontOfPlayer == Tile::TILE_SETBOMBAREA) && m_currentState != PS_SPECTRAL_SETBOMB && !m_moving && !m_diving)
	{
		return PS_SPECTRAL_SETBOMB;
	}

	//updates idle based on look dir
	if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_UP])
	{
		return PS_IDLE_UP;
	}
	else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_DOWN])
	{
		return PS_IDLE_DOWN;
	}
	else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_LEFT])
	{
		return PS_IDLE_LEFT;
	}
	else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
	{
		return PS_IDLE_RIGHT; 
	} // No interactions suit criteria
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

	// Force player to leave shadow when they land in a bright tile
	if (m_inShadow && _map->GetTileAt(GetMapPos())->GetLightLevel() > S_SPECTRE_DIVE_LIGHT_LIMIT && !m_jumping && !m_hosting)
	{
		m_inShadow = false;
		m_spectreMode = false;
		// Check where it player is facing when kicked out of shadow
		if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_UP])
		{
			m_currentState = PS_IDLE_UP;
		}
		else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_DOWN])
		{
			m_currentState = PS_IDLE_DOWN;
		}
		else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_LEFT])
		{
			m_currentState = PS_IDLE_LEFT;
		}
		else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
		{
			m_currentState = PS_IDLE_RIGHT; 
		}
	}

	if (m_currentState != PS_SPECTRAL_HAX)
	{
		if (m_moving)
		{
			move(dt, _map);
		}
		if (m_diving)
		{
			dive(dt, _map);
		}
		if (m_jumping)
		{
			jump(dt, _map);
		}
		if(m_hosting)
		{
			//TODO: set time limit
			UpdateHost(dt, _map);
			m_hostingTimeLimit += dt;
		}
		else
		{
			if(m__host)
			{
				m__host->SetPossesion(false);
				m__host = NULL;
			}
		}
		if(m_spectreMode)
		{
			m_animTime += dt;
			// total time taken = time 
			float divingAnimTime = m__animationList[PS_SPECTRAL_DIVING_UP]->animTime;
			//if animation finish
			if(m_animTime >= divingAnimTime)
			{
				if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_UP])
				{
					m_currentState = PS_SPECTRAL_DIVE_UP;
				}
				else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_DOWN])
				{
					m_currentState = PS_SPECTRAL_DIVE_DOWN;
				}
				else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_LEFT])
				{
					m_currentState = PS_SPECTRAL_DIVE_LEFT;
				}
				else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
				{
					m_currentState = PS_SPECTRAL_DIVE_RIGHT;
				}
			}
			//diving state set - will update the render
			else
			{
				if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_UP])
				{
					m_currentState = PS_SPECTRAL_DIVING_UP;
				}
				else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_DOWN])
				{
					m_currentState = PS_SPECTRAL_DIVING_DOWN;
				}
				else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_LEFT])
				{
					m_currentState = PS_SPECTRAL_DIVING_LEFT;
				}
				else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
				{
					m_currentState = PS_SPECTRAL_DIVING_RIGHT;
				}
			}
		
		}
		else
		{
			m_animTime = 0.f;
		}
		//update animation
		static SpriteAnimation* _sa = dynamic_cast<SpriteAnimation* >(GetMesh());
		if(_sa)
		{
			_sa->m_anim = m__animationList[m_currentState];
			_sa->Update(dt);
		}
	}

}

void Player::SetHostPTR(NPC* _enemy)
{
	m_hosting = true;
	//set enemy ptr to being possed
	_enemy->ForceSetEnemyState(NPC::ES_POSSESED);
	//give player ptr to become that
	m__host = _enemy;
	//set the host to be being possed
	m__host->SetPossesion(true);
}

bool Player::GetHosting() const
{
	return m_hosting;
}

void Player::UpdateHost(double dt, TileMap* _map)
{
	//update host here
	//if player walk - use the direction to change the direction moving
	//update enemy depending on lookdir
	m__host->SetLookDir(GetLookDir());
	m__host->SetMapPosition(GetMapPos() , _map->GetScrollOffset(), _map->GetTileSize());
	
}

void Player::Clear()
{
	if (s_instances != NULL)
	{
		delete[] s_instances;
		s_instances = NULL;
	}
}

void Player::SetState(Player::E_PLAYER_STATE currentState)
{
	this->m_currentState = currentState;
}

void Player::ForceSetAnimation(E_PLAYER_STATE currentState)
{
	this->m_currentState = currentState;
	SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(m__mesh);
	if (sa)
	{
		sa->m_anim = m__animationList[m_currentState];
	}
}

Player::E_PLAYER_STATE Player::GetState(void) const
{
	return this->m_currentState;
}

void Player::move(double dt, TileMap* _map)
{
	if (m_jumping)
	{
		m_currentSpeed = S_PLAYER_JUMP_SPEED;
	}
	else
	{
		m_currentSpeed = S_PLAYER_MOVE_SPEED;
	}
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
	Vector2 newPos = s_newOrigin + (m_lookDir * m_currentSpeed * dt); // New position if move
	if (_map->CheckCollision(newPos) || (m_moveDist + (m_currentSpeed * dt)) >= tileSize || ((m_inShadow && !m_jumping) && _map->GetTileAt(newPos)->GetLightLevel() > S_SPECTRE_DIVE_LIGHT_LIMIT)) // Collided or moving more than 1 tile
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
		SetMapPosition(tilePos, _map->GetScrollOffset(), _map->GetTileSize()); // Snap player to wall
		resetMove();
	}
	else
	{
		if (shiftOrigin) // Moving right or up
		{
			newPos -= GetLookDir() * _map->GetTileSize();
		}
		_map->AddToScrollOffset(GetLookDir() * m_currentSpeed * dt);
		SetMapPosition(newPos, _map->GetScrollOffset(), _map->GetTileSize()); // Remove tile size that was added previously
		m_moveDist += m_currentSpeed * dt;
	}
}

void Player::resetMove()
{
	m_moving = false;
	m_moveDist = 0.f;
	//updates idle based on look dir
	if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_UP])
	{
		m_currentState = PS_IDLE_UP;
	}
	else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_DOWN])
	{
		m_currentState = PS_IDLE_DOWN;
	}
	else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_LEFT])
	{
		m_currentState = PS_IDLE_LEFT;
	}
	else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
	{
		m_currentState = PS_IDLE_RIGHT; 
	}
}

void Player::resetDive()
{
	m_diving = false;
	//updates idle based on look dir
	if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_UP])
	{
		m_currentState = PS_IDLE_UP;
	}
	else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_DOWN])
	{
		m_currentState = PS_IDLE_DOWN;
	}
	else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_LEFT])
	{
		m_currentState = PS_IDLE_LEFT;
	}
	else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
	{
		m_currentState = PS_IDLE_RIGHT; 
	}
	m_inShadow = !m_inShadow;
	m_diveTimer = S_SPECTRE_DIVE_COOLDOWN;
}

void Player::resetJump()
{
	m_jumping = false;
	//updates idle based on look dir
	if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_UP])
	{
		m_currentState = PS_IDLE_UP;
	}
	else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_DOWN])
	{
		m_currentState = PS_IDLE_DOWN;
	}
	else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_LEFT])
	{
		m_currentState = PS_IDLE_LEFT;
	}
	else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
	{
		m_currentState = PS_IDLE_RIGHT; 
	}
	m_jumpTimer = S_SPECTRE_JUMP_COOLDOWN;
	m_tileMoved = 0;
}

void Player::resetHost()
{
	m_hosting = false;
}

void Player::SetMove(Vector2 dir)
{
	if (!m_moving && !m_jumping && !m_diving)
	{
		if (m_currentState != PS_SPECTRAL_HAX) // Move when not in hax mode
		{
			SetLookDir(dir);
			m_moving = true;
			//updates idle based on look dir
			if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_UP])
			{
				m_currentState = PS_WALK_UP;
			}
			else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_DOWN])
			{
				m_currentState = PS_WALK_DOWN;
			}
			else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_LEFT])
			{
				m_currentState = PS_WALK_LEFT;
			}
			else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
			{
				m_currentState = PS_WALK_RIGHT; 
			}
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

void Player::SetHost()
{
	if(m_currentState != PS_SPECTRAL_HAX)
	{
		m_hosting = true;
		m_currentState = PS_SPECTRAL_HOST;
	}
	else
	{
		m_hosting = false;
	}
}

void Player::dive(double dt, TileMap* _map)
{
	if (m_inShadow) // Already in shadow, jump out
	{
		// When animation ended, change m_inShadow to false
		resetDive();
		resetHost();
		m_spectreMode = false;
	}
	else // Jump into shadow
	{
		//shows player in spectre form - to be changed ->need to finish diving animation first
		m_spectreMode = true;

		if (_map->GetTileAt(GetMapPos())->GetLightLevel() <= S_SPECTRE_DIVE_LIGHT_LIMIT) // Dive in current tile
		{
			if (m_currentState != PS_SPECTRAL_DIVE)
			{
				m_animTime += dt;
				//m_currentState = PS_SPECTRAL_DIVE;
			}
			resetDive();
			// When animation ended, change m_inShadow to true
			m_inShadow = true;
		}
		else // Dive in the tile in front of player
		{
			if (m_currentState != (PS_WALK_UP || PS_WALK_DOWN || PS_WALK_LEFT || PS_WALK_RIGHT) )
			{
				forceSetMove(m_lookDir); // Set to move player to proper tile
			}
		}
	}
}

void Player::jump(double dt, TileMap* _map)
{
	if (m_tileMoved == 0) // Move once
	{
		forceSetMove(m_lookDir);
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
			forceSetMove(m_lookDir);
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
	this->m_spectreMode = inShadow;
}

bool Player::GetInShadow()
{
	return m_inShadow;
}

void Player::forceSetMove(Vector2 dir)
{
	if (m_currentState != PS_SPECTRAL_HAX) // Move when not in hax mode
	{
		SetLookDir(dir);
		m_moving = true;
		//updates idle based on look dir
		if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_UP])
		{
			m_currentState = PS_WALK_UP;
		}
		else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_DOWN])
		{
			m_currentState = PS_WALK_DOWN;
		}
		else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_LEFT])
		{
			m_currentState = PS_WALK_LEFT;
		}
		else if(m_lookDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
		{
			m_currentState = PS_WALK_RIGHT; 
		}
	}
	else // Disable all movement when in hax mode
	{
		m_moving = false;
	}
}

float Player::GetDiveTimer()
{
	return m_diveTimer;
}

float Player::GetJumpTimer()
{
	return m_jumpTimer;
}