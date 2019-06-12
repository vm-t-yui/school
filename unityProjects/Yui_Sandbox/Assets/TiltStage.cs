using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TiltStage : MonoBehaviour
{
	[SerializeField]
	float speed = 0.2f;

	// Use this for initialization
	void Start()
	{
	}

	// Update is called once per frame
	void Update()
	{
		//// ←→キーを押したらy軸回転させる.
		//if (Input.GetKey(KeyCode.RightArrow))
		//{
		//	gameObject.transform.localRotation *= Quaternion.Euler(0.0f, speed, 0.0f);
		//}
		//if (Input.GetKey(KeyCode.LeftArrow))
		//{
		//	gameObject.transform.localRotation *= Quaternion.Euler(0.0f, -speed, 0.0f);
		//}

        Rigidbody rigidbody = GetComponent<Rigidbody>();
        if (Input.GetKey(KeyCode.RightArrow))
        {
            rigidbody.MoveRotation(gameObject.transform.rotation * Quaternion.Euler(0.0f, speed, 0.0f));
        }
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            rigidbody.MoveRotation(gameObject.transform.rotation * Quaternion.Euler(0.0f, -speed, 0.0f));
        }
    }
}
