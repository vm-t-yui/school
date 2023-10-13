#include "Dxlib.h"
#include "Common.h"
#include "EnemyParam.h"
#include "EnemyUiHp.h"

/// <summary>
/// •`‰æ
/// </summary>
void EnemyUiHp::Draw(const EnemyParam& param)
{
	auto color = GetColor(255, 0, 0);
	DrawFormatString((int)(SCREEN_W * 0.5f), 20, color, "Life:%d\n", param.Life);
}