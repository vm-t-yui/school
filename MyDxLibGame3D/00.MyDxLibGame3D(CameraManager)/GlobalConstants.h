#pragma once

/// <summary>
/// グラフィック関係の定数群
/// </summary>
namespace Graphics
{
	constexpr int	ScreenW			= 1600;
	constexpr int	ScreenH			= 900;
	constexpr int	ColorBit		= 16;
	constexpr int	OneFrameNanoSec	= 16667;
}

// デバッグ表示のカラー
#if _DEBUG
namespace Debug
{
	constexpr unsigned int	EnemyHitSizeColor		= 0xff0000;
	constexpr unsigned int	PlayerHitSizeColor		= 0x00ff00;
	constexpr unsigned int	ShotHitSizeColor		= 0x0000ff;
	constexpr unsigned int	Virtual2DScreenColor	= 0xff00ff;
}
#endif