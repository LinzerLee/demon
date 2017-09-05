#ifndef __BATTLE_PANEL_H__
#define __BATTLE_PANEL_H__

#include "cocos2d.h"
#include "cocos-ext.h"

class BattlePanel : public cocos2d::Layer
{
public:
    enum class BattleButtonType : unsigned char
    {
        Physical,
        Magic,
        Alliance,
        Knapsack,
    };
    
    CREATE_FUNC(BattlePanel);
    // override
    virtual bool init() override;
    virtual void update(float dt) override;

    typedef std::function<void(BattleButtonType type)> ccBattleButtonTriggerCallback;
    
    ccBattleButtonTriggerCallback onBattleButtonTrigger;
private:
    void physicalAttackButtonTouchCallBack(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    cocos2d::ui::Button *m_physicalAttackButton;
    cocos2d::ui::Button *m_magicAttackButton;
    cocos2d::ui::Button *m_allianceAttackButton;
    cocos2d::ui::Button *m_knapsackButton;
};

#endif // __BATTLE_PANEL_H__
