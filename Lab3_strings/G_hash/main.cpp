#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
string str;
vector<long long> h;
vector<long long> p_step;
unsigned int mod = (1 << 31);//not use

long long hash_ij(int l, int r){ // hash_ij(i, j) - include both
    return (h[r] - h[l-1] * p_step[r - l + 1]);
}
ifstream in("search4.in");
ofstream out("search4.out");
int main() {
    // cout << mod;
    int k;
    in >> k;
    vector<string> strings;
    string tmp_s;
    for(int i = 0; i < k; ++i){
        in >> tmp_s;
        strings.emplace_back(tmp_s);
    }
    in >> str;
    int n = str.size(), m; //m not need
    //cin >> m;

    h.assign(n + 1, 0);
    long long const p = 3;

    p_step.assign(n + 1, 1);

    for(int i = 1; i <= n; ++i){ //p_step
        p_step[i] = (p * p_step[i - 1]);
    }

    for(int i = 1; i <= n; ++i){ //h[]
        h[i] = (p*h[i-1] + str[i - 1]);
    }
    //for each s
    string s;
    long long hash_s;
    bool flag;
    for(int j = 0; j < k; ++j) {
        flag = false;
        s = strings[j];
        if(s.size() > str.size()){
            out << "NO" << endl;
            continue;
        }
        long long hash_s = 0;
        //long long prev_h = 0;
        for (int i = 1; i <= s.size(); ++i) { //hash(s)
            long long cur = s[i];
            hash_s = p*hash_s + s[i-1];
            //hash_s += prev_h;
            //hash_s += p_step[s.size() - 1 - i] * cur;
        }
        //cout << hash_s << endl;
        for (int i = 1; i + s.size() - 1 <= str.size(); ++i) {
            //cout << hash_ij(i - 1, i + s.size() - 2) << endl;
            if (hash_ij(i, i + s.size() - 1) == hash_s) {
                flag = true;
                break;
            } /*else {
                cout << "no";
            }*/
        }
        out << (flag ? "YES" : "NO") << endl;
    }
    /*int l1, r1, l2, r2;
    for(int i = 0; i < m; ++i){
        cin >> l1 >> r1 >> l2 >> r2;
        cout << (hash_ij(l1, r1) == hash_ij(l2, r2) ? "Yes" : "No") << endl;
    }*/
    return 0;
}