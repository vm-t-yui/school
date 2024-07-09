#pragma once

namespace YuiLib {

/// <summary>
/// 当たり判定のデータ基底
/// </summary>
class ColliderData abstract
{
public:
	// 当たり判定種別
	enum class Kind
	{
		Circle2D,
		Line2D,
		OneWayLine2D,
	};

	// コンストラクタ
	ColliderData(Kind kind, bool isTrigger)
	{
		this->kind = kind;
		this->isTrigger = isTrigger;
	}

	virtual ~ColliderData() {}

	// 当たり判定種別取得
	Kind GetKind() const { return kind; }

	// トリガーかどうか
	bool IsTrigger() const { return isTrigger; }

private:
	Kind	kind;
	bool	isTrigger;
};

}