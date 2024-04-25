#pragma once

//struct GameObject;
class Physics;
class Collidable;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public YuiLib::Collidable
{
public:
	void Initialize(YuiLib::Physics* physics);
	void Finalize(YuiLib::Physics* physics);
	void Update();
	void Draw();

private:
	//GameObject	obj;
	//float		speed;
};