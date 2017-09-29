#ifndef _GAME_MENU_H_
#define _GAME_MENU_H_

#define ECLIPSE
//#undef ECLIPSE

#include <iostream>
#include "cocos2d.h"

//程序菜单类
class GameMenu :public cocos2d::Layer
{
public:
	//创建场景
	static cocos2d::Scene* createScene();
#ifndef ECLIPSE
	static std::string GBKToUTF8(const std::string& gbkStr, const char* toCode, const char* fromCode);
#endif // NO_ECLIPSE

	//初始化场景
	virtual bool init();

	//宏创建当前对象，返回对象将会由由自动释放池去释放
	CREATE_FUNC(GameMenu);
};

#endif
