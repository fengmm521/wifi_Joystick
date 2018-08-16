#include <ESP8266WiFi.h>
#define LED 0

#define pHIGH  HIGH
#define pLOW   LOW

//#define pHIGH  LOW
//#define pLOW   HIGH

//服务器相关
#define MAX_SRV_CLIENTS 1
WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

//手柄按键GOIP接口
#define COM_UDLR  2    //上下左右的GPIO接口公共端
#define COM_AB    5    //A1,A2,B1,B2的GPIO接口公共端
#define COM_LR    4    //左右四个按键的GPIO接口公共端

#define READ_LLA1  16   //读取左，则左上，A1三个按键数据GPIO
#define READ_DLB1  14   //读取下，则左下，B1三个按键数据GPIO
#define READ_URA2  12   //读取上，则右上，A2三个按键数据GPIO
#define READ_RRB2  13   //读取右，则右下，B2三个按键数据GPIO

//手柄按键设定值，在使用游戏手柄时，可以通过游戏端给游戏手柄发送数据来设置手柄按键值
//游戏客户端发送命令，100：手柄作为服务器端主要向游戏客户端发送数据（为手柄默认工作方式），101：获取手柄软件系统版本号，102：设置手柄为接收数据格式（此工作模式下可以按制手柄GPIO输出端电平），103：WIFI端数据直接转发到串口端

//游戏手柄按键对应值
#define KeyUpValue  0
#define KeyDownValue 1
#define KeyLeftValue 2
#define KeyRightValue 3
#define KeyA1Value 4
#define KeyA2Value 5
#define KeyB1Value 6
#define KeyB2Value 7
#define KeyLUValue 8
#define KeyLDValue 9
#define KeyRUValue 10
#define KeyRDValue 11

uint16_t KeyData = 0;      //按键数据
uint16_t oldKeyData = 0;

uint8_t keyData8bit[2] = {0,0};

//游戏手柄按键状态
int KeyUp_push = HIGH;
int KeyDown_push = HIGH;
int KeyLeft_push = HIGH;
int KeyRight_push = HIGH;

int KeyA1_push = HIGH;
int KeyA2_push = HIGH;
int KeyB1_push = HIGH;
int KeyB2_push = HIGH;

int KeyL1_push = HIGH;
int KeyL2_push = HIGH;
int KeyR1_push = HIGH;
int KeyR2_push = HIGH;

void updateKeySendDatas()
{
  keyData8bit[1] = 0x00FF & KeyData;
  keyData8bit[0] = 0x00FF & (KeyData >> 8);
  Serial.println("keydata8bit0:");
  Serial.println(keyData8bit[0]);
  Serial.println("keydata8bit1:");
  Serial.println(keyData8bit[1]);
}

uint16_t testKeyChange()
{
  //Serial.println("start test key cnage.");
  oldKeyData = KeyData;     //保存上一个刷新数据
  KeyData = 0;              //开始记录新数据
  //按键上状态
  if(KeyUp_push == pLOW)
  {
     KeyData = KeyData|(1<<KeyUpValue);
      Serial.println("keyUPPush");
  }else{
    KeyData = KeyData&(~(1<<KeyUpValue));
  }

  //按键下状态
 if(KeyDown_push == pLOW)
  {
    Serial.println("keyDownPush");
     KeyData = KeyData|(1<<KeyDownValue);
     
  }else{
    KeyData = KeyData&(~(1<<KeyDownValue));
  }

  //按键左状态
 if(KeyLeft_push == pLOW)
  {
    Serial.println("keyLeftPush");
     KeyData = KeyData|(1<<KeyLeftValue);
  }else{
    KeyData = KeyData&(~(1<<KeyLeftValue));
  }

  //按键右状态
 if(KeyRight_push == pLOW)
  {
     Serial.println("keyRightPush");
     KeyData = KeyData|(1<<KeyRightValue);
  }else{
    KeyData = KeyData&(~(1<<KeyRightValue));
  }

  //按键A1状态
 if(KeyA1_push == pLOW)
  {
    Serial.println("keyA1Push");
     KeyData = KeyData|(1<<KeyA1Value);
  }else{
    KeyData = KeyData&(~(1<<KeyA1Value));
  }

  //按键A2状态
 if(KeyA2_push == pLOW)
  {
    Serial.println("keyA2Push");
     KeyData = KeyData|(1<<KeyA2Value);
  }else{
    KeyData = KeyData&(~(1<<KeyA2Value));
  }

  //按键B1状态
 if(KeyB1_push == pLOW)
  {
    Serial.println("keyB1Push");
     KeyData = KeyData|(1<<KeyB1Value);
  }else{
    KeyData = KeyData&(~(1<<KeyB1Value));
  }

  //按键B2状态
 if(KeyB2_push == pLOW)
  {
    Serial.println("keyB2Push");
     KeyData = KeyData|(1<<KeyB2Value);
  }else{
     KeyData = KeyData&(~(1<<KeyB2Value));
  }

  //按键L1状态
 if(KeyL1_push == pLOW)
  {
    Serial.println("keyL1Push");
     KeyData = KeyData|(1<<KeyLUValue);
  }else{
     KeyData = KeyData&(~(1<<KeyLUValue));
  }

  //按键L2状态
 if(KeyL2_push == pLOW)
  {
     Serial.println("keyL2Push");
     KeyData = KeyData|(1<<KeyLDValue);
  }else{
     KeyData = KeyData&(~(1<<KeyLDValue));
  }

  //按键R1状态
 if(KeyR1_push == pLOW)
  {
    Serial.println("keyR1Push");
     KeyData = KeyData|(1<<KeyRUValue);
  }else{
     KeyData = KeyData&(~(1<<KeyRUValue));
  }

  //按键R2状态
 if(KeyR2_push == pLOW)
  {
    Serial.println("keyR2Push");
     KeyData = KeyData|(1<<KeyRDValue);
  }else{
     KeyData = KeyData&(~(1<<KeyRDValue));
  }
  
  if(oldKeyData == KeyData)
  {
    return 65535;
  }else{
    return KeyData;
  }
}

