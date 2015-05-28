#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <iostream>

class HelloWorld : public cocos2d::Layer {
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    //static CCScene* scene() - This is what is call a factory maker method in the industry

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    //No analog version in v3.2, Use CC_CALLBACK_2 with EventListenerTouch#### to register touch ctrls
    //DEPRECATED::void registerWithTouchDispatcher();

    void onTouchesBegan(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event);
    //DEPRECATED::bool ccTouchBegan(cocos2d::Touch *touch, cocos2d::CCEvent *event);

    void onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event);
    //DEPRECATED::void ccTouchEnded(cocos2d::Touch *touch, cocos2d::CCEvent *event);

    // implement the "static create()" method manually
    // macro expansion calls init() and marks macro created object with autorelease
    CREATE_FUNC(HelloWorld);


    /* PURE CUSTOM CLASS FUNCTIONS, no relation to cocos2d heirarchy
     ==================================================================== */
    void setPlayerPosition(cocos2d::Point position);
    void setViewPointCenter(cocos2d::Point position);

    /* TileMap PT II 
     ================================= */
    cocos2d::Point tileCoordForPosition(cocos2d::Point position);

private:
    /* TileMap PT I
     ================================= */	
    cocos2d::TMXTiledMap *_tileMap;
    //DEPRECATED::CCTMXTiledMap 
    cocos2d::TMXLayer *_background;
    //DEPRECATED::CCTMXLayer
    cocos2d::Sprite *_player;
    //DEPRECATED::CCSprite

    /* TileMap PT II
     ================================= */
    cocos2d::TMXLayer *_meta;
    //DEPRECATED::CCTMXLayer
    cocos2d::TMXLayer *_foreground;
    //DEPRECATED::CCTMXLayer
};

#endif // __HELLOWORLD_SCENE_H__
