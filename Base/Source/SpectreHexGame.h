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
		GS_WIN,
		GS_LOSE,
		NUM_STATES
	};

	// Static Constants
	static const unsigned MAX_BALLS = 30;			// Max balls to spawn at the start
	static const float MIN_BALL_RADIUS;				// The minimum radius of the ShadowBalls
	static const Vector2 MIN_BALL_SCALE;			// The minimum scale of the ShadowBalls, determined by MIN_BALL_RADIUS
	static const float MIN_BALL_MASS;				// The minimum mass of the ShadowBalls
	static const float PLAYER_BALL_MULTIPLIER;		// Determines the stats of the player PhysicalObject by multiplying this with the minimum stats of other balls above
	static const float PLAYER_MOVE_FORCE;			// The force of to effect on the player
	static const float WALL_THICKNESS;				// The thickness of the walls in this minigame

private:	// Variables
	GAME_STATE m_state;								// Controls the state of the minigame
	vector<PhysicalObject*> m_ballList;					// Stores all the ShadowBalls to be used
	PhysicalObject* m__player;							// The player PhysicalObject object

public:
	SpectreHexGame(void);
	virtual ~SpectreHexGame(void);

	void Init(Mesh* _shadowBallMesh, Mesh* _circuitWallMesh, int viewWidth, int viewHeight);
	void Update(double dt);
	void Exit(void);

	// Game Controls
	void Move(bool left, bool right, bool up, bool down, double dt);

	// Returns the objects to be rendered
	vector<GameObject2D*> GetRenderObjects(void);

private:
	PhysicalObject* fetchObject(void);

	// Update Functions
	void startUpdate(double dt);
	void ballsUpdate(double dt);
};

#endif