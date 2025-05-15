#include "DxLib.h"
#include "WorldSprite.h"
#include "BackGround3D.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BackGround3D::BackGround3D()
	: modelHandle(-1)
	, spriteGraphHandle(-1)
	, worldSprite(nullptr)
{
	// ���f�����[�h
	modelHandle = MV1LoadModel("data/model/obstructStatic/obstructStatic.pmd");
	spriteGraphHandle = LoadGraph("data/texture/marisa.png");
	worldSprite = new WorldSprite();
	worldSprite->Initialize(spriteGraphHandle, 130, 0);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BackGround3D::~BackGround3D()
{
	MV1DeleteModel(modelHandle);// ���f���̃A�����[�h
	delete worldSprite;
}

/// <summary>
/// �X�V
/// </summary>
void BackGround3D::Update()
{
	MV1SetPosition(modelHandle, VGet(0, 0, 0));

	worldSprite->SetTransform(VGet(0, 0, 0), 10.0f);
}

/// <summary>
/// �`��
/// </summary>
void BackGround3D::Draw()
{
	MV1DrawModel(modelHandle);
	worldSprite->Draw();
}