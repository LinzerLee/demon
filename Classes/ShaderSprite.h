
#ifndef __SHADER_SPRITE__
#define __SHADER_SPRITE__

#include "cocos2d.h"

class ShaderSprite : public cocos2d::Sprite{
    
public:
    
    ShaderSprite();
    virtual ~ShaderSprite();
    
    static ShaderSprite* create();
    static ShaderSprite* create(const std::string& filename);
    static ShaderSprite* create(const std::string& filename, const cocos2d::Rect& rect);
    
    
    static ShaderSprite* createWithTexture(cocos2d::Texture2D *pTexture);
    static ShaderSprite* createWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated=false);
    static ShaderSprite* createWithSpriteFrame(cocos2d::SpriteFrame *pSpriteFrame);
    static ShaderSprite* createWithSpriteFrameName(const std::string& spriteFrameName);
    void loadShaderVertex(const char *vert, const char *frag);
    bool initWithTexture(cocos2d::Texture2D* pTexture, const cocos2d::Rect& tRect);
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    void setFilterMat(cocos2d::Mat4 matrixArray);
    //to-do 提供一个设置滤镜的方法
protected:
    cocos2d::CustomCommand _customCommand;
private:
    cocos2d::Mat4   m_uSpriteFilter;
};
#endif // __SHADER_SPRITE__

