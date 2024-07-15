#pragma once

#include <fstream>
#include <iostream>


// Windows
#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#endif

#include <sstream>

using namespace std;


struct Random {

    
    static constexpr const char hex_chars[] = "0123456789abcdef";



    /**
     * Returns cryptographically secure random bytes on the following 
     * operating systems:
     * 
     * - Windows
     * - Linux
     * - macOS
     */
    static void crypto_random_bytes(char* buffer, int length) {

        #ifdef _WIN32
        HCRYPTPROV hProvider = 0;
        if(!CryptAcquireContext(&hProvider, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
            throw runtime_error("Failed to generate random number");
        }

        if(!CryptGenRandom(hProvider, length, reinterpret_cast<BYTE*>(buffer))) {
            CryptReleaseContext(hProvider, 0);
            throw runtime_error("Failed to generate random number");
        }

        CryptReleaseContext(hProvider, 0);
        #else


        std::ifstream urandom("/dev/urandom", std::ios::in|std::ios::binary);
        if(!urandom) throw runtime_error("Failed to open /dev/urandom");
    

        urandom.read(buffer, length);
        if(!urandom) throw runtime_error("Failed to read from /dev/urandom");

        urandom.close();
        #endif
    }

    /**
     * Generates a crypographically secure random uuid. Complies to UUID 4 standard.
     * 
     * It's just a 128-bit random number represented as a 32-character hex string.
     * 
     * Looks like this: e885fb15-9bf4-c97f-1c0f-14e413a4549d
     * 
     */
    static string uuid() {
        
        // get random array of bytes
        char id[16]; // 128 bits
        Random::crypto_random_bytes(id, 16);

        // Set the version to 4
        id[6] = (id[6] & 0x0F) | 0x40; // clears first 4 bits (& 0x0F) and sets them to '0100'

        // Set the variant to 1 (10xx)
        id[8] = (id[8] & 0x3F) | 0x80; // clears first 2 bits (& 0x3F)  and sets them to '10'


        // convert bytes to hex
        stringstream hex_stream;
        for (int i = 0; i < 16; ++i) {
            // Convert each byte to 2 hex characters
            unsigned char byte = id[i];
            hex_stream << Random::hex_chars[(byte >> 4) & 0x0F] // High 4 bits
                    << Random::hex_chars[byte & 0x0F];       // Low 4 bits

            // Add dashes
            if (i == 3 || i == 5 || i == 7 || i == 9)
                hex_stream << '-';
        }
        

        return hex_stream.str();
    }



};