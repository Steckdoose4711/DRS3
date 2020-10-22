#include <iostream>
#include <chrono>
#include <boost/asio.hpp>
#include <sys/time.h>
#include <vector>
#include <iostream>
#include <fstream>

#include "latency.hpp"

using boost::asio::ip::udp;

static size_t const size_payload_byte = 2048;

int ping_udp(const std::string& host, size_t count)
{
    std::cout << "Ping " << host << " * " << count << " ..." << std::endl;

    //auto l = latency<(std::uint64_t)1 * 1000 * 1000 * 1000>();

    boost::asio::io_service io_service;
    udp::socket socket{io_service, udp::endpoint(udp::v4(), 20123)};

    char data[size_payload_byte];
    udp::endpoint sender_endpoint = *(udp::resolver{io_service}.resolve(udp::resolver::query(host, "20124")));
    udp::endpoint dummy;

    std::vector<__suseconds_t> times_measured;

    for(size_t i = 0; i < count; i++)
    {
        bool warm = i >= count/2;

        timeval tim_send;
        timeval tim_rec;
        float elapsedTime;

        gettimeofday(&tim_send, NULL);

        socket.async_send_to(boost::asio::buffer(data, size_payload_byte), sender_endpoint,
                             [&](const boost::system::error_code &error, size_t bytes_recvd)
                             {
                                 socket.async_receive_from(boost::asio::buffer(data, size_payload_byte), dummy, [&](const boost::system::error_code &error, size_t bytes_recvd) {
                                     if(warm)
                                     {
                                        gettimeofday(&tim_rec, NULL);

                                        elapsedTime = (tim_rec.tv_sec - tim_send.tv_sec) * 1000000.0;      // sec to us
                                        elapsedTime += (tim_rec.tv_usec - tim_send.tv_usec);
                                        times_measured.push_back(elapsedTime);

                                    }
                                 });
                             });

        io_service.run();
        io_service.reset();
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
    udp::socket socket{io_service, udp::endpoint(udp::v4(), 20124)};

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