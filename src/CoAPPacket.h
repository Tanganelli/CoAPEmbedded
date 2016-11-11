//
// Created by Giacomo Tanganelli on 29/10/16.
//

#ifndef COAPESP8266_COAPPACKET_H
#define COAPESP8266_COAPPACKET_H


#include "defines.h"

#include "CoAPOption.h"

#ifdef NATIVE
#include "IPAddress.h"
#else
#include <IPAddress.h>
#endif

namespace coap {
    class CoAPPacket {
    private:
        CoAP_TYPE type;

        uint8_t code;
        uint8_t token[MAX_TOKEN_LENGTH];
        uint8_t tokenlen;
        uint8_t payload[MAX_PAYLOAD];
        uint32_t payloadlen;
        uint16_t messageid;
        uint8_t optionnum;
        CoAPOption options[MAX_OPTIONS];

        uint8_t buffer[BUF_MAX_SIZE];
        IPAddress ip;
        int port;
        unsigned long timestamp;
    public:

        CoAPPacket();

        virtual ~CoAPPacket();

        const CoAP_TYPE &getType() const;

        void setType(CoAP_TYPE type);

        uint8_t getCode() const;

        void setCode(uint8_t code);

        const uint8_t *getToken() const;

        void setToken(uint8_t* pt, uint8_t len);

        uint8_t getTokenlen() const;

        void setTokenlen(uint8_t tokenlen);

        const uint8_t *getPayload() const;

        void setPayload(uint8_t* pt, uint32_t len);

        uint32_t getPayloadlen() const;

        void setPayloadlen(uint32_t payloadlen);

        uint16_t getMessageid() const;

        void setMessageid(uint16_t messageid);

        uint8_t getOptionnum() const;

        void setOptionnum(uint8_t optionnum);

        const CoAPOption *getOptions() const;

        const uint8_t *getBuffer() const;

        const IPAddress &getIp() const;

        void setIp(const IPAddress &ip);

        int getPort() const;

        void setPort(int port);

        unsigned long getTimestamp() const;

        void setTimestamp(unsigned long timestamp);

        bool isRequest();

        bool isResponse();

        bool isEmpty();

        String getUriPath() const;

        int setUriPath(String uri);

        String getUriQuery() const;

        int addUriQuery(String uri);

        void setPayload(String);

        String getPayloadAsString() const;

        String getLocationPathAsString() const;

        int setContentType(CoAP_CONTENT_TYPE);

        CoAP_CONTENT_TYPE getContentType() const;

#ifdef DEBUG
        String print() const;
#endif
    };
}


#endif //COAPESP8266_COAPPACKET_H
