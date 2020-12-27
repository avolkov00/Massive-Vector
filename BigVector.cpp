#include <iostream>
#include <vector>
#include <future>
#include <cstdint>
#include <algorithm>
#include <functional>
#include <cmath>
 
using namespace std;


long int countOneBits(vector<long int>::const_iterator begin, vector<long int>::const_iterator end) {
    long int res = 0;
    for (std::vector<long int>::const_iterator it = begin; it != end; ++it) {
        long int n = *it;
        while (n) {
            ++res;
            n = (n & (n - 1));
        }
    }
    return res;

}

void fullfillVector(vector<long int>& vect) {
    srand(time(0));
    for (long long int i = 0; i < vect.size(); i++) {
        vect[i] = (rand() * i) % 100000;
    }
}

long int accumulate(vector<long int>& vec, long int threads) {
    vector<future<long int>> future_threads;
    long long int res = 0;

    long int part = vec.size() / threads;
    long int remainder = vec.size() % threads;

    auto begin = vec.begin();

    for (long int i = 0; i < threads; ++i) {
        future_threads.emplace_back(async(launch::async, countOneBits, begin, begin +(remainder ? 1 : 0)+ part));
        begin += (remainder ? 1 : 0) + part;
        if (remainder) --remainder;
    }

    for (auto& t : future_threads) {
        res += t.get();
    }

    return res;

}

void howLongItWorks(vector<long int>& vec, long int threads) {
    auto start = chrono::steady_clock::now();
    auto res = accumulate(vec, threads);
    auto end = chrono::steady_clock::now();

    chrono::duration<double> time = end - start;

    cout << threads << " threads work " << time.count() << " seconds and found " << res << " one bites." << endl;
   
}

long int main() {
    vector<long int> randomValues(pow(10, 6));
    fullfillVector(randomValues);

    howLongItWorks(randomValues, 1);

    howLongItWorks(randomValues, 2);

    howLongItWorks(randomValues, 3);

    howLongItWorks(randomValues, 5);

    howLongItWorks(randomValues, 7);

    return 0;
}