#pragma once


#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

struct STransaction {
    uint32_t id;
};


class CTransactionalWrapper {
public:
    CTransactionalWrapper() = default;
    ~CTransactionalWrapper() = default;

    using TKey = uint32_t;
    using TValType = std::string;


    void add(TKey key, TValType value);
    TValType get(TKey key);
    void remove(TKey key);

private:
    void beginTransaction();
    void commit(STransaction transaction);
    void rollback(STransaction transaction);

    std::unordered_map<TKey, TValType> container;
    std::vector<STransaction> traJournal;
};