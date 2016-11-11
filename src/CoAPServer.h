//
// Created by Giacomo Tanganelli on 29/10/16.
//

#ifndef COAPESP8266_COAPSERVER_H
#define COAPESP8266_COAPSERVER_H

#include "defines.h"
#include "Resources.h"
#include "Resource.h"
#include "Transaction.h"
#include "Serializer.h"

#ifdef NATIVE
#include "IPAddress.h"
#include "UDP.h"
#else
#include <IPAddress.h>
#include <Udp.h>
#endif

#include <map>



namespace coap {

    class CoAPServer {
    private:
        UDP *_udp;
        Resources _resources;
        Serializer _serializer;
        Transaction _received[MAX_STORAGE_TRANSACTION];
        uint8_t index;
        uint16_t mid;
#ifdef NATIVE
        int _sock;
#endif
        unsigned long int last_check;

        bool isDuplicate(uint16_t mid, IPAddress ip, int port, const CoAPPacket *resp);

        void clear_old_transactions();
    public:
        void check_retrasmission();

#ifdef NATIVE
        unsigned long int millis();

        void send(int sock, struct sockaddr_in remote, CoAPPacket *message);

        ssize_t received(CoAPPacket *p, uint8_t* buffer, int packetlen, struct sockaddr_in *remote, unsigned int clientlen);
#else
        void send(CoAPPacket *);

        int32_t received(CoAPPacket *p, uint8_t* buffer, int packetlen);

#endif

        CoAPServer(UDP &udp);

        virtual ~CoAPServer();

        void start(uint16_t);

        void start();

        void loop();

        void add(Resource *);

        Resources getResources() const;

        Serializer& getSerializer();

#ifdef NATIVE
        int getSocket() const;
#endif

    };

} /* namespace coap */


#endif //COAPESP8266_COAPSERVER_H
