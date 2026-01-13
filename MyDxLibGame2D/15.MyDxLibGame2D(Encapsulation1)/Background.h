#pragma once

class WorldSprite;

/// <summary>
/// 背景クラス
/// </summary>
class Background
{
public:
	Background();
	virtual ~Background() { /* 処理なし */ }

	void Initialize();
	void Draw() const;

private:
	int							modelHandle;
	int							spriteGraph;
	std::vector<WorldSprite>	worldSprites;
	WorldSprite					testSingleSprite;
};