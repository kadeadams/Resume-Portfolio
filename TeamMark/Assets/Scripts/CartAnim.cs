using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CartAnim : MonoBehaviour
{
    public Animator Anim;
    void Start()
    {
        Anim = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown("1"))
        {
            Anim.Play("MoveCart");
;        }
    }
}
