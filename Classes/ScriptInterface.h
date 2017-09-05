#ifndef __SCRIPT_INTERFACE_H__
#define __SCRIPT_INTERFACE_H__

extern "C" {
#include <lua/lua/lua.h>
#include <lua/lua/lualib.h>
#include <lua/lua/lauxlib.h>
}

#include "EventObject.h"

// 抽象类
class ScriptInterface
{
public:
    friend class EventObject;
    XMOUNT_DECLARE(ScriptInterface);
    // 待注册的API
    static int _getFullPathForFileName(lua_State *luaState);
    ScriptInterface();
    virtual ~ScriptInterface();
};
#endif // __SCRIPT_INTERFACE_H__
