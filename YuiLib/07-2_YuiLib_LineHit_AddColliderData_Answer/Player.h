#pragma once

class Physics;
class Collidable;
class YuiLib::ColliderDataCircle2D;

/// <summary>
/// プレイヤークラス。物理・衝突判定をするため、Collidableを継承
/// </summary>
class Player : public YuiLib::Collidable
{
public:
	Player();
	void Initialize(YuiLib::Physics* physics);
	void Finalize(YuiLib::Physics* physics);
	void Update();
	void Draw();

	// 衝突したとき
	void OnCollide(const Collidable& colider) override;

private:
	int		graphicHandle;	// 画像グラフィックハンドル
	float	speed;			// 移動スピード

	// 円の当たり判定型
	YuiLib::ColliderDataCircle2D* circleColliderData;
};