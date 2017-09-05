#include "Entity.h"
#include "GameMainScene.h"

using std::string;
using std::map;
USING_NS_CC;

const char *Entity::LIB_NAME = "Entity";
const int Entity::LIB_VERSION = GEN_VERSION(0, 1);
const luaL_Reg Entity::LIB_API[] = {
    { "create",			Entity::_create },
    { "setPos",			Entity::_setPos },
    { "go",				Entity::_go },
    { "dialog",			Entity::_dialog },
    { NULL, NULL }
};

Entity::Entity() :
m_enable(true)
{
    m_type = "Entity";
}

Entity::Entity(lua_State *luaState) :
EventObject(luaState)
{
    Entity();
}

Entity::~Entity()
{
}

// 待注册函数
int Entity::_create(lua_State *luaState)
{
    CCASSERT(nullptr != luaState, "lua state has not been opened");
    XLUA_CALL_C_API_INIT(arg_num, ret_num, id);
    /*
    // create 没有id,修改arg_num为0
    arg_num = 0;
    const char *name = XLUA_CHECK_STRING(luaState, arg_num);
    CCASSERT(NULL != name, "name are needed");
    // XLUA_IGNORE_ID(luaState); 该函数不能在create里调用
    Entity * entity = Entity::createWithLuaScript(name);
    CCASSERT(nullptr != entity, "Item create faild");
    Entity::mount(entity);
    NotificationCenter::getInstance()->postNotification("loadEntity", entity);
    XLUA_PUSH_INT(luaState, entity->identifier(), ret_num);
    */
    return ret_num;
}

int Entity::_setPos(lua_State *luaState)
{
    XLUA_CALL_C_API_INIT(arg_num, ret_num, id);
    Point point;
    /*
    Entity *entity = XLUA_CHECK_OBJECT(luaState, Entity, id);
    point.x = XLUA_CHECK_FLOAT(luaState, arg_num);
    point.y = XLUA_CHECK_FLOAT(luaState, arg_num);
    
    if (nullptr != entity)
    {
        entity->setPosition(point);
    }
    */
    return ret_num;
}

int Entity::_go(lua_State *luaState)
{
    XLUA_CALL_C_API_INIT(arg_num, ret_num, id);
    CHECK_GAME_STATE_OR_RETURN_RESULT(GameMainRunning, ret_num);
    
    Entity *entity = XLUA_CHECK_OBJECT(luaState, Entity, id);
    string sdire = XLUA_CHECK_STRING(luaState, arg_num);
    if ("北" == sdire)
    {
        entity->go(Direction::NORTH);
    }
    else if ("南" == sdire)
    {
        entity->go(Direction::SOUTH);
    }
    else if ("西" == sdire)
    {
        entity->go(Direction::WEST);
    }
    else if ("东" == sdire)
    {
        entity->go(Direction::EAST);
    }
    
    return ret_num;
}

int Entity::_dialog(lua_State *luaState)
{
    XLUA_CALL_C_API_INIT(arg_num, ret_num, id);
    XLUA_CHECK_ARG_NUM(luaState, args);
    
    Entity *entity = XLUA_CHECK_OBJECT(luaState, Entity, id);
    string content = XLUA_CHECK_STRING(luaState, arg_num);
    string msg = entity->name() + "\n" + content;
    if (args > 2)
    {
        string protrait = XLUA_CHECK_STRING(luaState, arg_num);
        msg += "\n" + protrait;
    }
    
    NotificationCenter::getInstance()->postNotification("pendingLuaState", (Ref *)luaState);
    NotificationCenter::getInstance()->postNotification("dialog", (Ref *)msg.c_str());
    
    return lua_yield(luaState, ret_num);
}

