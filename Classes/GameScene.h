#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "base/CCVector.h"
#include "FKSprite.h"
#include "SimpleAudioEngine.h"

class FKSprite;
USING_NS_CC;

//��ͬ�л�tag
static int E0TAG = 10;
static int E1TAG = 11;
static int E2TAG = 12;
static int E3TAG = 100;

class GameScene :public cocos2d::Layer {
private:
	Sprite* _plane;
	int _screenWidth, _screenHeight;

	Sprite* _bg1;
	Sprite* _bg2;

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void onEnter();

	// ��ö�������
	Animation* getAnimationByName(std::string animName, float delay, int animNum);

	CREATE_FUNC(GameScene);

	void update(float delta);

	void updateBackground();

	int _count;
	cocos2d::Vector< FKSprite* > _enemyVector;

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	//���µл�����
	void enemyPlaneDive(float delta);

	//�л��뿪��Ļɾ������
	void removeEnemy(float delta);

	//�ӵ�
	Vector<Sprite*> _bulletVector;
	void shootBullet(float delta);
	void collisionDetection(float delta);
	void removeBullet(float delta);
	void playBombAnimate(std::string strName, Vec2 pos);
	void gameOver(std::string message);
	void restart(int tag);
};

#endif // !_GAME_SCENE_H_
