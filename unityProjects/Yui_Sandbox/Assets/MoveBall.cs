using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveBall : MonoBehaviour {

    [SerializeField]
    float velocity = 0.3f;

	// 取得するRigidBody
	Rigidbody rigidBody;

	// Use this for initialization
	void Start () {
		// RigidBodyを取得して変数に保存する
		rigidBody = GetComponent<Rigidbody>();
	}
	
	// Update is called once per frame
	void Update () {
        // 取得したRigidBodyに力を加える
        if (Input.GetKey(KeyCode.UpArrow))
        {
            rigidBody.AddForce(new Vector3(0.0f, 0.0f, velocity), ForceMode.VelocityChange);
        }
        if (Input.GetKey(KeyCode.DownArrow))
        {
            rigidBody.AddForce(new Vector3(0.0f, 0.0f, -velocity), ForceMode.VelocityChange);
        }
    }
}