// on "init" you need to initialize your instance
bool Entity::init()
{
    //////////////////////////////
    // 1. super init first
    if (!EventObject::init())
    {
        return false;
    }
    
    m_direction = Direction::SOUTH;
    //////////////////////////////
    // 2. init self property
    lua_State *luaState = getLuaScriptEngine();
    XLUA_CLEAR_STACK(luaState);
    if (XLUA_TRY_GET_TABLE(luaState, m_name))
    {
        string  skey,  svalue;
        int ivalue;
        float fvalue;
        // 名称(必填)
        string name;
        XLUA_GET_FIELD_STRING_STRING(luaState, "名称", name);
        CCASSERT(m_name == name, "Item's name error");
        // 类型(必填)
        string category;
        XLUA_GET_FIELD_STRING_STRING(luaState, "类型", category);
        m_category = category;
        // 修行(可选)
        XLUA_GET_FIELD_STRING_INT(luaState, "修行", ivalue);
        m_level = ivalue;
        // 经历(可选)
        XLUA_GET_FIELD_STRING_INT(luaState, "经历", ivalue);
        m_exp = ivalue;
        // 体力(可选)
        XLUA_GET_FIELD_STRING_INT(luaState, "体力", ivalue);
        m_hp = ivalue;
        // 真气(可选)
        XLUA_GET_FIELD_STRING_INT(luaState, "真气", ivalue);
        m_mp = ivalue;
        // 吉运(可选)
        XLUA_GET_FIELD_STRING_INT(luaState, "吉运", ivalue);
        m_luck = ivalue;
        // 身法(可选)
        XLUA_GET_FIELD_STRING_INT(luaState, "身法", ivalue);
        m_sharp = ivalue;
        // 武术(可选)
        XLUA_GET_FIELD_STRING_INT(luaState, "武术", ivalue);
        m_attack = ivalue;
        // 防御(可选)
        XLUA_GET_FIELD_STRING_INT(luaState, "防御", ivalue);
        m_defensive = ivalue;
        // 物品(可选)
        XLUA_FOREACH_TABLE_STRING_INT(luaState, "物品", skey, ivalue)
        if ("" != skey)
        {
            m_items[skey] = ivalue;
        }
        XLUA_FOREACH_TABLE_END(luaState);
        // 偷窃(可选)
        XLUA_FOREACH_TABLE_STRING_INT(luaState, "偷窃", skey, ivalue)
        if ("" != skey)
        {
            m_steal[skey] = ivalue;
        }
        XLUA_FOREACH_TABLE_END(luaState);
        // 真气数(怪和敌人专有)
        XLUA_GET_FIELD_STRING_INT(luaState, "真气数", ivalue);
        m_magicTime = ivalue;
        // 效果(可选)
        XLUA_FOREACH_TABLE_STRING_INT(luaState, "效果", skey, ivalue)
        if ("" != skey)
        {
            m_effect[skey] = ivalue;
        }
        XLUA_FOREACH_TABLE_END(luaState);
        // 免疫(可选)
        XLUA_GET_FIELD_STRING_STRING(luaState, "免疫", svalue);
        m_immune = svalue;
        // 触发域(可选)
        XLUA_GET_FIELD_STRING_INT(luaState, "触发域", ivalue);
        if (0 != ivalue)
            setTriggerField(ivalue);
        // 追踪域(可选)
        XLUA_GET_FIELD_STRING_INT(luaState, "追踪域", ivalue);
        if (0 != ivalue)
            m_traceField = ivalue;
        // 速度(可选)
        XLUA_GET_FIELD_STRING_FLOAT(luaState, "速度", fvalue);
        if (0 != fvalue)
            m_speed = fvalue;
        // 伙伴(可选)
        XLUA_FOREACH_TABLE_STRING_STRING(luaState, "伙伴", skey, svalue)
        if ("" != skey)
        {
            m_party[skey] = svalue;
        }
        XLUA_FOREACH_TABLE_END(luaState);
        // 战斗地图(可选)
        string battleMap;
        XLUA_GET_FIELD_STRING_STRING(luaState, "战斗地图", battleMap);
        m_battleMap = battleMap;
        // 法攻(可选)
        XLUA_FOREACH_TABLE_STRING_STRING(luaState, "法攻", skey, svalue)
        if ("" != skey)
        {
            m_magic[skey] = svalue;
        }
        XLUA_FOREACH_TABLE_END(luaState);
        // 物攻(可选)
        XLUA_FOREACH_TABLE_STRING_STRING(luaState, "物攻", skey, svalue)
        if ("" != skey)
        {
            m_physical[skey] = svalue;
        }
        XLUA_FOREACH_TABLE_END(luaState);
        // 战斗状态(可选)
        XLUA_FOREACH_TABLE_STRING_STRING(luaState, "战斗状态", skey, svalue)
        if ("" != skey)
        {
            m_battleState[skey] = svalue;
        }
        XLUA_FOREACH_TABLE_END(luaState);
        // 描述(可选)
        XLUA_GET_FIELD_STRING_STRING(luaState, "描述", svalue);
        m_description = svalue;
        // 形象(必填)
        XLUA_FOREACH_TABLE_STRING_STRING(luaState, "形象", skey, svalue)
        if ("" != skey)
        {
            m_image[skey] = svalue;
        }
        XLUA_FOREACH_TABLE_END(luaState);
        PreloadFrame(m_image["帧池"]);
        auto frameCache = SpriteFrameCache::getInstance();
        for (auto var : m_image)
        {
            if ("帧池" == var.first)
            {
                continue;
            }
            else
            {
                Direction dire = Direction::INVAILD;
                Vector<SpriteFrame *> frames;
                Animation *animation;
                auto framenames = split(var.second, ",");
                if ("北" == var.first)
                {
                    dire = Direction::NORTH;
                }
                else if ("南" == var.first)
                {
                    dire = Direction::SOUTH;
                }
                else if ("西" == var.first)
                {
                    dire = Direction::WEST;
                }
                else if ("东" == var.first)
                {
                    dire = Direction::EAST;
                }
                else if ("上" == var.first)
                {
                    dire = Direction::UP;
                }
                else if ("下" == var.first)
                {
                    dire = Direction::DOWN;
                }
                else if ("左" == var.first)
                {
                    dire = Direction::LEFT;
                }
                else if ("右" == var.first)
                {
                    dire = Direction::RIGHT;
                }
                
                for (auto name : framenames)
                {
                    frames.pushBack(frameCache->getSpriteFrameByName(name));
                }
                // 创建精灵
                if (m_direction == dire)
                {
                    bindSpriteWithFrameName(framenames[0]);
                }
                // 创建动画
                animation = Animation::createWithSpriteFrames(frames);
                animation->setLoops(1);
                animation->setDelayPerUnit(0.2);
                bindAnimate(dire, Animate::create(animation));
                
                frames.clear();
            }
        }
    }
    
    return true;
}

