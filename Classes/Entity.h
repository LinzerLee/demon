#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "cocos2d.h"
#include "Util.h"
#include "EventObject.h"
#include "ScriptInterface.h"

class Entity :	public EventObject,
public ScriptInterface
{
public:
        friend class GameMainScene;
        friend class BattleScene;
        enum class Direction : unsigned char
        {
            NORTH = 0,
            SOUTH,
            WEST,
            EAST,
            UP,
            DOWN,
            LEFT,
            RIGHT,
            INVAILD
        };
        // implement the "static create()" method manually
        XMOUNT_DECLARE(Entity);
        XCREATE_WITH_LUA_SCRIPT_FUNC(Entity);
        Entity();
        Entity(lua_State *luaState);
        ~Entity();
        // 待注册的API
        static int _create(lua_State *luaState);
        static int _setPos(lua_State *luaState);
        static int _go(lua_State *luaState);
        static int _dialog(lua_State *luaState);
        // override
        virtual bool init() override;
        virtual void go(Direction dire);
        // 绑定精灵对象
        void bindSprite(cocos2d::Sprite *sprite);
        void setDirection(Direction dire);
        void bindAnimate(Direction dire, cocos2d::Animate *animate);
        void action(Direction dire);
            
    private:
        std::string m_category;
        int m_level, m_exp, m_hp, m_mp, m_luck, m_sharp, m_attack, m_defensive;
        std::map<std::string, int> m_items;
        std::map<std::string, std::string> m_image;
        std::map<std::string, int> m_steal;
        int m_magicTime;
        std::map<std::string, int> m_effect;
        std::string m_immune;
        int m_traceField;
        float m_speed;
        std::map<std::string, std::string> m_party; // m_player 不使用该字段
        std::string m_battleMap;
        std::map<std::string, std::string> m_magic;
        std::map<std::string, std::string> m_physical;
        std::map<std::string, std::string> m_battleState;
        std::string m_description;
        cocos2d::Map<unsigned char, cocos2d::Animate *>m_animateMap;
        Direction m_direction;
        bool m_enable;
    };
    
#endif // __ENTITY_H__
