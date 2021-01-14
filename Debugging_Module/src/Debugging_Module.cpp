#include <chrono>
#include <ctime>
#include <sstream>

#include "Debugging_Module.h"

#include <wiringPi.h>   //this library is deprecated, but we use it here as it is easy to use and were are no production environment

//http://wiringpi.com/pins/
//https://i.stack.imgur.com/w7fFL.png
//https://cdn.mos.cms.futurecdn.net/kSo96fYwdrfQKSvALMKqzc.png
                                //Header    GPIO    wiring pi
constexpr int GPIO_02 = 8;      //3         2       8
constexpr int GPIO_03 = 9;      //5         3       9
constexpr int GPIO_04 = 7;      //7         4       7
constexpr int GPIO_17 = 0;      //11        17      0


Debugging_Module::Debugging_Module(std::string const &logfile_name, std::string const &input_args, std::string const &own_ip, std::string const &own_port)
{
    try
    {
        mLogfile.open (logfile_name, std::ios_base::app);
        // writing info to head of the logfile
        mLogfile << "Application started with arguments: " << input_args << std::endl;
        mLogfile << "Device IP: " << own_ip << std::endl;
        mLogfile << "Application Port: " << own_port << std::endl;
    }
    catch(...)
    {
        std::cerr << "[ERROR]: failed to open logfile." << std::endl;
    }

    //setup gpio
    wiringPiSetup();

    pinMode(GPIO_02, OUTPUT);
    pinMode(GPIO_02, OUTPUT);
    pinMode(GPIO_03, OUTPUT);
    pinMode(GPIO_17, OUTPUT);

    //initialize to known value
    digitalWrite(GPIO_02, LOW);
    digitalWrite(GPIO_03, LOW);
    digitalWrite(GPIO_04, LOW); mGPIO4_state = LOW;
    digitalWrite(GPIO_17, LOW);

    mRole = Role::Client;
}

Debugging_Module::~Debugging_Module()
{
    mLogfile << "---------------------------------------------------------------" << std::endl << std::endl << std::endl;

    mLogfile.close();
}

// -------------------------------------------------------------
//Pin Debugging

