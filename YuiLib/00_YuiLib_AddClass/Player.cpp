// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "Screen.h"
#include "Rigidbody.h"
#include "YuiLib.h"
//#include "GameObject.h"
#include "Player.h"

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize(YuiLib::Physics* physics)
{
	physics->Entry(this);	// 物理情報に自身を登録

	//// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	//obj.graph = LoadGraph("data/texture/player.png");
	//obj.pos.x = 288;
	//obj.pos.y = 400;
	//speed = 10;

	//// プレイヤーの画像のサイズを得る
	//int w, h;
	//GetGraphSize(obj.graph, &w, &h);
	//obj.w = (float)w;
	//obj.h = (float)h;

	//obj.dir.x = 0;
	//obj.dir.y = 0;
}

/// <summary>
/// 終了
/// </summary>
void Player::Finalize(YuiLib::Physics* physics)
{
	physics->Exit(this);	// 物理情報登録解除
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
	//// 矢印キーを押していたらプレイヤーを移動させる
	//// 向きを決定する.
	//obj.dir.x = 0;
	//obj.dir.y = 0;
	//if (CheckHitKey(KEY_INPUT_UP) == 1)
	//{
	//	obj.dir.y = -1;
	//}
	//if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	//{
	//	obj.dir.y = 1;
	//}
	//if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	//{
	//	obj.dir.x = -1;
	//}
	//if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	//{
	//	obj.dir.x = 1;
	//}

	//// 余計な数値が入るので、純粋に長さ１の向きとして正規化する
	//if (VSquareSize(obj.dir) > 0)
	//{
	//	obj.dir = VNorm(obj.dir);
	//}

	//// 向きにスピードをかけて、移動力に変更する
	//VECTOR velocity = VScale(obj.dir, speed);
	//
	//// ポジションに移動力を足す.
	//obj.pos = VAdd(obj.pos, velocity);


	//// プレイヤーが画面端からはみ出そうになっていたら画面内の座標に戻してあげる
	//if (obj.pos.x < 0)
	//{
	//	obj.pos.x = 0;
	//}
	//if (obj.pos.x > SCREEN_WIDTH - obj.w)
	//{
	//	obj.pos.x = SCREEN_WIDTH - obj.w;
	//}
	//if (obj.pos.y < 0)
	//{
	//	obj.pos.y = 0;
	//}
	//if (obj.pos.y > SCREEN_HEIGHT - obj.h)
	//{
	//	obj.pos.y = SCREEN_HEIGHT - obj.h;
	//}
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	//DrawGraph((int)obj.pos.x, (int)obj.pos.y, obj.graph, TRUE);
}

/// <summary>
/// 衝突したとき
/// </summary>
void Player::OnCollide()
{
	printfDx("プレイヤー当たった！");
}

