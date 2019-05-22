#include "TransactionalWrapper.h"
#include <iostream>

void CTransactionalWrapper::add(TKey key, TValType value) {
    beginTransaction();

    auto tempContainer(container);

    try {
        tempContainer.insert(std::pair<TKey, TValType>(key, value));
        commit(*traJournal.rbegin());
        container = tempContainer;
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
    beginTransaction();

    auto tempContainer(container);

    try {
        tempContainer.erase(key);
        commit(*traJournal.rbegin());
        container = tempContainer;
    }
    catch (std::exception& e) {
        std::cerr << "EXCEPTION: " << e.what();
        rollback(*traJournal.rbegin());
    }
}
