#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "WiFiJoystickManger.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    /**  you can create scene with following comment code instead of using csb file.
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    **/
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    m_inputIPAddr = dynamic_cast<cocos2d::ui::TextField*>(rootNode->getChildByName("txtinput_serverIP"));
    m_inputIPAddr->addEventListener(CC_CALLBACK_2(HelloWorld::textFieldTextTouchEvent,this));
    
    
    txt_datalog = dynamic_cast<cocos2d::ui::Text*>(rootNode->getChildByName("txt_data"));
    
    m_UpTxt = dynamic_cast<cocos2d::ui::Text*>(rootNode->getChildByName("txt_up"));
    m_DownTxt = dynamic_cast<cocos2d::ui::Text*>(rootNode->getChildByName("txt_down"));
    m_LeftTxt = dynamic_cast<cocos2d::ui::Text*>(rootNode->getChildByName("txt_left"));
    m_RightTxt = dynamic_cast<cocos2d::ui::Text*>(rootNode->getChildByName("txt_right"));
    
    m_BtnTxt1 = dynamic_cast<cocos2d::ui::Text*>(rootNode->getChildByName("txt_b1"));
    m_BtnTxt2 = dynamic_cast<cocos2d::ui::Text*>(rootNode->getChildByName("txt_b2"));
    m_BtnTxt3 = dynamic_cast<cocos2d::ui::Text*>(rootNode->getChildByName("txt_a1"));
    m_BtnTxt4 = dynamic_cast<cocos2d::ui::Text*>(rootNode->getChildByName("txt_a2"));
    
    m_LBtnTxt1 = dynamic_cast<cocos2d::ui::Text*>(rootNode->getChildByName("txt_l1"));
    m_LBtnTxt2 = dynamic_cast<cocos2d::ui::Text*>(rootNode->getChildByName("txt_l2"));
    m_RBtnTxt1 = dynamic_cast<cocos2d::ui::Text*>(rootNode->getChildByName("txt_R1"));
    m_RBtnTxt2 = dynamic_cast<cocos2d::ui::Text*>(rootNode->getChildByName("txt_R2"));
    
    
    m_connectBtn = dynamic_cast<cocos2d::ui::Button*>(rootNode->getChildByName("btc_con"));
    m_connectBtn->addTouchEventListener([=](cocos2d::Ref* obj,cocos2d::ui::Widget::TouchEventType touchtype)->void{
        
        switch (touchtype) {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:    //点击开始
            {
                
            }
                break;
            case cocos2d::ui::Widget::TouchEventType::MOVED:    //点击移动
            {
                
            }
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:    //点击停止
            {
                this->connectJoystatic();
            }
                break;
            case cocos2d::ui::Widget::TouchEventType::CANCELED: //点击取消
            {
                
            }
                break;
                
            default:
                break;
        }
    });
    
    addChild(rootNode);

    return true;
}


void HelloWorld::connectJoystatic()
{
//    WiFiJoystickManger::sharedWiFiJoystickManger()->addNewJoystick(m_IpValue, "123", [=](int jdat)->void{
//        cocos2d::log("get joy Data:%x",jdat);
//    });
//    
    WiFiJoystickManger::sharedWiFiJoystickManger()->addNewJoystick("192.168.1.107", "123", [=](int jdat)->void{
        cocos2d::log("get joy Data:%x",jdat);
        this->joystaticChange(jdat);
    });
    
    WiFiJoystickManger::sharedWiFiJoystickManger()->connectSocket(1);
}

void HelloWorld::joystaticChange(int jdat)
{
    //手柄状态数据改变
}

void HelloWorld::btnPush(int btnID)
{
    switch (btnID) {
        case 1:
        {
            m_UpTxt->setColor(cocos2d::Color3B(255,0,0));
        }
            break;
        case 2:
        {
            m_DownTxt->setColor(cocos2d::Color3B(255,0,0));
        }
            break;
        case 3:
        {
            m_LeftTxt->setColor(cocos2d::Color3B(255,0,0));
        }
            break;
        case 4:
        {
            m_RightTxt->setColor(cocos2d::Color3B(255,0,0));
        }
            break;
        case 5:
        {
            m_BtnTxt1->setColor(cocos2d::Color3B(255,0,0));
        }
            break;
        case 6:
        {
            m_BtnTxt2->setColor(cocos2d::Color3B(255,0,0));
        }
            break;
        case 7:
        {
            m_BtnTxt3->setColor(cocos2d::Color3B(255,0,0));
        }
            break;
        case 8:
        {
            m_BtnTxt4->setColor(cocos2d::Color3B(255,0,0));
        }
            break;
        case 9:
        {
            m_LBtnTxt1->setColor(cocos2d::Color3B(255,0,0));
        }
            break;
        case 10:
        {
            m_LBtnTxt2->setColor(cocos2d::Color3B(255,0,0));
        }
            break;
        case 11:
        {
            m_RBtnTxt1->setColor(cocos2d::Color3B(255,0,0));
        }
            break;
        case 12:
        {
            m_RBtnTxt2->setColor(cocos2d::Color3B(255,0,0));
        }
            break;
        default:
            break;
    }
}
void HelloWorld::btnPress(int btnID)
{
    switch (btnID) {
        case 1:
        {
            m_UpTxt->setColor(cocos2d::Color3B(255,255,255));
        }
            break;
        case 2:
        {
            m_DownTxt->setColor(cocos2d::Color3B(255,255,255));
        }
            break;
        case 3:
        {
            m_LeftTxt->setColor(cocos2d::Color3B(255,255,255));
        }
            break;
        case 4:
        {
            m_RightTxt->setColor(cocos2d::Color3B(255,255,255));
        }
            break;
        case 5:
        {
            m_BtnTxt1->setColor(cocos2d::Color3B(255,255,255));
        }
            break;
        case 6:
        {
            m_BtnTxt2->setColor(cocos2d::Color3B(255,255,255));
        }
            break;
        case 7:
        {
            m_BtnTxt3->setColor(cocos2d::Color3B(255,255,255));
        }
            break;
        case 8:
        {
            m_BtnTxt4->setColor(cocos2d::Color3B(255,255,255));
        }
            break;
        case 9:
        {
            m_LBtnTxt1->setColor(cocos2d::Color3B(255,255,255));
        }
            break;
        case 10:
        {
            m_LBtnTxt2->setColor(cocos2d::Color3B(255,255,255));
        }
            break;
        case 11:
        {
            m_RBtnTxt1->setColor(cocos2d::Color3B(255,255,255));
        }
            break;
        case 12:
        {
            m_RBtnTxt2->setColor(cocos2d::Color3B(255,255,255));
        }
            break;
        default:
            break;
    }
}


//TextField
void HelloWorld::textFieldTextTouchEvent(Ref* obj, cocos2d::ui::TextField::EventType type)
{
    switch (type) {
        case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME://开启输入
        {
            
        }
            break;
        case cocos2d::ui::TextField::EventType::DETACH_WITH_IME://关闭输入
        {
            m_IpValue = m_inputIPAddr->getString();//输入框中的值
            
        }
            break;
        case cocos2d::ui::TextField::EventType::INSERT_TEXT://插入新文本
        {
            
        }
            break;
        case cocos2d::ui::TextField::EventType::DELETE_BACKWARD://删除文本
        {
            
        }
            break;
        default:
            break;
    }
}
