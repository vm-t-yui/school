// プレイヤー
#include "GameObject.h"
#include "Shot.h"
#include "Enemy.h"
#include "Player.h"
#include "Utility.h"
#include "DxLib.h"

//----------------------------//
// プレイヤー関数群.
//----------------------------//
// 初期化.
void InitPlayer(Player& player)
{
	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	if (player.obj.graph < 0)
	{
		player.obj.graph = LoadGraph("data/texture/EpicPlayer.png");
	}
	player.obj.pos.x = SCREEN_W / 2;			// 画面の半分（中央）
	player.obj.pos.y = SCREEN_H - 100;		// 画面下-100の位置
	player.obj.pos.z = 0;
	player.obj.speed = 0.0f;
	player.life = PLAYER_LIFE;

	// 『ダメージをうけていない』を表すFALSEを代入
	player.damageFlag = false;

	// プレイヤーの画像のサイズを得る
	GetGraphSize(player.obj.graph, &player.obj.w, &player.obj.h);

	// ショットを初期化
	int shotGraph = player.shot[0].obj.graph;
	int shotW = player.shot[0].obj.w;
	int shotH = player.shot[0].obj.h;
	if (shotGraph < 0)
	{
		shotGraph = LoadGraph("data/texture/SuperShot.png");
		GetGraphSize(shotGraph, &player.obj.w, &player.obj.h);
	}
	for (int i = 0; i < PLAYER_SHOT; i++)
	{
		InitShot(player.shot[i], shotGraph, shotW, shotH, 0, PLAYER_SHOT_SPEED, PLAYER_SHOT_POWER);
	}
}

// アップデート.
void UpdatePlayer(Player& player, Enemy& enemy)
{
	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		player.obj.dir = VGet(-1, 0, 0);
		player.obj.speed = PLAYER_SPEED;
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		player.obj.dir = VGet(1, 0, 0);
		player.obj.speed = PLAYER_SPEED;
	}
	else
	{
		player.obj.speed = 0.0f;
	}
	MoveGameObject(player.obj);

	// 弾の発射処理
	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		// ショットの発射インターバルがなくなっていたら撃てる
		if (player.shotIntervalCount == 0)
		{
			// 画面上にでていない弾があるか、弾の数だけ繰り返して調べる
			for (int i = 0; i < PLAYER_SHOT; i++)
			{
				// 弾iが画面上にでていない場合はその弾を画面に出す
				if (player.shot[i].visibleFlag == false)
				{
					// 弾iの位置をセット、位置はプレイヤーの中心にする
					player.shot[i].obj.pos.x = (player.obj.w - player.shot[i].obj.w) / 2 + player.obj.pos.x;
					player.shot[i].obj.pos.y = (player.obj.h - player.shot[i].obj.h) / 2 + player.obj.pos.y;

					// 弾iは現時点を持って存在するので、存在状態を保持する変数にtrueを代入する
					player.shot[i].visibleFlag = true;

					// 向きを設定
					player.shot[i].obj.dir = VGet(0, -1, 0);

					// 一つ弾を出したので弾を出すループから抜けます
					break;
				}
			}

			// インターバル用のカウンターを設定.
			player.shotIntervalCount = PLAYER_SHOT_INTERVAL;
		}
	}

	// ショット関連
	for (int i = 0; i < PLAYER_SHOT; i++)
	{
		// ショット移動
		MoveShot(player.shot[i]);

		// プレイヤーのショットと敵の当たり判定
		if (IsHitShot(player.shot[i], enemy.obj))
		{
			// あたってたらショットパワー分ダメージを与える
			enemy.life -= player.shot[i].power;

			enemy.damageCounter = 0;
			enemy.damageFlag = true;

			// 連続で当たらないように抜ける
			break;
		}
	}

	if (player.shotIntervalCount != 0)
	{
		--player.shotIntervalCount;
	}

	// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
	if (player.obj.pos.x < player.obj.w * 0.5f)
	{
		player.obj.pos.x = player.obj.w * 0.5f;
	}
	if (player.obj.pos.x > SCREEN_W - player.obj.w * 0.5f)
	{
		player.obj.pos.x = (float)(SCREEN_W - player.obj.w * 0.5f);
	}
	if (player.obj.pos.y < 0)
	{
		player.obj.pos.y = 0;
	}
	if (player.obj.pos.y > SCREEN_H - player.obj.w * 0.5f)
	{
		player.obj.pos.y = (float)(SCREEN_H - player.obj.h * 0.5f);
	}

	// ダメージを受けているかどうかで処理を分岐
	if (player.damageFlag == true)
	{
		player.damageCounter++;

		if (player.damageCounter == 5)
		{
			// 『ダメージをうけていない』を表すFALSEを代入
			player.damageFlag = false;
		}
	}
}

// 描画.
void DrawPlayer(Player& player)
{
	if (player.damageFlag == true)
	{
		// ダメージを受けている場合は透明＝表示しない
	}
	else
	{
		DrawGameObject(player.obj, player.obj.graph);
	}

	// ショットも表示
	for (int i = 0; i < PLAYER_SHOT; i++)
	{
		DrawShot(player.shot[i]);
	}
}
