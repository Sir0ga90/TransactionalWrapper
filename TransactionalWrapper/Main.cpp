#include "TransactionalWrapper.h"
#include <thread>
#include <future>

uint32_t CTransaction::transactionCntr = 0;

void testTransactionWrapper();



int main() {
    testTransactionWrapper();
    return 0;
}



void testTransactionWrapper() {
    CTransactionalWrapper traHashTable;

    std::future<CTransaction> tra1 =
        std::async(&CTransactionalWrapper::beginTransaction, &traHashTable);

    traHashTable.add(3, std::string("some_string"));    //dummy vals
    traHashTable.commit(tra1.get());

    traHashTable.print();

}
