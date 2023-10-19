#pragma once

// 入力
class Input
{
public:
	void	Update();

	int GetNowFrameInput() const { return NowFrameInput; }
	int GetNowFrameNewInput() const { return NowFrameNewInput; }

private:
	int		NowFrameInput;				// 現在のフレームで押されているボタン
	int		NowFrameNewInput;			// 現在のフレームで新たに押されたボタン
};


