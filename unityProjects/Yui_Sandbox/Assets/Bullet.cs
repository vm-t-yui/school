using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bullet : MonoBehaviour {

    [SerializeField]
    float speed = 0.1f;

    Rigidbody rigidBody;

	// Use this for initialization
	void Start () {
        rigidBody = GetComponent<Rigidbody>();
    }
	
	// Update is called once per frame
	void Update () {
        // 自分の位置を、自分の位置 ＋
        // 自分の向いている前方向ベクトル ✕ speedに変更する.
        rigidBody.MovePosition(transform.position +
            transform.forward * speed);
	}

    // ものに当たったとき.
    //private void OnCollisionEnter(Collision collision)
    //{
    //    Destroy(gameObject);
    //}
    private void OnTriggerEnter(Collider other)
    {
		// 敵に当たったら敵を消す.
		if(other.gameObject.tag == "Enemy")
		{
			Destroy(other.gameObject);
		}

		// 何に当たったとしても自分を消す.
		Destroy(gameObject);
	}
}
