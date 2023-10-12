#pragma once

class EnemyParam;

class EnemyParamModel
{
public:
	void OnChangeParam(const EnemyParam& param) { this->param = param; }
	const EnemyParam& GetParam() const { return param; }

private:
	EnemyParam param;
};

