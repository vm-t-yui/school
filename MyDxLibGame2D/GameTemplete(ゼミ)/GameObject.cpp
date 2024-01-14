//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
// ゲームオブジェクト
#include "GameObject.h"

// ゲームオブジェクト移動
void MoveGameObject(GameObject& obj)
{
	VECTOR velocity = VScale(obj.dir, obj.speed);
	obj.pos = VAdd(obj.pos, velocity);
}

// ゲームオブジェクト描画。違う画像も描画できるようにgraphだけ分けている
void DrawGameObject(GameObject& obj, int graph)
{
	DrawRotaGraph((int)obj.pos.x, (int)obj.pos.y,
		obj.scale,
		obj.angle,
		graph, TRUE);
}


// 当たり判定
bool IsHit(GameObject& target1, GameObject& target2)
{
	bool isHit = false;
	float target1Left = target1.pos.x - target1.w * 0.5f;
	float target1Right = target1.pos.x + target1.w * 0.5f;
	float target1Top = target1.pos.y - target1.h * 0.5f;
	float target1Bottom = target1.pos.y + target1.h * 0.5f;
	float target2Left = target2.pos.x - target2.w * 0.5f;
	float target2Right = target2.pos.x + target2.w * 0.5f;
	float target2Top = target2.pos.y - target2.h * 0.5f;
	float target2Bottom = target2.pos.y + target2.h * 0.5f;

	if (((target2Left <= target1Left && target1Left < target2Right) ||
		(target2Left > target1Left && target2Left < target1Right)) &&
		((target2Top <= target1Top && target1Top < target2Bottom) ||
			(target2Top > target1Top && target2Top < target1Bottom)))
	{
		// あたってるフラグをたてる
		isHit = true;
	}
	return isHit;
}