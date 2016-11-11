//
// Created by Giacomo Tanganelli on 29/10/16.
//

#ifndef COAPESP8266_DEFINES_H
#define COAPESP8266_DEFINES_H

#ifdef NATIVE
#include <iostream>
#include <string>
#include <math.h>
    typedef std::string String;
#else
    #include <Arduino.h>
#endif


#define COAP_HEADER_SIZE 4
#define COAP_OPTION_HEADER_SIZE 1
#define COAP_PAYLOAD_MARKER 0xFF

#define CoAP_DEFAULT_PORT 5683
#define REQUEST_CODE_LOWER_BOUND 1
#define REQUEST_CODE_UPPER_BOUND 31
#define RESPONSE_CODE_LOWER_BOUND 64
#define RESPONSE_CODE_UPPER_BOUND 191


#define BUF_MAX_SIZE 1152
#define MAX_PAYLOAD 1024
#define MAX_TOKEN_LENGTH 8
#define MAX_RESOURCES 5
#define MAX_STORAGE_TRANSACTION 5
#define MAX_OPTIONS 10
#define MAX_OPTION_BUFFER 255


#define ACK_TIMEOUT  2

#define SEPARATE_TIMEOUT ACK_TIMEOUT / 2

#define ACK_RANDOM_FACTOR 1.5

#define MAX_RETRANSMIT 4

#define MAX_TRANSMIT_SPAN ACK_TIMEOUT * (pow(2, (MAX_RETRANSMIT + 1)) - 1) * ACK_RANDOM_FACTOR

#define MAX_LATENCY 120

#define PROCESSING_DELAY ACK_TIMEOUT

#define MAX_RTT (2 * MAX_LATENCY) + PROCESSING_DELAY

#define EXCHANGE_LIFETIME MAX_TRANSMIT_SPAN + (2 * MAX_LATENCY) + PROCESSING_DELAY

#define DISCOVERY_URL "/.well-known/core"

#define ALL_COAP_NODES "224.0.1.187"

#define  ALL_COAP_NODES_IPV6 "FF00::FD"



#define MAX_NON_NOTIFICATIONS 10




#define RESPONSE_CODE(class, detail) ((class << 5) | (detail))
#define COAP_OPTION_DELTA(v, n) (v < 13 ? (*n = (0xFF & v)) : (v <= 0xFF + 13 ? (*n = 13) : (*n = 14)))

typedef enum {
    CoAP_CON = 0,
    CoAP_NON = 1,
    CoAP_ACK = 2,
    CoAP_RST = 3,
    CoAP_UNSET = -1
} CoAP_TYPE;

typedef enum {
    CoAP_GET = 1,
    CoAP_POST = 2,
    CoAP_PUT = 3,
    CoAP_DELETE = 4
} CoAP_METHOD;

typedef enum {
    CoAP_CREATED = RESPONSE_CODE(2, 1),
    CoAP_DELETED = RESPONSE_CODE(2, 2),
    CoAP_VALID = RESPONSE_CODE(2, 3),
    CoAP_CHANGED = RESPONSE_CODE(2, 4),
    CoAP_CONTENT = RESPONSE_CODE(2, 5),
    CoAP_BAD_REQUEST = RESPONSE_CODE(4, 0),
    CoAP_UNAUTHORIZED = RESPONSE_CODE(4, 1),
    CoAP_BAD_OPTION = RESPONSE_CODE(4, 2),
    CoAP_FORBIDDEN = RESPONSE_CODE(4, 3),
    CoAP_NOT_FOUND = RESPONSE_CODE(4, 4),
    CoAP_METHOD_NOT_ALLOWD = RESPONSE_CODE(4, 5),
    CoAP_NOT_ACCEPTABLE = RESPONSE_CODE(4, 6),
    CoAP_PRECONDITION_FAILED = RESPONSE_CODE(4, 12),
    CoAP_REQUEST_ENTITY_TOO_LARGE = RESPONSE_CODE(4, 13),
    CoAP_UNSUPPORTED_CONTENT_FORMAT = RESPONSE_CODE(4, 15),
    CoAP_INTERNAL_SERVER_ERROR = RESPONSE_CODE(5, 0),
    CoAP_NOT_IMPLEMENTED = RESPONSE_CODE(5, 1),
    CoAP_BAD_GATEWAY = RESPONSE_CODE(5, 2),
    CoAP_SERVICE_UNAVALIABLE = RESPONSE_CODE(5, 3),
    CoAP_GATEWAY_TIMEOUT = RESPONSE_CODE(5, 4),
    CoAP_PROXYING_NOT_SUPPORTED = RESPONSE_CODE(5, 5)
} CoAP_RESPONSE_CODE;

typedef enum {
    COAP_IF_MATCH = 1,
    COAP_URI_HOST = 3,
    COAP_E_TAG = 4,
    COAP_IF_NONE_MATCH = 5,
    COAP_URI_PORT = 7,
    COAP_LOCATION_PATH = 8,
    COAP_URI_PATH = 11,
    COAP_CONTENT_FORMAT = 12,
    COAP_MAX_AGE = 14,
    COAP_URI_QUERY = 15,
    COAP_ACCEPT = 17,
    COAP_LOCATION_QUERY = 20,
    COAP_PROXY_URI = 35,
    COAP_PROXY_SCHEME = 39
} CoAP_OPTION_NUMBER;

typedef enum {
    COAP_NONE = -1,
    COAP_TEXT_PLAIN = 0,
    COAP_APPLICATION_LINK_FORMAT = 40,
    COAP_APPLICATION_XML = 41,
    COAP_APPLICATION_OCTET_STREAM = 42,
    COAP_APPLICATION_EXI = 47,
    COAP_APPLICATION_JSON = 50
} CoAP_CONTENT_TYPE;



typedef enum {
    NONE = 0,
    GENERIC = 1,
    NOT_ALLOWED = 2
} ERROR_CODE;

#endif //COAPESP8266_DEFINES_H
