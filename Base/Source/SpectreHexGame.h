#ifndef SPECTRAL_HEX_GAME_H
#define SPECTRAL_HEX_GAME_H

// STL Includes
#include <vector>

// Other Includes
#include "PhysicalObject.h"

// Using Directives
using std::vector;

class SpectreHexGame
{
private:
	enum GAME_STATE									// Defines the different states that this minigame can possess
	{
		GS_START,
		GS_PLAYING,
		GS_WIN_CEREMONY,
		GS_LOSE_CEREMONY,
		GS_END_IN_WIN,
		GS_END_IN_LOSS,
		NUM_STATES
	};

	// Static Constants
	// -- Balls
	static const unsigned MAX_BALLS = 20;			// Max balls to spawn at the start
	static const float MIN_BALL_RADIUS;				// The minimum radius of the ShadowBalls
	static const Vector2 MIN_BALL_SCALE;			// The minimum scale of the ShadowBalls, determined by MIN_BALL_RADIUS
	// -- Ball Physics
	static const float MIN_BALL_MASS;				// The minimum mass of the ShadowBalls
	static const float PLAYER_BALL_MULTIPLIER;		// Determines the stats of the player PhysicalObject by multiplying this with the minimum stats of other balls above
	static const float PLAYER_MOVE_FORCE;			// The force of to effect on the player
	// -- Walls
	static const float WALL_THICKNESS;				// The thickness of the walls in this minigame
	static const float EXIT_WALL_THICKNESS;			// The thickness of the exit wall to the right in this minigame
	static const float MIN_PLAYER_EXIT_RADIUS;		// The minimum radius of the player before he can exit the level and win
	// -- Ball Spawning
	static const Vector2 BALL_SPAWN_MIN_VEL;		// The minimum velocity of normal shadow balls at spawn
	static const Vector2 BALL_SPAWN_MAX_VEL;		// The maximum velocity of normal shadow balls at spawn
	static const Vector2 BALL_SPAWN_MIN_POS;		// The minimum position of normal shadow balls at spawn
	static const Vector2 BALL_SPAWN_MAX_POS_OFFSET;	// The offset to add to the screen resolution to get the maximum position of normal shadow balls at spawn

private:	// Variables
	GAME_STATE m_state;								// Controls the state of the minigame
	vector<PhysicalObject*> m_ballList;				// Stores all the ShadowBalls to be used
	PhysicalObject* m__player;						// The player PhysicalObject object
	PhysicalObject* m__exitWall;					// The wall that can be passed through when the player has enough 
	GameObject2D* m__background;					// The background image of this hacking game
	Mesh* m__restrictedWallMesh;
	Mesh* m__destroyedWallMesh;						// Stores the mesh for a destroyed wall to be used on winning
	GameObject2D* m__loseScreen;					// Stores the mesh for the screen that will be displayed on losing

public:
	SpectreHexGame(void);
	virtual ~SpectreHexGame(void);

	void Init(Mesh* _shadowBallMesh, Mesh* _circuitWallMesh, Mesh* _destroyedCircuitMesh, Mesh* _restrictedCircuitMesh, Mesh* _loseScreen, Mesh* _bgMesh, int viewWidth, int viewHeight);
	void Update(double dt);
	void Exit(void);

	// Game Status: Use this to get the state of this minigame. If either is true, it means the game has ended.
	bool IsLoss() const;		// Check if is a loss yet
	bool IsVictory() const;		// Check if is victory yet

	// Game Controls
	void Move(bool left, bool right, bool up, bool down, double dt);

	// Returns the objects to be rendered
	vector<GameObject2D*> GetRenderObjects(void) const;

private:
	PhysicalObject* fetchObject(void);

	// Update Functions
	void startUpdate(double dt);
	void playingUpdate(double dt);
	void winCeremonyUpdate(double dt);
	void loseCeremonyUpdate(double dt); 
};

#endif