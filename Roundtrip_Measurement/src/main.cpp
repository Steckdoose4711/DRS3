#include <iostream>
#include <chrono>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int help()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << "ping-pong [ping/pong] [host=127.0.0.1] [message=10000]" << std::endl;
    
    return -1;
}

int ping_udp(const std::string& host, size_t count);
int pong_udp(const std::string& host);
int ping_tcp(const std::string& host, size_t count);
int pong_tcp(const std::string& host);

int main(int argc, char **argv)
{
    if(argc <= 1)
    {
        return help();
    }
    
    auto host = argc < 3 ? std::string("127.0.0.1") : std::string(argv[2]);
    auto count = argc < 4 ? 10000 : atol(argv[3]);

    if (argv[1] == std::string("ping"))
    {
           return ping_udp(host, count);
    }
    else
    {
        return pong_udp(host);
    }

    return help();
}