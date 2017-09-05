#include "Util.h"
#include "Entity.h"
#include "Player.h"
#include "BattlePanel.h"
#include "BattleScene.h"
#include "ShaderSprite.h"

using namespace std;
USING_NS_CC;

Scene* BattleScene::createSceneWithData(const BattleSceneData &data)
{
    BattleScene *scene =  new(std::nothrow) BattleScene();
    
    // 让脚本的初始化在init之前
    if (scene && scene->initWithData(data))
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
bool BattleScene::initWithData(const BattleSceneData &data)
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    //////////////////////////////
    // 2. init battle background
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = (origin + visibleSize) / 2;
    auto mapPath = "battle/" + data.map + ".png";
    auto backgroundSprite = Sprite::create(mapPath);
    
    if (nullptr == backgroundSprite)
        return false;
    
    backgroundSprite->setPosition(center);
    addChild(backgroundSprite, 0);

    //////////////////////////////
    // 3. calculate position
    //////////////////////////////
    //      4   2
    //  3   0
    //  1
    //                              7
    //                          5   9
    //                      6   8
    //
    Vec2    left_top(0, visibleSize.height),        right_top(visibleSize),
                  left_bottom(0, 0),                              right_bottom(visibleSize.width, 0);
    Point left_center, right_center;

    m_entityPos[0] = (right_bottom - left_top) / 4 + left_top;
    m_entityPos[1] = (right_top - left_bottom) / 8 + m_entityPos[0];
    m_entityPos[2] = (left_bottom - right_top) / 8 + m_entityPos[0];
    left_center = (left_top - right_bottom) / 8 + m_entityPos[0];
    m_entityPos[3] = (right_top - left_bottom) / 16 + left_center;
    m_entityPos[4] = (left_bottom - right_top) / 16 + left_center;
    
    m_entityPos[5] = (left_top - right_bottom) / 4 + right_bottom;
    m_entityPos[6] = (right_top - left_bottom) / 8 + m_entityPos[5];
    m_entityPos[7] = (left_bottom - right_top) / 8 + m_entityPos[5];
    right_center = (right_bottom - left_top) / 8 + m_entityPos[5];
    m_entityPos[8] = (right_top - left_bottom) / 16 + right_center;
    m_entityPos[9] = (left_bottom - right_top) / 16 + right_center;
    
    for(int i=MAIN_ENEMY; i<MAX_BATTLE_ENTITY; ++i)
        m_entitys[i] = nullptr;
    
    m_entitys[MAIN_ENEMY] = (Entity *)EventObject::getByIdentifier(data.enemy);
    m_entitys[MAIN_PLAYER] = (Player *)EventObject::getByIdentifier(data.player);
    
    int entityIndex = MAIN_ENEMY + 1;
    for(auto var : m_entitys[MAIN_ENEMY]->m_party)
    {
        if(entityIndex >= MAIN_PLAYER)
            break;
        
        int num = 0;
        if(string::npos == var.second.find('~'))
        {
            num = atoi(var.second.c_str());
        }
        else
        {
            vector<string> vs = split(var.second, "~");
            num = random(atoi(vs[0].c_str()), atoi(vs[1].c_str()));
        }
        
        for(int i=0; i<num && entityIndex<MAIN_PLAYER; ++i)
        {
            Entity *entity = Entity::createWithLuaScript(var.first);
            CCASSERT(entity != nullptr, "entity create faild");
            if(entity)
            {
                m_entitys[entityIndex ++] = entity;
            }
        }
    }
    
    entityIndex = MAIN_PLAYER + 1;
    for(int i=1; i<=data.teams[0]; ++i)
    {
        m_entitys[entityIndex++] = (Entity *)EventObject::getByIdentifier(data.enemy);
    }
    
    auto animation_cache = AnimationCache::getInstance();
    Animation *animation = nullptr;
    Animate *animateAction = nullptr;
    Sprite *sp = nullptr;
    
    for(int index=MAIN_ENEMY; index<MAX_BATTLE_ENTITY; ++index)
    {
        if(nullptr == m_entitys[index])
            continue;
        
        animation = GetAnimationByFrameNames(1, 0.2, m_entitys[index]->m_physical["攻击"], m_entitys[index]->m_physical["帧池"]);
        PreloadFrame(m_entitys[index]->m_battleState["帧池"]);
        sp = Sprite::createWithSpriteFrameName(m_entitys[index]->m_battleState["正常"]);
        animation_cache->addAnimation(animation, m_entitys[index ]->getName());
        sp->setPosition(m_entityPos[index]);
        m_entitySprite[index] = sp;
        // init shader
        // auto glProgram = GLProgram::createWithFilenames("shader/water.vsh", "shader/water.fsh");
        // auto state = GLProgramState::getOrCreateWithGLProgram(glProgram);
        // sp->setGLProgramState(state);
        addChild(sp);
    }
    
    // 记得退出场景时要从animation cache中移除
    
    //////////////////////////////
    // 3. battle panel
    m_battlePanel = BattlePanel::create();
    m_battlePanel->onBattleButtonTrigger = CC_CALLBACK_1(BattleScene::onBattleButtonTrigger, this);
    addChild(m_battlePanel);
    
    
    return true;
}

void BattleScene::onBattleButtonTrigger(BattlePanel::BattleButtonType type)
{
    auto animation_cache = AnimationCache::getInstance();
    switch (type) {
        case BattlePanel::BattleButtonType::Physical:
        {
            auto animateAction = Animate::create(animation_cache->getAnimation(m_entitys[MAIN_PLAYER]->getName()));
            m_entitySprite[5]->runAction(Sequence::create(MoveTo::create(0.5, m_entityPos[0]), animateAction, MoveTo::create(0.5, m_entityPos[5]), NULL));
            
        }
            break;
            
        default:
            break;
    }
}
