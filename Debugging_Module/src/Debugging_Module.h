#ifndef DEBUGGING_MODULE_H
#define DEBUGGING_MODULE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>


class Debugging_Module
{
public:
    Debugging_Module(std::string const &logfile_name);  //TODO: Input Argumente der Anwendung mitgeben, um diese sofort ins logfile zu schreiben
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

    // Call this function, when a new client has been registered
    void DBG_LOG_NewClientRegistered(std::string const &ip, std::string const &port);

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