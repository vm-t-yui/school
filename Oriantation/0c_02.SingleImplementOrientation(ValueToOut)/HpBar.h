#pragma once
class HpBar
{
public:
	const int HpBarStartY = 20;
	const int HpBarHight = 20;
	const int HpBarSideSpace = 20;

	int graph = -1;

	void Init();
	void Update();
	void Draw(int hp, int ScreenW, int HpMax);
};

