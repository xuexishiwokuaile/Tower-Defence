//
//  LevelLayer.h
//  thiefTD
//
//  Created by cocos2d-x on 14-5-17.
//
//

#ifndef __thiefTD__LevelLayer__
#define __thiefTD__LevelLayer__

#include <iostream>
#include "cocos2d.h"  

USING_NS_CC; 

#define SHORTEST_SLIDE_LENGTH (20)

class LevelLayer: public Layer
{
private:
    
    int pageNode;
    int curPageNode;
    Point touchDownPoint;
    Point touchUpPoint;
    Point touchCurPoint;
    float WINDOW_WIDTH;
    float WINDOW_HEIGHT;
    void goToCurrNode();

public:
    LevelLayer();
    ~LevelLayer();
    
    virtual bool init();
    static cocos2d::Scene* createScene();
    CREATE_FUNC(LevelLayer);
    
    void menuCloseCallback(Ref* pSender);
    bool onTouchBegan(Touch *touch, Event  *event);
    void onTouchMoved(Touch *touch, Event  *event);
    void onTouchEnded(Touch *touch, Event  *event);
    void addNode(Node *level);
};
  
#endif /* defined(__thiefTD__LevelLayer__) */
