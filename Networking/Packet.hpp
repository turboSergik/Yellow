#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include <cstddef>
#include <cstdint>
#include "../json/json.hpp"

typedef nlohmann::json json;

class Packet {
protected:
    std::unique_ptr<uint8_t[]> message;
    size_t processed;
    // default constructor and uint8_t * constructor 
    // used in receiving packages and should not
    // be used not in PacketQueue
    Packet();
    Packet(const uint8_t *);
    size_t remained();
    void process(size_t numberOfProcessed);
    size_t getProcessed();
    uint8_t * getPtr();
    uint32_t getJsonSize();
    friend class PacketQueue;
public:
    Packet(int code, json j);
    // you don't realy want to copy packets
    // so you should move them
    Packet(const Packet &) = delete;
    Packet(Packet &&);
    Packet & operator=(const Packet &) = delete;
    Packet & operator=(Packet &&);
    json getJson();
    // return Action or result of server depending
    // on sending or reseiving package it is
    int32_t getFlag();
};

#endif // PACKAGE_HPP
