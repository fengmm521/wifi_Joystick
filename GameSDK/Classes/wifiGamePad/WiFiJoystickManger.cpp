//
//  WiFiJoystickManger.cpp
//  sockettest
//
//  Created by 俊盟科技1 on 11/14/14.
//
//

#include "WiFiJoystickManger.h"
#include "ODSocket.h"
#include "Queue.h"
#include <thread>
//#include <endian.h>
#include "zlib.h"
#include<netinet/in.h>

#define MAX_BUFF    256
static ODSocket cSocket[4];
bool isConnect[4] = {false,false,false,false};

using namespace cocos2d;

CQueue<SocketData>  m_mainQueue[4];     //4个手柄的接收列队

std::mutex m_mutexx;


//WiFi游戏手柄地址列表,游戏手柄默认端口都为9093，且游戏手柄为服务器端模式，只有前四个手柄可连接上
std::map<int,ServerAdressST> sp_gameJoystickMap;

static WiFiJoystickManger *s_sharedWiFiJoystickManger = nullptr;

WiFiJoystickManger *WiFiJoystickManger::sharedWiFiJoystickManger()
{
    m_mutexx.lock();
    if (s_sharedWiFiJoystickManger == nullptr)
    {
        s_sharedWiFiJoystickManger = new WiFiJoystickManger();
        if (!s_sharedWiFiJoystickManger || !s_sharedWiFiJoystickManger->init())
        {
            CC_SAFE_DELETE(s_sharedWiFiJoystickManger);
        }
    }
    m_mutexx.unlock();
    return s_sharedWiFiJoystickManger;
}

void WiFiJoystickManger::destroyInstance()
{
    
    CC_SAFE_RELEASE_NULL(s_sharedWiFiJoystickManger);
}

WiFiJoystickManger::WiFiJoystickManger(void)
{

}

WiFiJoystickManger::~WiFiJoystickManger(void)
{
    
}

bool WiFiJoystickManger::init()
{
    bool bRet = false;
    do
    {
        //查看服务器反回数据方法
        Director::getInstance()->getScheduler()->scheduleUpdate(this, -1, false);
        
        bRet = true;
    }
    while (0);
    
    return bRet;
}
//发现一个新的游戏手柄，JIPAdd为新游戏手柄的IP地址，jMacAdd为新游戏手柄的物理Mac地址,手柄状态改变回调函数,手柄默认端口为23
void WiFiJoystickManger::addNewJoystick(std::string jIPAdd,std::string jMacAdd,std::function<void(int)> jBackFunc,int jport /*= 23*/)
{
    for(std::map<int,ServerAdressST>::iterator it = sp_gameJoystickMap.begin() ; it != sp_gameJoystickMap.end() ; it++)
    {
        if(it->second.macAddr.compare(jMacAdd.c_str()) == 0)
        {
            it->second.IPAddr = jIPAdd;
            it->second.callFunc = jBackFunc;
            it->second.port = jport;
            return;
        }
    }
    ServerAdressST tmp;
    tmp.port = jport;
    tmp.IPAddr = jIPAdd;
    tmp.macAddr = jMacAdd;
    tmp.callFunc = jBackFunc;
    sp_gameJoystickMap[int(sp_gameJoystickMap.size()+1)] = tmp;
    
}
int WiFiJoystickManger::send(int jNumber,unsigned char cmd, std::function<void(std::string backStr)> backFunc)
{
    if (!backFunc ||  !isConnect[jNumber-1]) {
        return -2;
    }
    
    char sendbuff[100];
    sendbuff[0] = cmd;
    sendCallFunc[jNumber-1] = backFunc;
    
    int ret = cSocket[jNumber-1].Send((char*)sendbuff,1,0);
    
    if (ret <= 0) {
        SocketData *errorData = newSocketData();
        errorData->eventType = DISCONNECT;
        errorData->sn = jNumber;
        m_mainQueue[jNumber-1].Push(errorData);
        return -1;
    }
    return 0;
}

void WiFiJoystickManger::removeJoystickCallBAckFunc(int jNumber)
{
    if (sp_gameJoystickMap.find(jNumber) != sp_gameJoystickMap.end()) {
        sp_gameJoystickMap[jNumber].callFunc = nullptr;
    }
}

void WiFiJoystickManger::setReciveJoystickCallBackFunc(int jNumber,std::function<void(int)> callFunc)
{
    if (sp_gameJoystickMap.find(jNumber) != sp_gameJoystickMap.end()) {
        sp_gameJoystickMap[jNumber].callFunc = callFunc;
    }
}

//主线程更新
void WiFiJoystickManger::update(float dt)
{
    
    for (int i = 0; i < 4; i++) {
        if (!m_mainQueue[i].IsEmpty()) {
            SocketData *data = m_mainQueue[i].Pop();
            
            if (data->eventType == REQUEST) {
                if (sp_gameJoystickMap.find(data->sn) != sp_gameJoystickMap.end()) {
                    sp_gameJoystickMap[data->sn].callFunc(data->jcmd);
                }
            }else if (data->eventType == RESPONSE) {
                if (sendCallFunc[i]) {
                    sendCallFunc[i](data->body);
                }
            }else if (data->eventType == CONNECT_SUCCEED || data->eventType == CONNECT_FAIL || data->eventType == DISCONNECT) {
                if (data->eventType != CONNECT_SUCCEED) {
                   
                    if (!threadRecive[i].joinable()) {
                        threadRecive[i].~thread();
                    }

                    while (!m_mainQueue[i].IsEmpty()) {
                        SocketData *mainData = m_mainQueue[i].Pop();
                        if (mainData != data) {
                        }
                    }
                    if (isConnect[i]) {
                        cSocket[i].Close();
                        cSocket[i].Clean();
                        isConnect[i] = false;
                    }
                }
                if (sp_gameJoystickMap.find(data->sn) != sp_gameJoystickMap.end()) {
                    sp_gameJoystickMap[data->sn].callFunc(-2);
                }
            }else{
                if (sp_gameJoystickMap.find(data->sn) != sp_gameJoystickMap.end()) {
                    sp_gameJoystickMap[data->sn].callFunc(-1);
                }
            }
            free(data);
        }
    }
    
}
void WiFiJoystickManger::connectSocket(int jNumber)
{
    
    if (sp_gameJoystickMap.find(jNumber) != sp_gameJoystickMap.end() && (!isConnect[jNumber-1])) {
        cSocket[jNumber-1].Init();
        //cSocket[jNumber-1].Create(AF_INET,SOCK_STREAM,0);
        cSocket[jNumber-1].Create(AF_INET,SOCK_STREAM,0);
        isConnect[jNumber-1] = cSocket[jNumber-1].Connect(sp_gameJoystickMap[jNumber].IPAddr.c_str(),sp_gameJoystickMap[jNumber].port);

        threadRecive[jNumber-1] = std::thread(&WiFiJoystickManger::reciveThread,this,jNumber);
        threadRecive[jNumber-1].detach();
    }
    
    log("start socket...");
}
void WiFiJoystickManger::endSocket(int jNumber)
{
    if (!threadRecive[jNumber-1].joinable()) {
        threadRecive[jNumber-1].~thread();
    }

    while (!m_mainQueue[jNumber-1].IsEmpty()) {
        SocketData *mainData = m_mainQueue[jNumber-1].Pop();
        CC_SAFE_FREE(mainData);
    }
    
    if (isConnect[jNumber-1]) {
        cSocket[jNumber-1].Close();
        cSocket[jNumber-1].Clean();
        isConnect[jNumber-1] = false;
    }
}
std::vector<ServerAdressST> WiFiJoystickManger::getServerList()
{
    std::vector<ServerAdressST> tmpvector;
    for (std::map<int,ServerAdressST>::iterator it = sp_gameJoystickMap.begin(); it != sp_gameJoystickMap.end() ; it++) {
        tmpvector.push_back(it->second);
    }
    return tmpvector;
}
//接收数据线程
inline void WiFiJoystickManger::reciveThread(int jNumber)
{
    char buff[MAX_BUFF];
    while(true)
    {
        int ret = cSocket[jNumber-1].Recv(buff, MAX_BUFF, 0);
        
        if (ret <= 0) {
            
            isConnect[jNumber-1] = false;
            SocketData *errorData = newSocketData();
            errorData->eventType = DISCONNECT;
            errorData->sn = jNumber;
            m_mainQueue[jNumber-1].Push(errorData);
            log("socket erro:%d",cSocket[jNumber-1].GetError());
            break;
        }
        if(ret < 1)
            break;
        
        SocketData *data = newSocketData();
        
        log("reciveData buff0:%x",buff[0]&0x0ff);
        log("reciveData buff1:%x",buff[1]&0x0ff);
        
        if (buff[0] == (char)0x0ff) {
            data->bodyLen = int(buff[1])&0x0ff;
            std::string datax(&buff[2], data->bodyLen);
            data->body = std::string(datax);
            data->jcmd = 0;
            data->eventType = RESPONSE;
        }else{
            data->bodyLen = 2;
            data->body = "";
            int tmp1 = (int)buff[0]&0x0ff;
            int tmp2 = (int)buff[1]&0x0ff;
            data->jcmd = (tmp2<<8)|tmp1;
            data->eventType = REQUEST;
        }
        data->sn = jNumber;
        
        log("reciveData:%x",data->jcmd);
        log("*************************************************");
        
        m_mainQueue[jNumber-1].Push(data);

        memset(buff, 0, sizeof(char)*MAX_BUFF);
        
        
    }
    
}