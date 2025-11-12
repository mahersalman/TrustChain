#include "Block.hpp"
#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>

class Blockchain
{
private:
    static Blockchain *instance;
    const std::string chainFilePath = "/archive/blockchain_data.txt";
    std::vector<Block> chain;

    Blockchain()
    {
        loadChain();
    }

    void loadChain()
    {
        std::ifstream file(chainFilePath);
        if (!file.is_open())
        {
            // Create genesis block if file does not exist
            Block genesisBlock(0, "0", DocEntry("genesis_hash", "uploader1", 0, "{}"));
            chain.push_back(genesisBlock);
            saveChainLocal(genesisBlock);
            return;
        }

        chain.clear();
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream ss(line);
            uint64_t index, timestamp, entryTimestamp;
            std::string previous_hash, docHash, uploaderAddress, metadata, blockHash;

            char delimiter; // to consume commas

            ss >> index >> delimiter >> timestamp >> delimiter >> previous_hash >> delimiter >> docHash >> delimiter >> uploaderAddress >> delimiter >> entryTimestamp >> delimiter >> metadata >> delimiter >> blockHash;

            DocEntry entry(docHash, uploaderAddress, entryTimestamp, metadata);
            Block block(index, previous_hash, entry);
            block.timestamp = timestamp;
            block.blockHash = blockHash;

            chain.push_back(block);
        }
    }

    void saveChainLocal(Block newBlock)
    {
        std::ofstream file("/archive/blockchain_data.txt");
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open blockchain_data.txt for writing");
        }

        file << newBlock.index << ","
             << newBlock.timestamp << ","
             << newBlock.previous_hash << ","
             << newBlock.entry.docHash << ","
             << newBlock.entry.uploaderAddress << ","
             << newBlock.entry.timestamp << ","
             << newBlock.entry.metadata << ","
             << newBlock.blockHash << "\n";

        file.close();
    }

public:
    static Blockchain *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new Blockchain();
        }
        return instance;
    }

    void addNewBlock(DocEntry entry)
    {
        Block new_block(chain.size(), chain.back().blockHash, entry);
        chain.push_back(new_block);
        saveChainLocal(new_block);
    }

    bool checkChainValidity()
    {
        for (size_t i = 1; i < chain.size(); ++i)
        {
            const Block &currentBlock = chain[i];
            const Block &previousBlock = chain[i - 1];

            if (currentBlock.previous_hash != previousBlock.blockHash)
            {
                return false;
            }

            if (currentBlock.blockHash != currentBlock.computeBlockHash())
            {
                return false;
            }
        }
        return true;
    }

    Block getLatestBlock() const
    {
        return chain.back();
    }

    Blockchain(const Blockchain &) = delete;
    Blockchain &operator=(const Blockchain &) = delete;
};