#include "TransactionalWrapper.h"
#include <algorithm>

uint32_t CTransaction::transactionCntr = 0;

int main() {
    CTransactionalWrapper traHashTable;

    for (uint32_t i = 0; i < 10u; i++) {
        traHashTable.add(i, std::to_string(i));
    }

    traHashTable.print();

    return 0;
}