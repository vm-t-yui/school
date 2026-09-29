//-----------------------------------------------------------------------------
// 2024 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include <math.h>
#include "DxLib.h"
#include "Screen.h"
#include "Camera.h"
#include "Player.h"
#include "Map.h"

/// <summary>
/// プレイヤーの初期化
/// </summary>
void InitPlayer(Player& player)
{
	// MEMO: float誤差をfloorで切り捨ててる関係上、偶数じゃないとズレるので注意
	player.w = 30;
	player.h = 60;

	player.pos = VGet(32.0f + player.h * 0.5f, 100, 0);
	player.dir = VGet(0, 0, 0);
	player.fallSpeed = 0.0f;
	player.isGround = false;
	player.isHitTop = false;
}

/// <summary>
/// プレイヤーの更新
/// </summary>
void UpdatePlayer(Player& player, const Map& map)
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

	// 落下速度を更新
	player.fallSpeed += Gravity;

	// HACK: 先に設定判定をすることでfallSpeed修正＋接地フラグ更新
	CheckIsGround(player, map);		
	CheckIsTopHit(player, map);

	// 地に足が着いている場合のみジャンプボタン(ボタン１ or Ｚキー)を見る
	if (player.isGround && !player.isHitTop && (input & PAD_INPUT_A))
	{
		player.fallSpeed = -JumpPower;	// ジャンプボタンを押したら即座に上方向の力に代わる
		player.isGround = false;
	}

	// 落下速度を移動量に加える
	auto fallVelocity = VGet(0, player.fallSpeed, 0);	// 落下をベクトルに。y座標しか変化しないので最後にベクトルにする
	velocity = VAdd(velocity, fallVelocity);

	// 当たり判定をして、壁にめり込まないようにvelocityを操作する
	velocity = CheckPlayerHitWithMap(player, map, velocity);

	// 移動
	player.pos = VAdd(player.pos, velocity);
}

