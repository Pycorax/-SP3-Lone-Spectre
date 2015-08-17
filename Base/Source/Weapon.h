#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include "Mesh.h"

using std::string;

class Weapon
{
	public:
		enum WEAPON_TYPE
		{
			MELEE_TYPE,
			RANGED_TYPE,
			NUM_WEAPON_TYPE
		};

		Weapon(string name = "Unnamed Weapon", float damage = 0.0f, float range = 0.0f);
		virtual ~Weapon();

		// Setters
		void SetName(string name);
		void SetMesh(Mesh* mesh);
		void SetDamage(float damage);
		void SetRange(float range);
		void SetShotSound(string filePath);

		// Getters
		string GetName(void);
		Mesh* GetMesh(void);
		float GetDamage(void);
		float GetRange(void);
		WEAPON_TYPE GetType(void);
		string GetAttackSound(void);

		// Others
		virtual bool Attack(void) = 0;
		virtual void StartReload(void) = 0;			// Function to initiate the reload
		virtual void Update(double dt) = 0;			// Function to update the reload process

	protected:
		Mesh* m_mesh;
		string m_name;
		float m_fDamage;
		float m_fRange;
		WEAPON_TYPE m_type;
		string m_shotSound;

		virtual void reload(void) = 0;				// Function to do the actual reload
};

#endif