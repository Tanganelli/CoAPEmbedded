//
// Created by Giacomo Tanganelli on 29/10/16.
//

#include "Resource.h"

namespace coap {

    Resource::Resource(String uri): _path(uri) {
            // TODO Auto-generated constructor stub

    }

    Resource::~Resource() {
        // TODO Auto-generated destructor stub
    }

    int Resource::handleRequest(CoAPPacket *request, IPAddress remote_ip,
                                int remote_port, CoAP_METHOD method, CoAPPacket *response){
        int ret = 1;
        switch(method){
            case(CoAP_GET):
#ifdef DEBUG
#ifdef NATIVE
                std::cout<<"GET Request"<<std::endl;
#else
                Serial.println("GET request");
#endif
#endif
                ret = get(*request, remote_ip, remote_port, *response);
#ifdef DEBUG
#ifdef NATIVE
                std::cout<<"GET ret: "<<ret<<std::endl;
#else
                Serial.print("GET ret:");
                Serial.println(ret);
#endif
#endif
                if(response->getCode() == 0){
                    if(ret != 0)
                        response->setCode(CoAP_BAD_REQUEST);
                    else
                        response->setCode(CoAP_CONTENT);
                }
                break;
            case(CoAP_POST):
                ret = post(*request, remote_ip, remote_port, *response);
                if(response->getCode() == 0){
                    if(ret != 0)
                        response->setCode(CoAP_BAD_REQUEST);
                    else
                        response->setCode(CoAP_CHANGED);
                }
                break;
            case(CoAP_PUT):
                ret = put(*request, remote_ip, remote_port, *response);
                if(response->getCode() == 0){
                    if(ret != 0)
                        response->setCode(CoAP_BAD_REQUEST);
                    else
                        response->setCode(CoAP_CREATED);
                }
                break;
            case(CoAP_DELETE):
                ret = del(*request, remote_ip, remote_port, *response);
                if(response->getCode() == 0){
                    if(ret != 0)
                        response->setCode(CoAP_BAD_REQUEST);
                    else
                        response->setCode(CoAP_DELETED);
                }
                break;
        }
        return ret;
    }

    int Resource::get(CoAPPacket &request, IPAddress remote_ip,
                      int remote_port, CoAPPacket &response) {
        response.setCode(CoAP_METHOD_NOT_ALLOWD);
        return NOT_ALLOWED;
    }

    int Resource::put(CoAPPacket &request, IPAddress remote_ip,
                      int remote_port, CoAPPacket &response) {
        response.setCode(CoAP_METHOD_NOT_ALLOWD);
        return NOT_ALLOWED;
    }

    int Resource::post(CoAPPacket &request, IPAddress remote_ip,
                       int remote_port, CoAPPacket &response) {
        response.setCode(CoAP_METHOD_NOT_ALLOWD);
        return NOT_ALLOWED;
    }

    int Resource::del(CoAPPacket &request, IPAddress remote_ip,
                      int remote_port, CoAPPacket &response) {
        response.setCode(CoAP_METHOD_NOT_ALLOWD);
        return NOT_ALLOWED;
    }
    String Resource::getPath(){
        return _path;
    }

    String Resource::getCoRELinkFormat(){
        //TODO add attributes
        return "<" + _path + ">;";
    }
} /* namespace coap */
