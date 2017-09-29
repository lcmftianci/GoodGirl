#ifndef _FK_SPRITE_H_
#define _FK_SPRITE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
//#include "CCPlatformMacros.h"

#define CC_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

USING_NS_CC;
using namespace ui;

class FKSprite:public cocos2d::Sprite
{
public:
	static FKSprite* createWithSpriteFrameName(const std::string& spriteFrameName)
	{
		FKSprite* sprite = new FKSprite();
		if (sprite && sprite->initWithSpriteFrameName(spriteFrameName))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	CC_SYNTHESIZE(int, _lifeValue, LifeValue);
	CC_SYNTHESIZE(LoadingBar*, _HP, HP);
	CC_SYNTHESIZE(float, _HPInterval, HPInterval);


//	void setLifeValue(int lifeValue) {
//		_lifeValue = lifeValue;
//	}
//
//	int getLifeValue() { return _lifeValue };
//
//private:
//	int _lifeValue;
//	LoadingBar* _HP;
//	float _HPInterval;

};
#endif