#include "GameScene.h"
#include "ui/CocosGUI.h"
#include "FKSprite.h"
#include "GameMenu.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

using namespace ui;
cocos2d::Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
		return false;
	Size vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	_screenWidth = vSize.width;
	_screenHeight = vSize.height;

	AnimationCache::getInstance()->addAnimation(getAnimationByName("plane", 0.08, 2), "fly");
	AnimationCache::getInstance()->addAnimation(getAnimationByName("blast", 0.09, 4), "blast");
	AnimationCache::getInstance()->addAnimation(getAnimationByName("bomb", 10.9, 4), "bomb");

	auto menuItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(GameScene::menuCloseCallback, this));
	menuItem->setPosition(Vec2(origin.x + vSize.width - menuItem->getContentSize().width / 2,
		origin.y + menuItem->getContentSize().height / 2));
	// create menu, it's an autorelease object
	auto menu = Menu::create(menuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	return true;
}

void GameScene::onEnter()
{
	Layer::onEnter();
	Sprite* bg1 = Sprite::create("bg1.png");
	bg1->setAnchorPoint(Vec2(0, 0));
	this->addChild(bg1, -1);
	_plane = Sprite::createWithSpriteFrameName("plane0.png");
	_plane->setPosition(Vec2(_screenWidth / 2, _plane->getContentSize().height / 2 + 5));
	auto planeFlyAnimation = AnimationCache::getInstance()->getAnimation("fly");
	auto animate = Animate::create(planeFlyAnimation);
	auto planeFlyAction = RepeatForever::create(animate);
	_plane->runAction(planeFlyAction);
	auto planeListener = EventListenerTouchOneByOne::create();
	planeListener->onTouchBegan = [](Touch* touch, Event* event) {
		//获取对象
		auto target = event->getCurrentTarget();
		//获取触摸对象OpenGL对象
		Vec2 location = touch->getLocation();
		Vec2 locationInNode = target->convertToNodeSpace(location);
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode)) {
			return true;
		}
		return false;
	};
	planeListener->onTouchMoved = [](Touch* touch, Event* event) {
		auto target = event->getCurrentTarget();
		target->setPosition(target->getPosition() + touch->getDelta());
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(planeListener, _plane);
	this->addChild(_plane);

#ifndef _BACK_
	//添加连续滚动背景
	std::string bgName;
	//自适应屏幕大小
	if (_screenHeight == 960) {
		bgName = "bg1.png";
	}
	else
	{
		bgName = "bg2.png";
	}


	//第一张背景
	_bg1 = Sprite::create(bgName);
	_bg1->setAnchorPoint(Vec2::ZERO);
	_bg1->getTexture()->setAliasTexParameters();
	this->addChild(_bg1);

	//第二张背景
	_bg2 = Sprite::create(bgName);
	_bg2->setAnchorPoint(Vec2::ZERO);
	_bg2->getTexture()->setAliasTexParameters();
	_bg2->setPosition(Vec2(0, _bg1->getContentSize().height));
	this->addChild(_bg2);
#endif

	this->scheduleUpdate();
}

Animation* GameScene::getAnimationByName(std::string animName, float delay, int animNum)
{
	auto animation = Animation::create();
	for (unsigned int i = 0; i < animNum; ++i) {
		std::string frameName = animName;
		frameName.append(StringUtils::format("%d", i)).append(".png");
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName.c_str()));
	}
	//设置动画播放
	animation->setDelayPerUnit(delay);
	animation->setRestoreOriginalFrame(true);
	return animation;
}

void GameScene::update(float delta)
{
	_count++;
	updateBackground();
	enemyPlaneDive(delta);
	removeEnemy(delta);
	shootBullet(delta);
	removeBullet(delta);
	collisionDetection(delta);
}

//更新背景图片
void GameScene::updateBackground()
{
	Vec2 p1 = _bg1->getPosition();
	p1.y = p1.y - 5;
	if (p1.y <= _bg1->getContentSize().height) {
		p1.y = _bg1->getContentSize().height;
	}

	_bg1->setPosition(p1);

	Vec2 p2 = _bg2->getPosition();
	p2.y = p2.y - 5;
	if (p2.y <= _bg2->getContentSize().height) {
		p2.y = _bg2->getContentSize().height;
	}

	_bg2->setPosition(p2);
}

void GameScene::menuCloseCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->end();
}

