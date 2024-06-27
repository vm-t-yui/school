#pragma once

namespace YuiLib {

	/// <summary>
	/// 線分に対する最近接点を求める
	/// </summary>
	VECTOR GetClosestPtOnSegment(VECTOR pt, VECTOR start, VECTOR end)
	{
		// 最近接点がstart側線分外領域の場合
		VECTOR startToPt = VSub(pt, start);
		VECTOR startToEnd = VSub(end, start);
		VECTOR startToEndN = VNorm(startToEnd);
		if (VDot(startToPt, startToEndN) < 0)
		{
			return start;
		}
		VECTOR endToPt = VSub(pt, end);
		VECTOR endToStart = VSub(start, end);
		VECTOR endToStartN = VNorm(endToStart);
		// 最近接点がend側線分外領域の場合
		if (VDot(endToPt, endToStartN) < 0)
		{
			return end;
		}
		// 中間領域の場合
		else
		{
			float t = VDot(startToEndN, startToPt);
			return VAdd(start, VScale(startToEndN, t));
		}
	}
}