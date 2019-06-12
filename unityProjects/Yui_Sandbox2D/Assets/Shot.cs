using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shot : MonoBehaviour {

    // 自分自身のリジッドボディ.
    public Rigidbody2D myRigid;

    // たまの進むスピード.
    public float speed = 2.0f;

    // Update is called once per frame
    void Update () {

        // たまが反対を向いている（スケールがマイナス）なら反対に進ませる
        if (transform.localScale.x < 0)
        {
            myRigid.velocity = transform.right * speed * -1;
        }
        else
        {
            myRigid.velocity = transform.right * speed;
        }
    }

    // 弾が当たった時に呼ばれる
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // 当たったものが、敵なら
        if(collision.gameObject.tag == "Enemy")
        {
            // 敵を消す
            Destroy(collision.gameObject);
        }

        // 何が当たっても自分（弾）を消す
        Destroy(gameObject);
    }
}
