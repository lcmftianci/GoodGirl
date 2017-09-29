#include "GameMenu.h"
#include "ui/CocosGUI.h"
#include "PreLoadScene.h"
#include "AudioControl.h"
#ifndef ECLIPSE
#include "..\external\win32-specific\icon\include\iconv.h"
#endif
USING_NS_CC;
using namespace ui;


//创建场景
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



//出实话场景
bool GameMenu::init()
{
	//调用父类的初始化场景的函数
	if (!Layer::init())
		return false;


	//获取设备可是图大小
	Size vSize = Director::getInstance()->getVisibleSize();

	//开始按钮
	auto start_menu = Button::create("button.png");
	start_menu->setScale(2);

#ifndef ECLIPSE
	std::string strBtn = "开始游戏";
	start_menu->setTitleText(GBKToUTF8(strBtn, "gb2312", "utf-8").c_str());
#else
	start_menu->setTitleText("START");
#endif

	//start_menu->setTitleFontName("微软雅黑");
	start_menu->setTitleFontSize(16);

	start_menu->setPosition(Vec2(vSize.width / 2, vSize.height*0.8));
	//开始按钮触发点击事件
	start_menu->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			//切换到场景游戏前
			auto transition = TransitionSlideInL::create(0.5, PreLoad::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(start_menu);



	//游戏设置按钮
	auto set_menu = Button::create("button.png");
	set_menu->setScale(2);
#ifndef ECLIPSE
	set_menu->setTitleText(GBKToUTF8("游戏设置", "gb2312", "utf-8").c_str());
#else
	set_menu->setTitleText("Setting");
#endif
	set_menu->setTitleFontSize(16);
	//set_menu->setTitleFontName("微软雅黑");
	set_menu->setPosition(Vec2(vSize.width / 2, vSize.height*0.55));
	set_menu->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto transition = TransitionSlideInL::create(0.5, AudioControl::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});

	this->addChild(set_menu);

	//退出游戏按钮
	auto close_menu = Button::create("button.png");
	close_menu->setScale(2);
#ifndef ECLIPSE
	close_menu->setTitleText(GBKToUTF8("退出游戏", "gb2312", "utf-8"));
#else
	close_menu->setTitleText("Quit");
#endif
	
	//close_menu->setTitleFontName("微软雅黑");
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

	//添加公司名
#ifndef ECLIPSE
	auto banner_text = Text::create(GBKToUTF8("盘古科技", "gb2312", "utf-8").c_str(), "Arial-BoldMT", 32);
#else
	auto banner_text = Text::create("PAGUTECH", "Arial-BoldMT", 32);
#endif
	banner_text->setPosition(Vec2(vSize.width / 2, vSize.height*0.15));
	this->addChild(banner_text);

	//网址
	auto email_text = Text::create("www.pg.com", "Arial-BoldMT", 32);
	email_text->setPosition(Vec2(vSize.width / 2, vSize.height*0.1));
	this->addChild(email_text);

#if 0
#endif
	return true;
}

