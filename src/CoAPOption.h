//
// Created by Giacomo Tanganelli on 31/10/16.
//

#ifndef COAPESP8266_COAPOPTION_H
#define COAPESP8266_COAPOPTION_H

#include "defines.h"

class CoAPOption {
private:
    uint8_t number;
    uint16_t length;
    uint8_t buffer[MAX_OPTION_BUFFER];
public:

    CoAPOption();

    ~CoAPOption();

    uint8_t getNumber() const;

    void setNumber(uint8_t number);

    uint16_t getLength() const;

    void setLength(uint16_t length);

    const uint8_t *getBuffer() const;

};

#endif //COAPESP8266_COAPOPTION_H
