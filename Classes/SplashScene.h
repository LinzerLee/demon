#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "base/CCEventKeyboard.h"
#include "base/CCEvent.h"

class SplashScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    // a selector callback
    void switchGameMainScene(cocos2d::Ref* pSender);
    virtual bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(SplashScene);
private:
    void onSelectSplashMenu(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    virtual void update(float dt) override;
    // Scene生命周期
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
    
    int m_itemSelected;
    cocos2d::Label *m_newGameLabel;
    cocos2d::Label *m_oldGameLabel;
};

#endif // __SPLASH_SCENE_H__
