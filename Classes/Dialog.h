#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "cocos2d.h"

class Dialog : public cocos2d::Node
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(Dialog);
	virtual bool init();

	void show(std::string name, std::string content, std::string protrait = "");
	void hide();
private:
	cocos2d::Sprite *m_background;
	cocos2d::Sprite *m_portrait;
	cocos2d::Sprite *m_next;
	cocos2d::Label *m_name;
	cocos2d::Label *m_content;
};

#endif // __DIALOG_H__
