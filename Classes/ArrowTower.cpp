//
//  ArrowTower.cpp
//  thiefTD
//
//  Created by cocos2d-x on 14-4-28.
//
//

#include "ArrowTower.h"
#include "GameManager.h"

bool ArrowTower::init()
{
	if (!TowerBase::init())
	{
		return false;
	}

	setScope(90);
	setLethality(2);
	setTowerValue(250);
	setRate(2);
	auto baseplate = Sprite::createWithSpriteFrameName("baseplate.png");
	addChild(baseplate);

	rotateArrow = Sprite::createWithSpriteFrameName("arrow.png");
	rotateArrow->setPosition(0, baseplate->getContentSize().height / 4);
	addChild(rotateArrow);

	schedule(schedule_selector(ArrowTower::rotateAndShoot), 0.8f);
	return true;
}


Sprite* ArrowTower::ArrowTowerBullet()
{
	Sprite* bullet = Sprite::createWithSpriteFrameName("arrowBullet.png");
	bullet->setPosition(rotateArrow->getPosition());
	bullet->setRotation(rotateArrow->getRotation());
	addChild(bullet);

	return bullet;
}

void ArrowTower::rotateAndShoot(float dt)
{
	checkNearestEnemy();
	if (nearestEnemy != NULL)
	{
		auto rotateVector = nearestEnemy->sprite->getPosition() - this->getPosition();
		auto rotateRadians = rotateVector.getAngle();
		auto rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians);

		auto speed = 0.5 / M_PI;
		auto rotateDuration = fabs(rotateRadians * speed);
		rotateArrow->runAction(Sequence::create(RotateTo::create(rotateDuration, rotateDegrees),
			CallFunc::create(CC_CALLBACK_0(ArrowTower::shoot, this)),
			NULL));
	}
}

void ArrowTower::shoot()
{
	GameManager *instance = GameManager::getInstance();
	auto bulletVector = instance->bulletVector;
	checkNearestEnemy();
	if (nearestEnemy != NULL && nearestEnemy->getCurrHp() > 0)
	{
		for (int i = 0; i < 10; i++)
		{
			auto currBullet = ArrowTowerBullet();
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
			Point offscreenPoint = (rotateArrow->getPosition() - overshotVector);

			currBullet->runAction(Sequence::create(MoveTo::create(moveDuration, offscreenPoint),
				CallFuncN::create(CC_CALLBACK_1(ArrowTower::removeBullet, this)),
				NULL));
			currBullet = NULL;
		}
	}
}

void ArrowTower::removeBullet(Node* pSender)
{
	GameManager *instance = GameManager::getInstance();

	auto bulletVector = instance->bulletVector;

	Sprite *sprite = (Sprite *)pSender;
	instance->bulletVector.eraseObject(sprite);
	sprite->removeFromParent();
}