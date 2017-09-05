#include "SplashScene.h"
#include "SimpleAudioEngine.h"
#include "GameMainScene.h"
#include "BattleScene.h"

#if __cplusplus
extern "C" {
#endif
#include "lua/lua/lua.h"
#include "lua/lua/lualib.h"
#include "lua/lua/lauxlib.h"
#if __cplusplus
}
#endif

USING_NS_CC;

#define MENU_ITME_SEL1_COL  Color3B(216, 172, 80)
#define MENU_ITME_SEL2_COL  Color3B(232, 208, 96)
#define MENU_ITME_USEL_COL  Color3B(196, 184, 172)

Scene* SplashScene::createScene()
{
    return SplashScene::create();
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    scheduleUpdate();
    
    //////////////////////////////
    // 2. init splash background
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = (origin + visibleSize) / 2;
    
    auto backgroundSprite = Sprite::create("ui/splash.png");
    
    if (nullptr == backgroundSprite)
        return false;
    
    backgroundSprite->setPosition(center);
    addChild(backgroundSprite, 0);
    
    //////////////////////////////
    // 3. init splash menu
    m_newGameLabel = Label::createWithTTF(u8"新的征程", "fonts/ZSXS.ttf", 15 * 2);
    m_oldGameLabel = Label::createWithTTF(u8"再续前缘", "fonts/ZSXS.ttf", 15 * 2);
    
    m_newGameLabel->setColor(MENU_ITME_SEL1_COL);
    m_oldGameLabel->setColor(MENU_ITME_USEL_COL);
    
    auto newGameItem = MenuItemLabel::create(
                                             m_newGameLabel,
                                             CC_CALLBACK_1(SplashScene::switchGameMainScene, this));
    auto oldGameItem = MenuItemLabel::create(
                                             m_oldGameLabel,
                                             [=](Ref*pSender) { log(u8"再续前缘"); });
    
    newGameItem->setPosition(center);
    oldGameItem->setPosition(center - Vec2(0, 17) * 2);
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(newGameItem, oldGameItem, NULL);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, 1);
    
    //////////////////////////////
    // 4. append key event listener
    auto listener = EventListenerKeyboard::create();
    
    listener->onKeyReleased = CC_CALLBACK_2(SplashScene::onSelectSplashMenu, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    m_itemSelected = 0;
    
    return true;
}

Color3B Color3B_Lerp(Color3B from, Color3B to, float factor)
{
    if (factor < 0.f)
        factor = 0.f;
    else if (factor > 1.f)
        factor = 1.f;
    
    return Color3B(from.r+factor*(to.r-from.r),
                   from.g + factor*(to.g - from.g),
                   from.b + factor*(to.b - from.b));
}

void SplashScene::update(float dt)
{
    static int count = 0;
    static bool flag = true;
#define HALF_CYCLE 15
    
    if (flag)
        ++ count;
    else
        -- count;
    
    if (count == 0)
        flag = true;
    else if (count == HALF_CYCLE)
        flag = false;
    
    Color3B newColor = Color3B_Lerp(MENU_ITME_SEL1_COL, MENU_ITME_SEL2_COL, count / (float)HALF_CYCLE);
    
    if (0 == m_itemSelected)
        m_newGameLabel->setColor(newColor);
    else
        m_oldGameLabel->setColor(newColor);
}

void SplashScene::onEnterTransitionDidFinish()
{
    //////////////////////////////
    // play background music
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/M001.mp3", true);
}

void SplashScene::onExitTransitionDidStart()
{
    //////////////////////////////
    // stop background music
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

void SplashScene::switchGameMainScene(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(2, GameMainScene::createSceneWithLuaScript("百草地")));
}

void SplashScene::onSelectSplashMenu(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            m_itemSelected = (m_itemSelected + 1) % 2;
            break;
        default:
            break;
    }
    
    if (0 == m_itemSelected)
    {
        m_newGameLabel->setColor(MENU_ITME_SEL1_COL);
        m_oldGameLabel->setColor(MENU_ITME_USEL_COL);
    }
    else
    {
        m_newGameLabel->setColor(MENU_ITME_USEL_COL);
        m_oldGameLabel->setColor(MENU_ITME_SEL1_COL);
    }
}
