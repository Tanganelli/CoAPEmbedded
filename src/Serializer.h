//
// Created by Giacomo Tanganelli on 29/10/16.
//

#ifndef COAPESP8266_SERIALIZER_H
#define COAPESP8266_SERIALIZER_H

#include "defines.h"
#include "CoAPPacket.h"

namespace coap {

    class Serializer {
    private:
        int parseOption(CoAPPacket *, uint16_t *, uint8_t **, size_t);

    public:
        Serializer();

        uint32_t serialize(CoAPPacket *, uint8_t *);

        uint8_t deserialize(uint8_t *, uint32_t, CoAPPacket *);

        virtual ~Serializer();
    };

} /* namespace coap */


#endif //COAPESP8266_SERIALIZER_H
