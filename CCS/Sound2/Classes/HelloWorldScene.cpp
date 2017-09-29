﻿#include "HelloWorldScene.h"
#include "AudioEngine.h"

USING_NS_CC;
using namespace experimental;

const std::string BGM_1_PATH = "Sounds/bgm1.mp3";
const std::string BGM_2_PATH = "Sounds/bgm2.mp3";

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

	auto wlayer = LayerColor::create(Color4B(0, 0, 0, 255));
	this->addChild(wlayer);

	/////////////////////////////

	_soundVolume = 1.0f;

	MenuItemFont::setFontSize(18);

	auto pMenuItem1A = MenuItemFont::create("PreLoad ALL",
		CC_CALLBACK_1(HelloWorld::preloadALL, this));
	
	auto pMenuItem1B = MenuItemFont::create("Uncache ALL",
		CC_CALLBACK_1(HelloWorld::uncacheALL, this));

	auto pMenuItem1C = MenuItemFont::create("Volume Up",
		CC_CALLBACK_1(HelloWorld::upVolume, this));

	auto pMenuItem1D = MenuItemFont::create("Volume Down",
		CC_CALLBACK_1(HelloWorld::downVolume, this));

	auto pMenu1 = Menu::create(pMenuItem1A, pMenuItem1B, pMenuItem1C, pMenuItem1D, NULL);
	pMenu1->alignItemsHorizontallyWithPadding(20);
	pMenu1->setPosition(Vec2(240, 280));

	this->addChild(pMenu1);

	//
	MenuItemFont::setFontSize(24);

	auto pMenuItem2A = MenuItemFont::create("PlaySound1",
		CC_CALLBACK_1(HelloWorld::playSound1, this));

	auto pMenuItem2B = MenuItemFont::create("PAUSESound1",
		CC_CALLBACK_1(HelloWorld::pauseSound1, this));

	auto pMenuItem2C = MenuItemFont::create("RESUMESound1",
		CC_CALLBACK_1(HelloWorld::resumeSound1, this));

	auto pMenuItem2D = MenuItemFont::create("STOPSound1",
		CC_CALLBACK_1(HelloWorld::stopSound1, this));


	auto pMenu2 = Menu::create(pMenuItem2A, pMenuItem2B, pMenuItem2C, pMenuItem2D, NULL);
	pMenu2->alignItemsVertically();
	pMenu2->setPosition(Vec2(140, 160));

	this->addChild(pMenu2);
	

	auto pMenuItem3A = MenuItemFont::create("PlaySound2",
		CC_CALLBACK_1(HelloWorld::playSound2, this));

	auto pMenuItem3B = MenuItemFont::create("PAUSESound2",
		CC_CALLBACK_1(HelloWorld::pauseSound2, this));

	auto pMenuItem3C = MenuItemFont::create("RESUMESound2",
		CC_CALLBACK_1(HelloWorld::resumeSound2, this));

	auto pMenuItem3D = MenuItemFont::create("STOPSound2",
		CC_CALLBACK_1(HelloWorld::stopSound2, this));


	auto pMenu3 = Menu::create(pMenuItem3A, pMenuItem3B, pMenuItem3C, pMenuItem3D, NULL);
	pMenu3->alignItemsVertically();
	pMenu3->setPosition(Vec2(360, 160));

	this->addChild(pMenu3);

    return true;
}

void HelloWorld::preloadALL(Ref *pSender)
{
	AudioEngine::preload(BGM_1_PATH);
	AudioEngine::preload(BGM_2_PATH);
}
void HelloWorld::uncacheALL(Ref *pSender)
{
	AudioEngine::stop(audioId1);
	AudioEngine::stop(audioId2);
}

void HelloWorld::upVolume(Ref *pSender)
{
	if (_soundVolume < 1.0f)
		_soundVolume += 0.1f;

	AudioEngine::setVolume(audioId1, _soundVolume);
	AudioEngine::setVolume(audioId2, _soundVolume);
}
void HelloWorld::downVolume(Ref *pSender)
{
	if (_soundVolume > 0.0f)
		_soundVolume = 0.1f;

	AudioEngine::setVolume(audioId1, _soundVolume);
	AudioEngine::setVolume(audioId2, _soundVolume);
}

void HelloWorld::playSound1(Ref *pSender)
{
	if (AudioEngine::getState(audioId1) != AudioEngine::AudioState::PLAYING)
		audioId1 = AudioEngine::play2d(BGM_1_PATH, true, _soundVolume);
}
void HelloWorld::playSound2(Ref *pSender)
{
	if (AudioEngine::getState(audioId2) != AudioEngine::AudioState::PLAYING)
		audioId2 = AudioEngine::play2d(BGM_2_PATH, true, _soundVolume);
}

void HelloWorld::pauseSound1(Ref *pSender)
{
	if (AudioEngine::getState(audioId1) == AudioEngine::AudioState::PLAYING)
		AudioEngine::pause(audioId1);
}
void HelloWorld::pauseSound2(Ref *pSender)
{
	if (AudioEngine::getState(audioId2) == AudioEngine::AudioState::PLAYING)
		AudioEngine::pause(audioId2);
}

void HelloWorld::resumeSound1(Ref *pSender)
{
	if (AudioEngine::getState(audioId1) == AudioEngine::AudioState::PAUSED)
		AudioEngine::resume(audioId1);
}
void HelloWorld::resumeSound2(Ref *pSender)
{
	if (AudioEngine::getState(audioId2) == AudioEngine::AudioState::PAUSED)
		AudioEngine::resume(audioId2);
}

void HelloWorld::stopSound1(Ref *pSender)
{
	if (AudioEngine::getState(audioId1) == AudioEngine::AudioState::PLAYING)
		AudioEngine::stop(audioId1);
}
void HelloWorld::stopSound2(Ref *pSender)
{
	if (AudioEngine::getState(audioId2) == AudioEngine::AudioState::PLAYING)
		AudioEngine::stop(audioId2);
}

void HelloWorld::bgmPlayFinishedCallBack(int audioId, const std::
	string & filepath)
{
}

HelloWorld::~HelloWorld()
{
	AudioEngine::end();
}