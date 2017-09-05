#ifndef __UTIL_H__
#define __UTIL_H__

#include "cocos2d.h"

enum class GameState : unsigned char
{
    GameNotStart,      // before main game scene init
    GameStart,         // after main game scene init
    GameMainRunning,   // after main game scene init finish
    GameEnd,           // exit game before clean, then to GameNotStart
    GameInBattle,      // in game battle secne
    GameInDialog,      // when NPC talk
};

GameState GetGameState();
GameState SetGameState(GameState gameState);

#define CHECK_GAME_STATE_OR_RETURN(_S) \
if (GameState::_S != GetGameState()) \
return \

#define CHECK_GAME_STATE_OR_RETURN_RESULT(_S, _R) \
if (GameState::_S != GetGameState()) \
return _R\

#define GEN_VERSION(b, l)		(b << 16 | (l & 0x0000FFFF))
#define GET_BIG_VERSION(v)		(v >> 16)
#define GET_LITTLE_VERSION(v)	(v & 0x0000FFFF)

// LUA 辅助函数
#define XLUA_CHECK_ARG_NUM(_L, _V)             int _V = lua_gettop(_L)
#define XLUA_CLEAR_STACK(_L)                             lua_settop(_L, 0)
#define XLUA_CALL_C_API_INIT(_A, _R, _I)            int _A = 1, _R = 0, _I = 0
#define XLUA_CALL_LUA_API_INIT(_A, _R)             int _A = 0, _R = 0
#define XLUA_CALL(_L, _A, _R)                                 lua_pcall(_L, _A, _R, 0)
#define XLUA_IGNORE_ID(_L)                                   luaL_checkint(_L, 1)
#define XLUA_CHECK_ID(_L)                                     luaL_checkint(_L, 1)
#define XLUA_CHECK_OBJECT(_L, _T, _I)              ((_T *)EventObject::getByIdentifier(_I = XLUA_CHECK_ID(_L)))
#define XLUA_CHECK_INT(_L, _R)                            luaL_checkint(_L, ++_R)
#define XLUA_CHECK_STRING(_L, _A)                   a2x(luaL_checkstring(_L, ++_A))
#define XLUA_CHECK_FLOAT(_L, _A)                   ((float)luaL_checknumber(_L, ++_A))
#define XLUA_PUSH_INT(_L, _I, _R)                         (lua_pushinteger(_L, _I), ++_R)
#define XLUA_PUSH_FLOAT(_L, _F, _R)                   (lua_pushnumber(_L, _F), ++_R)
#define XLUA_PUSH_BOOL(_L, _B, _R)                  (lua_pushboolean(_L, _B), ++_R)
#define XLUA_PUSH_STRING(_L, _S, _R)                  (lua_pushstring(_L, x2a(_S).c_str()), ++_R)
#define XLUA_POP_TOP(_L)                                      lua_pop(_L, 1)
#define XLUA_POP(_L, _N)                                        lua_pop(_L, _N)

#define XLUA_CALL_ERROR_LOG(_L) \
log("%s", x2u(lua_tostring(_L, -1)).c_str());

#define XLUA_TO_STRING(_L, _I, _V) \
if(NULL != lua_tostring(_L, _I)) \
    _V = a2x(lua_tostring(_L, _I)); \
else \
    _V = "" \

#define XLUA_TO_INT(_L, _I, _V) \
_V = (int)lua_tonumber(_L, _I) \

#define XLUA_TO_FLOAT(_L, _I, _V) \
_V = (float)lua_tonumber(_L, _I) \

#define XLUA_TRY_GET_TABLE(_L, _T) \
lua_getglobal(_L, x2a(_T).c_str()), lua_istable(_L, -1) \

#define XLUA_TRY_GET_ELEMENT_FROM_TABLE(_L, _F) \
lua_pushstring(_L, x2a(_F).c_str()), lua_gettable(_L, -2) \

#define XLUA_TRY_GET_FUNC_FROM_TABLE(_L, _F) \
XLUA_TRY_GET_ELEMENT_FROM_TABLE(_L, _F), lua_isfunction(_L, -1) \

#define XLUA_TRY_GET_STRING_FROM_TABLE(_L, _F, _S) \
XLUA_TRY_GET_ELEMENT_FROM_TABLE(_L, _F), _S = a2x(lua_tostring(_L, -1)),lua_isstring(_L, -1) \

#define XLUA_FOREACH_TABLE(_L, _T) \
lua_pushstring(_L, x2a(_T).c_str()); \
lua_gettable(_L, -2); \
if (lua_istable(_L, -1)) \
{ \
lua_pushnil(_L); \
while (0 != lua_next(_L, -2)) \
{ \

#define XLUA_FOREACH_TABLE_END(_L) \
lua_pop(_L, 1); \
} \
} \
lua_pop(_L, 1) \

