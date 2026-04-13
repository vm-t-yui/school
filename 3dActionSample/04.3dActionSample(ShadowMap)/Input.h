#pragma once

/// <summary>
/// 入力
/// </summary>
class Input
{
public:
	Input();
	~Input();

	void	Update();

	int GetNowFrameInput() const { return nowFrameInput; }
	int GetNowFrameNewInput() const { return nowFrameNewInput; }

private:
	int		nowFrameInput;				// 現在のフレームで押されているボタン
	int		nowFrameNewInput;			// 現在のフレームで新たに押されたボタン
};


