#ifndef OUTPUT_CACHE_HPP
#define OUTPUT_CACHE_HPP

#include <iostream>
#include <sstream>
#include <streambuf>
#include <ostream>

class TeeBuf : public std::streambuf {
    std::streambuf* dest1;
    std::streambuf* dest2;

protected:
    virtual int overflow(int c) override;
    virtual int sync() override;

public:
    TeeBuf(std::streambuf* buf1, std::streambuf* buf2);
};

class OutputCache {
    std::streambuf* originalBuffer;
    std::ostringstream cacheStream;

public:
    OutputCache();
    ~OutputCache();

    void start();
    void stop();
    void clear();

    std::string getCachedOutput() const;
    void flushToStdout() const;
};

#endif // OUTPUT_CACHE_HPP