void Entity::bindSprite(cocos2d::Sprite *sprite)
{
    m_sprite = sprite;
    
    // 修改锚点
    m_sprite->setAnchorPoint(ccp(0.5, 0));
    this->addChild(m_sprite);
    
    if (m_animateMap.end() != m_animateMap.find((unsigned char)m_direction))
    {
        setDirection(m_direction);
    }
}

void Entity::setDirection(Direction dire)
{
    m_direction = dire;
    if (nullptr != m_sprite && m_animateMap.end() != m_animateMap.find((unsigned char)m_direction))
    {
        CCASSERT(m_animateMap.at((unsigned char)m_direction)->getAnimation()->getFrames().size() != 0, "load resource faild!");
        m_sprite->setSpriteFrame(m_animateMap.at((unsigned char)m_direction)->getAnimation()->getFrames().front()->getSpriteFrame());
    }
}

void Entity::bindAnimate(Direction dire, cocos2d::Animate *animate)
{
    m_animateMap.insert((unsigned char)dire, animate);
}

void Entity::action(Direction dire)
{
    if (Direction::INVAILD == dire)
    {
        return;
    }
    
    if (dire != m_direction)
    {
        setDirection(dire);
    }
    
    if (m_animateMap.end() == m_animateMap.find((unsigned char)m_direction))
    {
        log("go animate faild!");
        return;
    }
    m_sprite->stopAllActions();
    m_sprite->runAction(m_animateMap.at((unsigned char)m_direction));
}

void Entity::go(Direction dire)
{
    if (CCRANDOM_0_1() <= m_speed)
    {
        action(dire);
        NotificationCenter::getInstance()->postNotification("moveEntity", this);
    }
}