void Debugging_Module::DBG_GPIO_ROLE_SET_MASTER() {
    mRole = Role::Master;

    #if DBG_GPIO_ENABLE == 1
        digitalWrite(GPIO_02, HIGH);
    #endif
}
void Debugging_Module::DBG_GPIO_ROLE_SET_CLIENT() {
    mRole = Role::Client;

    #if DBG_GPIO_ENABLE == 1
        digitalWrite(GPIO_02, LOW);
    #endif
}
void Debugging_Module::DBG_GPIO_SYSTICK_EVENT() {
    if(mGPIO4_state == LOW) {
        #if DBG_GPIO_ENABLE == 1
            digitalWrite(GPIO_04, HIGH);
        #endif
        mGPIO4_state = HIGH;
    }
    else {
        #if DBG_GPIO_ENABLE == 1
            digitalWrite(GPIO_04, LOW);
        #endif
        mGPIO4_state = LOW;
    }
}
void Debugging_Module::DBG_GPIO_MSG_RECEIVED() {
    if(mRole == Role::Master)
        #if DBG_GPIO_ENABLE == 1
            digitalWrite(GPIO_03, LOW);
        #endif
    else if(mRole == Role::Client) {
        #if DBG_GPIO_ENABLE == 1
            digitalWrite(GPIO_03, HIGH);
        #endif
    }
}
void Debugging_Module::DBG_GPIO_MSG_SENT() {
    if(mRole == Role::Master)
        #if DBG_GPIO_ENABLE == 1
            digitalWrite(GPIO_03, HIGH);
        #endif
    else if(mRole == Role::Client) {
        #if DBG_GPIO_ENABLE == 1
            digitalWrite(GPIO_03, LOW);
        #endif
    }
}
void Debugging_Module::DBG_GPIO_CONNECTED() {
    #if DBG_GPIO_ENABLE == 1
        digitalWrite(GPIO_17, HIGH);
    #endif
}
void Debugging_Module::DBG_GPIO_DISCONNECTED() {
    #if DBG_GPIO_ENABLE == 1
        digitalWrite(GPIO_17, LOW);
    #endif
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
    #if DBG_LOG_ROLE_ENABLE == 1
        std::string logmessage = "New client registered: " + ip + ":" + port;
        mLogBuffer.emplace_back(BuildLogMessage(ROLE, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_ReceivedPriority(std::string const &recPrio, std::string const &ownPrio, bool changesToMaster)
{
    #if DBG_LOG_ROLE_ENABLE == 1
        std::string logmessage = "Received priority from other node: " + recPrio + " (own priority: " + ownPrio + ") - changing role to master: " + (changesToMaster?"true":"false");
        mLogBuffer.emplace_back(BuildLogMessage(ROLE, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_ConnectedWithMaster(std::string const &ip, std::string const &port) 
{
    #if DBG_LOG_ROLE_ENABLE == 1
        std::string logmessage = "Now connected with master: " + ip + ":" + port;
        mLogBuffer.emplace_back(BuildLogMessage(ROLE, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_ConnectionLostMaster(std::string const &ip, std::string const &port)
{
    #if DBG_LOG_CONNECT_ENABLE == 1
        std::string logmessage = "Connection to master " + ip + ":" + port + " lost";
        mLogBuffer.emplace_back(BuildLogMessage(CONNECT, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_ConnectionLostSlave(std::string const &ip, std::string const &port)
{
    #if DBG_LOG_CONNECT_ENABLE == 1
        std::string logmessage = "Connection to slave " + ip + ":" + port + " lost";
        mLogBuffer.emplace_back(BuildLogMessage(CONNECT, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_NewConnectionStatus(std::string const &status)
{
    #if DBG_LOG_CONNECT_ENABLE == 1
        std::string logmessage = "New connection status: " + status;
        mLogBuffer.emplace_back(BuildLogMessage(CONNECT, logmessage));
    #endif
}
void Debugging_Module::DBG_LOG_NewConnectionStatus(bool isConnected)
{
    #if DBG_LOG_CONNECT_ENABLE == 1
        std::string logmessage = "New connection status: " + isConnected?"connected":"disconnected";
        mLogBuffer.emplace_back(BuildLogMessage(CONNECT, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_BroadcastingMasterStatus() 
{
    #if DBG_LOG_ROLE_ENABLE == 1
        std::string logmessage = "Broadcasting new master status";
        mLogBuffer.emplace_back(BuildLogMessage(ROLE, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_TimestampResult(std::string const &ip, std::string const &port, std::string const &timestamp)
{
    #if DBG_LOG_TIME_ENABLE == 1
        std::string logmessage = "Received timestamp result from client " + ip + ":" + port + ", value = " + timestamp;
        mLogBuffer.emplace_back(BuildLogMessage(TIME, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_TimestampRequest() 
{
    #if DBG_LOG_TIME_ENABLE == 1
        std::string logmessage = "Received timestamp request from master";
        mLogBuffer.emplace_back(BuildLogMessage(TIME, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_Deviations(std::vector<std::string> const &deviations, std::vector<std::string> const &ignored) 
{
    #if DBG_LOG_TIME_ENABLE == 1
        std::string logmessage = "Deviations: [";
        for(auto d : deviations) {;
            logmessage += "(" + d + ") ";
        }
        logmessage += "], ignoring [";
        for(auto i : ignored) {
            logmessage += "(" + i + ") ";
        }
        logmessage += "]";
        mLogBuffer.emplace_back(BuildLogMessage(TIME, logmessage));
    #endif
}
void Debugging_Module::DBG_LOG_Deviations(std::vector<int> const &deviations, std::vector<int> const &ignored) 
{
    #if DBG_LOG_TIME_ENABLE == 1
        std::string logmessage = "Deviations: [";
        for(auto d : deviations) {
            logmessage += "(" + std::to_string(d) + ") ";
        }
        logmessage += "], ignoring [";
        for(auto i : ignored) {
            logmessage += "(" + std::to_string(i) + ") ";
        }
        logmessage += "]";
        mLogBuffer.emplace_back(BuildLogMessage(TIME, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_CorrectingSysTime(std::string const &value) 
{
    #if DBG_LOG_TIME_ENABLE == 1
        std::string logmessage = "Correcting system time by " + value + "ms";
        mLogBuffer.emplace_back(BuildLogMessage(TIME, logmessage));
    #endif
}
void Debugging_Module::DBG_LOG_CorrectingSysTime(int value)
{
    #if DBG_LOG_TIME_ENABLE == 1
        std::string logmessage = "Correcting system time by " + std::to_string(value) + "ms";
        mLogBuffer.emplace_back(BuildLogMessage(TIME, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_CorrectionValueForClient(std::string const &ip, std::string const &port, std::string const &corrValue)
{
    #if DBG_LOG_TIME_ENABLE == 1
        std::string logmessage = "Correction value for client " + ip + ":" + port + " is " + corrValue;
        mLogBuffer.emplace_back(BuildLogMessage(TIME, logmessage));
    #endif
}
void Debugging_Module::DBG_LOG_CorrectionValueForClient(std::string const &ip, std::string const &port, int corrValue)
{
    #if DBG_LOG_TIME_ENABLE == 1
        std::string logmessage = "Correction value for client " + ip + ":" + port + " is " + std::to_string(corrValue);
        mLogBuffer.emplace_back(BuildLogMessage(TIME, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_ReceivedCorrectionValue(std::string const &corrValue)
{
    #if DBG_LOG_TIME_ENABLE == 1
        std::string logmessage = "Received correction value " + corrValue;
        mLogBuffer.emplace_back(BuildLogMessage(TIME, logmessage));
    #endif
}
void Debugging_Module::DBG_LOG_ReceivedCorrectionValue(int corrValue)
{
    #if DBG_LOG_TIME_ENABLE == 1
        std::string logmessage = "Received correction value " + std::to_string(corrValue);
        mLogBuffer.emplace_back(BuildLogMessage(TIME, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_MacrotickJump(std::string const &jumpValue)
{
    #if DBG_LOG_TIME_ENABLE == 1
        std::string logmessage = "Correcting macrotick by jumping " + jumpValue + "ms";
        mLogBuffer.emplace_back(BuildLogMessage(TIME, logmessage));
    #endif
}
void Debugging_Module::DBG_LOG_MacrotickJump(int jumpValue)
{
    #if DBG_LOG_TIME_ENABLE == 1
        std::string logmessage = "Correcting macrotick by jumping " + std::to_string(jumpValue) + "ms";
        mLogBuffer.emplace_back(BuildLogMessage(TIME, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_MacrotickRateAdaption(std::string const &oldRate, std::string const &newRate, std::string const &difference)
{
    #if DBG_LOG_TIME_ENABLE == 1
        std::string logmessage = "Correcting macrotick by rate adaption from " + oldRate + " to " + newRate + " (" + difference + "ms)";
        mLogBuffer.emplace_back(BuildLogMessage(TIME, logmessage));
    #endif
}
void Debugging_Module::DBG_LOG_MacrotickRateAdaption(int oldRate, int newRate, int difference)
{
    #if DBG_LOG_TIME_ENABLE == 1
        std::string logmessage = "Correcting macrotick by rate adaption from " + std::to_string(oldRate) + " to " + std::to_string(newRate) + " (" + std::to_string(difference) + "ms)";
        mLogBuffer.emplace_back(BuildLogMessage(TIME, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_Macrotick(std::string const &tickCount)
{
    #if DBG_LOG_TICK_ENABLE == 1
        std::string logmessage = "Current Macrotick: " + tickCount;
        mLogBuffer.emplace_back(BuildLogMessage(TICK, logmessage));
    #endif
}
void Debugging_Module::DBG_LOG_Macrotick(std::size_t tickCount)
{
    #if DBG_LOG_TICK_ENABLE == 1
        std::string logmessage = "Current Macrotick: " + std::to_string(tickCount);
        mLogBuffer.emplace_back(BuildLogMessage(TICK, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_MessageReceived(std::string const &srcIp, std::string const &jsonContent) 
{
    #if DBG_LOG_MESSAGES_ENABLE == 1
        std::string logmessage = "Received Message from " + srcIp + ": " + jsonContent;
        mLogBuffer.emplace_back(BuildLogMessage(MESSAGE, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_MessageSent(std::string const &destIp, std::string const &jsonContent)
{
    #if DBG_LOG_MESSAGES_ENABLE == 1
        std::string logmessage = "Sent Message to " + destIp + ": " + jsonContent;
        mLogBuffer.emplace_back(BuildLogMessage(MESSAGE, logmessage));
    #endif
}

void Debugging_Module::DBG_LOG_Error(std::string const &errorMsg)
{
    #if DBG_LOG_ERROR_ENABLE == 1
        mLogBuffer.emplace_back(BuildLogMessage(ERROR, errorMsg));
    #endif
}






// concat timestamp and log message
std::string Debugging_Module::BuildLogMessage(Categories const category, std::string const &logmessage)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_ = std::chrono::system_clock::to_time_t(now);
    std::string role_string;

    switch(category)
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