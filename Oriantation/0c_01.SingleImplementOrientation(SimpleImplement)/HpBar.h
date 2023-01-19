#pragma once
class HpBar
{
public:
	const int HpMax = 10;
	const int HpBarStartY = 20;
	const int HpBarHight = 20;
	const int HpBarSideSpace = 20;
	const int ScreenW = 640;
	const int ScreenH = 480;

	int hp = HpMax;
	int graph = -1;
	int prevHitKey = false;

	void Init();
	void Update();
	void Draw();
};

