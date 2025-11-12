#include <iostream>
#include <string>
#include <crypto.hpp>
#include <fstream>

struct DocEntry
{
    std::string docHash;
    std::string uploaderAddress;
    uint64_t timestamp;
    std::string metadata;

    DocEntry(const std::string &file_name, const std::string &uploader, uint64_t time, const std::string &meta)
        : uploaderAddress(uploader), timestamp(time), metadata(meta)
    {
        docHash = compute_doc_hash(file_name);
    }

    std::string compute_doc_hash(const std::string &file_name) const
    {
        return sha256_file(file_name);
    }

    bool verify_doc_hash(const std::string &file_name) const
    {
        return docHash == compute_doc_hash(file_name);
    }
};

struct Block
{
    uint64_t index;
    uint64_t timestamp;
    std::string previous_hash; // Hex
    // std::string merkleRoot;    // Hex,root of DocEntry hashes
    DocEntry entry;
    // uint64_t nonce;        // optinal (for PoW)
    std::string blockHash; // hex SHA-256 of block header
    // std::string signerPublicKey; // who signed the block
    // std::string signature;       // optinal ECDSA signature

    Block(uint64_t idx, std::string prev_hash, DocEntry entry)
        : index(idx), previous_hash(prev_hash), entry(entry)
    {
        timestamp = static_cast<uint64_t>(time(nullptr));
        blockHash = computeBlockHash();
    }

    std::string computeBlockHash() const
    {
        std::string data = std::to_string(index) + "$" +
                           std::to_string(timestamp) + "$" +
                           previous_hash + "$" +
                           entry.docHash + "$" +
                           entry.uploaderAddress + "$" +
                           std::to_string(entry.timestamp) + "$" +
                           entry.metadata;
        return sha256_string(data);
    }
};
