//
// Created by Giacomo Tanganelli on 29/10/16.
//

#include "CoAPPacket.h"

namespace coap {

    CoAPPacket::CoAPPacket():
            optionnum(0), type(CoAP_UNSET), tokenlen(0), messageid(0),
            payloadlen(0), code(0){
    }

    CoAPPacket::~CoAPPacket() {
        //TODO
    }

    void CoAPPacket::setType(CoAP_TYPE type) {
        CoAPPacket::type = type;
    }

    const CoAP_TYPE& CoAPPacket::getType() const {
        return type;
    }

    uint8_t CoAPPacket::getCode() const {
        return code;
    }

    void CoAPPacket::setCode(uint8_t code) {
        CoAPPacket::code = code;
    }

    const uint8_t* CoAPPacket::getToken() const {
        return token;
    }

    void CoAPPacket::setToken(uint8_t* pt, uint8_t len){
        if(len >= MAX_TOKEN_LENGTH) len = MAX_TOKEN_LENGTH;
        tokenlen = len;
        memcpy(token, pt, len);
    }

    const uint8_t* CoAPPacket::getPayload() const {
        return payload;
    }

    void CoAPPacket::setPayload(uint8_t* pt, uint32_t len){
        if(len >= MAX_PAYLOAD) len = MAX_PAYLOAD;
        payloadlen = len;
        memcpy(payload, pt, len);
    }

    uint8_t CoAPPacket::getTokenlen() const {
        return tokenlen;
    }

    void CoAPPacket::setTokenlen(uint8_t tokenlen) {
        CoAPPacket::tokenlen = tokenlen;
    }


    uint32_t CoAPPacket::getPayloadlen() const {
        return payloadlen;
    }

    void CoAPPacket::setPayloadlen(uint32_t payloadlen) {
        CoAPPacket::payloadlen = payloadlen;
    }

    uint16_t CoAPPacket::getMessageid() const {
        return messageid;
    }

    void CoAPPacket::setMessageid(uint16_t messageid) {
        CoAPPacket::messageid = messageid;
    }

    uint8_t CoAPPacket::getOptionnum() const {
        return optionnum;
    }

    void CoAPPacket::setOptionnum(uint8_t optionnum) {
        CoAPPacket::optionnum = optionnum;
    }

    const CoAPOption* CoAPPacket::getOptions() const {
        return options;
    }

    const uint8_t* CoAPPacket::getBuffer() const {
        return buffer;
    }

    const IPAddress& CoAPPacket::getIp() const {
        return ip;
    }

    void CoAPPacket::setIp(const IPAddress &ip) {
        CoAPPacket::ip = ip;
    }

    int CoAPPacket::getPort() const {
        return port;
    }

    void CoAPPacket::setPort(int port) {
        CoAPPacket::port = port;
    }

    unsigned long CoAPPacket::getTimestamp() const {
        return timestamp;
    }

    void CoAPPacket::setTimestamp(unsigned long timestamp) {
        CoAPPacket::timestamp = timestamp;
    }

    bool CoAPPacket::isRequest(){
        return (code >= REQUEST_CODE_LOWER_BOUND && code <= REQUEST_CODE_UPPER_BOUND);
    }
    bool CoAPPacket::isResponse(){
        return (code >= RESPONSE_CODE_LOWER_BOUND && code <= RESPONSE_CODE_UPPER_BOUND);
    }

    bool CoAPPacket::isEmpty(){
        return (code==0);
    }

    String CoAPPacket::getUriPath() const{
        String url = "";
        for (int i= 0; i< optionnum; i++)
        {
            if (options[i].getNumber() == COAP_URI_PATH && options[i].getLength() > 0) {
                char urlname[options[i].getLength() + 1];
                memcpy(urlname, options[i].getBuffer(), options[i].getLength());
                urlname[options[i].getLength()] = '\0';
                if(url.length() > 0)
                    url += "/";
                url += urlname;
            }
        }
        return url;
    }

    int CoAPPacket::setUriPath(String uri) {
        char* pch = strtok((char *) uri.c_str(), "/");
        while (pch != nullptr){
            CoAPOption path;
            path.setNumber(COAP_URI_PATH);
            path.setLength((uint16_t) strlen(pch));
            sprintf((char *) path.getBuffer(), "%s", pch);
            if(optionnum < MAX_OPTIONS){
                options[optionnum] = path;
                optionnum++;
            } else
                return 1;
            pch = strtok(nullptr, "/");
        }
        return 0;
    }

