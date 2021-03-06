#pragma once


#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>


class CTransaction {
public:
    enum class EState {
        NOT_SET,
        COMMITTED,
        ROLLED_BACK
    };

    CTransaction();
    ~CTransaction() = default;

    //TODO: move to CTransactionManager or CTraJournal
    static uint32_t transactionCntr;
    
    void setState(EState newState);
    EState getState();
    uint32_t getId();

    friend std::ostream& operator<<(const std::ostream& os, const CTransaction transaction);

private:
    uint32_t id;
    EState state;
};


class CTransactionalWrapper {
public:
    CTransactionalWrapper();
    ~CTransactionalWrapper() = default;

    using TKey = uint32_t;
    using TValType = std::string;

    enum class EState {
        NOT_IN_USE,
        IN_USE
    };

    void add(TKey key, TValType value);
    TValType get(TKey key);
    void remove(TKey key);

    void print() const;

    CTransaction beginTransaction();
    void commit(CTransaction transaction);
    void rollback(CTransaction transaction);

private:
    std::unordered_map<TKey, TValType> container;
    std::vector<CTransaction> traJournal;
    EState currentState;
};