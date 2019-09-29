using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class scoreboard : MonoBehaviour {

    public int score;
    public int lives;

    private TextMesh text;

	// Use this for initialization
	void Start () {
        score = 0;
        text = GetComponent<TextMesh>();
	}
	
	// Update is called once per frame
	void Update () {
        if (lives > 0)
        {
            text.text = "Score: " + score.ToString() + "\n" + "Lives: " + lives;
        }
        else
        {
            text.text = "Score: " + score.ToString() + "\n" + "Game over!";
        }
	}

    public void UpdateScore(int amount)
    {
        if (lives > 0)
            score = score + amount;
    }

    public void UpdateLives(int amount)
    {
        lives = lives + amount;
    }
}
