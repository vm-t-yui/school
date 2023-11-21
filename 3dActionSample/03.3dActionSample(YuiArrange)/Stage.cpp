#include "DxLib.h"
#include "Stage.h"
#include "Player.h"

/// <summary>
/// 初期化
/// </summary>
void Stage::Initialize()
{
	// ステージモデルの読み込み
	ModelHandle = MV1LoadModel("ColTestStage.mqo");

	// モデル全体のコリジョン情報のセットアップ
	MV1SetupCollInfo(ModelHandle, -1);

	isCreatedHitDim = false;
}

/// <summary>
/// 終了
/// </summary>
void Stage::Finalie()
{
	// ステージモデルの後始末
	MV1DeleteModel(ModelHandle);
	if (isCreatedHitDim)
	{
		MV1CollResultPolyDimTerminate(HitDim);
		isCreatedHitDim = false;
	}
}

/// <summary>
/// 当たり判定をして、補正した移動先のポジションを返す
/// </summary>
VECTOR Stage::CheckCollision(Player& player, const VECTOR& CheckPosition, const VECTOR& MoveVector)
{
	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	if (isCreatedHitDim)
	{
		MV1CollResultPolyDimTerminate(HitDim);
		isCreatedHitDim = false;
	}

	VECTOR FixedPos = CheckPosition;

	// プレイヤーの周囲にあるステージポリゴンを取得する
	// ( 検出する範囲は移動距離も考慮する )
	HitDim = MV1CollCheck_Sphere(ModelHandle, -1, CheckPosition, DefaultSize + VSize(MoveVector));

	// 検出されたポリゴンが壁ポリゴン( ＸＺ平面に垂直なポリゴン )か床ポリゴン( ＸＺ平面に垂直ではないポリゴン )かを判断し、保存する
	AnalyzeWallAndFloor(CheckPosition);

	// 壁ポリゴンとの当たり判定処理
	// 壁ポリゴンとの当たりをチェックし、移動ベクトルを補正する
	FixedPos = CheckHitWithWall(player, FixedPos);

	// 床ポリゴンとの当たり判定
	// 床ポリゴンとの当たりをチェックし、移動ベクトルを補正する
	FixedPos = CheckHitWithFloor(player, FixedPos);

	return FixedPos;
}

/// <summary>
/// 描画
/// </summary>
void Stage::Draw()
{
	MV1DrawModel(ModelHandle);
}

/// <summary>
/// 検出されたポリゴンが壁ポリゴン( ＸＺ平面に垂直なポリゴン )か床ポリゴン( ＸＺ平面に垂直ではないポリゴン )かを判断し、保存する
/// </summary>
void Stage::AnalyzeWallAndFloor(const VECTOR& CheckPosition)
{
	// 壁ポリゴンと床ポリゴンの数を初期化する
	WallNum = 0;
	FloorNum = 0;

	// 検出されたポリゴンの数だけ繰り返し
	for (int i = 0; i < HitDim.HitNum; i++)
	{
		// ＸＺ平面に垂直かどうかはポリゴンの法線のＹ成分が０に限りなく近いかどうかで判断する
		if (HitDim.Dim[i].Normal.y < 0.000001f && HitDim.Dim[i].Normal.y > -0.000001f)
		{
			// 壁ポリゴンと判断された場合でも、プレイヤーのＹ座標＋１．０ｆより高いポリゴンのみ当たり判定を行う
			if (HitDim.Dim[i].Position[0].y > CheckPosition.y + 1.0f ||
				HitDim.Dim[i].Position[1].y > CheckPosition.y + 1.0f ||
				HitDim.Dim[i].Position[2].y > CheckPosition.y + 1.0f)
			{
				// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
				if (WallNum < MaxHitColl)
				{
					// ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
					Wall[WallNum] = &HitDim.Dim[i];

					// 壁ポリゴンの数を加算する
					WallNum++;
				}
			}
		}
		else
		{
			// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
			if (FloorNum < MaxHitColl)
			{
				// ポリゴンの構造体のアドレスを床ポリゴンポインタ配列に保存する
				Floor[FloorNum] = &HitDim.Dim[i];

				// 床ポリゴンの数を加算する
				FloorNum++;
			}
		}
	}
}

/// <summary>
/// 壁ポリゴンとの当たりをチェックし、補正すべき移動ベクトルを返す
/// </summary>
VECTOR Stage::CheckHitWithWall(Player& player, const VECTOR& CheckPosition)
{
	VECTOR FixedPos = CheckPosition;

	// 壁の数が無かったら早期リターン
	if (WallNum == 0)
	{
		return FixedPos;
	}

	// 壁からの押し出し処理を試みる最大数だけ繰り返し
	for (int k = 0; k < HitTryNum; k++)
	{
		// 当たる可能性のある壁ポリゴンを全て見る
		bool isHitWall = false;
		for (int i = 0; i < WallNum; i++)
		{
			// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
			auto Poly = Wall[i];

			// プレイヤーと当たっているなら
			if (HitCheck_Capsule_Triangle(FixedPos, VAdd(FixedPos, VGet(0.0f, HitHeight, 0.0f)), HitWidth, Poly->Position[0], Poly->Position[1], Poly->Position[2]) == TRUE)
			{
				// 当たっていたら規定距離分プレイヤーを壁の法線方向に移動させる
				// 移動後の位置を更新（移動後の場所を補正）
				FixedPos = VAdd(FixedPos, VScale(Poly->Normal, HitSlideLength));

				// 移動した壁ポリゴンと接触しているかどうかを判定
				for (int j = 0; j < WallNum; j++)
				{
					// 当たっていたらループを抜ける
					Poly = Wall[j];
					if (HitCheck_Capsule_Triangle(FixedPos, VAdd(FixedPos, VGet(0.0f, HitHeight, 0.0f)), HitWidth, Poly->Position[0], Poly->Position[1], Poly->Position[2]) == TRUE)
					{
						isHitWall = true;
						break;
					}
				}

				// 全てのポリゴンと当たっていなかったらここでループ終了
				if (isHitWall == false)
				{
					break;
				}
			}
		}

		// 全部のポリゴンで押し出しを試みる前に
		// 全ての壁ポリゴンと接触しなくなったらループから抜ける
		if (isHitWall == false)
		{
			break;
		}
	}

	return FixedPos;
}

