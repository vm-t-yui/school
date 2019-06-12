using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CubeAnimator : MonoBehaviour {
	[SerializeField]
	Animator anim;

	// Update is called once per frame
	void Update () {
		// スペースキーを押したら、jumpのトリガーをON.
		if(Input.GetKeyDown(KeyCode.Space))
		{
			anim.SetTrigger("jump");
		}
	}
}
