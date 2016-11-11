//
// Created by Giacomo Tanganelli on 29/10/16.
//

#include "../include/UDP.h"

/* Constructor */
UDP::UDP() : _sock(0), _port(0), _size(0) {}

/* Start UDP socket, listening at local port PORT */
uint8_t UDP::begin(uint16_t port) {
    _port = port;
    return 0;

}

/* return number of bytes available in the current packet,
   will return zero if parsePacket hasn't been called yet */
int UDP::available() {

    return 1024;
}

/* Release any resources being used by this UDP instance */
void UDP::stop()
{

}

int UDP::beginPacket(const char *host, uint16_t port)
{
    return 0;
}

int UDP::beginPacket(IPAddress ip, uint16_t port)
{

    _remote.sin_family = AF_INET;
    _remote.sin_port = htons(port);
    _remote.sin_addr.s_addr = htonl(ip);


    return 0;
}

int UDP::endPacket()
{
    int slen = sizeof(_remote);
    return sendto(_sock, _buffer, _size, 0, (struct sockaddr*) &_remote, slen);
}

size_t UDP::write(uint8_t byte)
{
    return write(&byte, 1);
}

size_t UDP::write(const uint8_t *buffer, size_t size)
{
    bzero(_buffer, 1024);
    memcpy(_buffer, buffer, 1024);
    _size = size;
    return size;
}

int UDP::parsePacket()
{
    return available();
}

int UDP::read()
{
    return -1;
}

int UDP::read(unsigned char* buffer, size_t len)
{
    unsigned int slen = sizeof(_remote);
    bzero(_buffer, 1024);
    return recvfrom(_sock, (void*)_buffer, (unsigned long int)1024, 0, (struct sockaddr *) &_remote, &slen);
}

int UDP::peek()
{
    return 0;
}

void UDP::flush()
{
    // TODO: a real check to ensure transmission has been completed
}

IPAddress UDP::remoteIP()
{
    IPAddress ip(uint32_t(_remote.sin_addr.s_addr));
    return ip;
}

uint16_t UDP::remotePort()
{

    uint16_t port = ntohs(_remote.sin_port);
    return port;
}

