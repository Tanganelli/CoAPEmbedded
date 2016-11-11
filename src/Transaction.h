//
// Created by Giacomo Tanganelli on 29/10/16.
//

#ifndef COAPESP8266_TRANSACTION_H
#define COAPESP8266_TRANSACTION_H

#include "CoAPPacket.h"

namespace coap {

    class Transaction {

    private:
        CoAPPacket request;
        CoAPPacket response;
        bool empty;
        bool acknowledged;
        bool timeouted;
        unsigned long int retransmit_after;
        uint8_t attemps;

    public:
        Transaction();

        virtual ~Transaction();

        const CoAPPacket &getRequest() const;

        void setRequest(const CoAPPacket &request);

        const CoAPPacket &getResponse() const;

        void setResponse(const CoAPPacket &response);

        bool isEmpty();

        bool isAcknowledged() const;

        void setAcknowledged(bool acknowledged);

        bool isTimeouted() const;

        void setTimeouted(bool timeouted);

        unsigned long getRetransmit_after() const;

        void setRetransmit_after(unsigned long retransmit_after);

        uint8_t getAttemps() const;

        void setAttemps(uint8_t attemps);

    };

} /* namespace coap */

#endif //COAPESP8266_TRANSACTION_H
