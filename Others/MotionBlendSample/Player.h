#pragma once

/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:
	Player();
	void Update();
	void Draw();

	enum class AnimKind : int
	{
		Idle = 0,
		Run = 1,
		Num = 2
	};

private:
	void SetAnimation(AnimKind kind, int animIndex);
	int modelHandle;
	int animHandle[(int)AnimKind::Num];
	float currentAnimTotalTime;
	int currentAnimAttachIndex;
	float currentAnimTime;
	AnimKind currentAnim;
};