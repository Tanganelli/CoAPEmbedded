//
// Created by Giacomo Tanganelli on 29/10/16.
//

#ifndef COAPESP8266_RESOURCE_H
#define COAPESP8266_RESOURCE_H


#include "CoAPPacket.h"

#ifdef NATIVE
#include "IPAddress.h"
#else
#include <IPAddress.h>
#endif

namespace coap {

    class Resource {
    private:
        String _path;
    public:
        Resource(String);

        int handleRequest(CoAPPacket *, IPAddress, int, CoAP_METHOD, CoAPPacket *);

        virtual int get(CoAPPacket &, IPAddress, int, CoAPPacket &);

        virtual int post(CoAPPacket &, IPAddress, int, CoAPPacket &);

        virtual int put(CoAPPacket &, IPAddress, int, CoAPPacket &);

        virtual int del(CoAPPacket &, IPAddress, int, CoAPPacket &);

        String getPath();

        String getCoRELinkFormat();

        virtual ~Resource();
    };

} /* namespace coap */


#endif //COAPESP8266_RESOURCE_H
