using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class pipeCollision : MonoBehaviour
{
    public string currentColor = "RED";

    public GameObject scriptHolder;
    private scoreboard scoreboard_;

    void Awake()
    {
        //scriptHolder = GameObject.Find("Scoreboard");
        
    }

    void OnTriggerEnter(Collider other)
    {
        if (other.tag == "Ball")
        {
            scoreboard scoreboard_ = scriptHolder.GetComponent<scoreboard>();
            if (other.gameObject.GetComponent<ballCollision>().type == currentColor)
            {
                Debug.Log("##########################Collision with correct color");
                scoreboard_.UpdateScore(100);
            }
            else
            {
                Debug.Log("##########################Collision with INCORRECT color");
                scoreboard_.UpdateScore(-500);
                scoreboard_.UpdateLives(-1);
            }
        }
    }
}
