//
// Created by Giacomo Tanganelli on 02/11/16.
//

#include <WiFiUdp.h>
#include "CoAPClient.h"

coap::CoAPClient::CoAPClient(): port(0) {

    this->_udp = new WiFiUDP();

    this->_udp->begin(port);
}

coap::CoAPPacket coap::CoAPClient::sendSynchronousRequest(CoAPPacket *request){

    if(request->getType() == CoAP_UNSET)
        request->setType(CoAP_CON);
    if(request->getMessageid() == 0){
        mid = (uint16_t) ((mid + 1) % 65000);
        request->setMessageid(mid);
    }
    request->setTimestamp(millis());
    _sent.setRequest(*request);

    send(request);

    int32_t packetlen = _udp->parsePacket();
    uint8_t buffer[BUF_MAX_SIZE];
    while (packetlen > 0) {
        checkRetransmission();
        CoAPPacket packet;
        packetlen = received(&packet, buffer, packetlen);
        packet.setIp(_udp->remoteIP());
        packet.setPort(_udp->remotePort());
        int ret = _serializer.deserialize(buffer, (uint32_t) packetlen, &packet);
        if (ret != 0) {
            packetlen = _udp->parsePacket();
            continue;
        }
        if (packet.isResponse()) {
#ifdef DEBUG
            Serial.println("Response");
#endif
            CoAPPacket response = packet;
            response.setTimestamp(millis());
            return response;
        } else if(packet.isRequest()) {
            // NOTHING
        } else if(packet.isEmpty()){
            if(packet.getType() == CoAP_RST)
                return CoAPPacket();
        }
        packetlen = _udp->parsePacket();
    }
}

void coap::CoAPClient::checkRetransmission() {
    unsigned long sent_at = _sent.getRequest().getTimestamp();
    unsigned long now = millis();
    if(_sent.getRequest().getType() == CoAP_CON && (now - sent_at) > _sent.getRetransmit_after() && _sent.getAttemps()< MAX_RETRANSMIT)
    {
        uint8_t attemps = _sent.getAttemps();
        attemps++;
        _sent.setAttemps(attemps);
        unsigned long int future_time = _sent.getRetransmit_after();
        future_time *= 2;
        _sent.setRetransmit_after(future_time);
        send((CoAPPacket *) &_sent.getRequest());
    }
}

void coap::CoAPClient::send(coap::CoAPPacket *packet) {
    uint8_t buffer[BUF_MAX_SIZE];
#ifdef DEBUG
    Serial.println("Edited");
    Serial.print(packet->print());
#endif
    uint32_t packetSize = _serializer.serialize(packet, buffer);
#ifdef DEBUG
    Serial.println("Write to UDP");
#endif

    _udp->beginPacket(packet->getIp(), (uint16_t) packet->getPort());
    _udp->write(buffer, packetSize);
    _udp->endPacket();
}

int32_t coap::CoAPClient::received(coap::CoAPPacket *p, uint8_t *buffer, int packetlen) {
    CoAPPacket &packet = *p;

    packetlen = _udp->read((unsigned char *) buffer, packetlen >= BUF_MAX_SIZE ? BUF_MAX_SIZE : packetlen);
#ifdef DEBUG
        Serial.println("Packet Received");
#endif
    return packetlen;
}

