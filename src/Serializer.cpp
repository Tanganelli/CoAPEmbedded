//
// Created by Giacomo Tanganelli on 29/10/16.
//

#include "Serializer.h"

namespace coap {

    Serializer::Serializer() {
        // TODO Auto-generated constructor stub

    }

    Serializer::~Serializer() {
        // TODO Auto-generated destructor stub
    }


    uint32_t Serializer::serialize(CoAPPacket *packet, uint8_t *buffer){
        uint16_t running_delta = 0;
        uint16_t packetSize = 0;
        uint8_t *p = buffer;
        // make coap packet base header
        *p = 0x01 << 6;
        *p |= (packet->getType() & 0x03) << 4;
        *p++ |= (packet->getTokenlen() & 0x0F);
        *p++ = packet->getCode();
        *p++ = (uint8_t) (packet->getMessageid() >> 8);
        *p++ = (uint8_t) (packet->getMessageid() & 0xFF);
        p = buffer + COAP_HEADER_SIZE;
        packetSize += COAP_HEADER_SIZE;

        // make token
        if (packet->getToken() != nullptr && packet->getTokenlen() <= 0x0F) {
            memcpy(p, packet->getToken(), packet->getTokenlen());
            p += packet->getTokenlen();
            packetSize += packet->getTokenlen();
        }

        CoAPOption *o = (CoAPOption *) packet->getOptions();
        // make option header
        for (int i=0; i< packet->getOptionnum(); i++){
            uint32_t optdelta;
            uint8_t len, delta;


            if (packetSize + 5 + o->getLength() >= BUF_MAX_SIZE) {
                return 0;
            }
            optdelta = o->getNumber() - running_delta;
            COAP_OPTION_DELTA(optdelta, &delta);
            COAP_OPTION_DELTA((uint32_t)o->getLength(), &len);

            *p++ = (uint8_t) (0xFF & (delta << 4 | len));
            if (delta == 13) {
                *p++ = (uint8_t) (optdelta - 13);
                packetSize++;
            } else if (delta == 14) {
                *p++ = (uint8_t) ((optdelta - 269) >> 8);
                *p++ = (uint8_t) (0xFF & (optdelta - 269));
                packetSize+=2;
            } if (len == 13) {
                *p++ = (uint8_t) (o->getLength() - 13);
                packetSize++;
            } else if (len == 14) {
                *p++ = (uint8_t) (o->getLength() >> 8);
                *p++ = (uint8_t) (0xFF & (o->getLength() - 269));
                packetSize+=2;
            }

            memcpy(p, o->getBuffer(), o->getLength());

            p += o->getLength();
            packetSize +=o->getLength() + 1;
            running_delta = o->getNumber();
            o++;
        }

        // make payload
        if (packet->getPayloadlen() > 0) {
            if ((packetSize + 1 + packet->getPayloadlen()) >= BUF_MAX_SIZE) {
                return 0;
            }
            *p++ = 0xFF;
            //TODO check if binary

            memcpy(p, packet->getPayload(), packet->getPayloadlen());
            packetSize += 1 + packet->getPayloadlen();
        }
        return packetSize;

    }

    uint8_t Serializer::deserialize(uint8_t *buf, uint32_t packetSize, CoAPPacket *packet){
        memcpy((void *) packet->getBuffer(), buf, packetSize);
        uint8_t *buffer = (uint8_t *) packet->getBuffer();
        // parse coap packet header
        if (packetSize < COAP_HEADER_SIZE || (((buffer[0] & 0xC0) >> 6) != 1)) {
            return 1;
        }

        packet->setType((CoAP_TYPE) ((buffer[0] & 0x30) >> 4));
        packet->setTokenlen((uint8_t) (buffer[0] & 0x0F));
        packet->setCode(buffer[1]);
        uint16_t mid = (uint16_t) (0xFF00 & (buffer[2] << 8));
        mid |= 0x00FF & buffer[3];
        packet->setMessageid(mid);



        if (packet->getTokenlen() <= 8)
            memcpy((void *) packet->getToken(), buffer + 4, packet->getTokenlen());
        else {
            return 2;
        }

        // parse packet options/payload
        if (COAP_HEADER_SIZE + packet->getTokenlen() < packetSize) {
            int optionIndex = 0;
            uint16_t delta = 0;
            uint8_t *end = buffer + packetSize;
            uint8_t *p = buffer + COAP_HEADER_SIZE + packet->getTokenlen();
            while(*p != 0xFF && p < end) {
                if (0 != parseOption(packet, &delta, &p, end-p))
                    return 3;
                optionIndex++;
            }
            packet->setOptionnum((uint8_t) optionIndex);

            if (p+1 < end && *p == 0xFF) {
                packet->setPayloadlen((uint32_t) (end - (p + 1)));
                memcpy((void *) packet->getPayload(), p + 1, packet->getPayloadlen());

            } else {
                packet->setPayloadlen(0);
            }
        }
        return 0;
    }
    int Serializer::parseOption(CoAPPacket *packet, uint16_t *running_delta,
                                uint8_t **buf, size_t buflen) {
        uint8_t *p = *buf;
        uint8_t headlen = 1;
        uint16_t len, delta;

        if (buflen < headlen) return -1;

        delta = (uint16_t) ((p[0] & 0xF0) >> 4);
        len = (uint16_t) (p[0] & 0x0F);

        if (delta == 13) {
            headlen++;
            if (buflen < headlen) return -1;
            delta = (uint16_t) (p[1] + 13);
            p++;
        } else if (delta == 14) {
            headlen += 2;
            if (buflen < headlen) return -1;
            delta = (uint16_t) (((p[1] << 8) | p[2]) + 269);
            p+=2;
        } else if (delta == 15) return -1;

        if (len == 13) {
            headlen++;
            if (buflen < headlen) return -1;
            len = (uint16_t) (p[1] + 13);
            p++;
        } else if (len == 14) {
            headlen += 2;
            if (buflen < headlen) return -1;
            len = (uint16_t) (((p[1] << 8) | p[2]) + 269);
            p+=2;
        } else if (len == 15)
            return -1;

        if ((p + 1 + len) > (*buf + buflen))  return -1;
        CoAPOption option;
        option.setNumber((uint8_t) (delta + *running_delta));
        memcpy((void *) option.getBuffer(), p + 1, len);
        option.setLength(len);
        CoAPOption *o = (CoAPOption *) (packet->getOptions() + packet->getOptionnum() * sizeof(CoAPOption));
        memcpy(o, &option, sizeof(CoAPOption));
        *buf = p + 1 + len;
        *running_delta += delta;

        return 0;
    }

} /* namespace coap */
