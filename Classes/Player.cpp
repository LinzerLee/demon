#include "Player.h"

using std::string;

USING_NS_CC;

Player *Player::m_player = nullptr;

const char *Player::LIB_NAME = "Player";
const int Player::LIB_VERSION = GEN_VERSION(0, 1);
const luaL_Reg Player::LIB_API[] = {
    { "create",			Player::_create },
    { NULL, NULL }
};

Player *Player::getInstance(string name)
{
    if (nullptr == m_player)
    {
        m_player = Player::createWithLuaScript(name.c_str());
    }
    
    return m_player;
}

// 待注册函数
int Player::_create(lua_State *luaState)
{
    CCASSERT(nullptr != luaState, "lua state has not been opened");
    XLUA_CALL_C_API_INIT(arg_num, ret_num, id);
    /*
    // create 没有id,修改arg_num为0
    arg_num = 0;
    const char *name = XLUA_CHECK_STRING(luaState, arg_num);
    CCASSERT(NULL != name, "name are needed");
    // XLUA_IGNORE_ID(luaState); 该函数不能在create里调用
    Player *player = Player::getInstance(name);
    CCASSERT(nullptr != player, "player create faild");
    // 不需要注册接口
    NotificationCenter::getInstance()->postNotification("loadPlayer", player);
    XLUA_PUSH_INT(luaState, player->identifier(), ret_num);
    */
    return ret_num;
}

Player::Player()
{
    m_type = "Player";
}

// on "init" you need to initialize your instance
bool Player::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Entity::init())
    {
        return false;
    }
    
    return true;
}

void Player::go(Direction dire)
{
    action(dire);
}
