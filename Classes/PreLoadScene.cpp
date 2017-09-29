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
	//���ø���ĳ�ʼ�������ĺ���
	if (!Layer::init())
		return false;
	//��ȡ�豸��ͼ��С
	Size vSize = Director::getInstance()->getVisibleSize();
	//����������
	auto barSprite = Sprite::create("progressbar.png");
	//��ʼ��һ��ProgressTimer����������
	_progress = ProgressTimer::create(barSprite);
	_progress->setPercentage(0.0f);
	_progress->setScale(0.25);
	//������ʼλ�ã�Ĭ���е㣬Ҫ���ô����ҵĶ���Ч�����������óɣ�0��y��
	_progress->setMidpoint(Vec2(0.0f, 0.5f));

	//��ʾ�Ƿ�ı�ˮƽ�ͻ��ߴ�ֱ�ı�����1��ʾ�ı䣬0��ʾ���ı�
	_progress->setBarChangeRate(Vec2(1.0f, 0.f));
	//���ν�����
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
	//��������size��ӵĵ���Ҫ����Դ����
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

