#include "Item.h"

using std::string;
using std::map;
USING_NS_CC;

const char *Item::LIB_NAME = "Item";
const int Item::LIB_VERSION = GEN_VERSION(0, 1);
const luaL_Reg Item::LIB_API[] = {
    { "create",			Item::_create },
    { "preloadFrame",	Item::_preloadFrame },
    { "bindSprite",		Item::_bindSprite },
    { "setSpriteFrame",	Item::_setSpriteFrame },
    { "setPos",			Item::_setPos },
    { "setTriggerField",Item::_setTriggerField },
    { NULL, NULL }
};

// 待注册函数
int Item::_create(lua_State *luaState)
{
    CCASSERT(nullptr != luaState, "lua state has not been opened");
    XLUA_CALL_C_API_INIT(arg_num, ret_num, id);
    /*
    // create 没有id,修改arg_num为0
    arg_num = 0;
    const char *name = XLUA_CHECK_STRING(luaState, arg_num);
    CCASSERT(NULL != name, "name are needed");
    // XLUA_IGNORE_ID(luaState); 该函数不能在create里调用
    Item * item = Item::createWithLuaScript(name);
    CCASSERT(nullptr != item, "Item create faild");
    Item::mount(item);
    NotificationCenter::getInstance()->postNotification("loadItem", item);
    XLUA_PUSH_INT(luaState, item->identifier(), ret_num);
    */
    return ret_num;
}

int Item::_preloadFrame(lua_State *luaState)
{
    XLUA_CALL_C_API_INIT(arg_num, ret_num, id);
    size_t len1, len2;
    bool ret = true;
    XLUA_IGNORE_ID(luaState); // identifier
    /*
    const char *plist = XLUA_CHECK_LSTRING(luaState, arg_num, len1);
    const char *texture = XLUA_CHECK_LSTRING(luaState, arg_num, len2);
    auto frameCache = SpriteFrameCache::getInstance();
    
    if (false == frameCache->isSpriteFramesWithFileLoaded(plist))
    {
        if (len1 > 0 && len2 > 0)
        {
            frameCache->addSpriteFramesWithFile(plist, texture);
        }
        else if (len1 > 0)
        {
            frameCache->addSpriteFramesWithFile(plist);
        }
        else
        {
            ret = false;
        }
    }
    
    XLUA_PUSH_BOOL(luaState, ret, ret_num);
    */
    return ret_num;
}

int Item::_bindSprite(lua_State *luaState)
{
    XLUA_CALL_C_API_INIT(arg_num, ret_num, id);
    size_t len;
    bool ret = true;
    /*
    Item *item = XLUA_CHECK_OBJECT(luaState, Item, id);
    const char *frame = XLUA_CHECK_LSTRING(luaState, arg_num, len);
    if (len > 0 && nullptr != item)
    {
        item->bindSpriteWithFrameName(frame);
    }
    */
    return ret_num;
}

int Item::_setPos(lua_State *luaState)
{
    XLUA_CALL_C_API_INIT(arg_num, ret_num, id);
    Point point;
    /*
    Item *item = XLUA_CHECK_OBJECT(luaState, Item, id);
    point.x = XLUA_CHECK_FLOAT(luaState, arg_num);
    point.y = XLUA_CHECK_FLOAT(luaState, arg_num);
    
    if (nullptr != item)
    {
        item->setPosition(point);
    }
    */
    return ret_num;
}

int Item::_setTriggerField(lua_State *luaState)
{
    XLUA_CALL_C_API_INIT(arg_num, ret_num, id);
    int field = 0;
    Point point;
    /*
    Item *item = XLUA_CHECK_OBJECT(luaState, Item, id);
    field = XLUA_CHECK_INT(luaState, arg_num);
    
    if (nullptr != item && field > 0 && field <= 10)
    {
        item->setTriggerField(field);
    }
    */
    return ret_num;
}

int Item::_setSpriteFrame(lua_State *luaState)
{
    XLUA_CALL_C_API_INIT(arg_num, ret_num, id);
    
    Item *item = XLUA_CHECK_OBJECT(luaState, Item, id);
    string frame = XLUA_CHECK_STRING(luaState, arg_num);
    if ("" != frame && nullptr != item)
    {
        item->m_sprite->setSpriteFrame(frame);
    }
    
    return ret_num;
}

Item::Item() :
m_enable(true)
{
    m_type = "Item";
}

Item::Item(lua_State *luaState) :
EventObject(luaState)
{
    Item();
}

Item::~Item()
{
}

bool Item::init()
{
    //////////////////////////////
    // 1. super init first
    if (!EventObject::init())
    {
        return false;
    }
    
    //////////////////////////////
    // 2. init self property
    lua_State *luaState = getLuaScriptEngine();
    XLUA_CLEAR_STACK(luaState);
    if (XLUA_TRY_GET_TABLE(luaState, m_name))
    {
        string skey, svalue;
        int ivalue;
        // 名称(必填)
        string name;
        XLUA_GET_FIELD_STRING_STRING(luaState, "名称", name);
        CCASSERT(m_name == name, "Item's name error");
        // 类型(必填)
        string category;
        XLUA_GET_FIELD_STRING_STRING(luaState, "类型", category);
        m_category = category;
        // 子类型(可选)
        string subcategory;
        XLUA_GET_FIELD_STRING_STRING(luaState, "子类型", subcategory);
        m_subcategory = subcategory;
        // 价格(可选)
        int price = 0;
        XLUA_GET_FIELD_STRING_INT(luaState, "价格", price);
        m_price = price;
        // 场景(可选)
        string scene;
        XLUA_GET_FIELD_STRING_STRING(luaState, "场景", scene);
        m_scene = scene;
        // 适用(可选)
        XLUA_FOREACH_TABLE_STRING_INT(luaState, "适用", skey, ivalue)
        if ("" != skey)
        {
            m_apply[skey] = ivalue;
        }
        XLUA_FOREACH_TABLE_END(luaState);
        // 效果(必填)
        XLUA_FOREACH_TABLE_STRING_INT(luaState, "效果", skey, ivalue)
        if ("" != skey)
        {
            m_effect[skey] = ivalue;
        }
        XLUA_FOREACH_TABLE_END(luaState);
        // 物品(宝箱类专属)
        XLUA_FOREACH_TABLE_STRING_INT(luaState, "物品", skey, ivalue)
        if ("" != skey)
        {
            m_items[skey] = ivalue;
        }
        XLUA_FOREACH_TABLE_END(luaState);
        // 形象(必填)
        XLUA_FOREACH_TABLE_STRING_STRING(luaState, "形象", skey, svalue)
        if ("" != skey)
        {
            m_image[skey] = svalue;
        }
        XLUA_FOREACH_TABLE_END(luaState);
        PreloadFrame(m_image["帧池"]);
        size_t pos = m_image["帧集"].find_first_of(',', 0);
        bindSpriteWithFrameName(m_image["帧集"].substr(0, pos));
        // 触发域(可选)
        int field = 1;
        XLUA_GET_FIELD_STRING_INT(luaState, "触发域", field);
        if (0 != field)
            setTriggerField(field);
        // 描述(可选)
        string description;
        XLUA_GET_FIELD_STRING_STRING(luaState, "描述", description);
        m_description = description;
    }
    lua_pop(luaState, 1);
    
    return true;
}

void Item::onSearched()
{
    if (m_enable)
    {
        EventObject::onSearched();
        NotificationCenter::getInstance()->postNotification("gainItem", this);
        m_enable = false;
    }
}

map<std::string, int> Item::getItems()
{
    return m_items;
}

map<std::string, int> Item::getEffect()
{
    return m_effect;
}
