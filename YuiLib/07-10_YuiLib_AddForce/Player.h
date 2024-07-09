#pragma once

class Physics;
class Collidable;
class YuiLib::WorldSprite;
class Axe;

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

	// 右を向いているか
	bool IsRightFace() const { return isFaceRight; }

private:
	int		graphicHandle;	// 画像グラフィックハンドル
	float	speed;			// 移動スピード
	bool	isGround;		// 地面にヒットしているかどうか
	bool	isFaceRight;	// 右を向いているかのフラグ
	
	YuiLib::WorldSprite*	worldSprite;
	Axe*					axe;

	static constexpr float JumpPower = 0.25f;
};