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

	// 衝突したとき
	void OnCollide() override;

private:
	//GameObject	obj;
	//float		speed;
};