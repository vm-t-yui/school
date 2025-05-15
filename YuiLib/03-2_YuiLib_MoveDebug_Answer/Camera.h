#pragma once

/// <summary>
/// �J�����N���X
/// </summary>
class Camera
{
public:
	Camera();
	void Update();

private:
	float	fov;
	float	fovRad;
	VECTOR	pos;
	float	posRad;	// pos�ω��p��

	constexpr static float DefaultFov		= 60.0f;	// �f�t�H���g��fov
	constexpr static float FovRange			= 30.0f;	// fov�̕ω���
	constexpr static float FovChangeSpeed	= 0.01f;	// fov�̕ω��X�s�[�h
	constexpr static float PosRange			= 3.0f;		// pos�̕ω���
	constexpr static float PosChangeSpeed	= 0.03f;	// pos�̕ω��X�s�[�h
};

