using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using System;

namespace Valve.VR.InteractionSystem.Sample
{
    public class MenuClick : MonoBehaviour
    {
        public string levelName;
        // Start is called before the first frame update
        public void OnButtonDown(Hand fromHand)
        {
            //ColorSelf(Color.blue);
            //fromHand.TriggerHapticPulse(1000);
            SteamVR_LoadLevel.Begin(levelName);
        }

        private void ColorSelf(Color newColor)
        {
            Renderer[] renderers = this.GetComponentsInChildren<Renderer>();
            for (int rendererIndex = 0; rendererIndex < renderers.Length; rendererIndex++)
            {
                renderers[rendererIndex].material.color = newColor;
            }
        }


    }
}
