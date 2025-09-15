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

int N = 5;
vector<int> test(N, 0);
Barrier bar(N);

void f(int tid) {
    for (int r = 0; r < 1000000; r++) {
        test[(tid + r) % N]++;
        bar.arrive_and_wait();
    }
}

int main() {
    {
        vector<jthread> ths;
        for (int i = 0; i < N; i++) {
            ths.push_back(std::move(jthread(f, i)));
        }
    }

    for (int n : test) cout << n << " ";
    cout << endl;


}