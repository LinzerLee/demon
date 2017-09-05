#include "Util.h"
#include "GameMainScene.h"
#include "SplashScene.h"
#include "BattleScene.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "MapLayer.h"
#include "Item.h"
#include "Tip.h"
#include "Dialog.h"
#include "JoyRocker.h"

using std::string;
using std::map;
using std::vector;

USING_NS_CC;

GameMainScene *GameMainScene::m_currentMainScene = nullptr;

const char *GameMainScene::LIB_NAME = "GameMainScene";
const int GameMainScene::LIB_VERSION = GEN_VERSION(0, 1);
const luaL_Reg GameMainScene::LIB_API[] = {
    { "map",                    GameMainScene::_map },
    { "battle",                 GameMainScene::_battle },
    { NULL, NULL }
};

typedef struct _QuadratureComponent
{
    Entity::Direction x;
    Entity::Direction y;
} QuadratureComponent;

static QuadratureComponent GetQuadratureComponent(Entity::Direction dire)
{
    QuadratureComponent qc;
    switch (dire)
    {
        case Entity::Direction::NORTH:
            qc.x = Entity::Direction::RIGHT;
            qc.y = Entity::Direction::UP;
            break;
        case Entity::Direction::SOUTH:
            qc.x = Entity::Direction::LEFT;
            qc.y = Entity::Direction::DOWN;
            break;
        case Entity::Direction::WEST:
            qc.x = Entity::Direction::LEFT;
            qc.y = Entity::Direction::UP;
            break;
        case Entity::Direction::EAST:
            qc.x = Entity::Direction::RIGHT;
            qc.y = Entity::Direction::DOWN;
            break;
        case Entity::Direction::UP:
            qc.x = Entity::Direction::INVAILD;
            qc.y = Entity::Direction::UP;
            break;
        case Entity::Direction::DOWN:
            qc.x = Entity::Direction::INVAILD;
            qc.y = Entity::Direction::DOWN;
            break;
        case Entity::Direction::LEFT:
            qc.x = Entity::Direction::LEFT;
            qc.y = Entity::Direction::INVAILD;
            break;
        case Entity::Direction::RIGHT:
            qc.x = Entity::Direction::RIGHT;
            qc.y = Entity::Direction::INVAILD;
            break;
        default:
            qc.x = Entity::Direction::INVAILD;
            qc.y = Entity::Direction::INVAILD;
            break;
    }
    
    return qc;
}

// 待注册函数
int GameMainScene::_map(lua_State *luaState)
{
    XLUA_CALL_C_API_INIT(arg_num, ret_num, id);
    CHECK_GAME_STATE_OR_RETURN_RESULT(GameStart, ret_num);
    // create 没有id,修改arg_num为0
    arg_num = 0;
    string mapName = XLUA_CHECK_STRING(luaState, arg_num);
    
    GameMainScene *scene = GameMainScene::GetCurrentMainScene();
    scene->loadMap(mapName);
    
    return ret_num;
}

int GameMainScene::_battle(lua_State *luaState)
{
    XLUA_CALL_C_API_INIT(arg_num, ret_num, enemy_id);
    CHECK_GAME_STATE_OR_RETURN_RESULT(GameMainRunning, ret_num);
    GameMainScene *mainScene = GameMainScene::GetCurrentMainScene();
   
    enemy_id = XLUA_CHECK_ID(luaState);
    string mapName = XLUA_CHECK_STRING(luaState, arg_num);
    
    BattleScene::BattleSceneData data;
    data.map = mapName;
    data.player = mainScene->m_player->identifier();
    data.enemy = enemy_id;
    int index = 0;
    data.teams[index] = mainScene->m_teams.size();
    
    for(auto entity : mainScene->m_teams)
    {
        data.teams[++index] = entity->identifier();
    }
    
    Scene *battleScene = BattleScene::createSceneWithData(data);
    
    Director::getInstance()->pushScene(TransitionFade::create(2, battleScene));
    return ret_num;
}

Scene* GameMainScene::createScene()
{
    return GameMainScene::create();
}

