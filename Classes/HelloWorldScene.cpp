    #include "HelloWorldScene.h"

    USING_NS_CC;

    Scene* HelloWorld::createScene()
    {
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
    bool HelloWorld::init()
    {
        //////////////////////////////
        // 1. super init first
        if ( !Layer::init() )
        {
            return false;
        }
        
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        /////////////////////////////
        // 2. add a menu item with "X" image, which is clicked to quit the program
        //    you may modify it.

        // add a "close" icon to exit the progress. it's an autorelease object
        auto closeItem = MenuItemImage::create(
                                               "CloseNormal.png",
                                               "CloseSelected.png",
                                               CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
        
        
        closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                    origin.y + closeItem->getContentSize().height/2));

        // create menu, it's an autorelease object
        auto menu = Menu::create(closeItem, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);

        /////////////////////////////
        // 3. add your codes below...

        // add a label shows "Hello World"
        // create and initialize a label
        
        auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
        
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);

        // add "HelloWorld" splash screen"
        auto sprite = Sprite::create("HelloWorld.png");

        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
        
        /////////////////////
        
        b2Vec2 gravity;
        gravity.Set(0.0f, -10.0f);
        _world = new b2World(gravity);

        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0,0);
        _groundBody = _world->CreateBody(&groundBodyDef);
        
        b2EdgeShape groundBox;
        b2FixtureDef groundBoxDef;
        groundBoxDef.shape = &groundBox;
        
        groundBox.Set(b2Vec2(origin.x / PTM_RATIO, origin.y / PTM_RATIO), b2Vec2( (origin.x + visibleSize.width)/PTM_RATIO, origin.y / PTM_RATIO));
        b2Fixture * fixture = _groundBody->CreateFixture(&groundBoxDef);
        
        groundBox.Set(b2Vec2(origin.x / PTM_RATIO, origin.y / PTM_RATIO), b2Vec2(origin.x / PTM_RATIO   , (origin.y + visibleSize.height) / PTM_RATIO));
        _groundBody->CreateFixture(&groundBoxDef);
        
        groundBox.Set(b2Vec2(origin.x  / PTM_RATIO, (origin.y + visibleSize.height) / PTM_RATIO),
                      b2Vec2((origin.x + visibleSize.width) / PTM_RATIO, (origin.y + visibleSize.height) / PTM_RATIO));
        _groundBody->CreateFixture(&groundBoxDef);
        
        groundBox.Set(b2Vec2( (origin.x + visibleSize.width) / PTM_RATIO, (origin.y + visibleSize.height) / PTM_RATIO),
                      b2Vec2( (origin.x + visibleSize.width) / PTM_RATIO, origin.y / PTM_RATIO));
        _groundBody->CreateFixture(&groundBoxDef);
        
        b2Vec2 center = b2Vec2( (origin.x + visibleSize.width * 0.5) / PTM_RATIO, (origin.y + visibleSize.height * 0.5) / PTM_RATIO);

        for (int i=0; i<10; i++)
        {
            Sprite *ball = Sprite::create("CloseNormal.png");
            ball->setTag(1);
            this->addChild(ball);
            
            b2BodyDef ballBodyDef;
            ballBodyDef.type = b2_dynamicBody;
            ballBodyDef.position.Set(10.0 / PTM_RATIO * ((float)i + 1.0), center.y);
            ballBodyDef.userData = ball;
            
            b2Body *ballBody = _world->CreateBody(&ballBodyDef);
            
            b2CircleShape circle;
            circle.m_radius = ball->getContentSize().width * 0.5 / PTM_RATIO;
            
            b2FixtureDef ballShapeDef;
            ballShapeDef.shape = &circle;
            ballShapeDef.density = 1.0f;
            ballShapeDef.friction = 0.0f;
            ballShapeDef.restitution = 1.0f;
            b2Fixture * ballFixture = ballBody->CreateFixture(&ballShapeDef);
        }

        this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::update));
        
        return true;
    }

    void HelloWorld::update(float dt)
    {
        int velocityIterations = 5;
        int positionIterations = 1;
        
        _world->Step(dt, velocityIterations, positionIterations);
        
        for (b2Body* body = _world->GetBodyList(); body; body = body->GetNext())
        {
            if (body->GetUserData() != NULL) {

                Sprite* sprite = (Sprite*)body->GetUserData();
                Vec2 position = Vec2( body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO) ;
                CCLOG("Position:%.2f,%.2f", position.x, position.y);
                sprite->setPosition(position );
                sprite->setRotation( -1 * CC_RADIANS_TO_DEGREES(body->GetAngle()) );
            }
        }
    }

    void HelloWorld::menuCloseCallback(Ref* pSender)
    {
        Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
    #endif
    }
