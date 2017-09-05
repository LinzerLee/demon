#ifndef __TIP_H__
#define __TIP_H__

#include "cocos2d.h"

class Tip : public cocos2d::Node
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(Tip);
	virtual bool init();
	
	void show(std::string content, float timeout, cocos2d::Point pos = cocos2d::Point(-1, -1));
	void hide(float dt = 0);
	cocos2d::Size size();
private:
	cocos2d::Sprite *m_sprite;
	cocos2d::Label *m_label;
};

#endif // __TIP_H__
