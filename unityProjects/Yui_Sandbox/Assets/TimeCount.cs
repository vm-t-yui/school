using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TimeCount : MonoBehaviour {

	// 書き換えるテキストコンポーネント.
	[SerializeField]
	Text text;

	float timeCount;

	// 開始時間.
	float startTime;

	// Use this for initialization
	void Start () {
		// 開始時間をいれる.
		startTime = Time.time;	// Time.timeは現在時間！.
	}
	
	// Update is called once per frame
	void Update () {
		timeCount = 60 - (Time.time - startTime);

		// ゼロより小さくなったらゼロを入れる.
		if(timeCount < 0)
		{
			timeCount = 0;
		}

		// テキストの中身にtimeCountを入れる.
		text.text = ((int)timeCount).ToString();
	}
}
