﻿#pragma once

class Player;

/// <summary>
/// 背景物の描画と更新
/// </summary>
class Stage
{
public:
	Stage();
	~Stage();

	int GetModelHandle() const { return modelHandle; }

	void Load();			// 初期化処理
	void Unload();			// 後始末処理

	// 当たり判定をして、補正後のポジションを返す
	VECTOR CheckCollision(Player& player, const VECTOR& moveVector);

	void Draw();

private:
	static const int MaxHitColl = 2048;	// 処理するコリジョンポリゴンの最大数

	static constexpr float	DefaultSize		= 800.0f;	// 周囲のポリゴン検出に使用する球の初期サイズ
	static constexpr float	HitWidth		= 200.0f;	// 当たり判定カプセルの半径
	static constexpr float	HitHeight		= 700.0f;	// 当たり判定カプセルの高さ
	static constexpr int	HitTryNum		= 16;		// 壁押し出し処理の最大試行回数
	static constexpr float	HitSlideLength	= 5.0f;		// 一度の壁押し出し処理でスライドさせる距離

	int							modelHandle;		// モデルハンドル

	// HACK: 壁はXZ平面に垂直である前提で成り立っている。それ以外を置くとバグる
	int							wallNum;			// 壁ポリゴンと判断されたポリゴンの数
	int							floorNum;			// 床ポリゴンと判断されたポリゴンの数

	MV1_COLL_RESULT_POLY*		wall[MaxHitColl];	// 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY*		floor[MaxHitColl];	// 床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列

	// 検出されたポリゴンが壁ポリゴン( ＸＺ平面に垂直なポリゴン )か床ポリゴン( ＸＺ平面に垂直ではないポリゴン )かを判断し、保存する
	void AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const VECTOR& checkPosition);

	// 壁ポリゴンとの当たりをチェックし、補正すべき移動ベクトルを返す
	VECTOR CheckHitWithWall(Player& player, const VECTOR& checkPosition);

	// 床ポリゴンとの当たりをチェックし、補正すべき移動ベクトルを返す
	VECTOR CheckHitWithFloor(Player& player, const VECTOR& checkPosition);
};


