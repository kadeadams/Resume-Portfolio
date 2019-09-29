using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class bombTimer : MonoBehaviour {

    public float explosionDelay;
    public float radius = 5.0F;
    public float power = 10.0F;
    public float upForce = 10.0F;

    private int layer = 0;
    private float explodeTime;

    // Use this for initialization
    void Start () {
        explodeTime = Time.time + explosionDelay;
    }
	
	// Update is called once per frame
	void Update () {
		if (Time.time >= explodeTime)
        {
            Vector3 explosionPos = transform.position;
            Collider[] colliders = Physics.OverlapSphere(explosionPos, radius);
            foreach (Collider hit in colliders)
            {
                Rigidbody rb = hit.GetComponent<Rigidbody>();

                if (rb != null)
                    rb.AddExplosionForce(power, explosionPos, radius, upForce);
            }
            Destroy(gameObject);
        }
	}
}
