#include <iostream>
#include <cassert>
#include <unistd.h>     //sleep

#include "Debugging_Module.h"


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



void test_debug_messages(Debugging_Module & log) {

    log.DBG_LOG_NewClientRegistered("192.168.0.1", "8080");
    log.DBG_LOG_NewClientRegistered("192.168.0.2", "8080");
    log.DBG_LOG_NewClientRegistered("192.168.0.3", "8080");

    log.DBG_LOG_WriteLogToFile();

    log.DBG_LOG_NewClientRegistered("192.168.0.4711", "8081");
    log.DBG_LOG_NewClientRegistered("192.168.0.4712", "8082");
    log.DBG_LOG_NewClientRegistered("192.168.0.4713", "8083");

    log.DBG_LOG_WriteLogToFile();

    log.DBG_LOG_BroadcastingMasterStatus();
    log.DBG_LOG_ConnectedWithMaster("192.168.0.4713", "8083");
    log.DBG_LOG_ConnectionLost("192.168.0.4713", "8083");
    log.DBG_LOG_CorrectingSysTime(123);
    log.DBG_LOG_CorrectionValueForClient("192.168.0.4713", "8083", 123);
    log.DBG_LOG_Deviations(std::vector<int>{ 1,3,8,4,6,-5 }, std::vector<int>{ -5,8 });
    log.DBG_LOG_Error("Error message xyz");
    log.DBG_LOG_MacrotickRateAdaption(53, 42, 11);
    log.DBG_LOG_MacrotickJump(13);

    std::string msgJson = "{ \
        \"id\": \"936DA01F-9ABD-4D9D-80C7-02AF85C822A8\", \
        \"timestamp\" : \"1606406414488\", \
        \"node\" : { \
        \"address\": \"10.0.0.6\", \
            \"priority\" : 0x3 \
        }, \
        \"msg\" : { \
            \"type\": 0x1, \
                \"payload\" : \"\" \
        } \
    }";

    log.DBG_LOG_MessageReceived("192.168.0.4713", msgJson);
    log.DBG_LOG_MessageSent("192.168.0.4713", msgJson);
    log.DBG_LOG_NewConnectionStatus(true);
    log.DBG_LOG_ReceivedCorrectionValue(13);
    log.DBG_LOG_ReceivedPriority("high", "low", false);
    log.DBG_LOG_TimestampRequest();
    log.DBG_LOG_TimestampResult("192.168.0.4713", "8083", "timestamp");
    log.DBG_LOG_Macrotick(123);
    
    log.DBG_LOG_WriteLogToFile();
}

//this test toggles every pin every sleep() seconds
void test_debug_gpio(Debugging_Module & log) {
    for(;;) {
        log.DBG_GPIO_ROLE_SET_CLIENT();
        log.DBG_GPIO_SYSTICK_EVENT();
        log.DBG_GPIO_MSG_RECEIVED();    //
        log.DBG_GPIO_CONNECTED();
        sleep(2);

        log.DBG_GPIO_ROLE_SET_MASTER();
        log.DBG_GPIO_SYSTICK_EVENT();
        log.DBG_GPIO_MSG_RECEIVED();    //
        log.DBG_GPIO_DISCONNECTED();
        sleep(2);

        log.DBG_GPIO_ROLE_SET_CLIENT();
        log.DBG_GPIO_SYSTICK_EVENT();
        log.DBG_GPIO_MSG_SENT();    //
        log.DBG_GPIO_CONNECTED();
        sleep(2);

        log.DBG_GPIO_ROLE_SET_MASTER();
        log.DBG_GPIO_SYSTICK_EVENT();
        log.DBG_GPIO_MSG_SENT();    //
        log.DBG_GPIO_DISCONNECTED();
        sleep(2);
    }
}


int main()
{
    std::cout << std::endl << "[INFO] Started." << std::endl << std::endl;
    try {

        Debugging_Module log("/home/pi/Desktop/DRS3/Debugging_Module/bin/logfile.txt", "application_input_args", "192.168.0.100", "4711");

        test_debug_messages(log);

        //test_debug_gpio(log);

    }
    catch (std::string const& error) {
        assert(!error.empty());
        std::cerr << error << std::endl << std::endl;
        return 1;
    }
    catch (std::bad_alloc const& e) {
        std::cerr << "[ERROR] Allocation exception: " << e.what() << std::endl;
        return 1;
    }
    catch (std::exception const& e) {
        std::cerr << "[ERROR] Standard exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "[ERROR] Unhandled exception!" << std::endl;
        return 1;
    }

    std::cout << std::endl << std::endl << "[INFO] Finished" << std::endl << std::endl;
    return 0;
}