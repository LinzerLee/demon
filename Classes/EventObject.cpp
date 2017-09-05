#include "map"
#include "EventObject.h"
#include "ScriptInterface.h"

using std::map;
using std::string;

USING_NS_CC;

#define MAX_ID 1000

map<int, EventObject *> g_eventObjects;

static int make_identifier()
{
    static int last_id = 0;
    
    return last_id = last_id % MAX_ID + 1;
}

EventObject *EventObject::getByIdentifier(int id)
{
    if (0 == g_eventObjects.count(id))
    {
        return nullptr;
    }
    
    return g_eventObjects[id];
}

void EventObject::visitAll(const EventObjectCallback &callback)
{
    for (auto var : g_eventObjects)
    {
        callback(var.second);
    }
}

void EventObject::updateAll(float dt)
{
    for (auto var : g_eventObjects)
    {
        var.second->update(dt);
    }
}

EventObject::EventObject() :
m_sprite(nullptr),
m_type("EventObject"),
m_isShared(false),
m_triggerField(1)
{
    m_luaState = lua_open();
    luaopen_base(m_luaState);
    luaopen_math(m_luaState);
    luaopen_string(m_luaState);
    // 以下代码修正luaopen_io调用崩溃问题
    lua_pushcfunction(m_luaState, luaopen_io);
    lua_pushstring(m_luaState, LUA_IOLIBNAME);
    lua_call(m_luaState, 1, 0);
    // 挂在系统api
    ScriptInterface::mount(this);
    
    m_identifier = make_identifier();
    // 覆盖掉相同的ID
    g_eventObjects[m_identifier] = this;
}

EventObject::EventObject(lua_State *luaState) :
m_type("EventObject"),
m_isShared(true),
m_luaState(luaState),
m_triggerField(1)
{
    // 共享的lua state就不用再挂在系统api
    m_identifier = make_identifier();
    // 覆盖掉相同的ID
    g_eventObjects[m_identifier] = this;
}

EventObject::~EventObject()
{
    if(!m_isShared)
        lua_close(m_luaState);
    m_luaState = nullptr;
    
    if (0 < g_eventObjects.count(m_identifier))
    {
        g_eventObjects.erase(m_identifier);
    }
}

int EventObject::identifier()
{
    return m_identifier;
}

string EventObject::type()
{
    return m_type;
}

string EventObject::name()
{
    return m_name;
}


void EventObject::setTriggerField(int field)
{
    m_triggerField = field;
}

bool EventObject::checkTriggerField(Point pos)
{
    TMXTiledMap *map = (TMXTiledMap *)getParent();
    Size size = map->getTileSize();
    Point point = getPosition();
    Rect rect(point.x - size.width * (m_triggerField + 0.5),
              point.y - size.height * (m_triggerField + 0.5),
              size.width * (2 * m_triggerField + 1),
              size.height * (2 * m_triggerField + 1));
    
    return rect.containsPoint(pos);
}

void EventObject::bindSpriteWithFrameName(string frame)
{
    m_sprite = Sprite::createWithSpriteFrameName(frame);
    
    if (nullptr != m_sprite)
    {
        addChild(m_sprite);
    }
}

bool EventObject::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }
    
    return true;
}

bool EventObject::initWithLuaScript(string luaFile)
{
    CCASSERT(nullptr != m_luaState, "lua state open faild!");
    
    if (!Node::init())
    {
        return false;
    }
    string path = string("script/") + luaFile + string(".lua");

    string fullpath = FileUtils::getInstance()->fullPathForFilename(path);
    m_name = luaFile;
    int errcode = luaL_dofile(m_luaState, fullpath.c_str());
    
    if (errcode)
    {
        XLUA_CALL_ERROR_LOG(m_luaState);
        switch (errcode)
        {
            case LUA_YIELD:
                break;
            case LUA_ERRRUN:
                break;
            case LUA_ERRSYNTAX:
                log(u8"error code %d : syntax error during pre-compilation", errcode);
                break;
            case LUA_ERRMEM:
                log(u8"error code %d : memory allocation error", errcode);
                break;
            case LUA_ERRERR:
                log(u8"error code %d : some error", errcode);
                break;
            case LUA_ERRFILE:
                log(u8"error code %d : load file faild", errcode);
                break;
            default:
                log(u8"unknow error code : %d", errcode);
        }
        return false;
    }
    
    return init();
}

void EventObject::onEnterSceneTransitionDidFinish()
{
    XLUA_CLEAR_STACK(m_luaState);
    if (XLUA_TRY_GET_TABLE(m_luaState, m_name))
    {
        if (XLUA_TRY_GET_FUNC_FROM_TABLE(m_luaState, "onEnterSceneTransitionDidFinish"))
        {
            XLUA_CALL_LUA_API_INIT(arg_num, ret_num);
            if (XLUA_CALL(m_luaState, arg_num, ret_num))
            {
                XLUA_CALL_ERROR_LOG(m_luaState);
            }
        }
    }
    else
    {
        log(x2u("[%s]表 %s 获取失败").c_str(), __func__, x2u(m_name).c_str());
    }
}

