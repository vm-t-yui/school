using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class testRot : MonoBehaviour {

	// Update is called once per frame
	void Update () {
		if (Input.GetKey(KeyCode.RightArrow))
		{
			transform.Rotate(Quaternion.Inverse(transform.rotation) * Vector3.up, 10);
			//transform.Rotate(Vector3.up, 10);
			//transform.rotation = transform.rotation * Quaternion.AngleAxis(10, transform.rotation * Vector3.up);
		}
		if (Input.GetKey(KeyCode.UpArrow))
		{
			transform.Rotate(Quaternion.Inverse(transform.rotation) * Vector3.right, 10);
			//transform.Rotate(Vector3.right, 10);
			//transform.rotation = transform.rotation * Quaternion.AngleAxis(10, transform.rotation * Vector3.right);
		}
	}
}
