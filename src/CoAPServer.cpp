//
// Created by Giacomo Tanganelli on 29/10/16.
//

#ifdef NATIVE
#include <arpa/inet.h>
#endif
#include "CoAPServer.h"

namespace coap {

    CoAPServer::CoAPServer(UDP& udp): _udp(&udp), index(0), mid(0), last_check(0)
#ifdef NATIVE
            , _sock(0)
#endif
    {


    }

    CoAPServer::~CoAPServer() {
        // TODO Auto-generated destructor stub
    }

    void CoAPServer::start() {
        start(CoAP_DEFAULT_PORT);

    }

    void CoAPServer::start(uint16_t port) {
        this->_udp->begin(port);
#ifdef NATIVE
        struct sockaddr_in si_me;
        ssize_t packetlen = 1;
        //create a UDP socket
        if ((_sock=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            exit(1);
        }
        // zero out the structure
        memset((char *) &si_me, 0, sizeof(si_me));

        int optval = 1;
        setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

        si_me.sin_family = AF_INET;
        si_me.sin_port = htons(port);
        si_me.sin_addr.s_addr = htonl(INADDR_ANY);


        if( bind(_sock , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
        {
            exit(2);
        }
#endif
    }

    Resources CoAPServer::getResources() const{
        return _resources;
    }


    void CoAPServer::loop(){
        uint8_t buffer[BUF_MAX_SIZE];
#ifdef NATIVE
        struct sockaddr_in si_other;
        ssize_t packetlen = 1;
        unsigned int clientlen = sizeof(si_other);
#else
        int32_t packetlen = _udp->parsePacket();
        uint8_t ret = 0;
#ifdef DEBUG
        Serial.println("Loop");
#endif
#endif
        while (packetlen > 0) {
            check_retrasmission();
            CoAPPacket packet;
#ifdef NATIVE
            packetlen = received(&packet, buffer, (int) packetlen, &si_other, clientlen);
            IPAddress ip(uint32_t(si_other.sin_addr.s_addr));
            uint16_t remote_port = ntohs(si_other.sin_port);

            packet.setIp(ip);
            packet.setPort(remote_port);
#else
            packetlen = received(&packet, buffer, packetlen);
            packet.setIp(_udp->remoteIP());
            packet.setPort(_udp->remotePort());
#endif

            int ret = _serializer.deserialize(buffer, (uint32_t) packetlen, &packet);
#ifdef DEBUG
#ifdef NATIVE
            std::cout<<"Serializer: "<<ret<<std::endl;
#else
            Serial.print("Serializer:");
		    Serial.println(ret);
#endif
#endif
            if (ret != 0){
                packetlen = _udp->parsePacket();
                continue;
            }
            CoAPPacket *resp = nullptr;
            if(isDuplicate(packet.getMessageid(), packet.getIp(), packet.getPort(), resp)){
#ifdef DEBUG
#ifdef NATIVE
                std::cout<<"Duplicate "<<std::endl;
#else
                Serial.println("Duplicate");
#endif
#endif
#ifdef NATIVE
                send(_sock,si_other,resp);
#else
                send(resp);
#endif
                continue;
            }

            if(packet.isRequest()){
#ifdef DEBUG
#ifdef NATIVE
                std::cout<<"Request "<<std::endl;
#else
                Serial.println("Request");
#endif
#endif
                CoAPPacket request = packet;
                request.setTimestamp(millis());
                CoAPPacket response;
#ifdef DEBUG
#ifdef NATIVE
                std::cout<<packet.print()<<std::endl;
#else
                Serial.println(packet.print());
#endif
#endif
                _resources.handleRequest(&request, &response);
                response.setTimestamp(millis());
                Transaction t;
                t.setRequest(request);
                t.setResponse(response);
                if(response.getType() == CoAP_CON){
                    //retransmit
                    t.setAcknowledged(false);

                    unsigned int max = (unsigned int) (ACK_TIMEOUT * ACK_RANDOM_FACTOR) * 1000;
                    unsigned int min = ACK_TIMEOUT * 1000;
#ifdef NATIVE
                    unsigned int rnd = rand() % (min + max) + min;
#else
                    unsigned int rnd = random(min, max);
#endif
                    unsigned long int future_time = rnd;
                    t.setRetransmit_after(future_time);
                    t.setAttemps(0);
                }
                _received[index] = t;
                index = (uint8_t) ((index + 1) % MAX_STORAGE_TRANSACTION);
#ifdef NATIVE
                send(_sock,si_other,&response);
#else
                send(&response);
#endif
                clear_old_transactions();


            } else if(packet.isResponse()){
                //Nothing on the server
            } else if(packet.isEmpty()){
#ifdef DEBUG
#ifdef NATIVE
                std::cout<<"Empty"<<std::endl;
#else
                Serial.println("Empty");
#endif
#endif
                for(int i=0; i<MAX_STORAGE_TRANSACTION; i++){
                    if(!_received[i].isEmpty() && _received[i].getResponse().getIp() == packet.getIp()
                       && _received[i].getResponse().getPort() == packet.getPort()
                       && _received[i].getResponse().getMessageid() == packet.getMessageid()
                       && !_received[i].isAcknowledged()){
                        _received[i].setAcknowledged(true);
                        break;
                    } else if(!_received[i].isEmpty() && _received[i].getResponse().getIp() == packet.getIp()
                              && _received[i].getResponse().getPort() == packet.getPort() && !_received[i].isAcknowledged()
                              && _received[i].getResponse().getTokenlen() == packet.getTokenlen()){
                        const uint8_t *token_resp = _received[i].getResponse().getToken();
                        const uint8_t *token_pkt = packet.getToken();
                        if(memcmp(token_pkt, token_resp, packet.getTokenlen()) == 0){
                            _received[i].setAcknowledged(true);
                            break;
                        }
                    }

                }
            }
        }
    }
    void CoAPServer::clear_old_transactions(){
        for (int i=0; i < MAX_STORAGE_TRANSACTION; i++)
        {
            if(_received[i].isEmpty()) continue;

            unsigned long now = millis();
            unsigned long old = _received[i].getRequest().getTimestamp();
            if((now - old) > EXCHANGE_LIFETIME){
                //TODO

#ifdef DEBUG
#ifdef NATIVE
                std::cout<<"Remove old transaction"<<std::endl;
                std::cout<<_received[i].getRequest().print()<<std::endl;
                std::cout<<_received[i].getResponse().print()<<std::endl;
#else
                Serial.println("Remove old transaction");
                Serial.println(_received[i].getRequest().print());
                Serial.println(_received[i].getResponse().print());
#endif
#endif

            }

        }

    }
    bool CoAPServer::isDuplicate(uint16_t mid, IPAddress ip, int port, const CoAPPacket *resp){
        for(int i=0; i<MAX_STORAGE_TRANSACTION; i++){
            if(!_received[i].isEmpty() && _received[i].getRequest().getIp() == ip
               && _received[i].getRequest().getPort() == port
               && _received[i].getRequest().getMessageid() == mid){
                resp = &(_received[i].getResponse());
                return true;
            }
        }
        return false;
    }

#ifdef NATIVE
    void CoAPServer::send(int sock, struct sockaddr_in remote, CoAPPacket *packet){
#else
    void CoAPServer::send(CoAPPacket *packet){
#endif
#ifdef DEBUG
#ifdef NATIVE
        std::cout<<"Send Message Original:"<<std::endl;
        std::cout<<packet->print()<<std::endl;
#else
        Serial.println("Send message");
		Serial.println("Original");
        Serial.print(packet->print());
#endif
#endif
        if(packet->getType() == CoAP_UNSET)
            packet->setType(CoAP_CON);
        if(packet->getMessageid() == 0){
            mid = (uint16_t) ((mid + 1) % 65000);
            packet->setMessageid(mid);
        }
        uint8_t buffer[BUF_MAX_SIZE];
#ifdef DEBUG
#ifdef NATIVE
        std::cout<<"Edited"<<std::endl<<packet->print()<<std::endl;
#else
        Serial.println("Edited");
		Serial.print(packet->print());
#endif
#endif
        uint32_t packetSize = _serializer.serialize(packet, buffer);
#ifdef DEBUG
#ifdef NATIVE
        std::cout<<"Write to UDP"<<std::endl;
#else
        Serial.println("Write to UDP");
#endif
#endif
#ifdef NATIVE
        socklen_t slen = sizeof(remote);
        sendto(sock, buffer, packetSize, 0, (struct sockaddr*) &remote, slen);
#else
        _udp->beginPacket(packet->getIp(), packet->getPort());
        _udp->write(buffer, packetSize);
        _udp->endPacket();
#endif
    }

    void CoAPServer::add(Resource * resource){
        _resources.add(resource);
    }

    void CoAPServer::check_retrasmission() {
        for(int i=0; i<MAX_STORAGE_TRANSACTION; i++){
            if(!_received[i].isEmpty() && !_received[i].isAcknowledged()){
                //check time
                unsigned long sent_at = _received[i].getResponse().getTimestamp();
                unsigned long now = millis();
                if((now - sent_at) > _received[i].getRetransmit_after() && _received[i].getAttemps()< MAX_RETRANSMIT)
                {
                    uint8_t attemps = _received[i].getAttemps();
                    attemps++;
                    _received[i].setAttemps(attemps);
                    unsigned long int future_time = _received[i].getRetransmit_after();
                    future_time *= 2;
                    _received[i].setRetransmit_after(future_time);
#ifdef NATIVE
                    struct sockaddr_in si_other;
                    si_other.sin_family = AF_INET;
                    si_other.sin_port = htons(_received[i].getResponse().getPort());
                    si_other.sin_addr.s_addr = inet_addr(_received[i].getResponse().getIp().toString().c_str());
                    send(_sock, si_other, (CoAPPacket *) &_received[i].getResponse());
#else
                    send((CoAPPacket *) &_received[i].getResponse());
#endif
                }
            }
        }
    }
#ifdef NATIVE
    ssize_t CoAPServer::received(CoAPPacket *p, uint8_t* buffer, int packetlen, struct sockaddr_in *remote, unsigned int clientlen){
#else
        int32_t CoAPServer::received(CoAPPacket *p, uint8_t* buffer, int packetlen){
#endif

        CoAPPacket &packet = *p;

#ifdef NATIVE

        packetlen = recvfrom(_sock, buffer, BUF_MAX_SIZE, 0, (struct sockaddr *) remote, &clientlen);
#else
        packetlen = _udp->read((unsigned char *) buffer, packetlen >= BUF_MAX_SIZE ? BUF_MAX_SIZE : packetlen);
#endif

#ifdef DEBUG
#ifdef NATIVE
        std::cout<<"Packet Received"<<std::endl;
#else
        Serial.println("Packet Received");
#endif
#endif
        return packetlen;
    }


    Serializer& CoAPServer::getSerializer(){
        return _serializer;
    }

#ifdef NATIVE
    unsigned long int CoAPServer::millis(){
        return (unsigned long int)time(nullptr);
    }
    int CoAPServer::getSocket() const{
        return _sock;
    }

#endif
} /* namespace coap */