#pragma once
#include <functional>
#include <map>

/// <summary>
/// ステートマシン
/// </summary>
template <class TState> class StateMachine
{
public:
	typedef std::function<void(void)> Delegate;

private:
	class StateDelegateSet
	{
	public:
		TState state;
		Delegate enter, update, exit;

		// 変数初期化警告が出るので、デフォルトコンストラクタで初期化
		StateDelegateSet() : state(TState()){}
	};

	TState currentState;
	std::map<TState, StateDelegateSet> stateFuncMap;
	bool isInitialized = false;

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
	/// ステートの設定
	/// </summary>
	void SetState(TState state)
	{
		// 初回はenterだけ呼ぶ
		if (isInitialized == false)
		{
			isInitialized = true;
			currentState = state;
			stateFuncMap[currentState].enter();
		}
		// 直前のstateのexitを呼んでステートを更新
		else if (currentState != state)
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

