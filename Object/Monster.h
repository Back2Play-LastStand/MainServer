#pragma once

class GameOjbect;

class Monster : public GameObject
{
	using Super = GameObject;
public:
	Monster();
	virtual ~Monster();

	virtual void BeginPlay() override;
	virtual void Tick() override;
};

