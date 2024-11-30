
#include "sha1.h"
#include <cstring>
#include <sstream>
#include <iomanip>
using namespace std;

#define ROTLEFT(a,b) ((a << b) | (a >> (32-b)))

SHA1::SHA1()
    : h0(0x67452301), h1(0xEFCDAB89), h2(0x98BADCFE),
      h3(0x10325476), h4(0xC3D2E1F0), buffer(""), bitlen(0) {}

void SHA1::update(const string& s) 
{
    update(s.c_str(), s.size());
}

void SHA1::update(const char* data, size_t len) 
{
    for (size_t i = 0; i < len; ++i) {
        buffer += data[i];
        if (buffer.size() == 64) {
            transform();
            bitlen += 512;
            buffer.clear();
        }
    }
}

string SHA1::final() 
{
    unsigned int i = buffer.size();

    if (buffer.size() < 56) 
    {
        buffer.push_back(static_cast<char>(0x80));
        while (buffer.size() < 56)
            buffer.push_back(static_cast<char>(0x00));
    }
    else 
    {
        buffer.push_back(static_cast<char>(0x80));
        while (buffer.size() < 64)
            buffer.push_back(static_cast<char>(0x00));
        transform();
        buffer.clear();
        while (buffer.size() < 56)
            buffer.push_back(static_cast<char>(0x00));
    }

    bitlen += i * 8;
    unsigned long long bitlen_be = ((bitlen >> 56) & 0xFF) |
                                   ((bitlen >> 40) & 0xFF00) |
                                   ((bitlen >> 24) & 0xFF0000) |
                                   ((bitlen >> 8)  & 0xFF000000) |
                                   ((bitlen << 8)  & 0xFF00000000) |
                                   ((bitlen << 24) & 0xFF0000000000) |
                                   ((bitlen << 40) & 0xFF000000000000) |
                                   ((bitlen << 56) & 0xFF00000000000000);

    for (int j = 7; j >= 0; --j) {
        buffer.push_back(static_cast<char>((bitlen_be >> (j * 8)) & 0xFF));
    }

    transform();

    ostringstream oss;
    oss << hex << setfill('0');
    oss << setw(8) << h0;
    oss << setw(8) << h1;
    oss << setw(8) << h2;
    oss << setw(8) << h3;
    oss << setw(8) << h4;

    return oss.str();
}

string SHA1::hash(const string& s) {
    SHA1 sha1;
    sha1.update(s);
    return sha1.final();
}

void SHA1::transform() {
    unsigned int a, b, c, d, e, f, k, temp;
    unsigned int m[80];

    for (int i = 0; i < 16; ++i) 
    {
        m[i] = (static_cast<unsigned char>(buffer[i * 4]) << 24) |
               (static_cast<unsigned char>(buffer[i * 4 + 1]) << 16) |
               (static_cast<unsigned char>(buffer[i * 4 + 2]) << 8) |
               (static_cast<unsigned char>(buffer[i * 4 + 3]));
    }
    for (int i = 16; i < 80; ++i) {
        m[i] = ROTLEFT((m[i-3] ^ m[i-8] ^ m[i-14] ^ m[i-16]), 1);
    }

    a = h0;
    b = h1;
    c = h2;
    d = h3;
    e = h4;

    for (int i = 0; i < 80; ++i) 
    {
        if (i < 20) {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999;
        }
        else if (i < 40) 
        {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        }
        else if (i < 60) 
        {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        }
        else 
        {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }
        temp = ROTLEFT(a, 5) + f + e + k + m[i];
        e = d;
        d = c;
        c = ROTLEFT(b, 30);
        b = a;
        a = temp;
    }

    h0 += a;
    h1 += b;
    h2 += c;
    h3 += d;
    h4 += e;
}
