#ifndef __JOY_ROCKER_H__
#define __JOY_ROCKER_H__

#include "cocos2d.h"

class JoyRocker : public cocos2d::Node
{
public:
    //用于标识摇杆方向
    enum class Direction : unsigned char
    {
        Stay = 0,
        Up,
        Down,
        Left,
        Right,
        LeftUp,
        RightUp,
        LeftDown,
        RightDown,
    };
        
    CREATE_FUNC(JoyRocker);
    // override
    virtual bool init() override;
    void setPosition(const cocos2d::Vec2& position) override;
    const cocos2d::Size &getContentSize() const override;
    void setOpacity(GLubyte opacity) override;
    // 使用摇杆
    void start(void);
    // 停止使用摇杆
    void stop(void);
    
    // 获取 摇杆 方向
    Direction getDirection();
    
    //触屏事件
    virtual bool TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
private:
    // 摇杆 是否可以移动（是否超过摇杆背景）
    bool m_isCanMove;
    //摇杆 中心的坐标
    cocos2d::Point m_centerPos;
    //摇杆背景的半径
    float m_panelRadius;
    //摇杆 的半径
    float m_centerRadius;
    //判断控制杆方向
    Direction m_direction;
        
    cocos2d::Sprite *m_panel;
    cocos2d::Sprite *m_center;
    cocos2d::EventListenerTouchOneByOne *m_listener;
};

#endif // __JOY_ROCKER_H__
