#include <iostream>
#include "Debugging_Module.h"

int main()
{
    std::cout << "Hello World" << std::endl;

    Debugging_Module log("/home/pi/Desktop/DRS3/Debugging_Module/bin/logfile.txt");

    log.DBG_LOG_NewClientRegistered("192.168.0.1", "8080");
    log.DBG_LOG_NewClientRegistered("192.168.0.2", "8080");
    log.DBG_LOG_NewClientRegistered("192.168.0.3", "8080");

    log.DBG_LOG_WriteLogToFile();

    log.DBG_LOG_NewClientRegistered("192.168.0.4711", "8081");
    log.DBG_LOG_NewClientRegistered("192.168.0.4712", "8082");
    log.DBG_LOG_NewClientRegistered("192.168.0.4713", "8083");    

    log.DBG_LOG_WriteLogToFile();

    return 0;
}