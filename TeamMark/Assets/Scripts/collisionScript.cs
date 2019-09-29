using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class collisionScript : MonoBehaviour
{

    public GameObject scriptholder;
    public char colorType;
    private gameController gc;

    void Start()
    {
        gc = scriptholder.GetComponent<gameController>();
    }

    // This is going to check whether the right object was put in the pipe and destroy that object
    void OnTriggerEnter(Collider col)
    {
        char coltrig = col.gameObject.GetComponent<sortableController>().m_type;
        if (coltrig != null)
            Destroy(col.gameObject);
        //destroy when green ball hits collider
        if (coltrig == colorType)
        {
            gc.UpdateScore(1, coltrig); // score is not updating?
        }
        else
        {
            gc.UpdateMistakes(1, coltrig);
        }
    }
}