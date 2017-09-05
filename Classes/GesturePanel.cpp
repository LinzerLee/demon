#include "GesturePanel.h"

USING_NS_CC;
USING_NS_CC_EXT;

JoyRocker::Direction GesturePanel::GestureEvent::getDirection()
{
    return m_direction;
}

void GesturePanel::GestureEvent::setDirection(JoyRocker::Direction dire)
{
    m_direction = dire;
}

GesturePanel::GestureEvent::GestureEventType GesturePanel::GestureEvent::getType()
{
    return m_type;
}

void GesturePanel::GestureEvent::setType(GestureEventType type)
{
    m_type = type;
}

bool GesturePanel::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    scheduleUpdate();
    //////////////////////////////
    // 2. init joy rocker
    m_joyRocker = JoyRocker::create();
    m_joyRocker->start();
    addChild(m_joyRocker);
    Point m_joyRockerPos = m_joyRocker->getContentSize() / 2;
    m_joyRocker->setPosition(m_joyRockerPos);
    
    //////////////////////////////
    // 2. init check button
    m_checkButton = ui::Button::create("ui/joystick_check.png", "ui/joystick_check_down.png");
    Size visiableSize = Director::getInstance()->getVisibleSize();
    m_checkButton->setPosition(Point(visiableSize.width - m_joyRockerPos.x, m_joyRockerPos.y));
    addChild(m_checkButton);
    m_checkButton->addTouchEventListener(CC_CALLBACK_2(GesturePanel::checkButtonTouchCallBack,this));
    
    return true;
}

void GesturePanel::setOpacity(GLubyte opacity)
{
    m_joyRocker->setOpacity(opacity);
    m_checkButton->setOpacity(opacity);
}

void GesturePanel::checkButtonTouchCallBack(Ref *pSender, ui::Widget::TouchEventType type)
{
    GestureEvent event;
    
    switch(type)
    {
    case ui::Widget::TouchEventType::BEGAN :
        break;
    case ui::Widget::TouchEventType::ENDED :
        {
            event.setType(GestureEvent::CheckButtonUp);
            
            if(onCheckButtonUp)
                onCheckButtonUp(&event);
        }
        break;
    case ui::Widget::TouchEventType::CANCELED :
        break;
    case ui::Widget::TouchEventType::MOVED :
        break;
    }
}

void GesturePanel::update(float dt)
{
    static float duration = 0;
    
    duration += dt;
    if(duration >= .3)
        duration -= .3;
    else
        return;
    
    GestureEvent event;
    if(JoyRocker::Direction::Stay != m_joyRocker->getDirection())
    {
        event.setType(GesturePanel::GestureEvent::GestureEventType::JoyRockerMove);
        event.setDirection(m_joyRocker->getDirection());
        
        if(onJoyRockerMove)
            onJoyRockerMove(&event);
    }
}
