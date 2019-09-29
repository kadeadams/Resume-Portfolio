using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class floorCollision : MonoBehaviour
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
            if (other.gameObject.GetComponent<ballCollision>().type != "BOMB")
            {
                Debug.Log("##########################Ball fell on floor!!!");
                scoreboard_.UpdateLives(-1);
                Destroy(other.gameObject);
            }
        }
    }
}
