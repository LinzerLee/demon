#ifndef __GESTURE_PANEL_H__
#define __GESTURE_PANEL_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "JoyRocker.h"

class GesturePanel : public cocos2d::Layer
{
public:
    class GestureEvent
    {
    public:
        enum GestureEventType{
            JoyRockerMove,
            CheckButtonUp,
        };
        JoyRocker::Direction getDirection();
        void setDirection(JoyRocker::Direction dire);
        GestureEventType getType();
        void setType(GestureEventType type);
    private:
        JoyRocker::Direction m_direction;
        GestureEventType m_type;
    };
    
    typedef std::function<void(GestureEvent *)> ccGestureCallback;
    
    CREATE_FUNC(GesturePanel);
    // override
    virtual bool init() override;
    virtual void update(float dt) override;
    void setOpacity(GLubyte opacity) override;
    // 获取摇杆方向
    JoyRocker::Direction getDirection();
    
    ccGestureCallback onJoyRockerMove;
    ccGestureCallback onCheckButtonUp;
private:
    void checkButtonTouchCallBack(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    JoyRocker *m_joyRocker;
    cocos2d::ui::Button *m_checkButton;
};

#endif // __GESTURE_PANEL_H__
