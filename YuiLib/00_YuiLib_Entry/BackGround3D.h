#pragma once

class WorldSprite;

/// <summary>
/// 3Dîwåi
/// </summary>
class BackGround3D
{
public:
	BackGround3D();
	~BackGround3D();
	void Update();
	void Draw();

private:
	int				modelHandle;
	int				spriteGraphHandle;
	WorldSprite*	worldSprite;
};

