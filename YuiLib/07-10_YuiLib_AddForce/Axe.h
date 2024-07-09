#pragma once

class Physics;
class Collidable;
class YuiLib::WorldSprite;
class Player;

/// <summary>
/// プレイヤーが使う斧。物理・衝突判定をするため、Collidableを継承
/// </summary>
class Axe : public YuiLib::Collidable
{
public:
	Axe();
	void Initialize(YuiLib::Physics* physics) override;
	void Finalize(YuiLib::Physics* physics) override;
	void Update(YuiLib::Physics* physics);
	void Draw();

	// 衝突したとき
	void OnCollide(const Collidable& colider) override;

	// 撃たれたとき
	void OnFire(const Player& player);

	// 死んでるかどうか
	bool IsDead() const { return isDead; };

private:
	static constexpr int	LifeTimeCount = 120;		// 寿命

	// 斧の初回発射力
	static constexpr VECTOR	ShotForce = { 0.05f, 0.2f, 0 };

	int		graphicHandle;	// 画像グラフィックハンドル
	YuiLib::WorldSprite* worldSprite;

	int		lifeCount;
	bool	isDead;
};