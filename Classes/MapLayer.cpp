#include "MapLayer.h"
#include "Item.h"
#include "Entity.h"

USING_NS_CC;

bool MapLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }
    
    m_map = nullptr;
    
    // 启动帧循环调度
    // scheduleUpdate();
    
    return true;
}

void MapLayer::setPosition(Point point)
{
    // 安全域限制
    Rect safeRect = getSafeRect();
    
    if(safeRect.containsPoint(point))
        return m_map->setPosition(- point);
}

Point MapLayer::getPosition()
{
    return - m_map->getPosition();
}

Size MapLayer::getContentSize()
{
    Size titleNum = m_map->getMapSize();
    Size titleSize = getTileSize();
    
    return Size(titleSize.width * titleNum.width, titleSize.height * titleNum.height);
}

bool MapLayer::checkCollision(Point point)
{
    Size titleSize = getTileSize();
    Size pos((int)(point.x / titleSize.width), m_map->getMapSize().height - (int)(point.y / titleSize.height) - 1);
    TMXLayer *metaLayer = m_map->getLayer("Barrier");
    
    if (nullptr == metaLayer)
        return false;
    
    int gid = metaLayer->getTileGIDAt(pos);
    
    if (gid != 0)
    {
        ValueMap properties = m_map->getPropertiesForGID(gid).asValueMap();
        Value prop = properties["Collision"];
        return prop.asBool();
    }
    
    return false;
}

void MapLayer::resetMap(std::string mapName)
{
    //////////////////////////////
    // 1. 创建地图
    if (nullptr != m_map)
    {
        m_map->removeFromParent();
        m_map = nullptr;
    }
    mapName += ".tmx";
    m_map = TMXTiledMap::create(mapName);
    if (nullptr == m_map)
    {
        log("nullptr == map");
        return;
    }
    
    addChild(m_map);
    
    //////////////////////////////
    // 2. safe area
    setOriginPoint(Point(0, 0) + getTileSize());
    setMaxPoint(Point(0, 0) + getContentSize() - getTileSize());
    
    //////////////////////////////
    // 3. move origin point
    setPosition(getOriginPoint());
}

Size MapLayer::getTileSize()
{
    return m_map->getTileSize();
}

Point MapLayer::getSpawnPoint()
{
    auto objGroup = m_map->getObjectGroup("Object");
    ValueMap spawnPointMap = objGroup->getObject("SpawnPoint");
    // 调整SpawnPoint至中心,默认在左下角
    return Point(spawnPointMap.at("x").asFloat(), spawnPointMap.at("y").asFloat()) + getTileSize() / 2;
}


Point MapLayer::getOriginPoint()
{
    return m_originPoint;
}

void MapLayer::setOriginPoint(cocos2d::Point point)
{
    m_originPoint = point;
}

Point MapLayer::getMaxPoint()
{
    return m_maxPoint;
}

void MapLayer::setMaxPoint(cocos2d::Point point)
{
    m_maxPoint = point;
}

Rect MapLayer::getSafeRect()
{
    return Rect(m_originPoint.x, m_originPoint.y, 
                m_maxPoint.x - m_originPoint.x,
                m_maxPoint.y - m_originPoint.y);
}

void MapLayer::loadItem(Item *item)
{
    m_map->addChild(item);
}

void MapLayer::loadEntity(Entity *entity)
{
    m_map->addChild(entity);
}