Scene* GameMainScene::createSceneWithLuaScript(string file)
{
    // load scripts
    GameMainScene *scene =  nullptr;
    EventObject *script = EventObject::createWithLuaScript(file.c_str());
    CCASSERT(nullptr != script, "load script faild");
    if (nullptr == script)
    {
        return scene;
    }
    
    scene = new GameMainScene();
    if (scene)
    {
        
        scene->m_script = script;
        scene->addChild(script);
    }
    
    // 让脚本的初始化在init之前
    if (scene && scene->init())
    {
        scene->autorelease();
        return scene;
    }
    else
    {
        CC_SAFE_DELETE(scene);
        return nullptr;
    }
}

// on "init" you need to initialize your instance
bool GameMainScene::init()
{
    SetGameState(GameState::GameStart);
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    scheduleUpdate();
    
    m_pendingLuaState = NULL;
    //////////////////////////////
    // 2. 向场景脚本挂载接口
    CCASSERT(nullptr != m_script, "load script faild");
    GameMainScene::mount(m_script);
    Item::mount(m_script);
    Entity::mount(m_script);
    Player::mount(m_script);
    
    //////////////////////////////
    // 3. init self property
    lua_State *luaState = m_script->getLuaScriptEngine();
    lua_settop(luaState, 0);
    lua_getglobal(luaState, x2a(m_script->name()).c_str());
    if (lua_istable(luaState, -1))
    {
        string  skey, svalue, tname;
        int ivalue, index;
        float fvalue;
        bool flag;
        
        // 地图(必填)
        string mapName;
        XLUA_GET_FIELD_STRING_STRING(luaState, "地图", mapName);
        CCASSERT("" != mapName, "map name error");
        m_mapLayer = MapLayer::create();
        if (nullptr == m_mapLayer)
        {
            log("nullptr == m_mapLayer");
            return false;
        }
        
        addChild(m_mapLayer);
        m_mapLayer->resetMap(mapName);
        // 主角
        string playerName;
        XLUA_GET_FIELD_STRING_STRING(luaState, "主角", playerName);
        CCASSERT("" != playerName, "player name error");
        Player *player = Player::getInstance(playerName);
        CCASSERT(nullptr != player, "player create faild");
        m_player = player;
        addChild(m_player);
        auto spawnPoint = m_mapLayer->getSpawnPoint();
        setPositionInView(getVisibleSize() / 2);
        setCameraPostion(spawnPoint);
        // 物品
        Item *item = nullptr;
        XLUA_FOREACH_TABLE_INT_TABLE_STRING_STRING(luaState, flag, "物品", index, skey, svalue);
        if (flag)
        {
            XLUA_FOREACH_TABLE_REMOVE_KEY_VALUE(luaState);
            if(XLUA_TRY_GET_STRING_FROM_TABLE(luaState, "名称", tname))
            {
                item = Item::createWithLuaScript(tname);
                CCASSERT(nullptr != item, "Item create faild");
                Item::mount(item);
                m_mapLayer->loadItem(item);
            }
            XLUA_FOREACH_TABLE_RESTART(luaState, flag);
        }
        
        if (skey == string("位置"))
        {
            Point pos;
            auto vs = split(string(svalue), ",");
            pos.x = atoi(vs[0].c_str());
            pos.y = atoi(vs[1].c_str());
            item->setPosition(pos);
        }
        XLUA_FOREACH_TABLE_TABLE_END(luaState, flag);
        // 怪物
        Entity *entity = nullptr;
        XLUA_FOREACH_TABLE_INT_TABLE_STRING_STRING(luaState, flag, "怪物", index, skey, svalue);
        if (flag)
        {
            XLUA_FOREACH_TABLE_REMOVE_KEY_VALUE(luaState);
            if(XLUA_TRY_GET_STRING_FROM_TABLE(luaState, "名称", tname))
            {
                entity = Entity::createWithLuaScript(tname);
                CCASSERT(nullptr != entity, "Entity create faild");
                Entity::mount(entity);
                GameMainScene::mount(entity);
                m_mapLayer->loadEntity(entity);
            }
            XLUA_FOREACH_TABLE_RESTART(luaState, flag);
        }
        
        if (skey == string("位置"))
        {
            Point pos;
            auto vs = split(string(svalue), ",");
            pos.x = atoi(vs[0].c_str());
            pos.y = atoi(vs[1].c_str());
            entity->setPosition(pos);
        }
        XLUA_FOREACH_TABLE_TABLE_END(luaState, flag);
    }
    
    //////////////////////////////
    // 4. init event listener
    auto keyEventListener = EventListenerKeyboard::create();
    
    keyEventListener->onKeyPressed = CC_CALLBACK_2(GameMainScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyEventListener, this);
    
    auto movePlayerListener = EventListenerCustom::create("movePlayer", CC_CALLBACK_1(GameMainScene::movePlayer, this));
    _eventDispatcher->addEventListenerWithFixedPriority(movePlayerListener, 1);
    
    NotificationCenter::getInstance()->addObserver(
                                                   this,
                                                   callfuncO_selector(GameMainScene::loadPlayer),
                                                   "loadPlayer",
                                                   NULL);
    
    NotificationCenter::getInstance()->addObserver(
                                                   this,
                                                   callfuncO_selector(GameMainScene::loadItem),
                                                   "loadItem",
                                                   NULL);
    
    NotificationCenter::getInstance()->addObserver(
                                                   this,
                                                   callfuncO_selector(GameMainScene::gainItem),
                                                   "gainItem",
                                                   NULL);
    
    NotificationCenter::getInstance()->addObserver(
                                                   this,
                                                   callfuncO_selector(GameMainScene::loadEntity),
                                                   "loadEntity",
                                                   NULL);
    
    NotificationCenter::getInstance()->addObserver(
                                                   this,
                                                   callfuncO_selector(GameMainScene::moveEntity),
                                                   "moveEntity",
                                                   NULL);
    
    NotificationCenter::getInstance()->addObserver(
                                                   this,
                                                   callfuncO_selector(GameMainScene::dialog),
                                                   "dialog",
                                                   NULL);
    
    NotificationCenter::getInstance()->addObserver(
                                                   this,
                                                   callfuncO_selector(GameMainScene::pendingLuaState),
                                                   "pendingLuaState",
                                                   NULL);
    //////////////////////////////
    // 6. init dialog
    m_dialog = Dialog::create();
    CCASSERT(nullptr != m_dialog, "dialog create faild");
    addChild(m_dialog);
    
    //////////////////////////////
    // 7. init gesture panel
    m_gesturePanel = GesturePanel::create();
    m_gesturePanel->onJoyRockerMove = CC_CALLBACK_1(GameMainScene::onJoyRockerMove, this);
    m_gesturePanel->onCheckButtonUp = CC_CALLBACK_1(GameMainScene::onCheckButtonUp, this);
    m_gesturePanel->setOpacity(255 * 0.6);
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    addChild(m_gesturePanel);
#endif
    
    //////////////////////////////
    // 8. change game state(must be last step)
    SetGameState(GameState::GameMainRunning);
    
    return true;
}

