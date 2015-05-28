#include "HelloWorldScene.h"

USING_NS_CC; //macro expands to `using namespace cocos2d;`

Scene* HelloWorld::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() ){
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	/* REGISTERING TOUCH EVENTS
	 ===================================================== */
	auto controls = EventListenerTouchAllAtOnce::create();
	controls->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(controls, this);

	/*  NOTE: don't get in the habit of using auto everywhere
		An iron price to be paid (extra time == life) masquerading as convenience
		No reference can be made with auto and makes code unreadable
		Justified here since controls is a one-off and will not
		be referenced anywhere else but in this temporal block
		
		ALSO: This is what is called a observer pattern in the industry
		controls would be an "interface" of known `subscribers` to the _eventDispatcher `publisher` 
		and events are "published" to the subscribers as they happen. Interface just means a 
		contractual agreement between different parts of code to follow a uniform language
	*/

	/* CREATE A TMXTILEDMAP AND EXTRACT LAYER FOR DISPLAY
	 ===================================================== */
	_tileMap = TMXTiledMap::create("TileMap.tmx");
	// DEPRECATED: _tileMap = new CCTMXTiledMap();
	//			   _tileMap->initWithTMXFile("TileMap.tmx");

	_background = _tileMap->getLayer("Background");
	// DEPRECATED: _tileMap->layerNamed("Background");

	_meta = _tileMap->layerNamed("Meta");
	_meta->setVisible(false);

	this->addChild(_tileMap);

	TMXObjectGroup *objectGroup = _tileMap->objectGroupNamed("Objects");
	// DEPRECATED: CCTMXObjectGroup *objectGroup = _tileMap->objectGroupNamed("Objects");

	if(objectGroup == NULL) {
		CCLOG("tile map has no Objects object layer");
		// DEPRECATED: CCLog("tile map has no objects object layer");
		return false;
	}

	ValueMap spawnPoint = objectGroup->objectNamed("SpawnPoint");
	// DEPRECATED:	CCDictionary *spawnPoint = objectGroup->objectNamed("SpawnPoint");
	Vec2 spawnHere = Point(300, 300);
	if(spawnPoint.size() != 0) {
		CCLOG("LOGCAT!!! There is a spawn point");
	} else {
		CCLOG("LOGCAT!!! There isn't a spawn point. Using default 300 x 300.");
	}

	_player = Sprite::create("Player.png");
	_player->setPosition(spawnHere);

	this->addChild(_player);
	this->setViewPointCenter(_player->getPosition());

	this->setTouchEnabled(true);

	/////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    // create menu, it's an autorelease object
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void HelloWorld::onTouchesBegan(const std::vector<cocos2d::Touch*> &touches, Event *event) {
	//Does nothing
}

void HelloWorld::onTouchesEnded(const  std::vector<cocos2d::Touch*> &touches, Event *event) {
	for(Touch *touch : touches) {
		Point playerPos = _player->getPosition();
		Point touchLocation = touch->getLocationInView();
		//DEPRECATED: CCPoint touchLocation = touch->getLocationInView();
		touchLocation = Director::getInstance()->convertToGL(touchLocation);
		touchLocation = (Point)this->convertToNodeSpace(touchLocation);

		Point *diff = new Vec2(0.f, 0.f);
		Point::subtract(touchLocation, playerPos, diff);

		if ( abs(diff->x) > abs(diff->y) ) {
			if (diff->x > 0) {
				playerPos.x += _tileMap->getTileSize().width;
			} else {
				playerPos.x -= _tileMap->getTileSize().width;
			}
		} else {
			if (diff->y > 0) {
				playerPos.y += _tileMap->getTileSize().height;
			} else {
				playerPos.y -= _tileMap->getTileSize().height;
			}
		}
		delete diff;

		// safety check on the bounds of the map
		if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
			playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
			playerPos.y >= 0 &&
			playerPos.x >= 0 ) {
			this->setPlayerPosition(playerPos);
		}
    
		this->setViewPointCenter(_player->getPosition());
	}
}

void HelloWorld::setPlayerPosition(Point position) {
	// This part is completely different from version 2.X
	// There might be a better way to do this since this is what made sense
	// in the moment
	cocos2d::Point tileCoord = this->tileCoordForPosition(position);
	int tileFUCKEDUP = _meta->tileGIDAt(tileCoord);
	if(tileFUCKEDUP) {
		Value shit = _tileMap->propertiesForGID(tileFUCKEDUP);
		ValueMap doubleShit = shit.asValueMap();
		auto temp = doubleShit.find("Collidable");
		CCLOG(( temp->second.asBool() ? "Yes" : "No" ));
	}
	
	_player->setPosition(position);
}

void HelloWorld::setViewPointCenter(Point position) {
	Size winSize = Director::getInstance()->getWinSize();

    int x = MAX(position.x, winSize.width/2);
    int y = MAX(position.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
    CCPoint actualPosition = ccp(x, y);
    
    CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
    this->setPosition(viewPoint);
}

cocos2d::Point HelloWorld::tileCoordForPosition(cocos2d::Point position){
    int x = position.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
    return cocos2d::Point(x, y);
}
