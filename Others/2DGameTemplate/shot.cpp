//-----------------------------------------------------------------------------
// @brief  .
// 2016 VikingMaxx Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#include "common.h"
#include "shot.h"
#include "DxLib.h"

//----------------------------//
// ショット関数群.
//----------------------------//
// 初期化.
void InitShot(Shot& shot)
{
	// ショットのグラフィックをメモリにロード.
	int shotGraph;
	shotGraph = LoadGraph("data/texture/SuperShot.png");
	shot.Graph = shotGraph;

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	shot.VisibleFlag = false;

	// 弾のグラフィックのサイズをえる
	int shotW, shotH;
	GetGraphSize(shotGraph, &shotW, &shotH);
	shot.W = shotW;
	shot.H = shotH;
}

// アップデート.
void UpdateShot(Shot& shot, Enemy& enemy)
{
	// 自機の弾iの移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
	if (shot.VisibleFlag == true)
	{
		// 弾iを移動させる
		shot.Y -= SHOT_SPEED;

		// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
		if (shot.Y < 0 - shot.H)
		{
			shot.VisibleFlag = false;
		}
	}

	// 弾のあたり判定.
	// 弾iが存在している場合のみ次の処理に映る
	if (shot.VisibleFlag == 1 && enemy.Life > 0)
	{
		// エネミーとの当たり判定
		if (((shot.X > enemy.X && shot.X < enemy.X + enemy.W) ||
			(enemy.X > shot.X && enemy.X < shot.X + shot.W)) &&
			((shot.Y > enemy.Y && shot.Y < enemy.Y + enemy.H) ||
				(enemy.Y > shot.Y && enemy.Y < shot.Y + shot.H)))
		{
			// 接触している場合は当たった弾の存在を消す
			shot.VisibleFlag = 0;

			// エネミーの顔を歪めているかどうかを保持する変数に『歪めている』を表すTRUEを代入
			enemy.DamageFlag = true;

			// エネミーの顔を歪めている時間を測るカウンタ変数に０を代入
			enemy.DamageCounter = 0;

			enemy.Life -= 1;
		}
	}
}
// 描画.
void DrawShot(Shot& shot)
{
	if (shot.VisibleFlag == true)
	{
		// 画面に弾iを描画する
		DrawGraph(shot.X, shot.Y, shot.Graph, TRUE);
	}
}
