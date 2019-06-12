using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour {

	[SerializeField]
	public int HP = 10;
	
	/// <summary>
	/// ダメージを受けたとき.
	/// </summary>
	public void OnDamage(int damage)
	{
		// 指定されたダメージ分HPを減らす.
		HP -= damage;
	}

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
