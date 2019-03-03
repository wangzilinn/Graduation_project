#include "SIM800A.h"
/*****************************************************************************/
// FUNCTION NAME: test()    
//
// DESCRIPTION:测试组件是否连接
//  
//
// CREATED:  by zilin Wang
//
/*****************************************************************************/
SIM800A::ERROR_TYPE SIM800A::test()
{
    moduleError = sendSimpleCommand("AT\r\n", "OK\r\n"); 
    if (moduleError != ERROR_NONE)
        return SIGNAL_LOST;
    return ERROR_NONE;
}
/*****************************************************************************/
// FUNCTION NAME: sendSimpleCommand
//
// DESCRIPTION:
//  
//
// CREATED:  by zilin Wang
//
/*****************************************************************************/
SIM800A::ERROR_TYPE SIM800A::sendSimpleCommand(char* theCommand, char* expectedReturnString)
{
    int retryCnt = 0;
    int waitCnt = 0;
    char * receiveString;
    //发送命令
    intermediaryUsart.sendString(theCommand);
    //循环等待接收命令
    while(1)
    {
        if (waitCnt < 5 && retryCnt <= retryLimited)
        {
            receiveString = (*receiveFun)();		//消息出队
            if(strstr((char*)receiveString, expectedReturnString) != NULL)		//如果消息匹配
            {
                Method::ClearStringBuff(receiveString, SIM_RECEIVE_STRING_BUFFER_LENGTH);                       //清空消息缓冲区
                moduleError = ERROR_NONE;
                break;
            }
            else//不匹配
            {
                //这里可以加入识别返回字符串的代码
                Method::ClearStringBuff(receiveString, SIM_RECEIVE_STRING_BUFFER_LENGTH);  
                moduleError = ERROR_MISMACHING;
            }
            waitCnt++;
            OSTimeDlyHMSM(0, 0, 0, delayMs);
        }
        else if (retryCnt < retryLimited)//超过等待时间
        {
            waitCnt = 0;//等待时间清0
            retryCnt++;
            intermediaryUsart.sendString(theCommand);//重新发送命令
            OSTimeDlyHMSM(0, 0, 0, delayMs);                   
        }
        else
        {
            moduleError = RETRY_TOO_MANY_TIMES;
            break;
        }
    } 
    return moduleError;
}
/*****************************************************************************/
// FUNCTION NAME: sendMultipleCommand
//
// DESCRIPTION:发送具有多个交互的命令
//  
//
// CREATED:  by zilin Wang
//
/*****************************************************************************/
SIM800A::ERROR_TYPE SIM800A::sendMultipleCommand(char* theCommand, char** expectedReturnStringArray, int numberOfReturnStrings)
{
    sendSimpleCommand(theCommand, expectedReturnStringArray[0]);//先发送第一条
    if (moduleError == ERROR_NONE)//第一条命令发送成功，等待下一个返回值
    {
        int returnStringsCnt = 0;
        int waitCnt = 0;
        char* receiveString;
        while(returnStringsCnt < numberOfReturnStrings)
        {
            if (waitCnt < 5)
            {
                receiveString = (*receiveFun)();		//消息出队
                if(strstr((char*)receiveString, expectedReturnStringArray[returnStringsCnt]) != NULL)		//如果消息匹配
                {
                    Method::ClearStringBuff(receiveString, SIM_RECEIVE_STRING_BUFFER_LENGTH);                       //清空消息缓冲区
                    moduleError = ERROR_NONE;
                    returnStringsCnt++;
                    waitCnt = 0;
                }
                else//不匹配
                {
                    //这里可以加入识别返回字符串的代码
                    Method::ClearStringBuff(receiveString, SIM_RECEIVE_STRING_BUFFER_LENGTH);  
                    moduleError = ERROR_MISMACHING;
                }
            }
            else
            {
                moduleError = RETRY_TOO_MANY_TIMES;
                return moduleError;
            }
            waitCnt++;
            OSTimeDlyHMSM(0, 0, 0, delayMs);  
        }       
    }
    return moduleError;    
}
/*****************************************************************************/
// FUNCTION NAME: HTTPAccess
//
// DESCRIPTION:
//  
//
// CREATED:  by zilin Wang
//
/*****************************************************************************/
SIM800A::ERROR_TYPE SIM800A::HTTPAccess(char* theURL, SIM800A::HTTPMode theMode)
{
    DebugClass debug(USART2);
    if (theMode == GET)
    {
        if (MODULE_STATUS == IDLE)//原来设备空闲,开始连接HTTP
        {
            MODULE_STATUS = HTTP_CONNECTING;
            //Initial
            sendSimpleCommand("ATE0\r\n", "OK\r\n");
            debug.output("关闭回显");
            sendSimpleCommand("AT+CFUN=1\r\n", "OK\r\n");
            sendSimpleCommand("AT+SAPBR=1,1", "OK\r\n");
            if (moduleError == ERROR_NONE)//没有错误，说明初始化成功
            {
                sendSimpleCommand("AT+HTTPINIT\r\n", "OK\r\n");
                sendSimpleCommand("\"CID\",\"1\"\r\n", "OK\r\n");
                char urlCommand[200];
                Method::ClearStringBuff(urlCommand, 200);
                sprintf(urlCommand, "\"URL\",\"%s\"\r\n", theURL);
                sendSimpleCommand(urlCommand,"OK\r\n");
                char *returnString[2] = {"OK\r\n", "+HTTPACTION: 0,200,"};
                sendMultipleCommand("AT+HTTPACTION=0", returnString, 2);           
            }
            if (moduleError == ERROR_NONE)//传输没有错误，则结束HTTP连接
            {
                sendSimpleCommand("AT+HTTPTERM\r\n", "OK\r\n");
            }           
        }
        return moduleError;
    }
    else
    {
        moduleError = UNKNOWN_ERROR;
        return moduleError;
    }
}
