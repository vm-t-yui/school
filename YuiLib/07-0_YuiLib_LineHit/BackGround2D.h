#pragma once

/// <summary>
/// 2D背景
/// </summary>
class BackGround2D
{
public:
	BackGround2D();
	~BackGround2D();
	void Update();
	void Draw();

private:
	int graphHandle;
};

