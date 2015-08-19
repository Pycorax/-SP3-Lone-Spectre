#ifndef SPECTRAL_HEX_GAME_H
#define SPECTRAL_HEX_GAME_H

// STL Includes
#include <vector>

// Other Includes
#include "ShadowBall.h"

// Using Directives
using std::vector;

class SpectreHexGame
{
	// Static Constants
	static const unsigned MAX_BALLS = 30;			// Max balls to spawn at the start
	static const float MIN_BALL_RADIUS;				// The minimum radius of the ShadowBalls
	static const Vector2 MIN_BALL_SCALE;			// The minimum scale of the ShadowBalls, determined by MIN_BALL_RADIUS
	static const float MIN_BALL_MASS;				// The minimum mass of the ShadowBalls
	static const float PLAYER_BALL_MULTIPLIER;		// Determines the stats of the player ShadowBall by multiplying this with the minimum stats of other balls above
	static const float PLAYER_MOVE_FORCE;			// The force of to effect on the player

private:	// Variables
	vector<ShadowBall*> m_ballList;					// Stores all the ShadowBalls to be used
	ShadowBall* m__player;							// The player ShadowBall object

public:
	SpectreHexGame(void);
	virtual ~SpectreHexGame(void);

	void Init(Mesh* _shadowBallMesh);
	void Update(double dt);
	void Exit(void);

	// Game Controls
	void Move(bool left, bool right, bool up, bool down, double dt);

	// Returns the objects to be rendered
	vector<GameObject2D*> GetRenderObjects(void);

private:
	ShadowBall* fetchBall(void);
	void ballsUpdate(double dt);
};

#endif