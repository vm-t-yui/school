// 2025 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "ObstacleBase.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ObstacleBase::ObstacleBase(const char* path)
	:  modelHandle(-1)
	, pos(VGet(0, 0, 0))
{
	// �R�c���f���̓ǂݍ���
	modelHandle = MV1LoadModel(path);
	if (modelHandle < 0)
	{
		printfDx("ObstacleBase:�f�[�^�ǂݍ��݂Ɏ��s");
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ObstacleBase::~ObstacleBase()
{
	// ���f���̃A�����[�h.
	MV1DeleteModel(modelHandle);
}

/// <summary>
/// �R�c���f���̕`��
/// </summary>
void ObstacleBase::Draw() const
{
    MV1DrawModel(modelHandle);
}
