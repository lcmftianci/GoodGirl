#ifndef _PRE_LOADSCENE_H_
#define _PRE_LOADSCENE_H_

#include <iostream>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

class PreLoad :public cocos2d::Layer {
public:
	//��������
	static cocos2d::Scene* createScene();

	//��ʼ��
	virtual bool init();

	//������̨ʱ����
	virtual void onEnterTransitionDidFinish();

	//���ر�������
	void loadMusic(ValueVector musicValue);

	//������Ϸ��Ч
	void loadEffect(ValueVector effectFiles);

	//���ؾ����
	void loadSpriteSheets(ValueVector spriteSheets);

	//������
	void progressUpdate();

	CREATE_FUNC(PreLoad);

protected:
	//����������Ϸ��Ҫ���ص���Դ����
	int _sourceCount;

	//������
	ProgressTimer* _progress;

	//���������´���
	float _progressInterval;
};
#endif