void GameMainScene::update(float dt)
{
    EventObject::updateAll(dt);
}

void GameMainScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(2, SplashScene::createScene()));
}

Point GameMainScene::getCameraPostion()
{
    return m_mapLayer->getPosition() + getVisibleSize() / 2;
}

void GameMainScene::setCameraPostion(Point point)
{
    if(canCameraMoveToPoint(point))
        m_mapLayer->setPosition(point - getVisibleSize() / 2);
}

cocos2d::Point GameMainScene::getPlayerPosition()
{
    return getCameraPostion() + (getPositionInView() - getCenterInView());
}

bool GameMainScene::checkCollision(cocos2d::Point pos)
{
    return m_mapLayer->checkCollision(pos);
}

void GameMainScene::move(Entity::Direction dire)
{
    if (Entity::Direction::INVAILD == dire)
        return;
    
    QuadratureComponent qc = GetQuadratureComponent(dire);
    // 障碍物检测
    if (checkCollision(GameMainScene::GetNextPosition(getPlayerPosition(), dire, m_mapLayer->getTileSize())))
        return;
    
    m_player->go(dire);
    
    if (canCameraMove(dire))
    {
        moveCamera(dire);
    }
    else if (canCameraMove(qc.x))
    {
        moveCamera(qc.x);
        moveInView(qc.y);
    }
    else if (canCameraMove(qc.y))
    {
        moveCamera(qc.y);
        moveInView(qc.x);
    }
    else
    {
        moveInView(dire);
    }
}

void GameMainScene::loadMap(std::string mapName)
{
    m_mapLayer->resetMap(mapName);
}

