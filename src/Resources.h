//
// Created by Giacomo Tanganelli on 29/10/16.
//

#ifndef COAPESP8266_RESOURCES_H
#define COAPESP8266_RESOURCES_H

#include "defines.h"
#include "Resource.h"

namespace coap {

    class Resources {
    private:
        Resource* _resources[MAX_RESOURCES];
        Resource* find(String uri);
        uint8_t index;
    public:
        Resources();

        int add(Resource *);

        Resource** getResources();

        uint8_t getNumResources();

        int handleRequest(CoAPPacket *request, CoAPPacket *response);

        virtual ~Resources();
    };

} /* namespace coap */


#endif //COAPESP8266_RESOURCES_H
