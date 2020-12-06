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
    //Antwort von Lukas: Ich würde übergeben. Nur der Aufrufer weiß welche Ip von welchem Interface richtig ist


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
    digitalWrite(GPIO_02, HIGH);
}
void Debugging_Module::DBG_GPIO_ROLE_SET_CLIENT() {
    mRole = Role::Client;
    digitalWrite(GPIO_02, LOW);
}
void Debugging_Module::DBG_GPIO_SYSTICK_EVENT() {
    if(mGPIO4_state == LOW) {
        digitalWrite(GPIO_04, HIGH);
        mGPIO4_state = HIGH;
    }
    else {
        digitalWrite(GPIO_04, LOW);
        mGPIO4_state = LOW;
    }
}
void Debugging_Module::DBG_GPIO_MSG_RECEIVED() {
    if(mRole == Role::Master)
        digitalWrite(GPIO_03, LOW);
    else if(mRole == Role::Client) {
        digitalWrite(GPIO_03, HIGH);
    }
}
void Debugging_Module::DBG_GPIO_MSG_SENT() {
    if(mRole == Role::Master)
        digitalWrite(GPIO_03, HIGH);
    else if(mRole == Role::Client) {
        digitalWrite(GPIO_03, LOW);
    }
}
void Debugging_Module::DBG_GPIO_CONNECTED() {
    digitalWrite(GPIO_17, HIGH);
}
void Debugging_Module::DBG_GPIO_DISCONNECTED() {
    digitalWrite(GPIO_17, LOW);
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
    //TODO ifdef defines abfragen, nur wenn 1 dann in den LogBuffer schreiben
    std::string logmessage = "New Client Registered: " + ip + ":" + port;
    mLogBuffer.emplace_back(BuildLogMessage(ROLE, logmessage));
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