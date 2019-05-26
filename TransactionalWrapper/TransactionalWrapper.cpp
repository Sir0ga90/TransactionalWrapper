#include "TransactionalWrapper.h"
#include <iostream>


CTransactionalWrapper::CTransactionalWrapper() :
    currentState(EState::NOT_IN_USE) {}

//CTransactionalWrapper=============================================================
void CTransactionalWrapper::add(TKey key, TValType value) {

    if (currentState == EState::IN_USE) {
        try {
            auto tempContainer(container);
            auto newElement = std::pair<TKey, TValType>(key, value);

            tempContainer.insert(newElement);

            //TODO: not the best place for processing main container according to transactions
            container.insert(newElement);
        }
        catch (std::exception& e) {
            std::cerr << "EXCEPTION: " << e.what();
            rollback(*traJournal.rbegin());
        }
    }

}


CTransactionalWrapper::TValType CTransactionalWrapper::get(TKey key) {
    return container.find(key)->second;
}


void CTransactionalWrapper::remove(TKey key) {

    try {
        auto tempContainer(container);
        tempContainer.erase(key);

        //TODO: not the best place for processing main container according to transactions
        container.erase(key);
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

    if (currentState == EState::NOT_IN_USE) {
        traJournal.push_back(newTransaction);
        currentState = EState::IN_USE;
    }

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
