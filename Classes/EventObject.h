#ifndef __EVENT_OBJECT_H__
#define __EVENT_OBJECT_H__

extern "C" {
#include <lua/lua/lua.h>
#include <lua/lua/lualib.h>
#include <lua/lua/lauxlib.h>
}

#include "cocos2d.h"
#include "Util.h"

class EventObject : public cocos2d::Node
{
public:
    typedef std::function<void(EventObject *)> EventObjectCallback;
    // 对象通过CREATE_FUNC创建表示可以创建不靠脚本驱动的对象(同时还需要无参构造的能力)
    // 对象通过XCREATE_WITH_LUA_SCRIPT_FUNC创建表示该对象依靠脚本驱动
    XCREATE_WITH_LUA_SCRIPT_FUNC(EventObject);
    static EventObject *getByIdentifier(int id);
    static void visitAll(const EventObjectCallback &callback);
    static void updateAll(float dt);
    
    // 拥有lua state
    EventObject();
    // 共享其他EventObject的lua state
    EventObject(lua_State *luaState);
    ~EventObject();
    // 子类重写init初始化各自的属性
    virtual bool init() override;
    virtual bool initWithLuaScript(std::string luaFile) final;
    virtual void onEnterScene();
    virtual void onEnterSceneTransitionDidFinish();
    virtual void onExitSceneTransitionDidStart();
    virtual void onExitScene();
    virtual void onSearched();
    virtual void onEquiped();
    virtual void onUsed();
    virtual void update(float dt) override;
    
    virtual int identifier() final;
    virtual std::string type() final;
    virtual std::string name() final;
    virtual void setTriggerField(int field) final;
    virtual bool checkTriggerField(cocos2d::Point pos) final;
    virtual lua_State *getLuaScriptEngine() final { return m_luaState; }
    virtual void bindSpriteWithFrameName(std::string frame) final;
protected:
    std::string  m_type; // 各子类构造函数中赋值
    std::string  m_name; // 很重要,关系着脚本的查找,包名,在initWithScript中赋值
    cocos2d::Sprite *m_sprite;
private:
    int m_identifier;
    lua_State *m_luaState;
    bool m_isShared;
    int m_triggerField;
};

#endif // __EVENT_OBJECT_H__
