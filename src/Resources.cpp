//
// Created by Giacomo Tanganelli on 29/10/16.
//


#include "Resources.h"

namespace coap {

    Resources::Resources(): index(0) {

    }

    Resources::~Resources() {
        // TODO Auto-generated destructor stub
    }

    int Resources::add(Resource *resource){
        if(index < MAX_RESOURCES){
            _resources[index] = resource;
            index++;
            return 0;
        }
        return 1;
    }
    int Resources::handleRequest(CoAPPacket *request, CoAPPacket *response){
        IPAddress remote_ip = request->getIp();
        int remote_port = request->getPort();
        CoAP_METHOD method = (CoAP_METHOD) request->getCode();
        String request_path = request->getUriPath();
        memcpy((void *) response->getToken(), request->getToken(), request->getTokenlen());
        response->setIp(remote_ip);
        response->setPort(remote_port);

        if(request->getType() == CoAP_CON){
            response->setMessageid(request->getMessageid());
            response->setType(CoAP_ACK);
        } else {
            response->setType(CoAP_NON);
        }
#ifdef DEBUG
#ifdef NATIVE
        std::cout<<"Search for resource"<<std::endl;
#else
        Serial.println("Search for resource");
#endif
#endif
        Resource *resource = find(request_path);
        if(resource != nullptr){
#ifdef DEBUG
#ifdef NATIVE
            std::cout<<"Resource found"<<std::endl;
#else
            Serial.println("Resource found");
#endif
#endif
            int ret = resource->handleRequest(request, remote_ip, remote_port, method,
                                                response);
            return ret;
        }
        response->setCode(CoAP_NOT_FOUND);
        return 0;
    }

    Resource* Resources::find(String uri){
        for(int i = 0; i<index; i++ ){
            if(_resources[i]->getPath() == uri)
                return _resources[i];
        }
        return nullptr;
    }

    Resource** Resources::getResources(){
        return _resources;
    }

    uint8_t Resources::getNumResources(){
        return index;
    }

} /* namespace coap */
