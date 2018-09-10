#ifndef _SAMPLE_SCENE_H_
#define _SAMPLE_SCENE_H_

#include "framework\framework.h"

#include "subsystems\interfaces\IRenderer.h"

#include "subsystems\graphics\Scene.h"

using namespace OrisisEngine;

class SampleScene : public Scene{
public:
	SampleScene(Framework* framework);
	SampleScene(const SampleScene &SampleScene);
	~SampleScene();
	
	void OnSceneLoaded();

	void OnSceneClosed();
};

#endif