//更新敌机数量
void GameScene::enemyPlaneDive(float delta)
{
	bool bFlag = false;
	int lifeValue;
	//自定义精灵类FKSprite
	FKSprite* enemyPlane;
	//使用随机数设置飞机位置X坐标
	//int randX = arc4random() % (_screenWidth - 40) + 20;
	int randX = rand() % (_screenWidth - 40) + 20;

	//获取随机事件
	float durationTime = rand() % 2 + 2;

	//count 为 30倍数时添加一架敌机
	if (_count % 30 == 0) {
		int randNum = rand() % 2;
		switch (randNum)
		{
		case 0:
			enemyPlane = FKSprite::createWithSpriteFrameName("e0.png");
			enemyPlane->setTag(E0TAG);
			break;
		case 1:
			enemyPlane = FKSprite::createWithSpriteFrameName("e2.png");
			enemyPlane->setTag(E2TAG);
			break;
		default:
			break;
		}

		//设置敌机生命值，敌机图片比例
		lifeValue = 1;
		enemyPlane->setLifeValue(lifeValue);
		enemyPlane->setScale(0.6);
		bFlag = true;
	}
	else {
		if(_count % 200 == 0)
		{ 
			enemyPlane = FKSprite::createWithSpriteFrameName("e1.png");
			enemyPlane->setTag(E1TAG);
			enemyPlane->setScale(0.8);
			lifeValue = 10;
			enemyPlane->setLifeValue(lifeValue);
			bFlag = true;
		}
	}

	if (bFlag) {
		//设置敌机位置
		enemyPlane->setPosition(randX, _screenHeight + enemyPlane->getContentSize().height);
		//执行moveBy动作,高度是屏幕高度，加速飞机自身的高度
		auto moveBy = MoveBy::create(durationTime, Vec2(0, -enemyPlane->getPosition().y - enemyPlane->getContentSize().height));
		//敌机俯冲
		enemyPlane->runAction(moveBy);
		//将敌机添加到当前节点
		_enemyVector.pushBack(enemyPlane);
		this->addChild(enemyPlane);
		//为敌机 天机雪
		if (enemyPlane->getTag() == E1TAG)
		{
			//创建一个LoadingBar;
			auto loadingBar = LoadingBar::create("planeHP.png");
			loadingBar->setScale(0.15);
			//设置进度条方向
			loadingBar->setDirection(LoadingBar::Direction::LEFT);
			loadingBar->setPercent(100);
			Point pos = enemyPlane->getPosition();
			//设置坐标位置
			loadingBar->setPosition(Vec2(pos.x, pos.y + 48));
			//将LoadingBar添加到当前层的子节点
			this->addChild(loadingBar, 1);

			//设置执行moveBy
			auto moveBy2 = MoveBy::create(durationTime, Vec2(0, -pos.y - enemyPlane->getContentSize().height));
			loadingBar->runAction(moveBy2);
			//将进度条设置为敌机协调
			enemyPlane->setHP(loadingBar);
			enemyPlane->setHPInterval(100.0f / enemyPlane->getLifeValue());
		}
	}
}

void GameScene::removeEnemy(float delta)
{
	//便利删除敌人
	for(unsigned int i = 0; i < _enemyVector.size(); ++i)
	{
		//删除敌机精灵
		auto enemy = _enemyVector.at(i);
		if (enemy->getPosition().y <= -enemy->getContentSize().height)
		{
			this->removeChild(enemy, true);
			_enemyVector.eraseObject(enemy);
		}
	}
}

void GameScene::shootBullet(float delta)
{
	Vec2 pos = _plane->getPosition();
	if (_count % 8 == 0)
	{
		auto bullet = Sprite::createWithSpriteFrameName("bullet.png");
		bullet->setPosition(pos.x, pos.y + _plane->getContentSize().height / 2 + bullet->getContentSize().height);
		auto moveBy = MoveBy::create(0.4f, Vec2(0, _screenHeight - bullet->getPosition().y));
		bullet->runAction(moveBy);
		this->addChild(bullet, 4);
		_bulletVector.pushBack(bullet);
	}
}