//const int dpin3 = 0;     // 16=D2;14 = D5/D13; 12 = D6/D12; 13 = D7/D11; 15 = D10; 2 = D9; 4 = D4;5 = D3; 0 = D8;
void initButton()
{
  delay(100);
 // pinMode(COM_UDLR,INPUT_PULLUP);                 //将管脚设置为输入并且内部上拉模式
 // pinMode(COM_AB,INPUT_PULLUP);                 //将管脚设置为输入并且内部上拉模式
 // pinMode(COM_LR,INPUT_PULLUP);                 //将管脚设置为输入并且内部上拉模式
  pinMode(COM_UDLR, OUTPUT);
  pinMode(COM_AB, OUTPUT);
  pinMode(COM_LR, OUTPUT);

  pinMode(READ_LLA1,INPUT_PULLUP);                 //将管脚设置为输入并且内部上拉模式
  pinMode(READ_DLB1,INPUT_PULLUP);                 //将管脚设置为输入并且内部上拉模式
  pinMode(READ_URA2,INPUT_PULLUP);                 //将管脚设置为输入并且内部上拉模式
  pinMode(READ_RRB2,INPUT_PULLUP);                 //将管脚设置为输入并且内部上拉模式
  pinMode(READ_LLA1, INPUT);
  pinMode(READ_DLB1, INPUT);
  pinMode(READ_URA2, INPUT);
  pinMode(READ_RRB2, INPUT);
  delay(100);
}

void updateButtonType()
{
  
  //上下左右键状态
  pinMode(COM_UDLR, OUTPUT);
  pinMode(COM_AB, INPUT);
  pinMode(COM_LR, INPUT);
  digitalWrite(COM_UDLR, pLOW);    //各引脚初始化为低电平
  //digitalWrite(COM_AB, pHIGH);    //各引脚初始化为低电平
  //digitalWrite(COM_LR, pHIGH);    //各引脚初始化为低电平
  delay(1);
  
  KeyDown_push = digitalRead(READ_DLB1);
  KeyUp_push = digitalRead(READ_URA2);
  KeyRight_push = digitalRead(READ_RRB2);
  KeyLeft_push = digitalRead(READ_LLA1);
  
  //运作按键状态
  pinMode(COM_UDLR, INPUT);
  pinMode(COM_LR, INPUT);
   pinMode(COM_AB, OUTPUT);
 // digitalWrite(COM_UDLR, pHIGH);    //各引脚初始化为低电平
  //digitalWrite(COM_LR, pHIGH);    //各引脚初始化为低电平
  digitalWrite(COM_AB, pLOW);    //各引脚初始化为低电平
  
  
  delay(1);
  
  KeyB1_push = digitalRead(READ_DLB1);
  KeyA2_push = digitalRead(READ_URA2);
  KeyB2_push = digitalRead(READ_RRB2);
  KeyA1_push = digitalRead(READ_LLA1);

     //运作按键状态
   pinMode(COM_LR, OUTPUT);
   pinMode(COM_UDLR, INPUT);
  pinMode(COM_AB, INPUT);
  //digitalWrite(COM_UDLR, pHIGH);    //各引脚初始化为低电平
  //digitalWrite(COM_AB, pHIGH);    //各引脚初始化为低电平
  digitalWrite(COM_LR, pLOW);    //各引脚初始化为低电平
  
  delay(1);
  KeyL2_push = digitalRead(READ_DLB1);
  KeyR2_push = digitalRead(READ_URA2);
  KeyR1_push = digitalRead(READ_RRB2);
  KeyL1_push = digitalRead(READ_LLA1);
}



void smartConfig()
{
  WiFi.mode(WIFI_STA);
  Serial.println("\r\nWait for Smartconfig");
  WiFi.beginSmartConfig();
  while (1)
  {
    Serial.print(".");
    digitalWrite(LED, 0);
    delay(500);
    digitalWrite(LED, 1);
    delay(500);
    if (WiFi.smartConfigDone())
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      delay(100);
      server.begin();
      server.setNoDelay(true);
       Serial.println("Ready! Use telnet ");
       Serial.println(WiFi.localIP());
       Serial.println(" 23 to connect");
      break;
    }
    
  }
}
 
void setup()
{
  Serial.begin(115200);
  Serial.println("Start module");
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);
  delay(100);
  initButton();
  smartConfig();
}
 
void loop()
{
  delay(10);
while(1)
{
  delay(10);
  
  uint16_t tmpdat = testKeyChange();
  if( tmpdat != 65535)
  {
    Serial.println("keyChange");
    Serial.println(tmpdat);
    updateKeySendDatas();
    size_t slen = (size_t)2;
    uint8_t sendbuf[slen];
    sendbuf[0] = keyData8bit[0];
    sendbuf[1] = keyData8bit[1];
    uint8_t i;
  //check if there are any new clients
  if (server.hasClient()){
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Serial1.print("New client: "); 
        Serial1.print(i);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }

    //push UART data to all connected telnet clients
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
        serverClients[i].write(sendbuf, slen);
        delay(1);
        Serial.println("send---------");
      }
    }
  }
  updateButtonType();
}
}
