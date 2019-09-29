using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using Valve.VR;



public class LoadSceneOnClick : MonoBehaviour
{
   public void LoadByIndex(int sceneIndex)
    {
        Scene scene = SceneManager.GetSceneAt(sceneIndex);
        string levelName = scene.name;
        SteamVR_LoadLevel.Begin(levelName, false, 0.5F, 0F, 0F, 0F, 1F);
        //SceneManager.LoadScene(sceneIndex, LoadSceneMode.Single);
    }
}