Point GameMainScene::getCameraNextStep(Entity::Direction dire)
{
    return GameMainScene::GetNextPosition(getCameraPostion(), dire, m_mapLayer->getTileSize());
}

Point GameMainScene::GetNextPosition(Point pos, Entity::Direction dire, Size titleSize)
{
    switch (dire)
    {
        case Entity::Direction::NORTH:
            return Point(pos + titleSize);
            break;
        case Entity::Direction::SOUTH:
            return Point(pos - titleSize);
            break;
        case Entity::Direction::WEST:
            return Point(pos.x - titleSize.width, pos.y + titleSize.height);
            break;
        case Entity::Direction::EAST:
            return Point(pos.x + titleSize.width, pos.y - titleSize.height);
            break;
        case Entity::Direction::UP:
            return Point(pos.x, pos.y + titleSize.height);
            break;
        case Entity::Direction::DOWN:
            return Point(pos.x, pos.y - titleSize.height);
            break;
        case Entity::Direction::LEFT:
            return Point(pos.x - titleSize.width, pos.y);
            break;
        case Entity::Direction::RIGHT:
            return Point(pos.x + titleSize.width, pos.y);
            break;
            
        default:
            break;
    }
    
    return pos;
}

GameMainScene *GameMainScene::GetCurrentMainScene()
{
    return GameMainScene::m_currentMainScene;
}

void GameMainScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (GetGameState())
    {
        case GameState::GameNotStart :
            break;
        case GameState::GameStart :
            break;
        case GameState::GameMainRunning :
        {
            Entity::Direction *dire = new Entity::Direction(Entity::Direction::INVAILD);
            EventCustom event ("movePlayer");
            switch (keyCode)
            {
                case EventKeyboard::KeyCode::KEY_UP_ARROW:
                    *dire = Entity::Direction::NORTH;
                    break;
                case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                    *dire = Entity::Direction::SOUTH;
                    break;
                case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                    *dire = Entity::Direction::WEST;
                    break;
                case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                    *dire = Entity::Direction::EAST;
                    break;
                case EventKeyboard::KeyCode::KEY_W:
                    *dire = Entity::Direction::UP;
                    break;
                case EventKeyboard::KeyCode::KEY_S:
                    *dire = Entity::Direction::DOWN;
                    break;
                case EventKeyboard::KeyCode::KEY_A:
                    *dire = Entity::Direction::LEFT;
                    break;
                case EventKeyboard::KeyCode::KEY_D:
                    *dire = Entity::Direction::RIGHT;
                    break;
                case EventKeyboard::KeyCode::KEY_SPACE:
                {
                    CC_SAFE_DELETE(dire);
                    auto pos = getPlayerPosition();
                    EventObject::visitAll(std::bind(&GameMainScene::onSearch, this, std::placeholders::_1, pos));
                    
                    return;
                }
                default:
                    break;
            }
            if (Entity::Direction::INVAILD != *dire)
            {
                event.setUserData((void*)dire);
                _eventDispatcher->dispatchEvent(&event);
            }
        }
            break;
        case GameState::GameEnd :
            break;
        case GameState::GameInBattle :
            break;
        case GameState::GameInDialog :
        {
            switch (keyCode)
            {
                case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                case EventKeyboard::KeyCode::KEY_SPACE:
                    m_dialog->hide();
                    if(NULL != m_pendingLuaState)
                    {
                        // m_pendingLuaState的赋空一定得在resume之前
                        // 这里采用临时变量的方式
                        auto luaState = m_pendingLuaState;
                        m_pendingLuaState = NULL;
                        lua_resume(luaState, 0);
                    }
                    break;
                default:
                    break;
            }
        }
            break;
        default :
            break;
    }
}

