using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using Random = UnityEngine.Random;

public class spawnerController : MonoBehaviour
{
    //public variables
    public GameObject[] sortableTypes;
    public float[] sortableProbabilties;
    public bool isRandom;
    public double scalingDelay;
    public double minDelay;
    public double scalingFactor;
    public Transform spawnLocation;
    public string m_pattern;
    public int numberOfSpawns;

    public GameObject scriptHolder;

    //private variables
    private GameObject nextSpawn;
    private double nextFire;
    private int i = 0;   //keeps trac of what pattern we are on
    private int currentSpawnNumber;



    int Choose(float[] probs)
    {

        float total = 0;

        foreach (float elem in probs)    //totals all probabilties in array
        {
            total += elem;
        }

        float randomPoint = UnityEngine.Random.value * total;       //gets a random point by multiplying random value between 0 and 1 and the total

        for (int i = 0; i < probs.Length; i++)   //goes through each item in the array
        {
            if (randomPoint < probs[i])      //if the random point is less than a probablity return that position
            {
                return i;
            }
            else
            {
                randomPoint -= probs[i];     //else subtract that probability from the randompoint
            }
        }
        return probs.Length - 1;
    }

    private void Start()
    {
        currentSpawnNumber = numberOfSpawns;
    }

    private void Update()
    {
        if (currentSpawnNumber != 0)
        {
            if (nextFire <= Time.time)
            {

                if (isRandom == true)
                {
                    nextFire = Time.time + minDelay + scalingDelay * Math.Pow(2, (-1 * scalingFactor * Time.time));
                    Instantiate(sortableTypes[Choose(sortableProbabilties)]);
                    currentSpawnNumber--;
                }
                else
                {
                    nextFire = Time.time + minDelay + scalingDelay * Math.Pow(2, (-1 * scalingFactor * Time.time));
                    Instantiate(sortableTypes[m_pattern[i] - 'A']);
                    currentSpawnNumber--;
                    i++;
                    if (i >= m_pattern.Length)
                    {
                        i = 0;
                    }
                }
            }
        }
    }
}
