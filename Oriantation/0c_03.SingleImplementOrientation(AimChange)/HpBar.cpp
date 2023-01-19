#include "HpBar.h"
#include "DxLib.h"

void HpBar::Init(float firstDrawHp)
{
	graph = LoadGraph("data/texture/hp.png");
	drawHp = aimHp = firstDrawHp;
}

void HpBar::Update()
{
	// 毎フレーム緩やかに目標に近づく
	if (drawHp != aimHp)
	{
		drawHp -= AimHpSpeed;
		// 目標に合致したら止める
		if (drawHp < aimHp)
		{
			drawHp = aimHp;
		}
	}
}

void HpBar::OnDamage(float afterHp)
{
	aimHp = afterHp;
}

void HpBar::Draw(int ScreenW, float HpMax)
{
	// 画面サイズに合わせて、HPゲージを表示する
	printfDx("     drawHp:%f aimHp:%f\n", drawHp, aimHp);
	if (drawHp > 0.0f)
	{
		DrawExtendGraph(
			HpBarSideSpace,
			HpBarStartY,
			HpBarSideSpace + (int)((ScreenW - (HpBarSideSpace * 2)) * ((float)drawHp / HpMax)),
			HpBarStartY + HpBarHight,
			graph,
			TRUE);
	}
}
