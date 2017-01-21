//
// Created by Giacomo Tanganelli on 02/11/16.
//

#ifndef COAPESP8266_COAPCLIENT_H
#define COAPESP8266_COAPCLIENT_H


#include <Udp.h>
#include "CoAPPacket.h"
#include "Transaction.h"
#include "Serializer.h"

namespace coap{
    class CoAPClient {
    private:
        UDP *_udp;
        Serializer _serializer;
        Transaction _sent;
        uint16_t port;
        uint8_t index;
        uint16_t mid;

        void checkRetransmission();

        void send(CoAPPacket* packet);

        int32_t received(CoAPPacket *p, uint8_t* buffer, int packetlen);
    public:
        CoAPClient();
        CoAPPacket sendSynchronousRequest(CoAPPacket *request);
    };
}


#endif //COAPESP8266_COAPCLIENT_H
