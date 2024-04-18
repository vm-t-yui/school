#include "DxLib.h"
#include "BackGround3D.h"

/// <summary>
/// コンストラクタ
/// </summary>
BackGround3D::BackGround3D()
	: modelHandle(-1)
{
	// モデルロード
	modelHandle = MV1LoadModel("data/model/obstructStatic/obstructStatic.pmd");
}

/// <summary>
/// デストラクタ
/// </summary>
BackGround3D::~BackGround3D()
{
	MV1DeleteModel(modelHandle);// モデルのアンロード
}

/// <summary>
/// 更新
/// </summary>
void BackGround3D::Update()
{
	MV1SetPosition(modelHandle, VGet(0, 0, 0));
}

/// <summary>
/// 描画
/// </summary>
void BackGround3D::Draw()
{
	MV1DrawModel(modelHandle);
}