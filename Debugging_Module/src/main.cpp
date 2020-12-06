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

        Debugging_Module log("/home/pi/Desktop/DRS3/Debugging_Module/bin/logfile.txt");

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