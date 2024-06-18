#include "DxLib.h"
#include "WorldSprite.h"
#include "BackGround3D.h"

/// <summary>
/// コンストラクタ
/// </summary>
BackGround3D::BackGround3D()
	: modelHandle(-1)
	, spriteGraphHandle(-1)
	, worldSprite(nullptr)
{
	// モデルロード
	modelHandle = MV1LoadModel("data/model/obstructStatic/obstructStatic.pmd");
	spriteGraphHandle = LoadGraph("data/texture/marisa.png");
	worldSprite = new YuiLib::WorldSprite();
	worldSprite->Initialize(spriteGraphHandle, 130, 0);
}

/// <summary>
/// デストラクタ
/// </summary>
BackGround3D::~BackGround3D()
{
	MV1DeleteModel(modelHandle);// モデルのアンロード
	delete worldSprite;
}

/// <summary>
/// 更新
/// </summary>
void BackGround3D::Update()
{
	MV1SetPosition(modelHandle, VGet(0, 0, 10.0f));

	worldSprite->SetTransform(VGet(0, 0, 0), 10.0f);
}

/// <summary>
/// 描画
/// </summary>
void BackGround3D::Draw()
{
	MV1DrawModel(modelHandle);
	worldSprite->Draw();
}