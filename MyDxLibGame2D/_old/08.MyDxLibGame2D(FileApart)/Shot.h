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

void InitializeShot(Shot& shot, int shotGraph);							// �V���b�g������
void UpdateShot(Shot& shot, Enemy& enemy);								// �V���b�g�X�V
void DrawShot(Shot& shot);												// �V���b�g�`��

// �V���b�g������
void InitializeShot(Shot& shot, int shotGraph)
{
	// �V���b�g�̃O���t�B�b�N���������Ƀ��[�h.
	shot.Graph = shotGraph;

	// �e����ʏ�ɑ��݂��Ă��邩�ێ�����ϐ��Ɂw���݂��Ă��Ȃ��x���Ӗ�����false�������Ă���
	shot.visibleFlag = false;

	// �e�̃O���t�B�b�N�̃T�C�Y������
	GetGraphSize(shot.Graph, &shot.W, &shot.H);
}

// �V���b�g�X�V
void UpdateShot(Shot& shot, Enemy& enemy)
{
	// �e�̂����蔻��.
	// �ei�����݂��Ă���ꍇ�̂ݎ��̏����ɉf��
	if (shot.visibleFlag == 1)
	{
		// �G�l�~�[�Ƃ̓����蔻��
		if (((shot.X > enemy.X && shot.X < enemy.X + enemy.W) ||
			(enemy.X > shot.X && enemy.X < shot.X + shot.W)) &&
			((shot.Y > enemy.Y && shot.Y < enemy.Y + enemy.H) ||
				(enemy.Y > shot.Y && enemy.Y < shot.Y + shot.H)))
		{
			// �ڐG���Ă���ꍇ�͓��������e�̑��݂�����
			shot.visibleFlag = 0;

			// �G�l�~�[�̊��c�߂Ă��邩�ǂ�����ێ�����ϐ��Ɂw�c�߂Ă���x��\��TRUE����
			enemy.DamageFlag = true;

			// �G�l�~�[�̊��c�߂Ă��鎞�Ԃ𑪂�J�E���^�ϐ��ɂO����
			enemy.DamageCounter = 0;
		}
	}
}

// �V���b�g�`��
void DrawShot(Shot& shot)
{
	// ���@�̒ei�̈ړ����[�`��( ���ݏ�Ԃ�ێ����Ă���ϐ��̓��e��true(���݂���)�̏ꍇ�̂ݍs�� )
	if (shot.visibleFlag == true)
	{
		// �ei���P�U�h�b�g��Ɉړ�������
		shot.Y -= 16;

		// ��ʊO�ɏo�Ă��܂����ꍇ�͑��ݏ�Ԃ�ێ����Ă���ϐ���false(���݂��Ȃ�)��������
		if (shot.Y < 0 - shot.H)
		{
			shot.visibleFlag = false;
		}

		// ��ʂɒei��`�悷��
		DrawGraph(shot.X, shot.Y, shot.Graph, FALSE);
	}
}