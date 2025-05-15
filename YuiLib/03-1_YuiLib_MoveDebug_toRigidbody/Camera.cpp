#include "DxLib.h"
#include "Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>

/// <summary>
/// �R���X�g���N�^
/// </summary>
Camera::Camera()
	: fov		(60.0f)
	 , fovRad	(0.0f)
	 , pos		(VGet(0,0,0))
	 , posRad	(0.0f)
{
	//���s0.1�`1000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(0.1f, 1000.0f);
}

/// <summary>
/// �X�V
/// </summary>
void Camera::Update()
{
	// FOV(����p)
	fovRad += FovChangeSpeed;
	fov = DefaultFov + (sinf(fovRad) * FovRange);
	SetupCamera_Perspective(fov * (static_cast<float>(M_PI) / 180.0f));

	// �Ƃ肠�������_����G�ɗ��ꂽ�ʒu�Ō��_������
	posRad += PosChangeSpeed;
	pos = VGet(0, sinf(posRad) * PosRange, -20);
	SetCameraPositionAndTarget_UpVecY(pos, VGet(0, 0, 0));
}