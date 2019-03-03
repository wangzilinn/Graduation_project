#ifndef __SIM800A_H__
#define __SIM800A_H__
#include "public.h"
#include "usart.h"
class SIM800A
{
public:
    enum ERROR_TYPE 
    {
        UNKNOWN_ERROR = 0,
        RETRY_TOO_MANY_TIMES,
        THRARD_INTERFERENCE,//有其他功能正在运行 
        ERROR_NONE,
        ERROR_MISMACHING,
        SIGNAL_LOST,
    };
    enum HTTPMode
    {
        GET,
        POST
    };
private:
    char* localIP;
    char* accessIP;
    char* URL;
    USART intermediaryUsart;
    int retryLimited;
    int delayMs;
    char receiveStringBuff[100];
    char* (*receiveFun)();
    enum
    {
        IDLE = 0,
        HTTP_CONNECTING,
        HTTP_CONNECTED,
        HTTP_TRANSFERRING,
        HTTP_TERMINATING,
        HTTP_TERMINATED,
        TCP_CONNECTING,
        TCP_CONNECTED,
        TCP_TRANSFERRING,
        TCP_TERMINATING,
        TCP_TERMINATED,
    }MODULE_STATUS;
    ERROR_TYPE moduleError;
    ERROR_TYPE sendSimpleCommand(char* theCommand, char* expectedReturnString);
    ERROR_TYPE sendMultipleCommand(char* theCommand, char** expectedReturnStringArray, int numberOfReturnStrings);
public:
    SIM800A(USART_TypeDef* USARTx,char* (*theReceiveFun)(), int retryTimes = 5, int theDelayMs = 200):
        intermediaryUsart(USARTx), retryLimited(retryTimes), delayMs(theDelayMs), receiveFun(theReceiveFun),MODULE_STATUS(IDLE)
        {            
            receiveStringBuff[0] = '\0';//清空接受缓冲区
            //之所以没有在类内创建消息队列是因为我认为该消息队列只是传递消息用，而不应该是发送模块的一部分。
        }
    ERROR_TYPE test();
    char* HTTPAccess(char* theURL, char* returnHeadAddress);
    ERROR_TYPE HTTPAccess(char* theURL, SIM800A::HTTPMode theMode = SIM800A::GET);
    ERROR_TYPE TCPAccess(char* theIP, char* theElement);
    char* HTTPAccess(char* theIP, char* theElement, char* returnHeadAddress);
    ERROR_TYPE sendSMS(char* theSMS);
};
#endif
