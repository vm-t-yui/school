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

private:
	class StateDelegateSet
	{
	public:
		TState state;
		Delegate enter, update, exit;
	};

	TState currentState;
	std::map<TState, StateDelegateSet> stateFuncMap;

public:
	/// <summary>
	/// ステートの追加
	/// </summary>
	void AddState(TState state, Delegate enter, Delegate update, Delegate exit)
	{
		// FIXME: 重複考慮
		StateDelegateSet set;
		set.state = state;
		set.enter = enter;
		set.update = update;
		set.exit = exit;
		stateFuncMap.emplace(state, set);
	}

	/// <summary>
	/// ステートの変更
	/// </summary>
	void ChangeState(TState state)
	{
		// 直前のstateのexitを呼んでステートを更新
		if (currentState != state)
		{
			stateFuncMap[currentState].exit();
			currentState = state;
			stateFuncMap[currentState].enter();
		}
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update()
	{
		stateFuncMap[currentState].update();
	}

	// ステートの取得
	TState GetCurrentState() const { return currentState; }
};

