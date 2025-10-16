#include "DxLib.h"
#include "Enemy.h"

// �G�l�~�[������
void InitializeEnemy(Enemy& enemy)
{
	enemy.Graph = LoadGraph("data/texture/enemy.png");
	enemy.X = 0;
	enemy.Y = 50;

	// �G�l�~�[�̃O���t�B�b�N���������Ƀ��[�h���_���[�W���̏�ԊǗ��p�̕ϐ���������
	enemy.DamageGraph = LoadGraph("data/texture/enemyDamage.png");

	// �G�l�~�[�����c�߂Ă��邩�ǂ����̕ϐ��Ɂw�c�߂Ă��Ȃ��x��\��FALSE����
	enemy.DamageFlag = false;

	// �G�l�~�[�̃O���t�B�b�N�̃T�C�Y�𓾂�
	GetGraphSize(enemy.Graph, &enemy.W, &enemy.H);

	// �G�l�~�[���E�ړ����Ă��邩�ǂ����̃t���O�����Z�b�g
	enemy.RightMove = true;
}

// �G�l�~�[�X�V
void UpdateEnemy(Enemy& enemy)
{
	// �G�l�~�[�̍��W���ړ����Ă�������Ɉړ�����
	if (enemy.RightMove == true)
	{
		enemy.X += 3;
	}
	else
	{
		enemy.X -= 3;
	}

	// �G�l�~�[����ʒ[����ł����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă����A�ړ�������������]����
	if (enemy.X > 640 - enemy.W)
	{
		enemy.X = 640 - enemy.W;
		enemy.RightMove = false;
	}
	else if (enemy.X < 0)
	{
		enemy.X = 0;
		enemy.RightMove = true;
	}
}

// �G�l�~�[�`��
void DrawEnemy(Enemy& enemy)
{
	// �G�l�~�[��`��
	// ���c�߂Ă��邩�ǂ����ŏ����𕪊�
	if (enemy.DamageFlag == true)
	{
		// ���c�߂Ă���ꍇ�̓_���[�W���̃O���t�B�b�N��`�悷��
		DrawGraph(enemy.X, enemy.Y, enemy.DamageGraph, FALSE);

		// ���c�߂Ă��鎞�Ԃ𑪂�J�E���^�[�ɂP�����Z����
		enemy.DamageCounter++;

		// �������c�ߏ��߂� �R�O �t���[���o�߂��Ă������̘c�񂾏�Ԃ���
		// ���ɖ߂��Ă�����
		if (enemy.DamageCounter == 30)
		{
			// �w�c��ł��Ȃ��x��\��FALSE����
			enemy.DamageFlag = false;
		}
	}
	else
	{
		DrawGraph(enemy.X, enemy.Y, enemy.Graph, FALSE);
	}
}
