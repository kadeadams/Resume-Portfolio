using UnityEngine;
using UnityEngine.SceneManagement;

//to do: passes patterns (string of array indices) to spawn controller if not randomized
// has information on number of objects to be sorted, number of objects able to be missed, time remaining, and spawning sequence
// for now, omit level select logic


public class gameController : MonoBehaviour {
    //level info -> shove this in a class and have an array of it?
    public string pattern; //contains information of #objects
    public int mistakesAllowed;
    public int needToSort;
    public float timeAlloted; //in seconds

    //score scene data
    public string scoreBoardLevel;
    private string sortedObjectOrder = "";
    private string mistakeObjectOrder = "";

    //interface with bin collider
    public GameObject scriptHolder;
    public GameObject SortableSpawner;
    public int sorted;
    public int mistakes;
    public int StartTimerDelay;

    //score display
    public TextMesh sortedText;
    public TextMesh timeText;
    public TextMesh mistakeText;
    private float curTime;
    private float startTime;


	// Use this for initialization
	void Start () {
        startTime = Time.time;
        curTime = timeAlloted;
        mistakes = 0;
        SortableSpawner.SetActive(false);

	}
	
	// Update is called once per frame
	void Update () {
        //score
        sortedText.text = "Sorted: " + sorted.ToString() + " / " + needToSort;

        //time 
        if (Time.time > startTime + StartTimerDelay) {
            curTime -= Time.deltaTime;
            int intCurTime = (int)curTime;
            int timeMin = intCurTime / 60, timeSec = intCurTime % 60;
            timeText.text = "Time: " + timeMin.ToString("D2") + ":" + timeSec.ToString("D2");
            SortableSpawner.SetActive(true);
            if (timeMin <= 0 && timeSec <= 0)
                SceneManager.LoadScene(scoreBoardLevel, LoadSceneMode.Single);
        }
        

        //mistakes

        mistakeText.text = "Mistakes: " + mistakes.ToString() + " / " + mistakesAllowed.ToString();

        if (mistakes >= mistakesAllowed)
        {
            SceneManager.LoadScene(scoreBoardLevel, LoadSceneMode.Single);
        }

    }

    public string getPattern() 
    {
        return pattern;
    }

    public void UpdateScore(int amount, char type)
    {
        //if (mistakes > 0)
            sorted = sorted + amount;
        sortedObjectOrder += type; 
    }

    public void UpdateMistakes(int amount, char type)
    {
        mistakes = mistakes + amount;
        mistakeObjectOrder += type;
    }
}