void EventObject::onEnterScene()
{
    XLUA_CLEAR_STACK(m_luaState);
    if (XLUA_TRY_GET_TABLE(m_luaState, m_name))
    {
        if (XLUA_TRY_GET_FUNC_FROM_TABLE(m_luaState, "onEnterScene"))
        {
            XLUA_CALL_LUA_API_INIT(arg_num, ret_num);
            if (XLUA_CALL(m_luaState, arg_num, ret_num))
            {
                XLUA_CALL_ERROR_LOG(m_luaState);
            }
        }
    }
    else
    {
        log(x2u("[%s]表 %s 获取失败").c_str(), __func__, x2u(m_name).c_str());
    }
}

void EventObject::onExitScene()
{
    XLUA_CLEAR_STACK(m_luaState);
    if (XLUA_TRY_GET_TABLE(m_luaState, m_name))
    {
        if (XLUA_TRY_GET_FUNC_FROM_TABLE(m_luaState, "onExitScene"))
        {
            XLUA_CALL_LUA_API_INIT(arg_num, ret_num);
            if (XLUA_CALL(m_luaState, arg_num, ret_num))
            {
                XLUA_CALL_ERROR_LOG(m_luaState);
            }
        }
    }
    else
    {
        log(x2u("[%s]表 %s 获取失败").c_str(), __func__, x2u(m_name).c_str());
    }
}

void EventObject::onExitSceneTransitionDidStart()
{
    XLUA_CLEAR_STACK(m_luaState);
    if (XLUA_TRY_GET_TABLE(m_luaState, m_name))
    {
        if (XLUA_TRY_GET_FUNC_FROM_TABLE(m_luaState, "onExitSceneTransitionDidStart"))
        {
            XLUA_CALL_LUA_API_INIT(arg_num, ret_num);
            if (XLUA_CALL(m_luaState, arg_num, ret_num))
            {
                XLUA_CALL_ERROR_LOG(m_luaState);
            }
        }
    }
    else
    {
        log(x2u("[%s]表 %s 获取失败").c_str(), __func__, x2u(m_name).c_str());
    }
}

void EventObject::onSearched()
{
    XLUA_CLEAR_STACK(m_luaState);
    if (XLUA_TRY_GET_TABLE(m_luaState, m_name))
    {
        if (XLUA_TRY_GET_FUNC_FROM_TABLE(m_luaState, "onSearched"))
        {
            XLUA_CALL_LUA_API_INIT(arg_num, ret_num);
            XLUA_PUSH_INT(m_luaState, identifier(), arg_num);
            if (XLUA_CALL(m_luaState, arg_num, ret_num))
            {
                XLUA_CALL_ERROR_LOG(m_luaState);
            }
        }
    }
    else
    {
        log(x2u("[%s]表 %s 获取失败").c_str(), __func__, x2u(m_name).c_str());
    }
    
    lua_pop(m_luaState, 1);
}

void EventObject::onEquiped()
{
    XLUA_CLEAR_STACK(m_luaState);
    if (XLUA_TRY_GET_TABLE(m_luaState, m_name))
    {
        if (XLUA_TRY_GET_FUNC_FROM_TABLE(m_luaState, "onEquiped"))
        {
            XLUA_CALL_LUA_API_INIT(arg_num, ret_num);
            if (XLUA_CALL(m_luaState, arg_num, ret_num))
            {
                XLUA_CALL_ERROR_LOG(m_luaState);
            }
        }
    }
    else
    {
        log(x2u("[%s]表 %s 获取失败").c_str(), __func__, x2u(m_name).c_str());
    }
}

void EventObject::onUsed()
{
    XLUA_CLEAR_STACK(m_luaState);
    if (XLUA_TRY_GET_TABLE(m_luaState, m_name))
    {
        if (XLUA_TRY_GET_FUNC_FROM_TABLE(m_luaState, "onUsed"))
        {
            XLUA_CALL_LUA_API_INIT(arg_num, ret_num);
            if (XLUA_CALL(m_luaState, arg_num, ret_num))
            {
                XLUA_CALL_ERROR_LOG(m_luaState);
            }
        }
    }
    else
    {
        log(x2u("[%s]表 %s 获取失败").c_str(), __func__, x2u(m_name).c_str());
    }
}

void EventObject::update(float dt)
{
    XLUA_CLEAR_STACK(m_luaState);
    if (XLUA_TRY_GET_TABLE(m_luaState, m_name))
    {
        if (XLUA_TRY_GET_FUNC_FROM_TABLE(m_luaState, "update"))
        {
            XLUA_CALL_LUA_API_INIT(arg_num, ret_num);
            XLUA_PUSH_INT(m_luaState, identifier(), arg_num);
            XLUA_PUSH_FLOAT(m_luaState, dt, arg_num);
            if (XLUA_CALL(m_luaState, arg_num, ret_num))
            {
                XLUA_CALL_ERROR_LOG(m_luaState);
            }        }
    }
    else
    {
        log(x2u("[%s]表 %s 获取失败").c_str(), __func__, x2u(m_name).c_str());
    }
    
    lua_pop(m_luaState, 1);
}
