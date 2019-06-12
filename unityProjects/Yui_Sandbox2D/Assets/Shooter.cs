using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shooter : MonoBehaviour {

    // 生み出すもととなる弾のプレハブ
    public GameObject shotPrefab;

    // ショットを生み出す場所
    public GameObject shotPoint;

	// Update is called once per frame
	void Update () {
		// マウスボタンが押されたら、shotを生み出す
        if(Input.GetMouseButtonDown(0))
        {
            // 生み出した弾の位置をショットポジションと同じにする.
            GameObject newShot = GameObject.Instantiate(shotPrefab);
            newShot.transform.position = shotPoint.transform.position;

            // プレイヤーが反対を向いている場合は、ショットも反対を向かせる.
            // Xのスケールがマイナスの場合は反対をむく
            if(transform.localScale.x < 0)
            {
                newShot.transform.localScale =
                    new Vector3(newShot.transform.localScale.x * -1,
                    newShot.transform.localScale.y,
                    newShot.transform.localScale.z);
            }
        }
	}
}
