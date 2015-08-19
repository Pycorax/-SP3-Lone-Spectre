#ifndef CHAR_CLASS_H
#define CHAR_CLASS_H

#include "Mesh.h"
#include "Vector2.h"
#include "GameObject2D.h"
#include "MapEntity.h"

//TODO: factor in map for Character, Player, NPC , Enemy - update, pointer, constrains(moving, collision)
class Character : public GameObject2D , public MapEntity
{
protected:
	int m_health;
	int m_maxHealth;
	
	//mesh pointer 
	Mesh* m_animMesh;

	//char position and looking direction - 2D top down
	Vector2 m_lookDir;

	//check to render
	bool m_active;

	int m_moveSpeed;

public:
	Character(void);
	~Character(void);

	virtual void Init(bool active, int health, Vector2 dir, Mesh* mesh, int m_moveSpeed);
	virtual void Update(void);

	// === Set functions ===
		//char variables
	void SetHealth(int health);
	void SetMaxHealth(int health);
	void AddToHealth(int health_plus);

		//character positioning
	void SetLookDir(Vector2 dir);
	void SetMoveSpeed(int moveSpeed);

		//character rendering
	void SetAnimMesh(Mesh* mesh);
	void SetActive(bool active);

	// === Get functions ===
		//char variables
	int GetHealth(void) const;
	int GetMaxHealth(void) const; 

		//character positioning
	Vector2 GetLookDir(void) const;
	int GetMoveSpeed(void) const;

		//character rendering
	Mesh* GetAnimMesh(void) const;
	bool GetActive(void) const;

	// === Actions functions ===
	void Movetowards(Vector2 Dir, double dt);
protected:
	virtual void updateScreenPos(Vector2 pos);
};

#endif