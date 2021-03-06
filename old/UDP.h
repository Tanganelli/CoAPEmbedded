//
// Created by Giacomo Tanganelli on 29/10/16.
//

#ifndef COAPESP8266_UDP_H
#define COAPESP8266_UDP_H


#include "IPAddress.h"
#include <netinet/in.h>

class UDP {
    uint8_t _sock;
    uint16_t _port;
    struct sockaddr_in _remote;
    uint8_t _buffer[1024];
    size_t _size;
public:
    UDP();

    uint8_t begin(uint16_t);	// initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
    void stop();  // Finish with the UDP socket

    // Sending UDP packets

    // Start building up a packet to send to the remote host specific in ip and port
    // Returns 1 if successful, 0 if there was a problem with the supplied IP address or port
    int beginPacket(IPAddress ip, uint16_t port);
    // Start building up a packet to send to the remote host specific in host and port
    // Returns 1 if successful, 0 if there was a problem resolving the hostname or port
    int beginPacket(const char *host, uint16_t port);
    // Finish off this packet and send it
    // Returns 1 if the packet was sent successfully, 0 if there was an error
    int endPacket();
    // Write a single byte into the packet
    size_t write(uint8_t);
    // Write size bytes from buffer into the packet
    size_t write(const uint8_t *buffer, size_t size);


    // Start processing the next available incoming packet
    // Returns the size of the packet in bytes, or 0 if no packets are available
    int parsePacket();
    // Number of bytes remaining in the current packet
    int available();
    // Read a single byte from the current packet
    int read();
    // Read up to len bytes from the current packet and place them into buffer
    // Returns the number of bytes read, or 0 if none are available
    int read(unsigned char* buffer, size_t len);
    // Read up to len characters from the current packet and place them into buffer
    int read(char* buffer, size_t len) { return read((unsigned char*)buffer, len); };
    // Return the next byte from the current packet without moving on to the next byte
    int peek();
    void flush();	// Finish reading the current packet

    // Return the IP address of the host who sent the current incoming packet
    IPAddress remoteIP();
    // Return the port of the host who sent the current incoming packet
    uint16_t remotePort();

};

#endif //COAPESP8266_UDP_H
