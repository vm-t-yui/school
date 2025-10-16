//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
// エネミー
#include "GameObject.h"
#include "Shot.h"
#include "Enemy.h"
#include "Utility.h"
#include "DxLib.h"
#include "Player.h"

//----------------------------//
// エネミー関数群.
//----------------------------//
// 初期化.
void InitEnemy(Enemy& enemy)
{
	// エネミーのグラフィックをメモリにロード＆表示座標を初期化
	char* enemyGlaphStr = "data/texture/EpicEnemy.png";
	if (enemy.obj.graph < 0)
	{
		enemy.obj.graph = LoadGraph(enemyGlaphStr);
	}
	if (enemy.damageGraph < 0)
	{
		enemy.damageGraph = LoadGraph(enemyGlaphStr);
	}
	GraphFilter(enemy.damageGraph, DX_GRAPH_FILTER_HSB, 0, 0, 0, 256);
	enemy.obj.pos.x = SCREEN_W * 0.5f;
	enemy.obj.pos.y = 50;
	enemy.obj.pos.z = 0;
	enemy.obj.speed = 0;
	enemy.obj.angle = GetRadian(180.0f);
	enemy.life = ENEMY_LIFE;

	// 『ダメージをうけていない』を表すFALSEを代入
	enemy.damageFlag = false;

	// エネミーのグラフィックのサイズを得る
	GetGraphSize(enemy.obj.graph, &enemy.obj.w, &enemy.obj.h);

	// ショットを初期化
	int shotGraph = enemy.shot[0].obj.graph;
	int shotW = enemy.shot[0].obj.w;
	int shotH = enemy.shot[0].obj.h;
	if (shotGraph < 0)
	{
		shotGraph = LoadGraph("data/texture/SuperEnemyShot.png");
		GetGraphSize(shotGraph, &shotW, &shotH);
	}
	for (int i = 0; i < ENEMY_SHOT; i++)
	{
		InitShot(enemy.shot[i], shotGraph, shotW, shotH, GetRadian(180.0f), ENEMY_SHOT_SPEED, ENEMY_SHOT_POWER);
	}
}

// アップデート.
void UpdateEnemy(Enemy& enemy, Player& player)
{
	// エネミーの座標を移動している方向に移動する
	if (enemy.rightMove == true)
	{
		enemy.obj.dir = VGet(1, 0, 0);
	}
	else
	{
		enemy.obj.dir = VGet(-1, 0, 0);
	}
	enemy.obj.speed = ENEMY_SPEED;
	MoveGameObject(enemy.obj);

	// エネミーが画面端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
	if (enemy.obj.pos.x > SCREEN_W - enemy.obj.w)
	{
		enemy.obj.pos.x = (float)(SCREEN_W - enemy.obj.w);
		enemy.rightMove = false;
	}
	else if (enemy.obj.pos.x < 0)
	{
		enemy.obj.pos.x = 0;
		enemy.rightMove = true;
	}

	// 敵の弾の発射処理
	// ショットの発射インターバルがなくなっていたら撃つ
	if (enemy.shotIntervalCount == 0)
	{
		// 画面上にでていない弾があるか、弾の数だけ繰り返して調べる
		for (int i = 0; i < ENEMY_SHOT; i++)
		{
			// 弾iが画面上にでていない場合はその弾を画面に出す
			if (enemy.shot[i].visibleFlag == false)
			{
				// 弾iの位置をセット、位置は敵の中心にする
				enemy.shot[i].obj.pos.x = enemy.obj.pos.x;
				enemy.shot[i].obj.pos.y = enemy.obj.pos.y;

				// 弾iは現時点を持って存在するので、存在状態を保持する変数にtrueを代入する
				enemy.shot[i].visibleFlag = true;

				// 向きを設定
				enemy.shot[i].obj.dir = VGet(0, 1, 0);

				// 一つ弾を出したので弾を出すループから抜けます
				break;
			}
		}

		// インターバル用のカウンターを設定.
		enemy.shotIntervalCount = ENEMY_SHOT_INTERVAL;
	}

	if (enemy.shotIntervalCount != 0)
	{
		--enemy.shotIntervalCount;
	}

	// ショット処理
	for (int i = 0; i < ENEMY_SHOT; i++)
	{
		// ショット移動
		MoveShot(enemy.shot[i]);

		// エネミーのショットとプレイヤーの当たり判定
		if (IsHitShot(enemy.shot[i], player.obj))
		{
			// あたってたらショットパワー分ダメージを与える
			player.life -= enemy.shot[i].power;

			player.damageCounter = 0;
			player.damageFlag = true;

			// 連続で当たらないように抜ける
			break;
		}
	}

	// ダメージを受けているかどうかで処理を分岐
	if (enemy.damageFlag == true)
	{
		enemy.damageCounter++;

		if (enemy.damageCounter == 5)
		{
			// 『ダメージをうけていない』を表すFALSEを代入
			enemy.damageFlag = false;
		}
	}
}
// 描画.
void DrawEnemy(Enemy& enemy)
{
	if (enemy.life > 0)
	{
		// ダメージを受けている場合はダメージ時のグラフィックを描画する
		if (enemy.damageFlag == true)
		{
			DrawGameObject(enemy.obj, enemy.damageGraph);
		}
		else
		{
			DrawGameObject(enemy.obj, enemy.obj.graph);
		}
	}

	// ショットも表示
	for (int i = 0; i < ENEMY_SHOT; i++)
	{
		DrawShot(enemy.shot[i]);
	}
}
