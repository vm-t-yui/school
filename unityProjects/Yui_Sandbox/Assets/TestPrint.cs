using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestPrint : MonoBehaviour {

	// ここには変数宣言を書く
	int		hp;     // HP(仮)
	float	atk;	// ATK

	// Use this for initialization
	void Start () {
		// ここに書くと最初だけ実行される
		hp = 100;
		atk = 20.0f;
	}
	
	// Update is called once per frame
	void Update () {
		// ここは毎回（毎フレーム）呼ばれる
		hp -= 1;

		//atk = atk + 0.05f;　と同じ
		atk += 0.05f;

		// atkをConsoleに出力する
		Debug.Log("atk:" + atk);
	}
}
