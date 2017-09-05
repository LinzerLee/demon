#include "JoyRocker.h"

USING_NS_CC;

#define PI 3.1415926

static float Radius(Point x,Point y)
{
    //得到两点x的距离
    float xlen = y.x - x.x;
    //得到两点y的距离
    float ylen = y.y - x.y;
    //算出斜边长度
    float len = sqrt(pow(xlen, 2) + pow(ylen, 2));
    //得到这个角度的余弦值(通过三角函数中的点里：角度余弦值=斜边/斜边)
    float cosAngle = xlen / len;
    //通过反余弦定理获取到期角度的弧度
    float radius = acos(cosAngle);
    //注意：当触屏的位置Y坐标<摇杆的Y坐标，我们要去反值-0~-180
    if (y.y < x.y)
    {
        radius = -radius;
    }
    return radius;
}

// 根据角度，返回点坐标
static Point Angle2Position(float r, float angle)
{
    return Point(r * cos(angle), r * sin(angle));
}

bool JoyRocker::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Node::init() )
    {
        return false;
    }
    
    // 摇杆背景
    m_panel = Sprite::create("ui/joystick_panel.png");
    m_panel->setVisible(false);
    addChild(m_panel, 0);
    
    // 摇杆中心
    m_center = Sprite::create("ui/joystick_center.png");
    m_center->setVisible(false);
    addChild(m_center, 1);
    
    // 获取 摇杆背景 半径
    m_panelRadius = m_panel->getContentSize().width / 2;
    // 设置 摇杆中心 位置
    m_centerPos = Point(0, 0);
    m_panel->setPosition(m_centerPos);
    m_center->setPosition(m_centerPos);
    //表示摇杆方向不变
    m_direction = Direction::Stay;
    
    // 事件监听部分
    m_listener = EventListenerTouchOneByOne::create();
    // 吞掉这个触摸
    m_listener->setSwallowTouches(true);
    
    m_listener->onTouchBegan = CC_CALLBACK_2(JoyRocker::TouchBegan, this);
    m_listener->onTouchMoved = CC_CALLBACK_2(JoyRocker::TouchMoved, this);
    m_listener->onTouchEnded = CC_CALLBACK_2(JoyRocker::TouchEnded, this);
    
    return true;
}

void JoyRocker::start()
{
    m_panel->setVisible(true);
    m_center->setVisible(true);
    
    Director::getInstance()->getEventDispatcher() ->addEventListenerWithSceneGraphPriority(m_listener, this);
}


//停止摇杆(隐藏摇杆，取消摇杆的触屏监听)
void JoyRocker::stop()
{
    m_panel->setVisible(false);
    m_center->setVisible(false);
    
    Director::getInstance()->getEventDispatcher()->removeEventListener(m_listener);
}

bool JoyRocker::TouchBegan(Touch* touch, Event* event)
{
    //得到触屏点坐标
    Vec2 point = touch->getLocation() - m_centerPos;
    
    //判断是否点击到sp这个精灵：boundingBox()精灵大小之内的所有坐标
    if(m_center->getBoundingBox().containsPoint(point))
    {
        // 可以移动了
        m_isCanMove = true;
    }
    
    m_panel->setTexture("ui/joystick_panel_down.png");
    m_center->setTexture("ui/joystick_center_down.png");
    
    return true;
}

// 获取移动方向
JoyRocker::Direction JoyRocker::getDirection(void)
{
    return m_direction;
}

void JoyRocker::setPosition(const cocos2d::Vec2& position)
{
    Node::setPosition(position);
    // 同时要修改center position，Touch传递的Location是相对
    // 屏幕的，减去center position才是相对中心点的
    m_centerPos = position;
}

const cocos2d::Size &JoyRocker::getContentSize() const
{
    return m_panel->getContentSize();
}

void JoyRocker::setOpacity(GLubyte opacity)
{
    m_panel->setOpacity(opacity);
    m_center->setOpacity(opacity);
}

void JoyRocker::TouchMoved(Touch* touch, Event* event)
{
    // 如果不能移动，直接返回
    if(!m_isCanMove)
    {
        return;
    }
    
    Point point = touch->getLocation();
    
    //得到摇杆与触屏点所形成的角度
    
    float angle = Radius(m_centerPos, point);
    
    //判断两个圆的圆心距是否大于摇杆背景的半径
    if (sqrt(pow((m_centerPos.x - point.x), 2) + pow((m_centerPos.y - point.y), 2)) >= m_panelRadius)
    {
        //保证内部小圆运动的长度限制
        m_center->setPosition(Angle2Position(m_panelRadius, angle) );
    }
    else
    {
        //当没有超过，让摇杆跟随用户触屏点移动即可
        m_center->setPosition(point - Point(m_centerPos.x, m_centerPos.y));
    }
    
    //判断方向
    
    // 右方
    if( angle >= -PI / 8 && angle < PI / 8 )	{
        m_direction = Direction::Right;
    }
    // 右上方
    else if( angle >= PI / 8 && angle < 3 * PI / 8 )	{
        m_direction = Direction::RightUp;
    }
    // 上方
    else if( angle >= 3 * PI / 8 && angle < 5 * PI / 8 )	{
        m_direction = Direction::Up;
    }
    // 左上方
    else if( angle >= 5 * PI / 8 && angle < 7 * PI / 8 )	{
        m_direction = Direction::LeftUp;
    }
    // 左方
    else if( (angle >= 7 * PI / 8 && angle <= PI) || (angle >= - PI && angle < - 7 * PI / 8) )	{
        m_direction = Direction::Left;
    }
    // 左下方
    else if( angle >= - 7 * PI / 8 && angle < - 5 * PI / 8 )	{
        m_direction = Direction::LeftDown;
    }
    // 下方
    else if( angle >= - 5 * PI / 8 && angle < - 3 * PI / 8 )	{
        m_direction = Direction::Down;
    }
    // 右下方
    else if( angle >= - 3 * PI / 8 && angle < - PI / 8 )	{
        m_direction = Direction::RightDown;
    }
}

void JoyRocker::TouchEnded(Touch* touch, Event* event)
{
    if(!m_isCanMove)
    {
        return;
    }
    
    // 让 摇杆中心 停止之前所有动作，然后开始 执行归位
    m_center->stopAllActions();
    m_center->runAction(MoveTo::create(0.08, Point(0, 0)));
    
    // 设置 方向为 stay，并且 在下次触摸开始前 不可移动
    m_direction = Direction::Stay;
    m_isCanMove = false;
    
    m_panel->setTexture("ui/joystick_panel.png");
    m_center->setTexture("ui/joystick_center.png");
}
