#ifndef _GAME_MENU_H_
#define _GAME_MENU_H_

#define ECLIPSE
//#undef ECLIPSE

#include <iostream>
#include "cocos2d.h"

//����˵���
class GameMenu :public cocos2d::Layer
{
public:
	//��������
	static cocos2d::Scene* createScene();
#ifndef ECLIPSE
	static std::string GBKToUTF8(const std::string& gbkStr, const char* toCode, const char* fromCode);
#endif // NO_ECLIPSE

	//��ʼ������
	virtual bool init();

	//�괴����ǰ���󣬷��ض��󽫻������Զ��ͷų�ȥ�ͷ�
	CREATE_FUNC(GameMenu);
};

#endif