    String CoAPPacket::getUriQuery() const{
        String query = "";
        for (int i= 0; i< optionnum; i++)
        {
            if (options[i].getNumber() == COAP_URI_QUERY && options[i].getLength() > 0) {
                char urlname[options[i].getLength() + 1];
                memcpy(urlname, options[i].getBuffer(), options[i].getLength());
                urlname[options[i].getLength()] = '\0';
                if(query.length() > 0 && query == "")
                    query += "?";
                else if(query.length() > 0)
                    query += "&";
                query += urlname;
            }
        }
        return query;
    }

    int CoAPPacket::addUriQuery(String q) {

        CoAPOption query;
        query.setNumber(COAP_URI_QUERY);
        query.setLength((uint16_t) q.length());
        sprintf((char *) query.getBuffer(), "%s", q.c_str());
        if(optionnum < MAX_OPTIONS){
            options[optionnum] = query;
            optionnum++;
        } else
            return 1;


        return 0;
    }

    CoAP_CONTENT_TYPE CoAPPacket::getContentType() const{
        for (int i= 0; i< optionnum; i++)
        {
            if (options[i].getNumber() == COAP_CONTENT_FORMAT && options[i].getLength() > 0) {
                CoAP_CONTENT_TYPE type = (CoAP_CONTENT_TYPE) *(options[i].getBuffer());
                return type;
            }
        }
        return COAP_NONE;
    }

    int CoAPPacket::setContentType(CoAP_CONTENT_TYPE t){
        CoAPOption type;
        type.setNumber(COAP_CONTENT_FORMAT);
        type.setLength(sizeof(CoAP_CONTENT_TYPE));
        memcpy((void *) type.getBuffer(), &t, sizeof(CoAP_CONTENT_TYPE));
        if(optionnum < MAX_OPTIONS){
            options[optionnum] = type;
            optionnum++;
        } else
            return 1;
        return 0;
    }

    void CoAPPacket::setPayload(String p){
#ifdef DEBUG
#ifdef NATIVE
        std::cout<<"Payload: "<<p<<std::endl;
#else
        Serial.print("Payload: ");
		Serial.println(p);
#endif
#endif
        uint32_t len = (uint32_t) p.length();
        if(len > MAX_PAYLOAD) len = MAX_PAYLOAD;
#ifdef NATIVE
        strncpy((char *)payload, p.c_str(), len+1);
#else
        p.toCharArray((char*)payload, MAX_PAYLOAD);
#endif
        payloadlen = len;

#ifdef DEBUG
#ifdef NATIVE
        std::cout<<"Payload Edited:: "<<payload<<std::endl;
#else
        Serial.print("Payload Edited: ");
		Serial.println(*payload);
#endif
#endif
    }



    String CoAPPacket::getPayloadAsString() const{
        return String((char *)payload);
    }

    String CoAPPacket::getLocationPathAsString() const{
        String url = "";
        for (int i= 0; i< optionnum; i++)
        {
            if (options[i].getNumber() == COAP_LOCATION_PATH && options[i].getLength() > 0) {
                char urlname[options[i].getLength() + 1];
                memcpy(urlname, options[i].getBuffer(), options[i].getLength());
                urlname[options[i].getLength()] = '\0';
                if(url.length() > 0)
                    url += "/";
                url += urlname;
            }
        }
        return url;
    }

#ifdef DEBUG
    String CoAPPacket::print() const{
	String ret = "";
	switch(type){
		case(CoAP_CON):
			ret += "CON ";
			break;
		case(CoAP_NON):
			ret += "NON ";
			break;
		case(CoAP_ACK):
			ret += "ACK ";
			break;
		case(CoAP_RST):
			ret += "RST ";
			break;
		default:
			ret += "UNSET ";
			break;
	};
    char p[100];

	sprintf(p,"MID: %d, CODE: %d,", messageid, code);
    ret += p;
	if(tokenlen == 0)
		ret += " Token: NULL";
	else{
		//TODO
	}
	if(payloadlen == 0)
			ret += " payload: NULL";
	else{
			ret += " payload: " + getPayloadAsString();
	}
	return ret;

}
#endif

} /* namespace coap */
