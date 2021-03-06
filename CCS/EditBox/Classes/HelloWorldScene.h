﻿#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "cocos-ext.h"

class HelloWorld : public cocos2d::Scene,public cocos2d::ui::EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(HelloWorld);

	virtual void editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string &text);
	virtual void editBoxReturn(cocos2d::extension::EditBox* editBox);

	cocos2d::ui::EditBox *m_pEditName;
	cocos2d::ui::EditBox *m_pEditPassword;
	cocos2d::ui::EditBox *m_pEditEmail;

};

#endif // __HELLOWORLD_SCENE_H__
