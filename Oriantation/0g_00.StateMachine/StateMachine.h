#pragma once
#include <functional>
#include <map>

/// <summary>
/// ステートマシン
/// </summary>
template <class TState> class StateMachine
{
public:
	typedef const std::function<void(void)> Delegate;

	// ステートの登録
	void AddState(TState state, Delegate enter, Delegate update, Delegate exit);

	// ステートの変更
	void ChangeState(TState state);

	// 更新
	void Update() const;

	// ステートの取得
	TState GetCurrentState() const { return currentState; }

private:
	class StateDelegateSet
	{
		TState state;
		Delegate enter, update, exit;
	};

	TState currentState;
	std::map<TState, StateDelegateSet> stateFuncMap;
};

