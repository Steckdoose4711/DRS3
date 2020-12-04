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

    //ToDo: (eventuell eigene ip adresse rausschreiben? oder bekommen wir die übergeben?)
}

Debugging_Module::~Debugging_Module()
{
    mLogfile << "---------------------------------------------------------------" << std::endl << std::endl << std::endl;

    mLogfile.close();
}

// -------------------------------------------------------------
//Pin Debugging

void Debugging_Module::DBG_GPIO_ROLE_SET_MASTER()
{

}



// -------------------------------------------------------------
// Logging

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
    mLogBuffer.emplace_back(BuildLogMessage(ROLE, logmessage));
}








// concat timestamp and log message
std::string Debugging_Module::BuildLogMessage(Roles const role, std::string const &logmessage)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_ = std::chrono::system_clock::to_time_t(now);
    std::string role_string;

    switch(role)
    {
        case ROLE:
            role_string = "ROLE";
            break;
        case CONNECT:
            role_string = "CONNECT";    
            break;
        case TIME:
            role_string = "TIME";   
            break;
        case TICK:
            role_string = "TICK";   
            break;
        case MESSAGE:
            role_string = "MESSAGE";   
            break;
        case ERROR:
            role_string = "ERROR";   
            break;
        default:
            role_string = "UNDEFINED";   
            break;
    }


    // remove \n from ctime
    std::string time = std::ctime(&now_);
    time = time.substr(0, time.length() -1);

    std::stringstream log;
    log << time << " " << role_string << ": " << logmessage << std::endl;
    return log.str();
}