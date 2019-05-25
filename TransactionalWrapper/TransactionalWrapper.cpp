#include "TransactionalWrapper.h"
#include <iostream>


//CTransactionalWrapper=============================================================
void CTransactionalWrapper::add(TKey key, TValType value) {

    try {
        auto tempContainer(container);
        tempContainer.insert(std::pair<TKey, TValType>(key, value));
    }
    catch (std::exception& e) {
        std::cerr << "EXCEPTION: " << e.what();
        rollback(*traJournal.rbegin());
    }

}


CTransactionalWrapper::TValType CTransactionalWrapper::get(TKey key) {
    return container.find(key)->second;
}


void CTransactionalWrapper::remove(TKey key) {

    try {
        auto tempContainer(container);
        tempContainer.erase(key);
        commit(*traJournal.rbegin());
    }
    catch (std::exception& e) {
        std::cerr << "EXCEPTION: " << e.what();
        rollback(*traJournal.rbegin());
    }
}

void CTransactionalWrapper::print() const {
    for (const auto& keyVal : container) {
        std::cout << keyVal.first << " " << keyVal.second << "\n";
    }

    std::cout << "\n";
}


CTransaction CTransactionalWrapper::beginTransaction() {
    CTransaction newTransaction;
    traJournal.push_back(newTransaction);
    currentState = EState::IN_USE;

    return newTransaction;
}


void CTransactionalWrapper::commit(CTransaction transaction) {
    traJournal.rbegin()->setState(CTransaction::EState::COMMITTED);
    currentState = EState::NOT_IN_USE;
}


void CTransactionalWrapper::rollback(CTransaction transaction) {
    traJournal.rbegin()->setState(CTransaction::EState::ROLLED_BACK);
    currentState = EState::NOT_IN_USE;
}



//CTransaction===============================================================
CTransaction::CTransaction() {
    id = transactionCntr;
    state = EState::NOT_SET;
}


void CTransaction::setState(EState newState) {
    state = newState;
}


CTransaction::EState CTransaction::getState() {
    return state;
}

uint32_t CTransaction::getId() {
    return id;
}

std::ostream& operator<<(std::ostream& os, CTransaction transaction) {
    os << transaction.getId() << " " <<
        static_cast<uint32_t>(transaction.getState()) << "\n";

    return os;
}
