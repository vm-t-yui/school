#pragma once

class Physics;
class Collidable;

/// <summary>
/// 敵クラス。物理・衝突判定をするため、Collidableを継承
/// </summary>
class SystemWall : public YuiLib::Collidable
{
public:
	SystemWall();
	void Initialize(YuiLib::Physics* physics) override;
	void Update();

	// 衝突したとき
	void OnCollide(const Collidable& colider) override;

private:
};