#ifndef _PRE_LOADSCENE_H_
#define _PRE_LOADSCENE_H_

#include <iostream>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

class PreLoad :public cocos2d::Layer {
public:
	//创建场景
	static cocos2d::Scene* createScene();

	//初始化
	virtual bool init();

	//进入舞台时调用
	virtual void onEnterTransitionDidFinish();

	//加载北京音乐
	void loadMusic(ValueVector musicValue);

	//加载游戏音效
	void loadEffect(ValueVector effectFiles);

	//加载精灵表单
	void loadSpriteSheets(ValueVector spriteSheets);

	//进度条
	void progressUpdate();

	CREATE_FUNC(PreLoad);

protected:
	//用来保存游戏需要加载的资源总数
	int _sourceCount;

	//进度条
	ProgressTimer* _progress;

	//进度条跟新次数
	float _progressInterval;
};
#endif