/// <summary>
/// 未来のプレイヤー位置とマップの当たり判定を行い、調整したvelocity（移動ベクトル)を返す
/// </summary>
VECTOR CheckPlayerHitWithMap(Player& player, const Map& map, const VECTOR& velocity)
{
	// サイズが最初から0なら動かさず早期return
	if (VSize(velocity) == 0)
	{
		return velocity;
	}

	VECTOR ret = velocity;

	// 当たらなくなるまで繰り返す
	bool loop = true;
	bool isFirstHit = true;	// 初回で当たったか
	while (loop)
	{
		loop = false;

		// 未来のプレイヤーのポジションをまず出す
		VECTOR futurePos = VAdd(player.pos, ret);

		// 全マップチップ分繰り返す
		for (int wIndex = 0; wIndex < MapWidth; wIndex++)
		{
			bool isHit = false;
			for (int hIndex = 0; hIndex < MapHeight; hIndex++)
			{
				isHit = IsHitPlayerWithMapChip(player, futurePos, map.mapChips[wIndex][hIndex]);

				// 初回に当たったとき
				if (isHit && isFirstHit)
				{
					// 今後当たり判定でポジションやvelocityの補正をするとき、小数点以下の誤差が産まれる
					// 雑に1ドットずつ減らす、数学計算をしないマッシブ当たり判定には邪魔なので初回に丸めてしまい、
					// 以降改めて当たり判定
					// posもVelocityも丸める
					player.pos.x = floorf(player.pos.x);
					player.pos.y = floorf(player.pos.y);
					ret.x = floorf(ret.x);
					ret.y = floorf(ret.y);
					isFirstHit = false;
					loop = true;	// ループ継続
				}

				// 当たった時点でマップチップのループからぬけるが、当たらなくなるまで繰り返すループは継続
				if (isHit && !isFirstHit)
				{
					// HACK: 当たっていた場合にvelocityを1ドットずつ縮めていき、当たらなくなるまで繰り返す
					// そのまま縮めてしまうと、斜めのベクトルのとき（例えば壁に向かってジャンプしたとき）にジャンプの勢いも縮めてしまう
					// これを防ぐために、
					// 横成分から縮めていくことで、問題を回避する
					float absX = fabsf(ret.x);	// velocityのx成分の絶対値
					float absY = fabsf(ret.y);	// velocityのy成分の絶対値

					// x成分を縮め切っていなければx成分を縮める
					bool shrinkX = (absX != 0.0f);	// x成分を縮めるかどうか

					if (shrinkX)
					{
						if (ret.x > 0.0f)
						{
							ret.x -= 1.0f;
						}
						else
						{
							ret.x += 1.0f;
						}

						// 縮め切ったら消す
						if (fabs(ret.x) < 1.0f)
						{
							ret.x = 0.0f;
						}
						loop = true;
					}
					else
					{
						if (ret.y > 0.0f)
						{
							ret.y -= 1.0f;
						}
						else
						{
							ret.y += 1.0f;
						}

						// 縮め切ったら消す
						if (fabs(ret.y) < 1.0f)
						{
							ret.y = 0.0f;
						}
						loop = true;
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
	// MEMO: 処理負荷を少しでも減らすために、マップチップとプレイヤーの距離でいったん計算除外するとか、色々するのはアリ

	return ret;
}

/// <summary>
/// マップチップと未来のプレイヤーポジションの当たり判定
/// </summary>
bool IsHitPlayerWithMapChip(const Player& player, const VECTOR& futurePos, const MapChip& mapChip)
{
	// マップチップが当たらない種類なら早期return
	if(mapChip.chipKind == 0)
	{
		return false;
	}
	
	// 当たっているかどうか調べる
	float futurePosLeft = futurePos.x - player.w * 0.5f;
	float futurePosRight = futurePos.x + player.w * 0.5f;
	float futurePosTop = futurePos.y - player.h * 0.5f;
	float futurePosBottom = futurePos.y + player.h * 0.5f;
	float targetLeft = mapChip.pos.x - mapChip.w * 0.5f;
	float targetRight = mapChip.pos.x + mapChip.w * 0.5f;
	float targetTop = mapChip.pos.y - mapChip.h * 0.5f;
	float targetBottom = mapChip.pos.y + mapChip.h * 0.5f;
	// 矩形同士の当たり判定
	if (((targetLeft <= futurePosLeft && futurePosLeft < targetRight) ||
		(targetLeft > futurePosLeft && targetLeft < futurePosRight)) &&
		((targetTop <= futurePosTop && futurePosTop < targetBottom) ||
			(targetTop > futurePosTop && targetTop < futurePosBottom)))
	{
		return true;
	}
	return false;
}

/// <summary>
/// 頭上がぶつかっているか見る
/// </summary>
void CheckIsTopHit(Player& player, const Map& map)
{
	// 1ドット上にずらして当たれば頭上がぶつかっている （小数点無視）
	auto checkPos = VGet(player.pos.x, floorf(player.pos.y) - 1.0f, player.pos.z);
	// 全マップチップ分繰り返す
	bool isHit = false;
	for (int wIndex = 0; wIndex < MapWidth; wIndex++)
	{
		for (int hIndex = 0; hIndex < MapHeight; hIndex++)
		{
			isHit = IsHitPlayerWithMapChip(player, checkPos, map.mapChips[wIndex][hIndex]);
			if(isHit)
			{
				break;
			}
		}
		if (isHit)
		{
			break;
		}
	}
	if (isHit)
	{
		// 以前ぶつかっていないのにぶつかるならfallSpeedをゼロにし、即落下するように
		if (!player.isHitTop)
		{
			player.isHitTop = true;
			player.fallSpeed = 0.0f;

			// 後々の雑計算に響くので、y座標の小数点を消し飛ばす
			player.pos.y = floorf(player.pos.y);
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
void CheckIsGround(Player& player, const Map& map)
{
	// 1ドット下にずらして当たれば地面に足がぶつかっている （小数点無視）
	auto checkPos = VGet(player.pos.x, floorf(player.pos.y) + 1.0f, player.pos.z);
	// 全マップチップ分繰り返す
	bool isHit = false;
	for (int wIndex = 0; wIndex < MapWidth; wIndex++)
	{
		for (int hIndex = 0; hIndex < MapHeight; hIndex++)
		{
			isHit = IsHitPlayerWithMapChip(player, checkPos, map.mapChips[wIndex][hIndex]);
			if (isHit)
			{
				break;
			}
		}
		if (isHit)
		{
			break;
		}
	}
	if (isHit)
	{
		player.isGround = true;
		// fallSpeedをゼロにし、急激な落下を防ぐ
		player.fallSpeed = 0.0f;

		// 後々の雑計算に響くので、y座標の小数点を消し飛ばす
		player.pos.y = floorf(player.pos.y);	// ちょうど地面に付く位置に
	}
	else
	{
		player.isGround = false;
	}
}

/// <summary>
/// プレイヤー描画
/// </summary>
void DrawPlayer(const Player& player, const Camera& camera)
{
	// キャラクタの描画
	auto leftTop = static_cast<int>(player.pos.x - player.w * 0.5f);
	auto leftBottom = static_cast<int>(player.pos.y - player.h * 0.5f);
	auto rightTop = static_cast<int>(player.pos.x + player.w * 0.5f);
	auto rightBottom = static_cast<int>(player.pos.y + player.h * 0.5f);
	DrawBox(
		leftTop + static_cast<int>(camera.drawOffset.x),
		leftBottom + static_cast<int>(camera.drawOffset.y),
		rightTop + static_cast<int>(camera.drawOffset.x),
		rightBottom + static_cast<int>(camera.drawOffset.y),
		GetColor(255, 0, 0), TRUE);
}