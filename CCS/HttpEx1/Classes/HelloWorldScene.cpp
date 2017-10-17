﻿#include "HelloWorldScene.h"
//#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace cocos2d::network;

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

	auto wlayer = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(wlayer);

	/////////////////////////////

	MenuItemFont::setFontSize(24);
	MenuItemFont::setFontName("Courier New");

	auto pMenuItem1 = MenuItemFont::create("Get", CC_CALLBACK_1(HelloWorld::onGetTest, this));
	pMenuItem1->setColor(Color3B(0, 0, 0));

	auto pMenuItem2 = MenuItemFont::create("Post", CC_CALLBACK_1(HelloWorld::onPostTest, this));
	pMenuItem2->setColor(Color3B(0, 0, 0));

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, nullptr);

	pMenu->alignItemsVertically();

	this->addChild(pMenu);

	IblStatusCode = LabelTTF::create("HTTP Status Code", "Courier", 20);
	IblStatusCode->setPosition(Vec2(240, 260));
	IblStatusCode->setColor(Color3B::BLUE);
	this->addChild(IblStatusCode);

    return true;
}

void HelloWorld::onGetTest(Ref* sender)
{
	{
		HttpRequest* request = new HttpRequest();
		request->setUrl("http://just-make-this-request-faild.com");
		request->setRequestType(HttpRequest::Type::GET);
		request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onHttpRequestCompleted, this));

		request->setTag("GET test1");

		HttpClient::getInstance()->send(request);
		request->release();
	}

	{
		HttpRequest* request = new HttpRequest();
		request->setUrl("http://httpbin.org/get");
		request->setRequestType(HttpRequest::Type::GET);
		request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onHttpRequestCompleted, this));

		request->setTag("GET test2");

		HttpClient::getInstance()->send(request);
		request->release();
	}

	IblStatusCode->setString("waiting");
}

void HelloWorld::onPostTest(Ref *sender)
{
	{
		HttpRequest* request = new HttpRequest();
		request->setUrl("http://httpbin.org/post");
		request->setRequestType(HttpRequest::Type::POST);
		request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onHttpRequestCompleted, this));

		const char* postData = "visitor=cocos2d&TestSuite=NetworkTest";

		request->setRequestData(postData, strlen(postData));
		request->setTag("POST test1");

		HttpClient::getInstance()->send(request);
		request->release();
	}
	{
		HttpRequest* request = new HttpRequest();
		request->setUrl("http://httpbin.org/post");
		request->setRequestType(HttpRequest::Type::POST);
		request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onHttpRequestCompleted, this));

		std::vector<std::string> headers;
		headers.push_back("Content-Type: applications/json;charset=utf-8");
		request->setHeaders(headers);

		const char* postData = "visitor=cocos2d&TestSuite=NetworkTest";

		request->setRequestData(postData, strlen(postData));
		request->setTag("POST test2");

		HttpClient::getInstance()->send(request);
		request->release();
	}

	IblStatusCode->setString("waiting");
}

void HelloWorld::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	if (!response)
	{
		return;
	}

	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}

	int statusCode = response->getResponseCode();
	char statusString[64] = { 0 };
	sprintf(statusString, "HTTP Status Code:%d,\ntag=%s", statusCode, response->getHttpRequest()->getTag());
	IblStatusCode->setString(statusString);

	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer : %s", response->getErrorBuffer());
		return;
	}

	std::vector<char> *buffer = response->getResponseData();
	log("Http Test, dump data: ");
	char str[1024] = { 0 };
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		sprintf(str, "%s%c", str, (*buffer)[i]);
	}
	log("%s", str);
}