/// <summary>
/// 床ポリゴンとの当たりをチェックし、補正すべき移動ベクトルを返す
/// </summary>
VECTOR Stage::CheckHitWithFloor(Player& player, const VECTOR& CheckPosition)
{
	VECTOR FixedPos = CheckPosition;

	// 床の数が無かったら早期リターン
	if (FloorNum == 0)
	{
		return FixedPos;
	}

	// 当たったかどうかのフラグを初期化
	bool IsHitFloor = false;

	// ジャンプ中且つ上昇中の場合は処理を分岐
	if (player.GetState() == Player::State::JUMP
		&& player.GetJumpPower() > 0.0f)
	{
		// 天井に頭をぶつける処理を行う
		// 一番低い天井にぶつける為の判定用変数を初期化
		float MinY = 0.0f;

		// 床ポリゴンの数だけ繰り返し
		for (int i = 0; i < FloorNum; i++)
		{
			// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
			auto Poly = Floor[i];

			// 足先から頭の高さまでの間でポリゴンと接触しているかどうかを判定
			HITRESULT_LINE LineRes;					// 線分とポリゴンとの当たり判定の結果を代入する構造体
			LineRes = HitCheck_Line_Triangle(FixedPos, VAdd(FixedPos, VGet(0.0f, HitHeight, 0.0f)), Poly->Position[0], Poly->Position[1], Poly->Position[2]);

			// 接触していなかったら何もしない
			if (LineRes.HitFlag == TRUE)
			{
				// 既にポリゴンに当たっていて、且つ今まで検出した天井ポリゴンより高い場合は何もしない
				if (!(IsHitFloor == true && MinY < LineRes.Position.y))
				{
					// ポリゴンに当たったフラグを立てる
					IsHitFloor = true;

					// 接触したＹ座標を保存する
					MinY = LineRes.Position.y;
				}
			}
		}

		// 接触したポリゴンがあれば
		if (IsHitFloor == true)
		{
			// 接触した場合はプレイヤーのＹ座標を接触座標を元に更新
			FixedPos.y = MinY - HitHeight;
			player.OnHitRoof();
		}
	}
	else
	{
		// 下降中かジャンプ中ではない場合の処理
		// 一番高い床ポリゴンにぶつける為の判定用変数を初期化
		float MaxY = 0.0f;

		// 床ポリゴンの数だけ繰り返し
		for (int i = 0; i < FloorNum; i++)
		{
			// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
			auto Poly = Floor[i];

			// ジャンプ中かどうかで処理を分岐
			HITRESULT_LINE LineRes;					// 線分とポリゴンとの当たり判定の結果を代入する構造体
			if (player.GetState() == Player::State::JUMP)
			{
				// ジャンプ中の場合は頭の先から足先より少し低い位置の間で当たっているかを判定
				LineRes = HitCheck_Line_Triangle(VAdd(FixedPos, VGet(0.0f, HitHeight, 0.0f)), VAdd(FixedPos, VGet(0.0f, -1.0f, 0.0f)), Poly->Position[0], Poly->Position[1], Poly->Position[2]);
			}
			else
			{
				// 走っている場合は頭の先からそこそこ低い位置の間で当たっているかを判定( 傾斜で落下状態に移行してしまわない為 )
				LineRes = HitCheck_Line_Triangle(VAdd(FixedPos, VGet(0.0f, HitHeight, 0.0f)), VAdd(FixedPos, VGet(0.0f, -40.0f, 0.0f)), Poly->Position[0], Poly->Position[1], Poly->Position[2]);
			}

			// 当たっていなかったら何もしない
			if (LineRes.HitFlag == TRUE)
			{
				// 既に当たったポリゴンがあり、且つ今まで検出した床ポリゴンより低い場合は何もしない
				if (!(IsHitFloor == true && MaxY > LineRes.Position.y))
				{
					// ポリゴンに当たったフラグを立てる
					IsHitFloor = true;

					// 接触したＹ座標を保存する
					MaxY = LineRes.Position.y;
				}
			}
		}

		// 床ポリゴンに当たったかどうかで処理を分岐
		if (IsHitFloor == true)
		{
			// 当たった場合
			// 接触したポリゴンで一番高いＹ座標をプレイヤーのＹ座標にする
			FixedPos.y = MaxY;

			// 床に当たった時
			player.OnHitFloor();
		}
		else
		{
			// 床コリジョンに当たっていなくて且つジャンプ状態ではなかった場合は
			player.OnFall();
		}
	}

	return FixedPos;
}
