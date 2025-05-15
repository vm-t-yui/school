#include "DxLib.h"
#include "BackGround2D.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BackGround2D::BackGround2D()
	: graphHandle(-1)
{
	// �摜���[�h
	graphHandle = LoadGraph("data/texture/FancyBG_back.png");
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BackGround2D::~BackGround2D()
{
	// �摜�A�����[�h
	DeleteGraph(graphHandle);
}

/// <summary>
/// �X�V
/// </summary>
void BackGround2D::Update()
{
	// �����Ȃ�
}

/// <summary>
/// �`��
/// </summary>
void BackGround2D::Draw()
{
	DrawGraph(0,0, graphHandle, true);
}