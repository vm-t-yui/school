#include "DxLib.h"
#include "FrontGround2D.h"

/// <summary>
/// コンストラクタ
/// </summary>
FrontGround2D::FrontGround2D()
	: graphHandle(-1)
{
	// 画像ロード
	graphHandle = LoadGraph("data/texture/FancyBG_front.png");
}

/// <summary>
/// デストラクタ
/// </summary>
FrontGround2D::~FrontGround2D()
{
	// 画像アンロード
	DeleteGraph(graphHandle);
}

/// <summary>
/// 更新
/// </summary>
void FrontGround2D::Update()
{
	// 処理なし
}

/// <summary>
/// 描画
/// </summary>
void FrontGround2D::Draw()
{
	DrawGraph(0, 0, graphHandle, true);
}