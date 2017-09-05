#include "Tip.h"
#include "Util.h"

using std::string;
USING_NS_CC;

// on "init" you need to initialize your instance
bool Tip::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }
    
    m_label = Label::createWithTTF("", "fonts/ZSXS.ttf", 24);
    CCASSERT(nullptr != m_label, "label create faild");
    m_label->setColor(Color3B::BLACK);
    m_sprite = Sprite::create("sprite/tip.png");
    CCASSERT(nullptr != m_sprite, "label create faild");
    m_sprite->addChild(m_label);
    Point pos = m_sprite->boundingBox().size / 2;
    m_label->setPosition(pos);
    addChild(m_sprite);
    return true;
}

void Tip::show(string content, float timeout, Point pos)
{
    setVisible(true);
    m_label->setString(x2u(content));
    if (pos == Point(-1, -1))
    {
        pos = Director::getInstance()->getVisibleSize() / 2;
    }
    setPosition(pos);
    scheduleOnce(schedule_selector(Tip::hide), timeout);
}

void Tip::hide(float dt)
{
    setVisible(false);
    removeFromParent();
}

Size Tip::size()
{
    return m_sprite->boundingBox().size;
}
