#include <bits/stdc++.h>
#include <functional>
using namespace std;

// to prevent a thread from overtaking by 1 lap,
// invariant: next thread can enter only when previous lap threads all leave
// this invariant must be maintained

struct Barrier {
    int maxCount;
    atomic<int> cnt = 0;
    atomic<bool> flag = false; // if flag is true, means this round is completed
    counting_semaphore<1000> sem{0};

    Barrier(int maxCount): maxCount(maxCount) {}

    void arrive_and_wait(function<void()> f = [] () {}) {
        flag.wait(true);
        
        int c = cnt.fetch_add(1);
        if (c == maxCount - 1) {
            flag.store(true);
            sem.release();
        }

        // when a thread manage to leaves, flag == true
        sem.acquire();
        sem.release();

        int c2 = cnt.fetch_sub(1);
        if (c2 == 1) {
            f();
            sem.acquire(); // reset sem to its original state first
            flag.store(false);
            flag.notify_all();
            
        }
    }
};

void check(vector<int>& arr) {
    printf("%d %d\n", arr[0], arr[1]);
}

void test(int id, vector<int>& arr, Barrier& b) {
    for (int i = 0; i < 5; i++) {
        //this_thread::sleep_for(chrono::milliseconds(100 * id));
        b.arrive_and_wait();
        printf("T%d leaves\n", id);
    }
   
}



int main() {
    int len = 4;
    Barrier b(len);
    vector<int> arr(len, 0);

    jthread t0{test, 0, ref(arr), ref(b)};
    jthread t1{test, 1, ref(arr), ref(b)};
    jthread t2{test, 2, ref(arr), ref(b)};
    jthread t3{test, 3, ref(arr), ref(b)};

}