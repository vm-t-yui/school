#pragma once

// �G�l�~�[�\����.
struct Enemy
{
	int		X;
	int		Y;
	int		Graph;
	bool	DamageFlag;
	int		DamageCounter;
	int		DamageGraph;
	int		W;
	int		H;
	bool    RightMove;
};

void InitializeEnemy(Enemy& enemy);										// �G�l�~�[������
void UpdateEnemy(Enemy& enemy);											// �G�l�~�[�X�V
void DrawEnemy(Enemy& enemy);											// �G�l�~�[�`��
