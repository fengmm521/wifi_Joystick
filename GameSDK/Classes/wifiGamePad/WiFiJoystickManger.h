//
//  WiFiJoystickManger.h
//  sockettest
//  C++单例类模板，非线程安全单例类，并且实例化后会常注内存
//
//  Created by 俊盟科技1 on 11/14/14.
//
//

#ifndef __sockettest__WiFiJoystickManger__
#define __sockettest__WiFiJoystickManger__

#include "cocos2d.h"
#include "SocketData.h"
#include <thread>
#include <mutex>
#include <condition_variable>

#define MaxJoystickCount 4      //游戏最大可同时与四个游戏手柄建立连接

//手柄上的12个按键位,当相应按键被按下时，相应位被置1，否则相应位会被清0
enum JoystickBtnNum_E
{
    e_up        = 1,                //方向按键上
    e_down,                         //方向按键下
    e_left,                         //方向按键左
    e_right,                        //方向按键右
    e_btn1,                         //功能按键1
    e_btn2,                         //功能按键2
    e_btn3,                         //功能按键3
    e_btn4,                         //功能按键4
    e_btnLup,                       //左前则功能按键1
    e_btnLdown,                     //左前则功能按键2
    e_btnRup,                       //右前则功能按键1
    e_btcRdown                      //右前则功能按键2
};


typedef struct serverAdress
{
    std::string IPAddr;                 //服务器IP
    unsigned short port;                //端口
    std::string macAddr;                //手柄Mac地址
    std::function<void(unsigned int)> callFunc;     //手柄按键状态改变时的返回数据处理函数
    
}ServerAdressST;


class WiFiJoystickManger:public cocos2d::Ref
{
public:
    static WiFiJoystickManger* sharedWiFiJoystickManger();
    static void destroyInstance();
private:
    /**
     * @js ctor
     */
    WiFiJoystickManger(void);
    /**
     * @js NA
     * @lua NA
     */
    ~WiFiJoystickManger(void);
    
public:
    
    /**
     * Init WiFiJoystickManger
     */
    virtual bool init();
    
    void connectSocket(int jNumber);
    void endSocket(int jNumber);
    
    
    //向wifi手柄发送请求命令，以获取手柄名称信息或更改手柄名称信息,手柄工作类型等,1.wifi模块为游戏手柄，非1值.wifi模块为其他工作模式
    //向手柄发送命令并获取手柄返回信息,手柄返回数据第一个字节如果为0xff则说明手柄返回数据为游戏命令请求数据，第二个字节则为返回字符串长度，之后返回的字节为命令返回数据
    int send(int jNumber,unsigned char cmd, std::function<void(std::string backStr)> backFunc);
    
    //更改某个手柄回调函数,jNumber四个手柄中的某个手柄编号(1,4),callFunc:手柄按键改变回调函数,返回值为-1时表示网络连接成功,-2表示网络断开
    void setReciveJoystickCallBackFunc(int jNumber,std::function<void(int)> callFunc);
    
    //删除某个手柄的按键回调函数
    void removeJoystickCallBAckFunc(int jNumber);
    
    
    //获取服务器列表,游戏只与最前边四个游戏手柄进行连接
    std::vector<ServerAdressST> getServerList();
    
    //主线程更新
    void update(float delta);
    
    //发现一个新的游戏手柄，JIPAdd为新游戏手柄的IP地址，jMacAdd为新游戏手柄的物理Mac地址,手柄状态改变回调函数,手柄默认端口为23
    void addNewJoystick(std::string jIPAdd,std::string jMacAdd,std::function<void(int)> jBackFunc,int jport = 9093);
    
    //获取所有网络中的游戏手柄信息
    std::vector<ServerAdressST> getAllJoystickServer();
    
private:
    
    std::thread threadRecive[4];
    
    std::function<void(std::string backStr)> sendCallFunc[4];
    
    //发送数据线程
    void sendThread(int jNumber);
    //接收数据线程
    inline void reciveThread(int jNumber);
};

#endif /* defined(__sockettest__netSocketManger__) */
