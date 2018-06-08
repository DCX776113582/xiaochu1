#include "GameScene.h"
#include "JewelsGrid.h"
#include "GameOverScene.h"

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

LoadingBar* GameScene::m_bonusbar = nullptr;
int GameScene::m_score = 0;
Label* GameScene::m_scorelabel = nullptr;

bool GameScene::init()
{
	Layer::init();

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto texturecache = TextureCache::getInstance();

	m_bg = Sprite::createWithTexture(texturecache->getTextureForKey("bground1.png"));
	m_bg->setAnchorPoint(Vec2(0, 0));
	m_bg->setTag(100);
	addChild(m_bg);

	auto board = Sprite::createWithTexture(texturecache->getTextureForKey("board.png"));
	board->setAnchorPoint(Vec2(0, 1));
	board->setPosition(Vec2(0, visibleSize.height));
	board->setOpacity(80);
	addChild(board);

	m_jewelsgrid = JewelsGrid::create(8, 8);
	addChild(m_jewelsgrid);
	m_jewelsgrid->setPosition(0, visibleSize.height - m_jewelsgrid->getRow() * GRID_WIDTH);

	auto bounusbar_frame = Sprite::createWithTexture(texturecache->getTextureForKey("bonusbar.png"));
	bounusbar_frame->setPosition(Vec2(visibleSize.width / 2, bounusbar_frame->getContentSize().height / 2 + 10));
	addChild(bounusbar_frame);

	m_bonusbar = LoadingBar::create("bonusbar_fill.png");
	m_bonusbar->setPercent(100);
	m_bonusbar->setPosition(bounusbar_frame->getPosition());
	addChild(m_bonusbar);

	schedule(schedule_selector(GameScene::onReducingBonus), 0.1);

	auto bonus = Sprite::createWithTexture(texturecache->getTextureForKey("bonus.png"));
	bonus->setPosition(visibleSize.width - bonus->getContentSize().width / 2 - 10, 80);
	addChild(bonus);

	m_scorelabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 24);
	m_scorelabel->setAnchorPoint(Vec2(1, 0));
	m_scorelabel->setPosition(visibleSize.width - 10, 35);
	m_scorelabel->setTag(10);
	m_scorelabel->enableOutline(Color4B::BLACK, 1);
	addChild(m_scorelabel);

	auto updateMenu = MenuItemFont::create("Update Map", CC_CALLBACK_1(GameScene::onUpdateMenuCallback, this));
	updateMenu->setAnchorPoint(Vec2(1, 0));
	updateMenu->setPosition(visibleSize.width / 2, -visibleSize.height / 2);
	auto menu = Menu::create(updateMenu, nullptr);
	addChild(menu);

	menu->setEnabled(false);
	menu->setVisible(false);

	return true;
}

void GameScene::publishScore()
{
	auto userdefault = UserDefault::getInstance();
	
	log(userdefault->getXMLFilePath().c_str());
	
	char score_str[100] = {0};
	sprintf(score_str, "%d", m_score);
	userdefault->setStringForKey("LastScore", score_str);

	auto bestscore = userdefault->getStringForKey("BestScore");
	if (m_score > atoi(bestscore.c_str()))
		userdefault->setStringForKey("BestScore", score_str);
}

void GameScene::onReducingBonus(float dt)
{
	m_bonusbar->setPercent(m_bonusbar->getPercent()-0.2);

	if (m_bonusbar->getPercent() == 0)
	{
		unschedule(schedule_selector(GameScene::onReducingBonus));

		log("game over!");
		publishScore();
		auto scene = GameOverScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
	}

	auto fadein = FadeIn::create(0.1);
	auto fadeout = FadeOut::create(0.1);
	CallFunc* call = nullptr;
	if (m_bonusbar->getPercent() < 100 && m_bonusbar->getPercent() > 75)
	{
		if (m_bg->getTag() == 100)
			call = nullptr;
		else
		{
			call = CallFunc::create([this](){
				m_bg->setTexture(TextureCache::getInstance()->getTextureForKey("bground1.png"));
			});
			m_bg->setTag(100);
		}
	}

	if (m_bonusbar->getPercent() < 75 && m_bonusbar->getPercent() > 50)
	{
		if (m_bg->getTag() == 101)
			call = nullptr;
		else
		{
			call = CallFunc::create([this](){
				m_bg->setTexture(TextureCache::getInstance()->getTextureForKey("bground2.png"));
				m_bg->setTag(101);
			});
		}
	}

	if (m_bonusbar->getPercent() < 50 && m_bonusbar->getPercent() > 25)
	{
		if (m_bg->getTag() == 102)
			call = nullptr;
		else
		{
			call = CallFunc::create([this](){
				m_bg->setTexture(TextureCache::getInstance()->getTextureForKey("bground3.png"));
				m_bg->setTag(102);
			});
		}
	}

	if (m_bonusbar->getPercent() < 25)
	{
		if (m_bg->getTag() == 103)
			call = nullptr;
		else
		{
			call = CallFunc::create([this](){
				m_bg->setTexture(TextureCache::getInstance()->getTextureForKey("bground4.png"));
				m_bg->setTag(103);
			});
		}
	}

	if (call)
	{
		m_bg->runAction(Sequence::create(fadeout, call, fadein, nullptr));
	}
}

void GameScene::addBonus(int bonus)
{
	m_score += 20 * bonus;
	char buf[100] = {0};
	sprintf(buf, "%d", m_score);
	m_scorelabel->setString(buf);

	m_bonusbar->setPercent(m_bonusbar->getPercent()+bonus);
}

void GameScene::onUpdateMenuCallback(Ref*)
{
	m_jewelsgrid->updateMap();

	if (m_jewelsgrid->isDeadMap())
	{
		log("this is a dead map!");
	}
	else
	{
		log("this is not a dead map~");
	}
}