void GameMainScene::onJoyRockerMove(GesturePanel::GestureEvent *event)
{
    switch (GetGameState())
    {
        case GameState::GameNotStart :
            break;
        case GameState::GameStart :
            break;
        case GameState::GameMainRunning :
        {
            Entity::Direction *dire = new Entity::Direction(Entity::Direction::INVAILD);
            EventCustom custom_event ("movePlayer");
            switch (event->getDirection())
            {
                case JoyRocker::Direction::RightUp:
                    *dire = Entity::Direction::NORTH;
                    break;
                case JoyRocker::Direction::LeftDown:
                    *dire = Entity::Direction::SOUTH;
                    break;
                case JoyRocker::Direction::LeftUp:
                    *dire = Entity::Direction::WEST;
                    break;
                case JoyRocker::Direction::RightDown:
                    *dire = Entity::Direction::EAST;
                    break;
                case JoyRocker::Direction::Up:
                    // *dire = Entity::Direction::UP;
                    break;
                case JoyRocker::Direction::Down:
                    // *dire = Entity::Direction::DOWN;
                    break;
                case JoyRocker::Direction::Left:
                    // *dire = Entity::Direction::LEFT;
                    break;
                case JoyRocker::Direction::Right:
                    // *dire = Entity::Direction::RIGHT;
                    break;
                default:
                    break;
            }
            if (Entity::Direction::INVAILD != *dire)
            {
                custom_event.setUserData((void*)dire);
                _eventDispatcher->dispatchEvent(&custom_event);
            }
        }
            break;
        case GameState::GameEnd :
            break;
        case GameState::GameInBattle :
            break;
        case GameState::GameInDialog :
            break;
        default :
            break;
    }
}

void GameMainScene::onCheckButtonUp(GesturePanel::GestureEvent *event)
{
    switch (GetGameState())
    {
        case GameState::GameNotStart :
            break;
        case GameState::GameStart :
            break;
        case GameState::GameMainRunning :
        {
            EventObject::visitAll(std::bind(&GameMainScene::onSearch, this, std::placeholders::_1, getPlayerPosition()));
        }
            break;
        case GameState::GameEnd :
            break;
        case GameState::GameInBattle :
            break;
        case GameState::GameInDialog :
            m_dialog->hide();
            if(NULL != m_pendingLuaState)
            {
                // m_pendingLuaState的赋空一定得在resume之前
                // 这里采用临时变量的方式
                auto luaState = m_pendingLuaState;
                m_pendingLuaState = NULL;
                lua_resume(luaState, 0);
            }
            break;
        default :
            break;
    }
}
/*
 void GameMainScene::onEnter()
 {
	// GameMainScene::m_currentMainScene = this;
 }
 */
void GameMainScene::onEnterTransitionDidFinish()
{
    //////////////////////////////
    // play background music
    GameMainScene::m_currentMainScene = this;
    CCASSERT(nullptr != m_script, "scene script is null");
    m_script->onEnterSceneTransitionDidFinish();
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/M002.mp3", true);
}

