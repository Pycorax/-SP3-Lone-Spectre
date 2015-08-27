#ifndef OBJECTIVE_H
#define OBJECTIVE_H

class Objective
{
public: //Variables

public: //Functions
	Objective();
	virtual ~Objective();

	virtual void Init();
	virtual void Update(double dt);
	virtual void IsCompleted() = 0;
	
	virtual void Activate() = 0;
};

#endif