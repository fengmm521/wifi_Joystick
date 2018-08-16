#ifndef __SocketData_H__
#define __SocketData_H__

struct NetWorkCommandStruct{
    int msgLen;
    int cmd;
};

enum EventType{
    REQUEST = 1,
    RESPONSE = 2,
    CONNECT_SUCCEED = 11,
    CONNECT_FAIL = 12,
    DISCONNECT = 13
};

typedef struct _SocketData
{
    EventType eventType;
    
    int sn;
    int jcmd;
    
    std::string body = "";
    char* sendData = NULL;
    int bodyLen;
    
    _SocketData *next;
} SocketData;

static SocketData* newSocketData() {
    //SocketData *data = (SocketData*)malloc(sizeof(SocketData));
    SocketData *data = new SocketData();
    data->eventType = DISCONNECT;
    data->body = "";
    data->sendData = NULL;
    data->bodyLen = 0;
    return data;
}


#endif



