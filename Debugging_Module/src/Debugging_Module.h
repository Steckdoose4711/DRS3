#ifndef DEBUGGING_MODULE_H
#define DEBUGGING_MODULE_H


#include <vector>
#include <string>
#include <fstream>
#include <iostream>


#define DBG_GPIO_ENABLE 1
#define DBG_LOG_ENABLE 1
#define DBG_LOG_MESSAGES_ENABLE 1


#define DBG_GPIO_ENABLE 1
#define DBG_GPIO_ENABLE 1

#define DBG_LOG_TICK_ENABLE 1
#define DBG_LOG_TIME_ENABLE 1
#define DBG_LOG_ROLE_ENABLE 1
#define DBG_LOG_CONNECT_ENABLE 1
#define DBG_LOG_ERROR_ENABLE 1



class Debugging_Module
{
    public:
    Debugging_Module(std::string const &logfile_name);
    ~Debugging_Module();


    //---------------------------------------------------------------------------
    //Pin Debugging
    void DBG_GPIO_ROLE_SET_MASTER();
    //... ToDo Lukas: implementieren der debug Pin Schnittstellen


    //---------------------------------------------------------------------------
    //Logging

    // Write the cached logs from the internal logbuffer to the logfile
    void DBG_LOG_WriteLogToFile();

    // Call this function, when a new client has been registered
    void DBG_LOG_NewClientRegistered(std::string const &ip, std::string const &port);

    private:
    std::string BuildLogMessage(std::string const &logmessage);

    std::vector<std::string> mLogBuffer;    // internal log message buffer
    std::ofstream mLogfile;                 // logfile-stream

};

#endif