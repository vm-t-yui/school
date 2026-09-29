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

	///////////////////////////////////////////////////////
	// 2Dの座標系を、3Dに変換するための定数群
	// スケール定数
	inline const static	float	ScaleFactor3D = 0.01f;	// スケール割合
	///////////////////////////////////////////////////////

	// 2Dポジションを3Dに変換
	inline VECTOR Get3DPosition(const VECTOR& pos2D)
	{
		VECTOR pos3D = VScale(pos2D, ScaleFactor3D);
		pos3D = VGet(pos3D.x, -1.0f * pos3D.y, pos3D.z);
		return pos3D;
	}
}
// 
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