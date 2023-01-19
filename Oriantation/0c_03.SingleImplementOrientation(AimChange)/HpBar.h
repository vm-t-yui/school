#pragma once
class HpBar
{
public:
	void Init(float firstDrawHp);
	void Update();
	// void SetHp(float hp);		// HPの設定？回復や初期化にも使えるの？
	void OnDamage(float afterHp);	// ダメージを受けた時。回復と処理を分けられる。
	void Draw(int ScreenW, float HpMax);

private:
	const int HpBarStartY = 20;
	const int HpBarHight = 20;
	const int HpBarSideSpace = 20;
	const float AimHpSpeed = 0.02f;

	int graph = -1;
	float drawHp = 0;
	float aimHp = 0;
};

