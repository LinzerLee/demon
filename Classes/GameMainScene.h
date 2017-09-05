#ifndef __GAME_MAIN_SCENE_H__
#define __GAME_MAIN_SCENE_H__

#include "vector"
#include "cocos2d.h"
#include "ScriptInterface.h"
#include "Entity.h"
#include "GesturePanel.h"

class Player;
class MapLayer;
class Dialog;

class GameMainScene :	public cocos2d::Scene,
                                                public ScriptInterface
{
public:
    static cocos2d::Scene* createScene();
    static cocos2d::Scene *createSceneWithLuaScript(std::string file);
    static cocos2d::Point GetNextPosition(cocos2d::Point pos, Entity::Direction dire, cocos2d::Size titleSize);
    static GameMainScene *GetCurrentMainScene();
    // 待注册的API
    static int _map(lua_State *luaState);
    static int _battle(lua_State *luaState);
    // implement the "static create()" method manually
    XMOUNT_DECLARE(GameMainScene);
    CREATE_FUNC(GameMainScene);
    virtual bool init() override;
    virtual void update(float dt) override;
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    bool checkCollision(cocos2d::Point pos);
    void move(Entity::Direction dire);
    void loadMap(std::string mapName);
    // Player位置包括考虑Camera位置和InView位置
    cocos2d::Point getPlayerPosition();
private:
    // 键盘事件
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    // Gesture Panel事件
    void onJoyRockerMove(GesturePanel::GestureEvent *event);
    void onCheckButtonUp(GesturePanel::GestureEvent *event);
    // Scene生命周期
    // virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExitTransitionDidStart() override;
    void onSearch(EventObject *eo, cocos2d::Point pos);
    
    cocos2d::Size getMapContentSize();
    cocos2d::Size getVisibleSize();
    
    cocos2d::Point getCameraPostion();
    void setCameraPostion(cocos2d::Point point);
    cocos2d::Point getCameraNextStep(Entity::Direction dire);
    void moveCamera(Entity::Direction dire);
    // 只考虑地图的边界和障碍物
    bool canMoveInMap(cocos2d::Point pos);
    // 用来检测视图是否已到达地图边界
    // 如果到达就进行InView移动
    bool canCameraMoveToPoint(cocos2d::Point point);
    bool canCameraMove(Entity::Direction dire);
    void moveInView(Entity::Direction dire);
    void setPositionInView(cocos2d::Point point);
    cocos2d::Point getPositionInView();
    cocos2d::Point getInViewNextStep(Entity::Direction dire);
    cocos2d::Point getCenterInView();
    // 物品
    void loadPlayer(Ref *pdata);
    void loadItem(Ref *pdata);
    void gainItem(Ref *pdata);
    void loadEntity(Ref *pdata);
    void moveEntity(Ref *pdata);
    void pendingLuaState(Ref *pdata);
    void dialog(Ref *pdata);
    void movePlayer(cocos2d::EventCustom *event);
    
    static GameMainScene *m_currentMainScene;
    std::vector<Entity *> m_teams;
    Player *m_player;
    MapLayer *m_mapLayer;
    Dialog *m_dialog;
    GesturePanel *m_gesturePanel;
    EventObject *m_script;
    lua_State *m_pendingLuaState;
};

#endif // __GAME_MAIN_SCENE_H__
