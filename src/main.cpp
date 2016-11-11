#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <CoAPServer.h>
#include <Resource.h>
#include <CoAPPacket.h>


WiFiUDP Udp;
coap::CoAPServer server(Udp);

class Dimmer : public coap::Resource{
public:
    String value;
    Dimmer(String path):Resource(path), value("7"){}

    int get(coap::CoAPPacket &request, IPAddress remote_ip,
            int remote_port, coap::CoAPPacket &response) {
        response.setPayload(value);
        return 0;
    }

    int put(coap::CoAPPacket &request, IPAddress remote_ip,
            int remote_port, coap::CoAPPacket &response) {
        value = request.getPayloadAsString();
        return 0;
    }
};


Dimmer dimmer("dimmer");
//The setup function is called once at startup of the sketch
void setup()
{
    server.add(&dimmer);
    server.start();
}

// The loop function is called in an endless loop
void loop()
{
    server.loop();
}

int main(){
    setup();
    while(true){
        //std::cout << "Loop" << std::endl;
        loop();
    }
}