//碰撞检测
void GameScene::collisionDetection(float delta)
{
	//遍历敌机
	for (unsigned int i = 0; i < _enemyVector.size(); ++i)
	{
		auto enemy = _enemyVector.at(i);
		//判断碰撞
		if (_plane->getBoundingBox().intersectsRect(enemy->getBoundingBox())) {
			playBombAnimate("blast", enemy->getPosition());
			_enemyVector.eraseObject(enemy);
			this->removeChild(enemy, true);

			//玩家所有动作停止
			_plane->stopAllActions();
			_plane->setVisible(false);
#ifndef ECLIPSE
			gameOver(GameMenu::GBKToUTF8("重新开始", "gb2312", "utf-8"));
#else
			gameOver("重新开始");
#endif
		}
		else
		{
			for (unsigned i = 0; i < _bulletVector.size(); ++i)
			{
				auto bullet = _bulletVector.at(i);
				if (enemy->getBoundingBox().intersectsRect(bullet->getBoundingBox()))
				{
					SimpleAudioEngine::getInstance()->playEffect("bullet.mp3");
					_bulletVector.eraseObject(bullet);
					this->removeChild(bullet, true);
					enemy->setLifeValue(enemy->getLifeValue() - 1);
					if (enemy->getTag() == E1TAG)
					{
						if (enemy->getHP() != nullptr) 
						{
							enemy->getHP()->setPercent(enemy->getHPInterval()*enemy->getLifeValue());
						}
					}

					//判断敌机的生命值
					if (enemy->getLifeValue() <= 0) {
						//播放爆炸画面
						playBombAnimate("blast", enemy->getPosition());
						SimpleAudioEngine::getInstance()->playEffect("b0.mp3");
						//杀出敌机精灵
						_enemyVector.eraseObject(enemy);
						this->removeChild(enemy, true);
					}
					break;
				}
			}
		}
	}
}

void GameScene::removeBullet(float delta)
{
	//循环子弹删除子弹
	for (int i = 0; i < _bulletVector.size(); ++i)
	{
		auto mbullet = _bulletVector.at(i);
		if (mbullet->getPositionY() >= _screenHeight)
		{
			_bulletVector.eraseObject(mbullet);
		}
	}
}

void GameScene::playBombAnimate(std::string strName, Vec2 pos)
{
	//获得爆炸画面
	Animation* blastAnimation;
	if (strName.compare("bomb") == 0)
	{
		blastAnimation = AnimationCache::getInstance()->getAnimation("bomb");
	}
	else
	{
		blastAnimation = AnimationCache::getInstance()->getAnimation("blast");
	}

	std::string bombName = strName + "0.png";
	//创建一个精灵
	auto blast = Sprite::createWithSpriteFrameName(bombName);
	blast->setPosition(pos);
	//获得动画帧
	Animation* blastAnimationFrame = getAnimationByName(strName, 0.5, 4);
	//组合动作，1播放动画， 播放完爆炸动画之后删除动画
	Animate* animate = Animate::create(blastAnimationFrame);
	CallFunc* callFunc = CallFunc::create([=] {
		this->removeChild(blast, true);
	});
	Action* action = Sequence::create(animate, callFunc, NULL);
	blast->runAction(action);
	//将精灵添加到精灵表单
	this->addChild(blast);
}

void GameScene::gameOver(std::string message)
{
	//停止毁掉更新
	this->unscheduleUpdate();
	auto resart_btn = Button::create("button.png");
	resart_btn->setScale(2);
	resart_btn->setTitleText(message);
#ifndef ECLIPSE
	resart_btn->setTitleFontName("微软雅黑");
#endif
	resart_btn->setTitleFontSize(16);
	resart_btn->setPosition(Vec2(_screenWidth/2, _screenHeight*0.6));
	resart_btn->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			this->restart(1);
		}
	});
	this->addChild(resart_btn);

	auto back_btn = Button::create("button.png");
	back_btn->setScale(2);
#ifndef ECLIPSE
	back_btn->setTitleText(GameMenu::GBKToUTF8("返回主菜单", "gb2312", "UTF-8"));
	back_btn->setTitleFontName("微软雅黑");
#endif
	back_btn->setTitleFontSize(16);
	back_btn->setPosition(Vec2(_screenWidth / 2, _screenHeight * 0.4));
	back_btn->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			this->restart(2);
		}
	});
	this->addChild(back_btn);
}

void GameScene::restart(int tag)
{
	if (tag == 1) {
		Director::getInstance()->replaceScene(GameScene::createScene());
	}
	else
	{
		Director::getInstance()->replaceScene(GameMenu::createScene());
	}
}

