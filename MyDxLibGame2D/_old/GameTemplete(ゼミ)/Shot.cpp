//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
// ショット
#include "GameObject.h"
#include "Shot.h"
#include "Utility.h"

//----------------------------//
// ショット関数群.
//----------------------------//
// 初期化.
void InitShot(Shot& shot, int shotGraph, int shotW, int shotH, float angle, float speed, int power)
{
	// ショットのグラフィックをメモリにロード.
	shot.obj.graph = shotGraph;

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	shot.visibleFlag = false;
	shot.obj.w = shotW;
	shot.obj.h = shotH;
	shot.obj.angle = angle;
	shot.obj.speed = speed;
	shot.power = power;
}

// ショットの移動.
void MoveShot(Shot& shot)
{
	// 弾の移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
	if (shot.visibleFlag == true)
	{
		// 弾を移動させる
		MoveGameObject(shot.obj);

		// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
		if (shot.obj.pos.y < 0 - shot.obj.h * 0.5f
			|| shot.obj.pos.y > SCREEN_H + shot.obj.h * 0.5f
			|| shot.obj.pos.x < 0 - shot.obj.w * 0.5f
			|| shot.obj.pos.x > SCREEN_W + shot.obj.w * 0.5f)
		{
			shot.visibleFlag = false;
		}
	}
}
// ショットの当たり判定チェック
bool IsHitShot(Shot& shot, GameObject& target)
{
	bool isHit = false;
	// 弾のあたり判定.
	if (shot.visibleFlag == 1)
	{
		if (IsHit(shot.obj, target))
		{
			// 接触している場合は当たった弾の存在を消す
			shot.visibleFlag = 0;

			// あたってるフラグをたてる
			isHit = true;
		}
	}
	return isHit;
}

// 描画.
void DrawShot(Shot& shot)
{
	if (shot.visibleFlag == true)
	{
		// 画面に弾iを描画する
		DrawGameObject(shot.obj, shot.obj.graph);
	}
}
