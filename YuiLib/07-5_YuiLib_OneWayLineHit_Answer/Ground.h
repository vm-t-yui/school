#pragma once

class Physics;
class Collidable;

/// <summary>
/// システム壁
/// </summary>
class Ground : public YuiLib::Collidable
{
public:
	Ground();
	void Initialize(YuiLib::Physics* physics) override;
	void Update();

	// 衝突したとき
	void OnCollide(const Collidable& colider) override;

private:
};