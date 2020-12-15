#ifndef DEBUGGING_MODULE_H
#define DEBUGGING_MODULE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#define DBG_GPIO_ENABLE 1
#define DBG_LOG_ENABLE 1
#define DBG_LOG_MESSAGES_ENABLE 1
#define DBG_LOG_TICK_ENABLE 1
#define DBG_LOG_TIME_ENABLE 1
#define DBG_LOG_ROLE_ENABLE 1
#define DBG_LOG_CONNECT_ENABLE 1
#define DBG_LOG_ERROR_ENABLE 1

#define DBG_GPIO_ENABLE 1
#define DBG_GPIO_ENABLE 1

class Debugging_Module
{
public:
    //logfile_name: path and name of the logfile
    //input_args: input arguments of the Application
    //own_ip: own ip of the device
    //own_port: on port, which is used by the application
    Debugging_Module(std::string const &logfile_name, std::string const &input_args, std::string const &own_ip, std::string const &own_port);
    ~Debugging_Module();


    //---------------------------------------------------------------------------
    //Pin Debugging
    void DBG_GPIO_ROLE_SET_MASTER();
    void DBG_GPIO_ROLE_SET_CLIENT();
    void DBG_GPIO_SYSTICK_EVENT();
    void DBG_GPIO_MSG_RECEIVED();
    void DBG_GPIO_MSG_SENT();
    void DBG_GPIO_CONNECTED();
    void DBG_GPIO_DISCONNECTED();


    //---------------------------------------------------------------------------
    //Logging

    // Write the cached logs from the internal logbuffer to the logfile
    void DBG_LOG_WriteLogToFile();

    // R1: Call this function, when a new client has been registered
    void DBG_LOG_NewClientRegistered(std::string const &ip, std::string const &port);
    // R2: Call this function, when priorities from other nodes were received during master failure
    void DBG_LOG_ReceivedPriority(std::string const &recPrio, std::string const &ownPrio, bool changesToMaster);
    // R3: Call this function, when connecting to a new master
    void DBG_LOG_ConnectedWithMaster(std::string const &ip, std::string const &port);
    // R4: Call this function, when connection to master was lost
    void DBG_LOG_ConnectionLostMaster(std::string const &ip, std::string const &port);
    // R5: Call this function, when connection status changed (connected/disconnected)
    void DBG_LOG_NewConnectionStatus(std::string const &status);
    void DBG_LOG_NewConnectionStatus(bool isConnected);
    // R6: Call this function, when broadcasting that node is now master
    void DBG_LOG_BroadcastingMasterStatus();
    // R7: Call this function, when connection to slave was lost
    void DBG_LOG_ConnectionLostSlave(std::string const &ip, std::string const &port); 
    // T1: Call this function, when receiving timestamp responses from clients
    void DBG_LOG_TimestampResult(std::string const &ip, std::string const &port, std::string const &timestamp);
    // T2: Call this function, when receiving timestamp request from master
    void DBG_LOG_TimestampRequest();
    // T3-1: Call this function, when all deviations are collected and ignored deviations are found out
    void DBG_LOG_Deviations(std::vector<std::string> const &deviations, std::vector<std::string> const &ignored);
    void DBG_LOG_Deviations(std::vector<int> const &deviations, std::vector<int> const &ignored);
    // T3-2: Call this function, when correcting system time
    void DBG_LOG_CorrectingSysTime(std::string const &value);
    void DBG_LOG_CorrectingSysTime(int value);
    // T3-3: Call this function, when sending out correction value to client
    void DBG_LOG_CorrectionValueForClient(std::string const &ip, std::string const &port, std::string const &corrValue);
    void DBG_LOG_CorrectionValueForClient(std::string const &ip, std::string const &port, int corrValue);
    // T4: Call this function, when receiving correction value from master
    void DBG_LOG_ReceivedCorrectionValue(std::string const &corrValue);
    void DBG_LOG_ReceivedCorrectionValue(int corrValue);
    // T5: Call this function, when correcting macrotick by jumping given value
    void DBG_LOG_MacrotickJump(std::string const &jumpValue);
    void DBG_LOG_MacrotickJump(int jumpValue);
    // T6: Call this function, when correcting macrotick by rate adaption to new rate
    void DBG_LOG_MacrotickRateAdaption(std::string const &oldRate, std::string const &newRate, std::string const &difference);
    void DBG_LOG_MacrotickRateAdaption(int oldRate, int newRate, int difference);
    // T7: Call this function, when macrotick happens
    void DBG_LOG_Macrotick(std::string const &tickCount);
    void DBG_LOG_Macrotick(std::size_t tickCount);
    // M1: Call this function, when receiving any message
    void DBG_LOG_MessageReceived(std::string const &srcIp, std::string const &jsonContent);
    // M2: Call this function, when sending any message
    void DBG_LOG_MessageSent(std::string const &destIp, std::string const &jsonContent);
    // E1: Call this function to log an error
    void DBG_LOG_Error(std::string const &errorMsg);

private:
    enum Categories {ROLE, CONNECT, TIME, TICK, MESSAGE, ERROR};
    enum class Role {Master, Client};


    std::string BuildLogMessage(Categories const category, std::string const &logmessage);

    std::vector<std::string> mLogBuffer;    // internal log message buffer
    std::ofstream mLogfile;                 // logfile-stream

    Role mRole;
    size_t mGPIO4_state;
};



#endif