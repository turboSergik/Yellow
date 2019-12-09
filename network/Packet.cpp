#include "Packet.hpp"

Packet::Packet(int code, json j) {
    std::string json_string;
    if (j == nullptr) {
        json_string = "";
    } else {
        json_string = j.dump();
    }
    message.reset(new uint8_t[json_string.size() + 8]);
    *reinterpret_cast<int32_t *>(message.get()) = code;
    reinterpret_cast<uint32_t *>(message.get())[1] = 
            static_cast<uint32_t>(json_string.size());
    memcpy(message.get() + 8, json_string.data(), json_string.size());
    processed = 0;
}

Packet::Packet(int code) : message(new uint8_t[8]) {
    *reinterpret_cast<int32_t *>(message.get()) = code;
    reinterpret_cast<uint32_t *>(message.get())[1] = 0;
    processed = 0;
}

Packet::Packet(const Packet & p) : message(new uint8_t[p.getJsonSize() + 8]) {
    memcpy(message.get(), p.message.get(), p.getJsonSize() + 8);
    this->processed = p.processed;
}

Packet::Packet(Packet && rPacket) : 
    message(std::move(rPacket.message)),
    processed(rPacket.processed) {}

Packet & Packet::operator=(const Packet & p) {
    this->~Packet();
    new (this) Packet(p);
    return *this;
}

Packet & Packet::operator=(Packet && rPacket) {
    message = std::move(rPacket.message);
    processed = rPacket.processed;
    return *this;
}

json Packet::getJson() const {
    uint8_t * json_addr = message.get() + 8;
    if (getJsonSize() == 0) {
        return json{};
    } else {
        return json::parse(json_addr, json_addr + getJsonSize());        
    }
}

int32_t Packet::getFlag() const {
    return *reinterpret_cast<int32_t *>(message.get());
}

uint8_t * Packet::getPtr() {
    return message.get() + processed;
}

uint32_t Packet::getJsonSize() const {
    return reinterpret_cast<uint32_t *>(message.get())[1];
}

size_t Packet::remained() {
    return getJsonSize() + 8 - processed;
}

Packet::Packet() : message(new uint8_t[8]), 
                   processed(0)
{}

Packet::Packet(const uint8_t * oldPacket) :
        message(new uint8_t[reinterpret_cast<const uint32_t *>(oldPacket)[1] + 8]) 
{
    memcpy(message.get(), oldPacket, 8);
    processed = 8;
}

size_t Packet::getProcessed() {
    return processed;
}

void Packet::process(size_t numberOfProcessed) {
    processed += numberOfProcessed;
}
