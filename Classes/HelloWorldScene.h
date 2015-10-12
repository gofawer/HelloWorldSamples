    #ifndef __HELLOWORLD_SCENE_H__
    #define __HELLOWORLD_SCENE_H__

    #include "cocos2d.h"
    #include <Box2D/Box2D.h>

    const static float PTM_RATIO = 32.0;

    class HelloWorld : public cocos2d::Layer
    {
    public:
        static cocos2d::Scene* createScene();

        virtual bool init();
        
        // a selector callback
        void menuCloseCallback(cocos2d::Ref* pSender);
        
        // implement the "static create()" method manually
        CREATE_FUNC(HelloWorld);
        
        b2World* _world;
        b2Body *_groundBody;
        
        void update(float dt);
    };

    #endif // __HELLOWORLD_SCENE_H__
