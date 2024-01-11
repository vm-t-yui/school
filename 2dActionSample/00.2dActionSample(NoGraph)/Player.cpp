#include "DxLib.h"
#include "Screen.h"
#include "Player.h"
#include "Map.h"

/// <summary>
/// プレイヤーの初期化
/// </summary>
void InitPlayer(Player& player)
{
	player.pos = VGet(320.0f, 240.0f, 0);
	player.dir = VGet(0, 0, 0);
	player.w = 20;
	player.h = 60;
	player.fallSpeed = 0.0f;
	player.isGround = false;
}

/// <summary>
/// プレイヤーの更新
/// </summary>
void UpdatePlayer(Player& player, Map& map)
{
	// 入力状態を更新
	// パッド１とキーボードから入力を得る
	auto input = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// プレイヤーの移動処理
	// 左右の移動方向を出す
	player.dir = VGet(0, 0, 0);
	if (input & PAD_INPUT_LEFT)
	{
		player.dir = VAdd(player.dir, VGet(-1, 0, 0));
	}
	if (input & PAD_INPUT_RIGHT)
	{
		player.dir = VAdd(player.dir, VGet(1, 0, 0));
	}

	// 正規化
	if (VSquareSize(player.dir) > 0)
	{
		player.dir = VNorm(player.dir);
	}

	// 移動量を出す
	auto velocity = VScale(player.dir, Speed);

	// 地に足が着いている場合のみジャンプボタン(ボタン１ or Ｚキー)を見る
	if (player.isGround && (input & PAD_INPUT_A))
	{
		player.fallSpeed = -JumpPower;	// ジャンプボタンを押したら即座に上方向の力に代わる
		player.isGround = false;
	}

	// 落下速度を更新
	player.fallSpeed += Gravity;

	// 落下速度を移動量に加える
	auto fallVelocity = VGet(0, player.fallSpeed, 0);	// 落下をベクトルに。y座標しか変化しないので最後にベクトルにする
	velocity = VAdd(velocity, fallVelocity);

	// 当たり判定をして、壁にめり込まないようにvelocityを操作する
	velocity = CheckPlayerHitWithMap(player, velocity);

	// 移動
	player.pos = VAdd(player.pos, velocity);

	// 最後に接地、天井判定を行う
	CheckIsTopHit(player, map);
	CheckIsGround(player, map);
}

/// <summary>
/// 未来のプレイヤー位置とマップの当たり判定を行い、調整したvelocity（移動ベクトル)を返す
/// </summary>
VECTOR CheckPlayerHitWithMap(Player& player, VECTOR velocity)
{
	VECTOR ret = velocity;

	// 当たらなくなるまで繰り返す
	bool loop = true;
	bool isHit = true;
	while (loop)
	{
		loop = false;

		// 未来のプレイヤーのポジションをまず出す
		VECTOR futurePos = VAdd(player.pos, velocity);
		float futurePosLeft = futurePos.x - player.w * 0.5f;
		float futurePosRight = futurePos.x + player.w * 0.5f;
		float futurePosTop = futurePos.y - player.h * 0.5f;
		float futurePosBottom = futurePos.y + player.h * 0.5f;

		// 全マップチップ分繰り返す
		for (int i = 0; i < MapHeight; i++)
		{
			for (int j = 0; j < MapWidth; i++)
			{
				// 当たっているかどうか調べる
				VECTOR mapPos = VGet(static_cast<float>(j * MapChipSize), static_cast<float>(i * MapChipSize), 0);
				float targetLeft = mapPos.x - MapChipSize * 0.5f;
				float targetRight = mapPos.x + MapChipSize * 0.5f;
				float targetTop = mapPos.y - MapChipSize * 0.5f;
				float targetBottom = mapPos.y + MapChipSize * 0.5f;
				// 矩形同士の当たり判定
				if (((futurePosLeft > targetLeft && futurePosLeft < targetRight) ||
					(targetLeft > futurePosLeft && targetLeft < futurePosRight)) &&
					((futurePosTop > targetTop && futurePosTop < targetBottom) ||
						(targetTop > futurePosTop && targetTop < futurePosBottom)))
				{
					isHit = true;
				}
				// 当たった時点でマップチップのループからぬけるが、当たらなくなるまで繰り返すループは継続
				if (isHit)
				{
					// 当たっていたらvelocityを1ドット分縮める
					auto velocitySize = VSize(ret);
					// 仮に今のサイズが1.0fより小さくて当たっているなら、それ以上縮めてもめり込むのループは継続しない
					if (velocitySize <= 1.0f)
					{
						ret = VGet(0, 0, 0);	// 動かなくする
						printfDx("めり込んでますよ\n");	// 想定外なのでエラー出す（許可するならエラー消してよし
					}
					else
					{
						loop = true;
						// いったん長さ1にした後、もともとのサイズ-1の状態に再度伸ばす
						auto normalizedVelocity = VNorm(ret);
						ret = VScale(normalizedVelocity, velocitySize - 1);
					}
					break;
				}
			}
			if (isHit)
			{
				break;
			}
		}
	}

	// MEMO: 超高速で移動してマップ突き抜けた場合は考慮していない
	// MEMO: 「1ドット」と言っているが、1～√2の間の誤差＋float誤差があるが気にしない。ミニマム1なので長さ1減らす。問題出たら対応
	// MEMO: 処理負荷を少しでも減らすために、マップチップとプレイヤーの距離でいったん計算除外するとか、色々するのはアリ

	return ret;
}

/// <summary>
/// 頭上がぶつかっているか見る
/// </summary>
void CheckIsTopHit(Player& player, Map& map)
{
	// 1ドット上の位置がマップチップの中にあるなら頭上にぶつかっている
	if (IsInMapChip(map, VGet(player.pos.x, player.pos.y + 1.0f, player.pos.z)))
	{
		// 以前ぶつかっていないのにぶつかるならfallSpeedをゼロにし、即落下するように
		if (player.isHitTop == false)
		{
			player.isHitTop = true;
			player.fallSpeed = 0.0f;
		}
	}
	else
	{
		player.isHitTop = false;
	}
}

/// <summary>
/// 地面に接地しているか見る
/// </summary>
void CheckIsGround(Player& player, Map& map)
{
	// 1ドット下の位置がマップチップの中にあるなら接地
	if (IsInMapChip(map, VGet(player.pos.x, player.pos.y - 1.0f, player.pos.z)))
	{
		player.isGround = true;
		// fallSpeedをゼロにし、急激な落下を防ぐ
		player.fallSpeed = 0.0f;
	}
}

/// <summary>
/// プレイヤー描画
/// </summary>
void DrawPlayer(Player& player)
{
	// キャラクタの描画
	DrawBox((int)(player.pos.x - player.w * 0.5f), (int)(player.pos.y - player.h * 0.5f),
		(int)(player.pos.x + player.w * 0.5f) + 1, (int)(player.pos.y + player.h * 0.5f) + 1,
		GetColor(255, 0, 0), TRUE);
}