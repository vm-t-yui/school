﻿#pragma once

class Physics;
class Collidable;

/// <summary>
/// システム壁
/// </summary>
class SystemWall : public YuiLib::Collidable
{
public:
	SystemWall(const VECTOR& start, const VECTOR& end);
	void Initialize(YuiLib::Physics* physics) override;
	void Update();

	// 衝突したとき
	void OnCollide(const Collidable& colider) override;

private:
};