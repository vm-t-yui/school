#pragma once

class Physics;
class Collidable;

/// <summary>
/// 足場
/// </summary>
class StepGround : public YuiLib::Collidable
{
public:
	StepGround(const VECTOR& start, const VECTOR& end);
	void Initialize(YuiLib::Physics* physics) override;
	void Finalize(YuiLib::Physics* physics) override;
	void Update();
	void Draw();

	// 衝突したとき
	void OnCollide(const Collidable& colider) override;

private:
	int sourceModelHandle;
	std::vector<int> modelHandles;
};