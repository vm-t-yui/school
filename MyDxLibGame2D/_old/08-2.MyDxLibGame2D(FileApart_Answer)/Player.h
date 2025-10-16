#pragma once

// �v���C���[�\����.
struct Player
{
	int Graph;
	int X;
	int Y;
	int W;
	int H;
	bool PrevShotFlag;
};

// �v���g�^�C�v�錾
struct Shot;

void InitializePlayer(Player& player);									// �v���C���[������
void UpdatePlayer(Player& player, Shot shotArray[], int shotArraySize);	// �v���C���[�̍X�V����
void DrawPlayer(Player& player);										// �v���C���[�̕`�揈��
