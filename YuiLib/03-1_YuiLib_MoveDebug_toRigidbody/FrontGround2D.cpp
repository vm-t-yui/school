#include "DxLib.h"
#include "FrontGround2D.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
FrontGround2D::FrontGround2D()
	: graphHandle(-1)
{
	// �摜���[�h
	graphHandle = LoadGraph("data/texture/FancyBG_front.png");
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FrontGround2D::~FrontGround2D()
{
	// �摜�A�����[�h
	DeleteGraph(graphHandle);
}

/// <summary>
/// �X�V
/// </summary>
void FrontGround2D::Update()
{
	// �����Ȃ�
}

/// <summary>
/// �`��
/// </summary>
void FrontGround2D::Draw()
{
	DrawGraph(0, 0, graphHandle, true);
}