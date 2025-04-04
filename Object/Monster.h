#pragma once

class GameOjbect;

class Monster : public GameObject
{
	using Super = GameObject;
public:
	Monster();
	virtual ~Monster();
};

