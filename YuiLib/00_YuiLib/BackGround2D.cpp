#include "DxLib.h"
#include "BackGround2D.h"

/// <summary>
/// コンストラクタ
/// </summary>
BackGround2D::BackGround2D()
	: graphHandle(-1)
{
	// 画像ロード
	graphHandle = LoadGraph("data/texture/FancyBG_back.png");
}

/// <summary>
/// デストラクタ
/// </summary>
BackGround2D::~BackGround2D()
{
	// 画像アンロード
	DeleteGraph(graphHandle);
}

/// <summary>
/// 更新
/// </summary>
void BackGround2D::Update()
{
	// 処理なし
}

/// <summary>
/// 描画
/// </summary>
void BackGround2D::Draw()
{
	DrawGraph(0,0, graphHandle, true);
}