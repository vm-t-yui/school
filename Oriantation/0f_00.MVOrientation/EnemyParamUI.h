#pragma once

#include "EnemyUiHp.h"
#include "EnemyParam.h"

/// <summary>
/// EnemyのUIとりまとめ (Controller, Presenter)
/// モデルであるEnemyParamを実質コントロール可能だが、直接の書き換えは行わない
/// </summary>
class EnemyParamUI
{
public:
	const EnemyParam& GetParam() const { return param; }
	void OnChangeParam(const EnemyParam& param) { this->param = param; }
	void Draw();

private:
	EnemyParam param;
	EnemyUiHp hpUi;
};

