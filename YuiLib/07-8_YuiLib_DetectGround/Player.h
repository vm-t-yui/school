#pragma once

class Physics;
class Collidable;
class YuiLib::WorldSprite;

/// <summary>
/// プレイヤークラス。物理・衝突判定をするため、Collidableを継承
/// </summary>
class Player : public YuiLib::Collidable
{
public:
	Player();
	void Initialize(YuiLib::Physics* physics) override;
	void Finalize(YuiLib::Physics* physics) override;
	void Update(YuiLib::Physics* physics);
	void Draw();

	// 衝突したとき
	void OnCollide(const Collidable& colider) override;

private:
	int		graphicHandle;	// 画像グラフィックハンドル
	float	speed;			// 移動スピード
	bool	isGround;		// 地面にヒットしているかどうか
	YuiLib::WorldSprite* worldSprite;

	static constexpr float JumpPower = 0.25f;
};