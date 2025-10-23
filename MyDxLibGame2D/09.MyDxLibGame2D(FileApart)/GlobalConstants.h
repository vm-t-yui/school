#pragma once

/// <summary>
/// グラフィック関係の定数群
/// </summary>
namespace Graphics
{
	constexpr int	ScreenW			= 640;
	constexpr int	ScreenH			= 480;
	constexpr int	ColorBit		= 16;
	constexpr int	OneFrameNanoSec	= 16667;
}

// デバッグ表示のカラー
#if _DEBUG
namespace Debug
{
	constexpr unsigned int	EnemyHitSizeColor	= 0xff0000;
	constexpr unsigned int	ShotHitSizeColor	= 0x00ffff;
}
#endif