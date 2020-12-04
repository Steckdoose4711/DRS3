#include "Debugging_Module.h"
#include <chrono>
#include <ctime>   
#include <sstream>

Debugging_Module::Debugging_Module(std::string const &logfile_name)
{
    try
    {
        mLogfile.open (logfile_name, std::ios_base::app);
    }
    catch(...)
    {
        std::cerr << "[ERROR]: failed to open logfile." << std::endl;
    }
}

Debugging_Module::~Debugging_Module()
{

    mLogfile.close();
}

void Debugging_Module::DBG_GPIO_ROLE_SET_MASTER()
{

}


void Debugging_Module::DBG_LOG_WriteLogToFile()
{
    for(auto it : mLogBuffer)
    {
        mLogfile << it;
    }
    mLogBuffer.clear();
}

void Debugging_Module::DBG_LOG_NewClientRegistered(std::string const &ip, std::string const &port)
{
    std::string logmessage = "New Client Registered: " + ip + ":" + port;
    mLogBuffer.emplace_back(BuildLogMessage(logmessage));
}


// concat timestamp and log message
std::string Debugging_Module::BuildLogMessage(std::string const &logmessage)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_ = std::chrono::system_clock::to_time_t(now);

    // remove \n from ctime
    std::string time = std::ctime(&now_);
    time = time.substr(0, time.length() -1);

    std::stringstream log;
    log << time << ": " << logmessage << std::endl;
    std::cout << log.str();
    return log.str();
}