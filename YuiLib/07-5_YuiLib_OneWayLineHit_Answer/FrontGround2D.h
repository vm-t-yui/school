#pragma once

/// <summary>
/// 2D前景
/// </summary>
class FrontGround2D
{
public:
	FrontGround2D();
	~FrontGround2D();
	void Update();
	void Draw();

private:
	int graphHandle;
};

