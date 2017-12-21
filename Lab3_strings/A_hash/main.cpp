#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
string s;
vector<long long> h;
vector<long long> p_step;
unsigned int mod = (1 << 31);//not use

long long hash_ij(int l, int r){ // hash_ij(i, j) - include both
    return (h[r] - h[l-1] * p_step[r - l + 1]);
}
int main() {
   // cout << mod;
    ifstream in("test.in");

    cin >> s;
    int n = s.size(), m;
    cin >> m;

    h.assign(n + 1, 0);
    long long const p = 19;

    p_step.assign(n + 1, 1);

    for(int i = 1; i <= n; ++i){
        p_step[i] = (p * p_step[i - 1]);
    }

    for(int i = 1; i <= n; ++i){
        h[i] = (p*h[i-1] + s[i - 1]);
    }

    int l1, r1, l2, r2;
    for(int i = 0; i < m; ++i){
        cin >> l1 >> r1 >> l2 >> r2;
        cout << (hash_ij(l1, r1) == hash_ij(l2, r2) ? "Yes" : "No") << endl;
    }
    return 0;
}