//
//  AttackTower.cpp
//  thiefTD
//
//  Created by cocos2d-x on 14-5-7.
//
//

#include "AttackTower.h"
#include "GameManager.h"

bool AttackTower::init()
{
	if (!TowerBase::init())
	{
		return false;
	}

	setScope(90);
	setTowerValue(200);
	setLethality(1);
	setRate(2);
	tower = Sprite::createWithSpriteFrameName("attackTower.png");
	this->addChild(tower);

	schedule(schedule_selector(AttackTower::shoot), 0.8f);
	return true;
}


Sprite* AttackTower::AttackTowerBullet()
{
	Sprite* bullet = Sprite::createWithSpriteFrameName("bullet1.png");
	bullet->setPosition(0, tower->getContentSize().height / 4);
	this->addChild(bullet);

	return bullet;
}

void AttackTower::shoot(float dt)
{
	GameManager *instance = GameManager::getInstance();
	auto bulletVector = instance->bulletVector;

	checkNearestEnemy();
	if (nearestEnemy != NULL && nearestEnemy->getCurrHp() > 0)
	{
		for (int i = 0; i < 10; i++)
		{
			auto currBullet = AttackTowerBullet();
			instance->bulletVector.pushBack(currBullet);

			auto moveDuration = getRate();

			Point shootVector;
			shootVector.x = 1;
			shootVector.y = tan(i * 2 * M_PI / 10);
			Point normalizedShootVector;
			if (i >= 10 / 2)
			{
				normalizedShootVector.x = shootVector.x;
				normalizedShootVector.y = shootVector.y;
			}
			else {
				normalizedShootVector.x = -shootVector.x;
				normalizedShootVector.y = -shootVector.y;
			}
			auto farthestDistance = Director::getInstance()->getWinSize().width;
			Point overshotVector = normalizedShootVector * farthestDistance;
			Point offscreenPoint = (currBullet->getPosition() - overshotVector);

			currBullet->runAction(Sequence::create(MoveTo::create(moveDuration, offscreenPoint),
				CallFuncN::create(CC_CALLBACK_1(AttackTower::removeBullet, this)),
				NULL));
			currBullet = NULL;
		}
	}
}

void AttackTower::removeBullet(Node* pSender)
{
	GameManager *instance = GameManager::getInstance();

	auto bulletVector = instance->bulletVector;

	Sprite *sprite = (Sprite *)pSender;
	instance->bulletVector.eraseObject(sprite);
	sprite->removeFromParent();
}