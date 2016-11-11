//
// Created by Giacomo Tanganelli on 31/10/16.
//

#include "CoAPOption.h"

CoAPOption::CoAPOption(){
    //TODO
}

CoAPOption::~CoAPOption() {
    //TODO
}

uint8_t CoAPOption::getNumber() const {
    return number;
}

void CoAPOption::setNumber(uint8_t number) {
    CoAPOption::number = number;
}

uint16_t CoAPOption::getLength() const {
    return length;
}

void CoAPOption::setLength(uint16_t length) {
    CoAPOption::length = length;
}

const uint8_t* CoAPOption::getBuffer() const {
    return buffer;
}