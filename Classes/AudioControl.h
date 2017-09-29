#ifndef AUDIO_CONTROL_H_
#define AUDIO_CONTROL_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "..\extensions/GUI/CCControlExtension/CCControlSlider.h"

using namespace CocosDenshion;
using namespace cocos2d::extension;

class AudioControl:public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(AudioControl);
};

#endif // !AUDIO_CONTROL_H_
