#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Entity.h"

class Player : public Entity
{
public:
    XMOUNT_DECLARE(Player);
    XCREATE_WITH_LUA_SCRIPT_FUNC(Player);
    static Player *getInstance(std::string name = "");
    Player();
    // 待注册的API
    static int _create(lua_State *luaState);
    // override
    virtual bool init() override;
    virtual void go(Direction dire) override;
private:
    // implement the "static create()" method manually
    CREATE_FUNC(Player);
    static Player *m_player;
};

#endif // __PLAYER_H__
