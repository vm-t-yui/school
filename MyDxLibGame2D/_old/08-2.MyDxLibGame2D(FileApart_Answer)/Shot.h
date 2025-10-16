#pragma once

// �V���b�g�̐�.
#define SHOT 20

// �V���b�g�\����.
struct Shot
{
	int		Graph;
	int		X;
	int		Y;
	bool	visibleFlag;
	int		W;
	int		H;
};

// �v���g�^�C�v�錾
struct Enemy;

void InitializeShot(Shot& shot, int shotGraph);							// �V���b�g������
void UpdateShot(Shot& shot, Enemy& enemy);								// �V���b�g�X�V
void DrawShot(Shot& shot);												// �V���b�g�`��