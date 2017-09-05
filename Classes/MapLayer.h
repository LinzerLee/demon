#ifndef _MAP_LAYER_H__
#define _MAP_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;

class Item;
class Entity;

class MapLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(MapLayer);
    
    virtual bool init();
    cocos2d::Point getPosition();
    void setPosition(cocos2d::Point point);
    cocos2d::Size getTileSize();
    cocos2d::Point getSpawnPoint();
    cocos2d::Point getOriginPoint();
    void setOriginPoint(cocos2d::Point point);
    cocos2d::Point getMaxPoint();
    void setMaxPoint(cocos2d::Point point);
    cocos2d::Size getContentSize();
    bool checkCollision(cocos2d::Point point);
    void resetMap(std::string mapName);
    // 添加物品
    void loadItem(Item *item);
    // 添加实体
    void loadEntity(Entity *entity);
public:
    cocos2d::Rect getSafeRect();
    
    cocos2d::TMXTiledMap* m_map;
    cocos2d::Point m_originPoint;
    cocos2d::Point m_maxPoint;
};

#endif // _MAP_LAYER_H__
