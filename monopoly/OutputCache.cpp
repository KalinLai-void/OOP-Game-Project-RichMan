#include "OutputCache.hpp"
#include <iostream>

TeeBuf::TeeBuf(std::streambuf* buf1, std::streambuf* buf2)
    : dest1(buf1), dest2(buf2) {}

int TeeBuf::overflow(int c) {
    if (c == EOF) return !EOF;

    const int r1 = dest1 ? dest1->sputc(c) : c;
    const int r2 = dest2 ? dest2->sputc(c) : c;
    return (r1 == EOF || r2 == EOF) ? EOF : c;
}

int TeeBuf::sync() {
    int r1 = dest1 ? dest1->pubsync() : 0;
    int r2 = dest2 ? dest2->pubsync() : 0;
    return (r1 == 0 && r2 == 0) ? 0 : -1;
}

// Implementation of OutputCache
OutputCache::OutputCache()
    : originalBuffer(nullptr) {}

OutputCache::~OutputCache() {
    stop();
}

void OutputCache::start() {
    originalBuffer = std::cout.rdbuf();
    std::cout.rdbuf(cacheStream.rdbuf());
}

void OutputCache::stop() {
    if (originalBuffer) {
        std::cout.rdbuf(originalBuffer);
        originalBuffer = nullptr;
    }
}

void OutputCache::clear() {
    cacheStream.str("");
    cacheStream.clear();
}

std::string OutputCache::getCachedOutput() const {
    return cacheStream.str();
}

void OutputCache::flushToStdout() const {
    std::cout << cacheStream.str();
}
