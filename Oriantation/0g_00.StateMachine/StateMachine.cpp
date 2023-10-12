#include "StateMachine.h"

/// <summary>
/// ステートの追加
/// </summary>
template <class TState>
void StateMachine<TState>::AddState(TState state, Delegate enter, Delegate update, Delegate exit)
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
template <class TState>
void StateMachine<TState>::ChangeState(TState state)
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
template <class TState>
void StateMachine<TState>::Update() const
{
	stateFuncMap[currentState].update();
}