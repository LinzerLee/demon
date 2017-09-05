#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "BattlePanel.h"
#include "cocos2d.h"

class BattleScene : public cocos2d::Scene
{
public:
#define MAX_BATTLE_ENTITY       10
#define MAX_TEAM_NUM            (MAX_BATTLE_ENTITY / 2)
#define MAIN_ENEMY                   0
#define MAIN_PLAYER                  (MAX_BATTLE_ENTITY / 2)
    struct BattleSceneData
    {
        std::string map;
        int player;
        int teams[MAX_TEAM_NUM]; // 0存放个数
        int enemy;
    };
    
    // implement the "static create()" method manually
    CREATE_FUNC(BattleScene);
    static cocos2d::Scene* createSceneWithData(const BattleSceneData &data);

    virtual bool initWithData(const BattleSceneData &data);
    void onBattleButtonTrigger(BattlePanel::BattleButtonType type);
    
private:
    cocos2d::Point m_entityPos[MAX_BATTLE_ENTITY];
    cocos2d::Sprite *m_entitySprite[MAX_BATTLE_ENTITY];
    Entity *m_entitys[MAX_BATTLE_ENTITY];
    BattlePanel *m_battlePanel;
};

#endif // __BATTLE_SCENE_H__
