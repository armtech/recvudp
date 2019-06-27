# UDP Multicast Receiver 

A simple UDP receiver with multicast addressing.

This code just receive multicast UDP packets an port 1900.

This mechanism based on the Simple Service Discovery Protocol (SSDP).

Reference:  https://tools.ietf.org/html/draft-cai-ssdp-v1-00

Andrejs Tihomirovs (c) 2019

armtech@its.lv

## Compile

Compile this code using the standard GNU C or C++ compiler:


    git clone https://github.com/armtech/recvudp.git
    cd recvudp/
    cc -o recvudp recvudp.c
    
## Run

From within the `recvudp` directory just issue:

    ./recvudp 239.255.255.250 1900
