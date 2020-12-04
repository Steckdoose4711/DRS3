#ifndef DEBUGGING_MODULE_H
#define DEBUGGING_MODULE_H


#include <vector>
#include <string>

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
    Debugging_Module();

    private:


    std::vector<std::string> mLogBuffer;

};

#endif