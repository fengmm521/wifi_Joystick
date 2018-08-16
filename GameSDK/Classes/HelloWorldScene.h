#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CocosGUI.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    ui::Button* m_connectBtn;
    
    ui::Text* txt_datalog;
    
    ui::Text* m_UpTxt;
    ui::Text* m_DownTxt;
    ui::Text* m_LeftTxt;
    ui::Text* m_RightTxt;
    
    ui::Text* m_BtnTxt1;
    ui::Text* m_BtnTxt2;
    ui::Text* m_BtnTxt3;
    ui::Text* m_BtnTxt4;
    
    ui::Text* m_LBtnTxt1;
    ui::Text* m_LBtnTxt2;
    ui::Text* m_RBtnTxt1;
    ui::Text* m_RBtnTxt2;
    
    
    
    
    
    ui::TextField* m_inputIPAddr;
    std::string m_IpValue;//输入框中的值
    void textFieldTextTouchEvent(Ref* obj, ui::TextField::EventType type);
    
    
    void btnPush(int btnID);
    void btnPress(int btnID);
    
    
    void connectJoystatic();
    
    int m_lastJoyStatic();
    void joystaticChange(int jdat);
    
};

#endif // __HELLOWORLD_SCENE_H__
