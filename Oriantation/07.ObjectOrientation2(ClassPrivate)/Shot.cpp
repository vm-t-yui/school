//-----------------------------------------------------------------------------
// @brief  ショット処理.
//-----------------------------------------------------------------------------
#include "Common.h"
#include "DxLib.h"
#include "Shot.h"
#include "Enemy.h"

//----------------------------//
// ショット関数群.
//----------------------------//
// 初期化.
void Shot::Init()
{
	// ショットのグラフィックをメモリにロード.
	Graph = LoadGraph("data/texture/SuperShot.png");

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	isVisible = false;

	// 弾のグラフィックのサイズをえる
	GetGraphSize(Graph, &W, &H);
}

// アップデート.
void Shot::Update(Enemy& enemy)
{
	// 自機の弾iの移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
	if (isVisible == true)
	{
		// 弾iを移動させる
		Y -= SHOT_SPEED;

		// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
		if (Y < 0 - H)
		{
			isVisible = false;
		}
	}

	// 弾のあたり判定.
	// 弾iが存在している場合のみ次の処理に映る
	if (isVisible == 1 && enemy.GetLife() > 0)
	{
		// エネミーとの当たり判定
		if (((X > enemy.GetX() && X < enemy.GetX() + enemy.GetW()) ||
			(enemy.GetX() > X && enemy.GetX() < X + W)) &&
			((Y > enemy.GetY() && Y < enemy.GetY() + enemy.GetH()) ||
				(enemy.GetY() > Y && enemy.GetY() < Y + H)))
		{
			// 接触している場合は当たった弾の存在を消す
			isVisible = 0;

			enemy.OnDamage();
		}
	}
}
// 描画.
void Shot::Draw()
{
	if (isVisible == true)
	{
		// 画面に弾iを描画する
		DrawGraph(X, Y, Graph, TRUE);
	}
}

/// <summary>
/// 弾が撃たれたとき
/// </summary>
void Shot::OnShoot(int shooterX, int shooterY, int shooterW, int shooterH)
{
	// HACK:引数を(int startX, int startY)とし、(shooterW - W) / 2 + shooterXの計算を外でやるほうがスマート

	// 弾iの位置をセット、位置は弾を撃ったやつの中心にする
	X = (shooterW - W) / 2 + shooterX;
	Y = (shooterH - H) / 2 + shooterY;

	// 弾iは現時点を持って存在するので、存在状態を保持する変数にtrueを代入する
	isVisible = true;
}