#define XLUA_FOREACH_TABLE_STRING_INT(_L, _T, _K, _V) \
XLUA_FOREACH_TABLE(_L, _T) \
    XLUA_TO_STRING(_L, -2, _K); \
    XLUA_TO_INT(_L, -1, _V); \

#define XLUA_FOREACH_TABLE_STRING_STRING(_L, _T, _K, _V) \
XLUA_FOREACH_TABLE(_L, _T) \
    XLUA_TO_STRING(_L, -2, _K); \
    XLUA_TO_STRING(_L, -1, _V); \

#define XLUA_FOREACH_TABLE_STRING_TABLE_STRING_STRING(_L, _F, _T, __T, _K, _V) \
lua_pushstring(_L, x2a(_T).c_str()); \
lua_gettable(_L, -2); \
if (lua_istable(_L, -1)) \
{ \
    lua_pushnil(_L); \
    while (0 != lua_next(_L, -2)) \
    { \
        XLUA_TO_STRING(_L, -2, __T); \
        if (lua_istable(_L, -1)) \
        { \
            _F = true; \
            lua_pushnil(_L); \
            while (0 != lua_next(_L, -2)) \
            { \
                XLUA_TO_STRING(_L, -2, _K); \
                XLUA_TO_STRING(_L, -1, _V) \

#define XLUA_FOREACH_TABLE_REMOVE_KEY_VALUE(_L) \
XLUA_POP(_L, 2) \

#define XLUA_FOREACH_TABLE_RESTART(_L, _F) \
XLUA_POP_TOP(_L); \
lua_pushnil(_L); \
_F = false; \
continue \

#define XLUA_FOREACH_TABLE_INT_TABLE_STRING_STRING(_L, _F, _T, _I, _K, _V) \
lua_pushstring(_L, x2a(_T).c_str()); \
lua_gettable(_L, -2); \
if (lua_istable(_L, -1)) \
{ \
lua_pushnil(_L); \
while (0 != lua_next(_L, -2)) \
{ \
XLUA_TO_INT(_L, -2, _I); \
if (lua_istable(_L, -1)) \
{ \
_F = true; \
lua_pushnil(_L); \
while (0 != lua_next(_L, -2)) \
{ \
XLUA_TO_STRING(_L, -2, _K); \
XLUA_TO_STRING(_L, -1, _V) \

#define XLUA_FOREACH_TABLE_TABLE_END(_L, _F) \
                _F = false; \
                lua_pop(_L, 1); \
            } \
        } \
        lua_pop(_L, 1); \
    } \
} \
lua_pop(_L, 1) \

#define XLUA_GET_FIELD_STRING_STRING(_L, _F, _V) \
lua_pushstring(_L, x2a(_F).c_str()); \
lua_gettable(_L, -2); \
XLUA_TO_STRING(_L, -1, _V); \
lua_pop(_L, 1) \

#define XLUA_GET_FIELD_STRING_INT(_L, _F, _V) \
lua_pushstring(_L, x2a(_F).c_str()); \
lua_gettable(_L, -2); \
XLUA_TO_INT(_L, -1, _V); \
lua_pop(_L, 1) \

#define XLUA_GET_FIELD_STRING_FLOAT(_L, _F, _V) \
lua_pushstring(_L, x2a(_F).c_str()); \
lua_gettable(_L, -2); \
XLUA_TO_FLOAT(_L, -1, _V); \
lua_pop(_L, 1) \

#define XCREATE_WITH_LUA_SCRIPT_FUNC(__TYPE__) \
static __TYPE__* createWithLuaScript(std::string luaFile) \
{ \
__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->initWithLuaScript(luaFile)) \
{ \
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = nullptr; \
return nullptr; \
} \
}

#define XMOUNT_DECLARE(__TYPE__) \
static const char *LIB_NAME; \
static const int LIB_VERSION; \
static const luaL_Reg LIB_API[]; \
static void mount(EventObject *eo) \
{ \
luaL_openlib(eo->getLuaScriptEngine(), \
__TYPE__::LIB_NAME, \
__TYPE__::LIB_API, 0); \
} \

/*UTF8-->GB2312*/
std::string u2a(const char *inbuf);
std::string u2a(const std::string inbuf);
/*GB2312-->UTF8*/
std::string a2u(const char *inbuf);
std::string a2u(const std::string inbuf);
/*GB2312|UTF8-->UTF8*/
std::string x2u(const char *inbuf);
std::string x2u(const std::string inbuf);
/*GB2312|UTF8-->GB2312*/
std::string x2a(const char *inbuf);
std::string x2a(const std::string inbuf);
/*GB2312-->GB2312|UTF8*/
std::string a2x(const char *inbuf);
std::string a2x(const std::string inbuf);

std::vector<std::string> split(std::string str, std::string pattern);
void PreloadFrame(std::string plist);
cocos2d::Animation *GetAnimationByFrameNames(unsigned int loop, float duration, std::string names, std::string plist = "");
#endif // __UTIL_H__
