#pragma once

class Physics;
class Collidable;
class YuiLib::WorldSprite;

/// <summary>
/// 敵クラス。物理・衝突判定をするため、Collidableを継承
/// </summary>
class Enemy : public YuiLib::Collidable
{
public:
	Enemy();
	void Initialize(YuiLib::Physics* physics) override;
	void Finalize(YuiLib::Physics* physics) override;
	void Update(const Player& player);
	void Draw();

	// 衝突したとき
	void OnCollide(const Collidable& colider) override;

private:
	// ノックバックの力
	static constexpr float	KnockbackForce = 0.6f;

	// プレイヤーを追いかける測度
	static constexpr float MoveSpeed = 0.01f;

	// ノックバック中、ノックバックに対するブレーキ
	static constexpr float KnockbackBreakForce = 0.1f;

	int		graphicHandle;	// 画像グラフィックハンドル
	YuiLib::WorldSprite* worldSprite;
	bool	isKnockBacing;	// ノックバック中か
};