void GameMainScene::onExitTransitionDidStart()
{
    //////////////////////////////
    // stop background music
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

void GameMainScene::onSearch(EventObject *eo, Point pos)
{
    CHECK_GAME_STATE_OR_RETURN(GameMainRunning);
    
    if (eo->type() == "Item" || eo->type() == "Entity")
    {
        if (eo->checkTriggerField(pos))
        {
            eo->onSearched();
        }
    }
}

Size GameMainScene::getMapContentSize()
{
    if (nullptr == m_mapLayer)
        return Size();
    
    return m_mapLayer->getContentSize();
}

Size GameMainScene::getVisibleSize()
{
    return Director::getInstance()->getVisibleSize();
}

void GameMainScene::moveCamera(Entity::Direction dire)
{
    if (Entity::Direction::INVAILD == dire)
        return;
    
    setCameraPostion(getCameraNextStep(dire));
}

bool GameMainScene::canMoveInMap(Point pos)
{
    auto maxPoint = m_mapLayer->getMaxPoint();
    auto originPoint = m_mapLayer->getOriginPoint();
    
    if (pos.x < originPoint.x)
    {
        return false;
    }
    
    if (pos.y < originPoint.y)
    {
        return false;
    }
    
    if (pos.x > maxPoint.x)
    {
        return false;
    }
    
    if (pos.y > maxPoint.y)
    {
        return false;
    }
    
    return !checkCollision(pos);
}

bool GameMainScene::canCameraMoveToPoint(Point point)
{
    // 先检测是否已经进行了InView的移动
    auto visibleSize = getVisibleSize();
    Point curInViewPos = m_player->getPosition();
    Point centerInViewPos = visibleSize / 2;
    Point curCameraPos = getCameraPostion();
    
    Vec2 vector1 = curInViewPos - centerInViewPos;
    Vec2 vector2 = point - curCameraPos;
    
    if (vector1.x * vector2.x < 0 || vector1.y * vector2.y < 0)
        return false;
    
    // 以Map安全域为基准, 累加上InView区域
    auto maxPoint = m_mapLayer->getMaxPoint();
    auto originPoint = m_mapLayer->getOriginPoint();
    auto minCameraPos = originPoint + visibleSize / 2;
    auto maxCameraPos = maxPoint - visibleSize / 2;
    
    if (point.x < minCameraPos.x)
    {
        return false;
    }
    
    if (point.y < minCameraPos.y)
    {
        return false;
    }
    
    if (point.x > maxCameraPos.x)
    {
        return false;
    }
    
    if (point.y > maxCameraPos.y)
    {
        return false;
    }
    
    return true;
}

bool GameMainScene::canCameraMove(Entity::Direction dire)
{
    if (Entity::Direction::INVAILD == dire)
        return false;
    
    return canCameraMoveToPoint(getCameraNextStep(dire));
}

void GameMainScene::moveInView(Entity::Direction dire)
{
    if (Entity::Direction::INVAILD == dire)
        return;
    
    return setPositionInView(getInViewNextStep(dire));
}

void GameMainScene::setPositionInView(Point point)
{
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Rect viewRect(origin, visibleSize);
    
    if (viewRect.containsPoint(point))
    {
        m_player->setPosition(point);
    }
}

cocos2d::Point GameMainScene::getPositionInView()
{
    return m_player->getPosition();
}

Point GameMainScene::getInViewNextStep(Entity::Direction dire)
{
    return GameMainScene::GetNextPosition(m_player->getPosition(), dire, m_mapLayer->getTileSize());
}

cocos2d::Point GameMainScene::getCenterInView()
{
    return getVisibleSize() / 2;
}

void GameMainScene::loadPlayer(Ref *pdata)
{
    Player *player = (Player *)pdata;
    CCASSERT(nullptr != player, "loadPlayer faild");
    m_player = player;
    addChild(m_player);
    auto spawnPoint = m_mapLayer->getSpawnPoint();
    setPositionInView(getVisibleSize() / 2);
    setCameraPostion(spawnPoint);
}

void GameMainScene::loadItem(Ref *pdata)
{
    Item *item = (Item *)pdata;
    m_mapLayer->loadItem(item);
}

void GameMainScene::gainItem(Ref *pdata)
{
    Item *item = (Item *)pdata;
    string content;
    char buffer[32];
    
    Point pos = getVisibleSize() / 2;
    float base_y = pos.y;
    int num = item->getItems().size() / 2;
    for (auto var : item->getItems())
    {
        Tip *tip = Tip::create();
        addChild(tip);
        content = var.first + " X ";
        sprintf(buffer, "%d", var.second);
        content = content + buffer;
        pos.y = base_y + (num --) * (tip->size().height + 5);
        tip->show(content, 2.0f, pos);
    }
}

void GameMainScene::loadEntity(Ref *pdata)
{
    Entity *entity = (Entity *)pdata;
    m_mapLayer->loadEntity(entity);
}

void GameMainScene::moveEntity(Ref *pdata)
{
    Entity *entity = (Entity *)pdata;
    Point pos = GetNextPosition(entity->getPosition(), entity->m_direction, m_mapLayer->getTileSize());
    if (canMoveInMap(pos))
    {
        entity->setPosition(pos);
    }
}

void GameMainScene::dialog(Ref *pdata)
{
    string msg = (const char *)pdata;
    vector<string> ss = split(msg, "\n");
    CCASSERT(ss.size() >= 2, "name and content are needed");
    if (ss.size() > 2)
    {
        m_dialog->show(ss[0], ss[1], ss[2]);
    }
    else
    {
        m_dialog->show(ss[0], ss[1]);
    }
}

void GameMainScene::pendingLuaState(Ref *pdata)
{
    m_pendingLuaState = (lua_State *)pdata;
}

void GameMainScene::movePlayer(EventCustom *event)
{
    Entity::Direction *dire = (Entity::Direction *)event->getUserData();
    move(*dire);
    CC_SAFE_DELETE(dire);
}
