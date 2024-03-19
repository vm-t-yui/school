// 2023 Takeru Yui All Rights Reserved.
#include <math.h>
#include <vector>
#include "DxLib.h"
#include "Map.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
	:	modelHandle		(-1)
		, pos			(VGet(10, 0, 0))
		, velocity		(VGet(0, 0, 0))
		, dir			(VGet(0, 0, 1))
		, w				(30)
		, h				(60)
		, fallSpeed		(0.0f)
		, isGround		(false)
		, isHitTop		(false)
{
	// ３Ｄモデルの読み込み
	modelHandle = MV1LoadModel("data/Robot.mv1");
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	// モデルのアンロード.
	MV1DeleteModel(modelHandle);
}

/// <summary>
/// 更新
/// </summary>
void Player::Update(const Map& map)
{
	// キー入力取得
	int input = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 単純に方向転換
	dir = VGet(0, 0, 0);
	if (input & PAD_INPUT_RIGHT)
	{
		dir = VAdd(dir, VGet(1, 0, 0));
	}
	else if (input & PAD_INPUT_LEFT)
	{
		dir = VAdd(dir, VGet(-1, 0, 0));
	}

	// ゼロ除算避け
	if (VSquareSize(dir) > 0)
	{
		// 正規化
		dir = VNorm(dir);
	}

	// 移動量を出す
	auto velocity = VScale(dir, Speed);

	// 落下速度を更新
	fallSpeed += Gravity;

	// HACK: 先に設定判定をすることでfallSpeed修正＋接地フラグ更新
	CheckIsGround(map);
	CheckIsTopHit(map);

	// 地に足が着いている場合のみジャンプボタン(ボタン１ or Ｚキー)を見る
	if (isGround && !isHitTop && (input & PAD_INPUT_A))
	{
		fallSpeed = -JumpPower;	// ジャンプボタンを押したら即座に上方向の力に代わる
		isGround = false;
	}

	// 落下速度を移動量に加える
	auto fallVelocity = VGet(0, fallSpeed, 0);	// 落下をベクトルに。y座標しか変化しないので最後にベクトルにする
	velocity = VAdd(velocity, fallVelocity);

	// 当たり判定をして、壁にめり込まないようにvelocityを操作する
	velocity = CheckHitWithMap(map, velocity);

	// 移動
	pos = VAdd(pos, velocity);

	// 力をかけ終わったベロシティの方向にディレクションを調整.
	if (VSize(velocity) != 0)
	{
		dir = VNorm(velocity);
	}
	//printfDx("%f %f %f\n", dir.x, dir.y, dir.z);

	// 3Dモデルのスケール決定
	MV1SetScale(modelHandle, VGet(Scale, Scale, Scale));

	// ３Dモデルのポジション設定
	MV1SetPosition(modelHandle, pos);

	// 回転
	MV1SetRotationXYZ(modelHandle, VGet(0.0f, -90.0f, 0.0f));
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	// ３Ｄモデルの描画
	MV1DrawModel(modelHandle);
}


/// <summary>
/// 未来のプレイヤー位置とマップの当たり判定を行い、調整したvelocity（移動ベクトル)を返す
/// </summary>
VECTOR Player::CheckHitWithMap(const Map& map, const VECTOR& velocity)
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
		VECTOR futurePos = VAdd(pos, ret);

		// 全マップチップ分繰り返す
		for (int wIndex = 0; wIndex < map.GetMapColNum(); wIndex++)
		{
			bool isHit = false;
			for (int hIndex = 0; hIndex < map.GetMapRowNum(); hIndex++)
			{
				isHit = IsHitWithMapChip(futurePos, map.GetChip(wIndex, hIndex));

				// 初回に当たったとき
				if (isHit && isFirstHit)
				{
					// 今後当たり判定でポジションやvelocityの補正をするとき、小数点以下の誤差が産まれる
					// 雑に1ドットずつ減らす、数学計算をしないマッシブ当たり判定には邪魔なので初回に丸めてしまい、
					// 以降改めて当たり判定
					// posもVelocityも丸める
					pos.x = floorf(pos.x);
					pos.y = floorf(pos.y);
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
bool Player::IsHitWithMapChip(const VECTOR& futurePos, const Map::Chip& mapChip)
{
	// マップチップが当たらない種類なら早期return
	if (mapChip.chipKind == 0)
	{
		return false;
	}

	// 当たっているかどうか調べる
	float futurePosLeft = futurePos.x - w * 0.5f;
	float futurePosRight = futurePos.x + w * 0.5f;
	float futurePosTop = futurePos.y - h * 0.5f;
	float futurePosBottom = futurePos.y + h * 0.5f;
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
void Player::CheckIsTopHit(const Map& map)
{
	// 1ドット上にずらして当たれば頭上がぶつかっている （小数点無視）
	auto checkPos = VGet(pos.x, floorf(pos.y) - 1.0f, pos.z);
	// 全マップチップ分繰り返す
	bool isHit = false;
	for (int wIndex = 0; wIndex < map.GetMapColNum(); wIndex++)
	{
		for (int hIndex = 0; hIndex < map.GetMapRowNum(); hIndex++)
		{
			isHit = IsHitWithMapChip(checkPos, map.GetChip(wIndex, hIndex));
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
		// 以前ぶつかっていないのにぶつかるならfallSpeedをゼロにし、即落下するように
		if (!isHitTop)
		{
			isHitTop = true;
			fallSpeed = 0.0f;

			// 後々の雑計算に響くので、y座標の小数点を消し飛ばす
			pos.y = floorf(pos.y);
		}
	}
	else
	{
		isHitTop = false;
	}
}

/// <summary>
/// 地面に接地しているか見る
/// </summary>
void Player::CheckIsGround(const Map& map)
{
	// 1ドット下にずらして当たれば地面に足がぶつかっている （小数点無視）
	auto checkPos = VGet(pos.x, floorf(pos.y) + 1.0f, pos.z);
	// 全マップチップ分繰り返す
	bool isHit = false;
	for (int wIndex = 0; wIndex < map.GetMapColNum(); wIndex++)
	{
		for (int hIndex = 0; hIndex < map.GetMapRowNum(); hIndex++)
		{
			isHit = IsHitWithMapChip(checkPos, map.GetChip(wIndex, hIndex));
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
		isGround = true;
		// fallSpeedをゼロにし、急激な落下を防ぐ
		fallSpeed = 0.0f;

		// 後々の雑計算に響くので、y座標の小数点を消し飛ばす
		pos.y = floorf(pos.y);	// ちょうど地面に付く位置に
	}
	else
	{
		isGround = false;
	}
}