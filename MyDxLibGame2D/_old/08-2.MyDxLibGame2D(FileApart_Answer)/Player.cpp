#include "DxLib.h"
#include "Shot.h"
#include "Player.h"

// �v���C���[������
void InitializePlayer(Player& player)
{
	// �v���C���[�̃O���t�B�b�N���������Ƀ��[�h���\�����W��������
	player.Graph = LoadGraph("data/texture/player.png");
	player.X = 288;
	player.Y = 400;

	// �V���b�g�{�^�����O�̃t���[���ŉ����ꂽ���ǂ�����ۑ�����ϐ���false(�����ꂢ�Ȃ�)����
	player.PrevShotFlag = false;

	// �v���C���[�ƒe�̉摜�̃T�C�Y�𓾂�
	GetGraphSize(player.Graph, &player.W, &player.H);
}

// �v���C���[�̍X�V����
void UpdatePlayer(Player& player, Shot shotArray[], int shotArraySize)
{
	// ���L�[�������Ă�����v���C���[���ړ�������
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		player.Y -= 3;
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		player.Y += 3;
	}
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		player.X -= 3;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		player.X += 3;
	}

	// �e�̔��ˏ���
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		// �O�t���[���ŃV���b�g�{�^���������������ۑ�����Ă���ϐ���false��������e�𔭎�
		if (player.PrevShotFlag == false)
		{
			// ��ʏ�ɂłĂ��Ȃ��e�����邩�A�e�̐������J��Ԃ��Ē��ׂ�
			for (int i = 0; i < shotArraySize; i++)
			{
				// �ei����ʏ�ɂłĂ��Ȃ��ꍇ�͂��̒e����ʂɏo��
				if (shotArray[i].visibleFlag == false)
				{
					// �ei�̈ʒu���Z�b�g�A�ʒu�̓v���C���[�̒��S�ɂ���
					shotArray[i].X = (player.W - shotArray[i].W) / 2 + player.X;
					shotArray[i].Y = (player.H - shotArray[i].H) / 2 + player.Y;

					// �ei�͌����_�������đ��݂���̂ŁA���ݏ�Ԃ�ێ�����ϐ���true��������
					shotArray[i].visibleFlag = true;

					// ��e���o�����̂Œe���o�����[�v���甲���܂�
					break;
				}
			}
		}

		// �O�t���[���ŃV���b�g�{�^����������Ă�������ۑ�����ϐ���true(������Ă���)����
		player.PrevShotFlag = true;
	}
	else
	{
		// �V���b�g�{�^����������Ă��Ȃ������ꍇ��
		// �O�t���[���ŃV���b�g�{�^����������Ă�������ۑ�����ϐ���false(������Ă��Ȃ�)����
		player.PrevShotFlag = false;
	}

	// �v���C���[����ʍ��[����͂ݏo�����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă�����
	if (player.X < 0)
	{
		player.X = 0;
	}
	if (player.X > 640 - player.W)
	{
		player.X = 640 - player.W;
	}
	if (player.Y < 0)
	{
		player.Y = 0;
	}
	if (player.Y > 480 - player.H)
	{
		player.Y = 480 - player.H;
	}
}

// �v���C���[�̕`�揈��
void DrawPlayer(Player& player)
{
	// �v���C���[��`��
	DrawGraph(player.X, player.Y, player.Graph, FALSE);
}