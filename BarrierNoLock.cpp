#include <bits/stdc++.h>
using namespace std;

struct SingleBarrierNoLock {
    int maxCnt = 0;
    atomic<int> cnt = 0;
    atomic<bool> raised = false;
    atomic<bool> flag = false;

    SingleBarrierNoLock(int maxCnt): maxCnt(maxCnt) {}

    void arrive_and_wait(function<void()> f1 = [](){}, function<void()> f2 = [](){}) {
        raised.wait(true);

        int c = cnt.fetch_add(1);
        if (c == maxCnt - 1) {
            raised.store(true); // if we store flag first, the some thread can zoom past and join the barrier again
            flag.store(true);
        }
        
        while (!flag.load()); // cant use raised.wait(false) as notification can be lost 

        c = cnt.fetch_sub(1);
        if (c == 1) {
            flag.store(false); // change flag to init value of false first 
            raised.store(false);
            raised.notify_all();
        }
    }

};

int N = 5;
SingleBarrierNoLock bar(N);

vector<int> test(N, 0);

void f1() {
    cout << "all threads arrived" << endl;
}

void f2() {
    cout << "all threads left" << endl;
}

void f(int tid) {
    for (int r = 0; r < 1000000; r++) {
        test[(tid + r) % N]++; // each round, a thread modifies at a different index
        bar.arrive_and_wait(); // comment this out for a negative experiment
    }
}

int main() {
    {
        vector<jthread> ths;
        for (int i = 0; i < N; i++) {
            ths.push_back(std::move(jthread(f, i)));
        }
    }

    for (int n : test) {
        cout << n << " ";
    }
    cout << endl;

}