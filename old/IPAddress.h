//
// Created by Giacomo Tanganelli on 29/10/16.
//

#ifndef COAPESP8266_IPADDRESS_H
#define COAPESP8266_IPADDRESS_H

#include <string>

class IPAddress {
private:
    union {
        uint8_t bytes[4];  // IPv4 address
        uint32_t dword;
    } _address;

    // Access the raw byte array containing the address.  Because this returns a pointer
    // to the internal structure rather than a copy of the address this function should only
    // be used when you know that the usage of the returned uint8_t* will be transient and not
    // stored.
    uint8_t* raw_address() { return _address.bytes; };
public:
    IPAddress();
    virtual ~IPAddress();
    IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    IPAddress(uint32_t address);
    IPAddress(const uint8_t *address);

    bool fromString(const char *address);
    bool fromString(const std::string &address) { return fromString(address.c_str()); }

    // Overloaded cast operator to allow IPAddress objects to be used where a pointer
    // to a four-byte uint8_t array is expected
    operator uint32_t() const { return _address.dword; };
    bool operator==(const IPAddress& addr) const { return _address.dword == addr._address.dword; };
    bool operator==(const uint8_t* addr) const;

    // Overloaded index operator to allow getting and setting individual octets of the address
    uint8_t operator[](int index) const { return _address.bytes[index]; };
    uint8_t& operator[](int index) { return _address.bytes[index]; };

    // Overloaded copy operators to allow initialisation of IPAddress objects from other types
    IPAddress& operator=(const uint8_t *address);
    IPAddress& operator=(uint32_t address);
    std::string toString() const;
};


#endif //COAPESP8266_IPADDRESS_H
