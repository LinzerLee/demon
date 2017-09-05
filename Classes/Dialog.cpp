#include "Dialog.h"
#include "Util.h"

using std::string;
USING_NS_CC;

// on "init" you need to initialize your instance
bool Dialog::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }
    
    m_name = Label::createWithTTF("", "fonts/ZSXS.ttf", 18);
    CCASSERT(nullptr != m_name, "label create faild");
    m_name->setColor(Color3B::BLACK);
    
    m_content = Label::createWithTTF("", "fonts/ZSXS.ttf", 18);
    CCASSERT(nullptr != m_content, "label create faild");
    m_content->setColor(Color3B::BLACK);
    
    PreloadFrame("system");
    PreloadFrame("protrait");
    
    m_next = Sprite::create();
    CCASSERT(nullptr != m_next, "sprite create faild");
    
    Vector<SpriteFrame *> frames;
    Animation *animation;
    auto frameCache = SpriteFrameCache::getInstance();
    for (int i = 1; i <= 16; ++i)
    {
        string name = StringUtils::format("49-%d.png", i);
        frames.pushBack(frameCache->getSpriteFrameByName(name));
    }
    animation = Animation::createWithSpriteFrames(frames);
    animation->setLoops(-1);
    animation->setDelayPerUnit(0.1);
    m_next->runAction(Animate::create(animation));
    frames.clear();
    
    m_background = Sprite::createWithSpriteFrameName("dialog.png");
    CCASSERT(nullptr != m_background, "sprite create faild");
    
    m_portrait = Sprite::create();
    CCASSERT(nullptr != m_portrait, "sprite create faild");
    
    addChild(m_background);
    addChild(m_name);
    addChild(m_content);
    addChild(m_portrait);
    // addChild(m_next);
    
    Size vsize = Director::getInstance()->getVisibleSize();
    float ratio = vsize.width / m_background->boundingBox().size.width;
    m_background->setScale(ratio);
    Size bsize = m_background->boundingBox().size;
    setPosition(Point(vsize.width / 2, bsize.height / 2));
    
    m_name->setPosition(40 - (vsize.width / 2), 15);
    m_portrait->setPosition((vsize.width / 2) - 50, 15);
    // 修改锚点
    m_content->setAnchorPoint(ccp(0, 1));
    m_content->setMaxLineWidth(bsize.width - m_portrait->boundingBox().size.width);
    m_content->setPosition(-bsize.width / 2, 0);
    
    setVisible(false);
    return true;
}

void Dialog::show(string name, string content, string protrait)
{
    CHECK_GAME_STATE_OR_RETURN(GameMainRunning);
    SetGameState(GameState::GameInDialog);
    if ("" == protrait)
    {
        m_portrait->setVisible(false);
    }
    else
    {
        m_portrait->setSpriteFrame(protrait);
        m_portrait->setVisible(true);
    }
    m_name->setString(x2u(name));
    Size bsize = m_background->boundingBox().size;
    m_content->setMaxLineWidth(bsize.width - m_portrait->boundingBox().size.width);
    content = "    " + content;
    m_content->setString(x2u(content));
    setVisible(true);
}

void Dialog::hide()
{
    CHECK_GAME_STATE_OR_RETURN(GameInDialog);
    setVisible(false);
    SetGameState(GameState::GameMainRunning);
}
