#include "BattlePanel.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool BattlePanel::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    scheduleUpdate();
    //////////////////////////////
    // 2. init button
    
    Size visiableSize = Director::getInstance()->getVisibleSize();
    Vec2 centerPos = visiableSize / 4;
    m_physicalAttackButton = ui::Button::create("ui/physical_attack.png", "ui/physical_attack.png", "ui/physical_attack_disable.png");
    m_physicalAttackButton->setScale(0.5);
    m_physicalAttackButton->setPosition(Point(centerPos.x, centerPos.y + m_physicalAttackButton->getContentSize().height / 2));
    m_physicalAttackButton->addTouchEventListener(CC_CALLBACK_2(BattlePanel::physicalAttackButtonTouchCallBack, this));
    addChild(m_physicalAttackButton);
    
    m_magicAttackButton = ui::Button::create("ui/magic_attack.png", "ui/magic_attack.png", "ui/magic_attack_disable.png");
    m_magicAttackButton->setScale(0.5);
    m_magicAttackButton->setPosition(Point(centerPos.x - m_magicAttackButton->getContentSize().width / 2, centerPos.y));
    m_magicAttackButton->addTouchEventListener(CC_CALLBACK_2(BattlePanel::physicalAttackButtonTouchCallBack, this));
    addChild(m_magicAttackButton);
    
    m_allianceAttackButton = ui::Button::create("ui/alliance_attack.png", "ui/alliance_attack.png", "ui/alliance_attack_disable.png");
    m_allianceAttackButton->setScale(0.5);
    m_allianceAttackButton->setPosition(Point(centerPos.x + m_allianceAttackButton->getContentSize().width / 2, centerPos.y));
    m_allianceAttackButton->addTouchEventListener(CC_CALLBACK_2(BattlePanel::physicalAttackButtonTouchCallBack, this));
    addChild(m_allianceAttackButton);
    
    m_knapsackButton = ui::Button::create("ui/magic_attack.png", "ui/magic_attack.png", "ui/magic_attack_disable.png");
    m_knapsackButton->setScale(0.5);
    m_knapsackButton->setPosition(Point(centerPos.x, centerPos.y - m_knapsackButton->getContentSize().height / 2));
    m_knapsackButton->addTouchEventListener(CC_CALLBACK_2(BattlePanel::physicalAttackButtonTouchCallBack, this));
    
    addChild(m_knapsackButton);
    
    return true;
}

void BattlePanel::update(float dt)
{
    static float duration = 0;
    
    duration += dt;
    if(duration >= .3)
        duration -= .3;
    else
        return;
}

void BattlePanel::physicalAttackButtonTouchCallBack(Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type == ui::Widget::TouchEventType::ENDED && onBattleButtonTrigger)
        onBattleButtonTrigger(BattleButtonType::Physical);
}
