#include "crypto.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>
#include <stdexcept>

std::string sha256_file(const std::string &filepath)
{
    // Open file in binary mode
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filepath);

    // Create EVP context
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx)
        throw std::runtime_error("Failed to create EVP_MD_CTX");

    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1)
        throw std::runtime_error("EVP_DigestInit_ex failed");

    // Read file in chunks (e.g., 4KB)
    std::vector<char> buffer(4096);
    while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0)
    {
        if (EVP_DigestUpdate(ctx, buffer.data(), file.gcount()) != 1)
            throw std::runtime_error("EVP_DigestUpdate failed");
    }

    // Finalize hash
    std::vector<unsigned char> hash(EVP_MD_size(EVP_sha256()));
    unsigned int length = 0;
    if (EVP_DigestFinal_ex(ctx, hash.data(), &length) != 1)
        throw std::runtime_error("EVP_DigestFinal_ex failed");

    EVP_MD_CTX_free(ctx);

    // Convert hash to hex string
    std::stringstream ss;
    for (unsigned int i = 0; i < length; ++i)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];

    return ss.str();
}

int main()
{
    try
    {
        std::string fileHash = sha256_file("example.txt");
        std::cout << "SHA-256: " << fileHash << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}