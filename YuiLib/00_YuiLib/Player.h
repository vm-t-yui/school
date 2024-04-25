#pragma once

struct GameObject;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:
	void Init();
	void Update();
	void Draw();

private:
	GameObject	obj;
	float		speed;
};