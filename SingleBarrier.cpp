#include <bits/stdc++.h>
using namespace std;


struct SingleBarrier {
    int maxCount;
    atomic<int> cnt = 0;
    counting_semaphore<1000> sem{0};

    SingleBarrier(int maxCount): maxCount(maxCount) {}

    void arrive_and_wait() {
        int c = cnt.fetch_add(1);
        if (c == maxCount - 1) {
            sem.release();
        }

        sem.acquire();
        sem.release();
    }
};

void test(int id, SingleBarrier& sb) {
    printf("T%d arrived\n", id);
    sb.arrive_and_wait();
    printf("T%d leaves\n", id);
   
}

int main() {
    SingleBarrier sb(3);

    jthread t0{test, 0, ref(sb)};
    jthread t1{test, 1, ref(sb)};
    jthread t2{test, 2, ref(sb)};

}