# C++ UDP Ping Pong

UDP Latency benchmark inspired from https://github.com/farnyser/cpp-ping-pong

First, start the echo server (pong):

```./Measurement pong 192.168.43.186```

Then, start the ping client

```./Measurement ping 192.168.43.150 50000```

## Install Instructions

### Boost Library:
sudo apt-get install libboost1.67-*
### VS Code Tutorial:
https://electrobotify.wordpress.com/2019/08/16/remote-development-on-raspberry-pi-with-vs-code/

### Modifications Raspberry:

Increasing size of swap partition from 256MB to 1024MB.