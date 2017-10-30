﻿#include "HelloWorldScene.h"
//#include "SimpleAudioEngine.h"

USING_NS_CC;

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

	/////////////////////////////

	winSize = Director::getInstance()->getWinSize();

	texture = Director::getInstance()->getTextureCache()->addImage("ball.png");

	if (this->createBox2dWorld(true))
	{
		srand((int)time(nullptr));
		SetBox2dWorld();
		this->schedule(schedule_selector(HelloWorld::tick));
	}

    return true;
}

void HelloWorld::onEnter()
{
	Scene::onEnter();

	// 싱글터치모드로 터치리스너 등록
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool HelloWorld::createBox2dWorld(bool debug)
{
	b2Vec2 gravity = b2Vec2(0.0f, -30.0f); // 중력의 방향을 결정한다.
	_world = new b2World(gravity);

	_world->SetAllowSleeping(true); // 휴식중 멈출지 여부. 켜라
	_world->SetContinuousPhysics(true); //지속적 물리작용 여부
	
	if (debug)
	{
		//적색 : 현재 물리 운동 중 <-> 회색

		m_debugDraw = new GLESDebugDraw(PTM_RATIO);
		_world->SetDebugDraw(m_debugDraw);

		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;
		//flags += b2Draw::e_aabbBit;
		//flags += b2Draw::e_pairBit;
		//flags += b2Draw::e_centerOfMassBit;

		m_debugDraw->SetFlags(flags);
	}


	//가장자리 생성

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);
	b2Body *groundBody = _world->CreateBody(&groundBodyDef);


	//경계면을 그릴 수 있는 객체 생성
	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;

	//아래
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(winSize.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	//왼쪽
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	//위
	groundEdge.Set(b2Vec2(0, winSize.height / PTM_RATIO),
		b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	//오른쪽
	groundEdge.Set(b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO),
		b2Vec2(winSize.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	return true;
}

void HelloWorld::SetBox2dWorld()
{
	playerIsFlying = false;

	// 새로운 물리 객체의 바디와 해당 스프라이트를 추가한다.
	addNewSpriteAtPosition(Vec2(240, 160));
}

HelloWorld::~HelloWorld()
{
	delete _world;
	_world = NULL;
}


void HelloWorld::onDraw(const Mat4 &transform, uint32_t flags)
{
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	_world->DrawDebugData();
	CHECK_GL_ERROR_DEBUG();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	ccGLEnableVertexAttribs(kCCVertexAttrib_Position);
	kmGLPushMatrix();
	_world->DrawDebugData();
	kmGLPopMatrix();

	Scene::draw(renderer, transform, flags);

	_customCmd.init(_globalZOrder, transform, flags);
	_customCmd.func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);

}




void HelloWorld::tick(float dt)
{
	int velocityIterations = 8;
	int positionIterations = 3;

	// Step : 물리 세계를 시뮬레이션한다.
	_world->Step(dt, velocityIterations, positionIterations);

	// 만들어진 객체 만큼 루프를 돌리면서 바디에 붙인 스프라이트를 여기서 제어한다.
	for (b2Body* b = _world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != nullptr) {
			Sprite* spriteData = (Sprite *)b->GetUserData();

			spriteData->setPosition(Vec2(b->GetPosition().x * PTM_RATIO,
				b->GetPosition().y * PTM_RATIO));
			spriteData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}

	if (playerIsFlying)
	{
		myBall->ApplyLinearImpulse(b2Vec2(0, playerVelocity),
			myBall->GetWorldCenter(),
			true);
		playerVelocity += 0.01f;

		if (playerVelocity > 1.5f)
			playerVelocity = 1.5f;
	}
}

void HelloWorld::onExit()
{
	_eventDispatcher->removeAllEventListeners();
	Scene::onExit();
}


bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	playerVelocity = 0.5f;
	playerIsFlying = true;

	return true;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
	playerIsFlying = false;
	playerVelocity = 0.0f;
}

void HelloWorld::onTouchMoved(Touch *touch, Event * event)
{
}


void HelloWorld::addNewSpriteAtPosition(Vec2 location)
{
	// 바디데프 만들고 속성들을 지정한다.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(location.x / PTM_RATIO, location.y / PTM_RATIO);

	// 보통은 유져데이터에 스프라이트를 붙이는데
	// 여기서는 아무 데이타를 넣지 않고 디버그 드로잉만 수행한다.
	bodyDef.userData = nullptr;

	// 월드에 바디데프의 정보로 바디를 만든다.
	myBall = _world->CreateBody(&bodyDef);

	// 바디에 적용할 물리 속성용 바디의 모양을 만든다. 여기서는 원을 만든다.
	b2CircleShape circle;
	// 바디의 크기 지정 - 원의 경우엔 반지름
	circle.m_radius = 0.45f;

	b2FixtureDef fixtureDef;
	// 모양
	fixtureDef.shape = &circle;
	// 밀도
	fixtureDef.density = 1.0f;
	// 마찰력
	fixtureDef.friction = 0.2f;
	// 반발력
	fixtureDef.restitution = 0.6f;

	myBall->CreateFixture(&fixtureDef);
}