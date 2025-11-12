#include <iostream>
#include "Blockchain.cpp"
#include "Block.hpp"

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage:\n"
                  << "  trustchain submit-file <path> <address>\n"
                  << "  trustchain verify-file <path> <address>\n";
        return 1;
    }

    std::string command = argv[1];
    std::string path = argv[2];
    std::string address = argv[3];

    Blockchain *blockchain = Blockchain::getInstance();

    if (command == "submit-file")
    {
        DocEntry entry(path, address, static_cast<uint64_t>(time(nullptr)), "{}");
        blockchain->addNewBlock(entry);
        std::cout << "File submitted and block added to the blockchain.\n";
    }
    else if (command == "verify-file")
    {
        Block latestBlock = blockchain->getLatestBlock();
        if (latestBlock.entry.verify_doc_hash(path))
        {
            std::cout << "File is verified and matches the hash in the latest block.\n";
        }
        else
        {
            std::cout << "File verification failed. Hash does not match the latest block.\n";
        }
    }
    else
    {
        std::cerr << "Unknown command: " << command << "\n";
        return 1;
    }
}