#pragma once

// ショットの数.
#define SHOT 20

// ショット構造体.
struct Shot
{
	int		Graph;
	int		X;
	int		Y;
	bool	visibleFlag;
	int		W;
	int		H;
};

void InitializeShot(Shot& shot, int shotGraph);							// ショット初期化
void UpdateShot(Shot& shot, Enemy& enemy);								// ショット更新
void DrawShot(Shot& shot);												// ショット描画

// ショット初期化
void InitializeShot(Shot& shot, int shotGraph)
{
	// ショットのグラフィックをメモリにロード.
	shot.Graph = shotGraph;

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	shot.visibleFlag = false;

	// 弾のグラフィックのサイズをえる
	GetGraphSize(shot.Graph, &shot.W, &shot.H);
}

// ショット更新
void UpdateShot(Shot& shot, Enemy& enemy)
{
	// 弾のあたり判定.
	// 弾iが存在している場合のみ次の処理に映る
	if (shot.visibleFlag == 1)
	{
		// エネミーとの当たり判定
		if (((shot.X > enemy.X && shot.X < enemy.X + enemy.W) ||
			(enemy.X > shot.X && enemy.X < shot.X + shot.W)) &&
			((shot.Y > enemy.Y && shot.Y < enemy.Y + enemy.H) ||
				(enemy.Y > shot.Y && enemy.Y < shot.Y + shot.H)))
		{
			// 接触している場合は当たった弾の存在を消す
			shot.visibleFlag = 0;

			// エネミーの顔を歪めているかどうかを保持する変数に『歪めている』を表すTRUEを代入
			enemy.DamageFlag = true;

			// エネミーの顔を歪めている時間を測るカウンタ変数に０を代入
			enemy.DamageCounter = 0;
		}
	}
}

// ショット描画
void DrawShot(Shot& shot)
{
	// 自機の弾iの移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
	if (shot.visibleFlag == true)
	{
		// 弾iを１６ドット上に移動させる
		shot.Y -= 16;

		// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
		if (shot.Y < 0 - shot.H)
		{
			shot.visibleFlag = false;
		}

		// 画面に弾iを描画する
		DrawGraph(shot.X, shot.Y, shot.Graph, FALSE);
	}
}