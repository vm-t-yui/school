using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyShooter : MonoBehaviour {

    // 弾オブジェクトのプレハブ
    [SerializeField]
    GameObject bulletPrefab;

    // 弾を生成するポイント.
    [SerializeField]
    GameObject shotPoint;

    // 弾を格納する空のゲームオブジェクト.
    [SerializeField]
    GameObject shotRoot;

    // 弾が生成される間隔（秒）.
    [SerializeField]
    float shotInterval = 0.3f;

    // 前回弾を撃った時間.
    float prevShotTime = 0;
    
	// Update is called once per frame
	void Update () {
        // Zキーが押されたら、自分と同じ場所に弾オブジェクトを生成する.
        if (Time.time - prevShotTime > shotInterval)
        {
            // 弾が打たれたら前回弾が撃たれた時間を更新.
            prevShotTime = Time.time;

            GameObject newBullet = Instantiate(bulletPrefab);
            newBullet.transform.position = shotPoint.transform.position;
            newBullet.transform.rotation = shotPoint.transform.rotation;

            newBullet.transform.parent = shotRoot.transform;
        }
    }
}
