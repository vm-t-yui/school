#pragma once

class Physics;
class Collidable;

/// <summary>
/// 敵クラス。物理・衝突判定をするため、Collidableを継承
/// </summary>
class Enemy : public YuiLib::Collidable
{
public:
	Enemy();
	void Initialize(YuiLib::Physics* physics);
	void Finalize(YuiLib::Physics* physics);
	void Update();
	void Draw();

	// 衝突したとき
	void OnCollide(const Collidable& colider) override;

private:
	int		graphicHandle;	// 画像グラフィックハンドル
	
	// 円の当たり判定型
	YuiLib::ColliderDataCircle2D* circleColliderData;
};