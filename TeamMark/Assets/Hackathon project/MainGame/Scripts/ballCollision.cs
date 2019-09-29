using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ballCollision : MonoBehaviour {

    public string type = "RED";

    void OnTriggerEnter(Collider other)
    {
        if (other.tag == "Pipe")
        {
            Destroy(gameObject);
        }
    }

}
