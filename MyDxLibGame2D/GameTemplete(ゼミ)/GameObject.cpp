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
