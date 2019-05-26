#include "TransactionalWrapper.h"
#include <thread>
#include <iomanip>

uint32_t CTransaction::transactionCntr = 0;

void testTransactionWrapper(CTransactionalWrapper& traHashTable,
                            uint32_t idx);

void check100Times();

int main() {

    check100Times();

    return 0;
}



void testTransactionWrapper(CTransactionalWrapper& traHashTable,
                            uint32_t idx) {
    CTransaction tra1 = traHashTable.beginTransaction();

    traHashTable.add(idx, std::to_string(idx));    //dummy vals
    traHashTable.commit(tra1);
}


void check100Times() {
    constexpr auto MAX_THREAD_NUM = 2u;
    CTransactionalWrapper traHashTable;
    std::vector<std::thread> testThreads;

    for (auto i = 0u; i < 100; ++i)
    {
        for (auto i = 0u; i < MAX_THREAD_NUM; i++) {
            testThreads.push_back(std::thread(testTransactionWrapper,
                                              std::ref(traHashTable),
                                              i));
        }

        for (auto& thread : testThreads) {
            thread.join();
        }

        traHashTable.print();
        testThreads.clear();

        std::cout << std::setfill('=') << std::setw(50) << "\n";
    }
    
}
