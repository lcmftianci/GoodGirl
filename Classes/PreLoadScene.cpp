#include "PreloadScene.h"
#include "GameScene.h"
USING_NS_CC;

cocos2d::Scene* PreLoad::createScene()
{
	auto scene = Scene::create();
	auto layer = PreLoad::create();
	scene->addChild(layer);
	return scene;
}

bool PreLoad::init()
{
	//调用父类的初始化场景的函数
	if (!Layer::init())
		return false;
	//获取设备视图大小
	Size vSize = Director::getInstance()->getVisibleSize();
	//创建进度条
	auto barSprite = Sprite::create("progressbar.png");
	//初始化一个ProgressTimer进度条对象
	_progress = ProgressTimer::create(barSprite);
	_progress->setPercentage(0.0f);
	_progress->setScale(0.25);
	//设置起始位置，默认中点，要想获得从左到右的动画效果，必须设置成（0，y）
	_progress->setMidpoint(Vec2(0.0f, 0.5f));

	//表示是否改变水平和或者垂直的比例，1表示改变，0表示不改变
	_progress->setBarChangeRate(Vec2(1.0f, 0.f));
	//条形进度条
	_progress->setType(ProgressTimer::Type::BAR);
	_progress->setPosition(vSize.width / 2, vSize.height / 2);
	this->addChild(_progress);
	return true;
}

void PreLoad::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	ValueMap map = FileUtils::getInstance()->getValueMapFromFile("preloadResources.plist");
	ValueVector spriteSheets = map.at("SpriteSheets").asValueVector();
	ValueVector effects = map.at("Sounds").asValueVector();
	ValueVector musics = map.at("Musics").asValueVector();
	//多个数组的size相加的到需要的资源总量
	_sourceCount = spriteSheets.size() + effects.size() + musics.size();
	_progressInterval = 100 / _sourceCount;
	loadMusic(musics);
	loadEffect(effects);
	loadSpriteSheets(spriteSheets);
}

void PreLoad::loadMusic(ValueVector musicValue)
{
	for (unsigned int i = 0; i < musicValue.size(); ++i)
	{
		Value v = musicValue.at(i);
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic(v.asString().c_str());
		progressUpdate();
	}
}

void PreLoad::loadEffect(ValueVector effectFiles)
{
	for (unsigned int i = 0; i < effectFiles.size(); ++i)
	{
		Value v = effectFiles.at(i);
		SimpleAudioEngine::getInstance()->preloadEffect(v.asString().c_str());
		progressUpdate();
	}
}

void PreLoad::loadSpriteSheets(ValueVector spriteSheets)
{
	for (unsigned int i = 0; i < spriteSheets.size(); ++i)
	{
		Value v = spriteSheets.at(i);
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(v.asString().c_str());
		progressUpdate();
	}
}

void PreLoad::progressUpdate()
{
	if (--_sourceCount) {
		_progress->setPercentage(100.0f - (_progressInterval * _sourceCount));
	}
	else
	{
		auto pft = ProgressFromTo::create(0.5f, _progress->getPercentage(), 100);
		auto callFunc = CallFunc::create([=] {
			auto delay = DelayTime::create(2.0f);
			auto callFunc = CallFunc::create([]{
				Director::getInstance()->replaceScene(GameScene::createScene());
			});
			auto action = Sequence::create(delay, callFunc, NULL);
			this->runAction(action);
		});
		auto action = Sequence::create(pft, callFunc, NULL);
		_progress->runAction(action);
	}
}

