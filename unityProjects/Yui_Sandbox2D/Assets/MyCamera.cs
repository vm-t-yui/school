using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyCamera : MonoBehaviour {

    public GameObject target;

	// Update is called once per frame
	void Update () {
        // ターゲットを追いかける
        transform.position = new Vector3(target.transform.position.x, transform.position.y, transform.position.z);
	}
}
