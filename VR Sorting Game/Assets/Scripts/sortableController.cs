using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class sortableController : MonoBehaviour
{
    public char m_type;

    void Update()
    {
        if (transform.position.y <= -3)
        {
            Destroy(gameObject);
        }
    }
}
