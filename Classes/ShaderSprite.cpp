#include "ShaderSprite.h"

USING_NS_CC;
ShaderSprite::ShaderSprite(void)
{
    m_uSpriteFilter=Mat4::IDENTITY;
}

ShaderSprite::~ShaderSprite()
{
    
}

ShaderSprite* ShaderSprite::create()
{
    ShaderSprite *sprite = new (std::nothrow) ShaderSprite();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

ShaderSprite* ShaderSprite::create(const std::string& filename)
{
    ShaderSprite *sprite = new (std::nothrow) ShaderSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

ShaderSprite* ShaderSprite::create(const std::string& filename, const Rect& rect)
{
    ShaderSprite *sprite = new (std::nothrow) ShaderSprite();
    if (sprite && sprite->initWithFile(filename, rect))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

ShaderSprite* ShaderSprite::createWithTexture(Texture2D *pTexture)
{
    ShaderSprite *sprite = new (std::nothrow) ShaderSprite();
    Rect rect = Rect::ZERO;
    rect.size = pTexture->getContentSize();
    if (sprite && sprite->initWithTexture(pTexture,rect))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

ShaderSprite* ShaderSprite::createWithTexture(Texture2D *texture, const Rect& rect, bool rotated)
{
    ShaderSprite *sprite = new (std::nothrow) ShaderSprite();
    if (sprite && sprite->initWithTexture(texture, rect))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

ShaderSprite* ShaderSprite::createWithSpriteFrame(SpriteFrame *spriteFrame)
{
    ShaderSprite *sprite = new (std::nothrow) ShaderSprite();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

ShaderSprite* ShaderSprite::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    
    return createWithSpriteFrame(frame);
}

void ShaderSprite::loadShaderVertex(const char *vert, const char *frag)
{
    GLProgram* shader = new GLProgram();
    shader->initWithVertexShaderFilename(vert, frag);   //载入着色器程序
    setGLProgramState(GLProgramState::getOrCreateWithGLProgram(shader));
}

bool ShaderSprite::initWithTexture(Texture2D* pTexture, const Rect& tRect){
    do{
        CC_BREAK_IF(!Sprite::initWithTexture(pTexture, tRect));
        
        GLchar* pszFragSource =
        "#ifdef GL_ES \n \
        precision mediump float; \n \
        #endif \n \
        uniform sampler2D u_texture; \n \
        varying vec2 v_texCoord; \n \
        varying vec4 v_fragmentColor; \n \
        uniform mat4 fiterMat; \n \
        void main(void) \n \
        { \n \
        vec4 value = v_fragmentColor*texture2D(u_texture, v_texCoord); \n \
        gl_FragColor = fiterMat*value; \n \
        }";
        
        auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_vert, pszFragSource);
        auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
        setGLProgramState(glprogramstate);
        
        CHECK_GL_ERROR_DEBUG();
        
        return true;
    } while (0);
    return false;
}

void  ShaderSprite::setFilterMat(cocos2d::Mat4 matrixArray)
{
    m_uSpriteFilter=matrixArray;
}

void ShaderSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(ShaderSprite::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void ShaderSprite::onDraw(const Mat4 &transform, uint32_t flags)
{
    auto glProgramState = getGLProgramState();
    glProgramState->setUniformMat4("fiterMat",m_uSpriteFilter);
    glProgramState->apply(transform);
    
    GL::blendFunc( _blendFunc.src, _blendFunc.dst );
    
    GL::bindTexture2D( _texture->getName() );
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX );
    
#define kQuadSize sizeof(_quad.bl)
    size_t offset = (size_t)&_quad;
    
    // vertex
    int diff = offsetof( V3F_C4B_T2F, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( V3F_C4B_T2F, texCoords);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
    // color
    diff = offsetof( V3F_C4B_T2F, colors);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    CHECK_GL_ERROR_DEBUG();
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,4);
}
