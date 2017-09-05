#ifndef __ITEM_H__
#define __ITEM_H__

#include "cocos2d.h"

#include "Util.h"
#include "ScriptInterface.h"
#include "EventObject.h"

class Item :	public EventObject,
public ScriptInterface
{
public:
    XMOUNT_DECLARE(Item);
    XCREATE_WITH_LUA_SCRIPT_FUNC(Item);
    // 待注册的API
    static int _create(lua_State *luaState);
    static int _preloadFrame(lua_State *luaState);
    static int _bindSprite(lua_State *luaState); // 绑定精灵对象
    static int _setPos(lua_State *luaState);
    static int _setTriggerField(lua_State *luaState);
    static int _setSpriteFrame(lua_State *luaState);
    
    Item();
    Item(lua_State *luaState);
    ~Item();
    // override
    virtual bool init() override;
    virtual void onSearched() override;
    // 实际的操作
    std::map<std::string, int>getItems();
    std::map<std::string, int>getEffect();
private:
    std::string m_category;
    std::string m_subcategory;
    int m_price;
    std::string m_scene;
    std::map<std::string, std::string> m_apply;
    std::map<std::string, int> m_effect;
    std::map<std::string, int> m_items;
    std::map<std::string, std::string> m_image;
    std::string m_description;
    bool m_enable;
};


#endif // __ITEM_H__
