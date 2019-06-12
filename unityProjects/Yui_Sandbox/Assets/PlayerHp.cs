using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerHp : MonoBehaviour {

	[SerializeField]
	Player player;

	[SerializeField]
	Text text;

	[SerializeField]
	Image image;

	// Update is called once per frame
	void Update () {
		text.text = player.HP.ToString();
	}
}
