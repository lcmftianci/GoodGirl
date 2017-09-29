#include "GameMenu.h"
#include "ui/CocosGUI.h"
#include "PreLoadScene.h"
#include "AudioControl.h"
#ifndef ECLIPSE
#include "..\external\win32-specific\icon\include\iconv.h"
#endif
USING_NS_CC;
using namespace ui;


//��������
cocos2d::Scene* GameMenu::createScene()
{
	auto scene = Scene::create();
	auto layer = GameMenu::create();
	//CCLog("----------------------------------Is there-------------------------");
	scene->addChild(layer);
	return scene;
}

#ifndef ECLIPSE
std::string GameMenu::GBKToUTF8(const std::string& gbkStr, const char* toCode, const char* fromCode)
{
	iconv_t iconvH;
	iconvH = iconv_open(fromCode, toCode);
	if (iconvH == 0)
	{
		return "";
	}

	const char * strChar = gbkStr.c_str();
	const char** pin = &strChar;
	size_t strLength = gbkStr.length();
	char* outBuf = (char*)malloc(strLength * 4);
	char *pBuf = outBuf;
	memset(outBuf, 0, strLength * 4);
	size_t outLength = strLength * 4;

	if (-1 == iconv(iconvH, pin, &strLength, &outBuf, &outLength)) {
		iconv_close(iconvH);
		return "";
	}

	std::string gbkStr1 = pBuf;
	iconv_close(iconvH);
	return gbkStr1;
}
#endif // !ECLIPSE



//��ʵ������
bool GameMenu::init()
{
	//���ø���ĳ�ʼ�������ĺ���
	if (!Layer::init())
		return false;


	//��ȡ�豸����ͼ��С
	Size vSize = Director::getInstance()->getVisibleSize();

	//��ʼ��ť
	auto start_menu = Button::create("button.png");
	start_menu->setScale(2);

#ifndef ECLIPSE
	std::string strBtn = "��ʼ��Ϸ";
	start_menu->setTitleText(GBKToUTF8(strBtn, "gb2312", "utf-8").c_str());
#else
	start_menu->setTitleText("START");
#endif

	//start_menu->setTitleFontName("΢���ź�");
	start_menu->setTitleFontSize(16);

	start_menu->setPosition(Vec2(vSize.width / 2, vSize.height*0.8));
	//��ʼ��ť��������¼�
	start_menu->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			//�л���������Ϸǰ
			auto transition = TransitionSlideInL::create(0.5, PreLoad::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(start_menu);



	//��Ϸ���ð�ť
	auto set_menu = Button::create("button.png");
	set_menu->setScale(2);
#ifndef ECLIPSE
	set_menu->setTitleText(GBKToUTF8("��Ϸ����", "gb2312", "utf-8").c_str());
#else
	set_menu->setTitleText("Setting");
#endif
	set_menu->setTitleFontSize(16);
	//set_menu->setTitleFontName("΢���ź�");
	set_menu->setPosition(Vec2(vSize.width / 2, vSize.height*0.55));
	set_menu->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto transition = TransitionSlideInL::create(0.5, AudioControl::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});

	this->addChild(set_menu);

	//�˳���Ϸ��ť
	auto close_menu = Button::create("button.png");
	close_menu->setScale(2);
#ifndef ECLIPSE
	close_menu->setTitleText(GBKToUTF8("�˳���Ϸ", "gb2312", "utf-8"));
#else
	close_menu->setTitleText("Quit");
#endif
	
	//close_menu->setTitleFontName("΢���ź�");
	close_menu->setTitleFontSize(16);
	close_menu->setPosition(Vec2(vSize.width / 2, vSize.height*0.3));
	close_menu->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
				MessageBox("You pressed the close button, window app store do not implement a close button", "Alert");
				return;
#endif
				Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0)
#endif
		}
	});
	this->addChild(close_menu);

	//��ӹ�˾��
#ifndef ECLIPSE
	auto banner_text = Text::create(GBKToUTF8("�̹ſƼ�", "gb2312", "utf-8").c_str(), "Arial-BoldMT", 32);
#else
	auto banner_text = Text::create("PAGUTECH", "Arial-BoldMT", 32);
#endif
	banner_text->setPosition(Vec2(vSize.width / 2, vSize.height*0.15));
	this->addChild(banner_text);

	//��ַ
	auto email_text = Text::create("www.pg.com", "Arial-BoldMT", 32);
	email_text->setPosition(Vec2(vSize.width / 2, vSize.height*0.1));
	this->addChild(email_text);

#if 0
#endif
	return true;
}

