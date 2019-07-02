#include "GameObject.h"
#include "BG.h"
#include "DxLib.h"

// ‰Šú‰».
void InitBG(BG& bg, int graph, int index)
{
	bg.obj.graph = graph;
	GetGraphSize(bg.obj.graph, &bg.obj.w, &bg.obj.h);
	bg.obj.pos.x = bg.obj.w * 0.5f;
	bg.obj.pos.y = (float)(index * bg.obj.h);
}

// ƒAƒbƒvƒf[ƒg.
void UpdateBG(BG& bg)
{
	// ”wŒicƒXƒNƒ[ƒ‹.
	bg.obj.pos.y += BG_SCROLL_SPEED;

	// ”wŒi‚ÌyÀ•W‚ª”wŒiˆê‚Â•ª—]Œv‚É‰º‚É‚È‚Á‚½‚çã‚ÉˆÚ“®.
	if (bg.obj.pos.y > bg.obj.h + bg.obj.h * 0.5f)
	{
		bg.obj.pos.y -= bg.obj.h * 2;
	}
}

// •`‰æ.
void DrawBG(BG& bg)
{
	DrawGameObject(bg.obj, bg.obj.graph);
}