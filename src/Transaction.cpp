//
// Created by Giacomo Tanganelli on 29/10/16.
//

#include "Transaction.h"

namespace coap {

    Transaction::Transaction(): empty(true), acknowledged(true), timeouted(false), retransmit_after(0), attemps(0) {
        // TODO Auto-generated constructor stub

    }

    Transaction::~Transaction() {

    }

    const CoAPPacket& Transaction::getRequest() const {
        return request;
    }

    void Transaction::setRequest(const CoAPPacket &request) {
        Transaction::request = request;
        empty = false;
    }

    const CoAPPacket& Transaction::getResponse() const {
        return response;
    }

    void Transaction::setResponse(const CoAPPacket &response) {
        Transaction::response = response;
        empty = false;
    }

    bool Transaction::isEmpty() {
        return empty;
    }

    bool Transaction::isAcknowledged() const {
        return acknowledged;
    }

    void Transaction::setAcknowledged(bool acknowledged) {
        Transaction::acknowledged = acknowledged;
    }

    bool Transaction::isTimeouted() const {
        return timeouted;
    }

    void Transaction::setTimeouted(bool timeouted) {
        Transaction::timeouted = timeouted;
    }

    unsigned long Transaction::getRetransmit_after() const {
        return retransmit_after;
    }

    void Transaction::setRetransmit_after(unsigned long retransmit_after) {
        Transaction::retransmit_after = retransmit_after;
    }

    uint8_t Transaction::getAttemps() const {
        return attemps;
    }

    void Transaction::setAttemps(uint8_t attemps) {
        Transaction::attemps = attemps;
    }
} /* namespace coap */