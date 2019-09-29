using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class ballEmitter : MonoBehaviour {

    public GameObject [] shots;
    public Transform shotSpawn;
    public double scalingDelay;
    public double minDelay;
    public double scalingFactor;

    public GameObject scriptHolder;

    private double nextFire;
    private int i = 0;
    private static System.Random rnd;

    void Start()
    {
        rnd = new System.Random();
        
    }

    void Update()
    {
        if (nextFire <= Time.time)
        {

            if (scriptHolder.GetComponent<scoreboard>().lives > 0)
            {
                nextFire = Time.time + minDelay + scalingDelay * Math.Pow(2, (-1 * scalingFactor * Time.time));
                Instantiate(shots[i % shots.Length], shotSpawn.position, shotSpawn.rotation);
                i = rnd.Next();
            }
            
        }
    }
}
