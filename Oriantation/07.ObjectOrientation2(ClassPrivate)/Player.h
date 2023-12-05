//-----------------------------------------------------------------------------
// @brief  プレイヤー処理.
//-----------------------------------------------------------------------------

#pragma once

class Shot;

// プレイヤー.
class Player
{
public:
	void Init();				// 初期化.
	void Update(Shot shotArray[], int shotArrayNum);	// アップデート.
	void Draw();				// 描画.



private:
	int Graph;
	int X;
	int Y;
	int W;
	int H;

	// ショットのインターバルを設定するカウンター
	int shotIntervalCount;
};