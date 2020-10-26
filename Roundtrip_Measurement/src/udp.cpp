#include <iostream>
#include <chrono>
#include <boost/asio.hpp>
#include <sys/time.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "latency.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

using boost::asio::ip::udp;
using boost::asio::deadline_timer;

static size_t const size_payload_byte = 256;

int ping_udp(const std::string& host, size_t count)
{
    std::cout << "Ping " << host << " * " << count << " ..." << std::endl;

    boost::asio::io_service io_service;
    udp::socket socket{io_service, udp::endpoint(udp::v4(), 20123)};

    char data[size_payload_byte];
    udp::endpoint sender_endpoint = *(udp::resolver{io_service}.resolve(udp::resolver::query(host, "10000")));
    udp::endpoint dummy;
    size_t percentage_cnt = count /100;
    size_t percentage_print = 1;

    std::vector<size_t> times_measured;
    times_measured.reserve(count);

    bool timeout_recognized = true;

    for(size_t i = 0; i < count; i++)
    {
        //bool warm = i >= count/2;

        timeval tim_send;
        timeval tim_rec;
        size_t elapsedTime;

        if(i > percentage_cnt)
        {
            std::cout << percentage_print << "%" << std::endl;
            percentage_print += 1;
            percentage_cnt += count / 100;
        }

        timeout_recognized = true;
        gettimeofday(&tim_send, NULL);
        

        socket.async_send_to(boost::asio::buffer(data, size_payload_byte), sender_endpoint,
                             [&](const boost::system::error_code &error, size_t bytes_recvd)
                             {
                                 socket.async_receive_from(boost::asio::buffer(data, size_payload_byte), dummy, [&](const boost::system::error_code &error, size_t bytes_recvd) {
                                    gettimeofday(&tim_rec, NULL);

                                    elapsedTime = (tim_rec.tv_sec - tim_send.tv_sec) * 1000000.0;      // sec to us
                                    elapsedTime += (tim_rec.tv_usec - tim_send.tv_usec);
                                    times_measured.push_back(elapsedTime);
                                    std::cout << i << " elapsed: " << elapsedTime << "us " << std::endl;
                                    timeout_recognized = false;
                                 });
                             });

        // if the package will not be received after one second, abort the measurement for this package
        io_service.run_for(std::chrono::milliseconds(1000));
        io_service.reset();
        io_service.restart();

        if(timeout_recognized)
        {
            std::cout << "Package: " << i << " timeout" << std::endl;
        }
    }

    try
    {
        std::ofstream myfile;
        myfile.open ("durations.csv");
        for(size_t i = 0; i < times_measured.size(); i++)
        {
            myfile << times_measured[i] << ",";
        }

        myfile.close();
    }
    catch(...)
    {
        std::cerr << "Error writing to csv file" << std::endl;
    }

    socket.close();
    return 0;
}

int pong_udp(const std::string& host)
{
    std::cout << "Pong " << host << " ..." << std::endl;

    boost::asio::io_service io_service;
    udp::socket socket{io_service, udp::endpoint(udp::v4(), 10000)};

    char data[512];
    udp::endpoint sender_endpoint;


    while(true)
    {
        socket.async_receive_from(boost::asio::buffer(data, 512), sender_endpoint, [&](const boost::system::error_code& error, size_t bytes_recvd){
            socket.async_send_to(boost::asio::buffer(data, 512), sender_endpoint, [](const boost::system::error_code& error, size_t bytes_recvd){});
        });

        io_service.run();
        io_service.reset();
    }

    socket.close();
    return 0;
}