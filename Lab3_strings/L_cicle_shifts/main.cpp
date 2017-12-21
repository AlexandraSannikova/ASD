#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

void check_p(vector<int> p) {
    cout << "P: ";
    for (auto &i : p) {
        cout << i << " ";
    }
    cout << endl;
}

void check_c(vector<int> c) {
    cout << "C: ";
    for (auto &i : c) {
        cout << i << " ";
    }
    cout << endl;
}

void check_C2(vector<pair<pair<int, int>, int>> c2) {
    cout << "C2: ";
    for (int i = 0; i < c2.size(); ++i) {
        cout << c2[i].first.first << " " << c2[i].first.second << " " << c2[i].second << ", ";
    }
    cout << endl;
}

//end of check
ifstream in("shifts.in");
ofstream out("shifts.out");

//output of answers:
void suffix_array(vector<int> p) {
    for (int i = 0; i < p.size(); ++i) {
        out << p[i] + 1 << " ";
    }
    out << endl;
}

//end of output
bool compare(pair<pair<int, int>, int> x, pair<pair<int, int>, int> y) {
    if (x.first < y.first) {
        return true;
    }
    return false;
}

int main() {
    string str; //without $
    vector<int> p; //перестановка
    vector<int> c; // класс эквив-ти
    vector<pair<char, int>> start;
    in >> str;
    int n = str.size();
    p.resize(n);
    c.resize(n);
    //presort
    for (int i = 0; i < n; ++i) {
        start.emplace_back(make_pair(str[i], i));
    }
    sort(start.begin(), start.end());
    int cur_c = 0;
    for (int i = 0; i < start.size(); ++i) {
        p[i] = start[i].second;
        if (i == 0) {
            c[start[i].second] = cur_c;
            continue;
        }
        if (start[i].first == start[i - 1].first) {
            c[start[i].second] = cur_c; //c[count]?
        } else {
            c[start[i].second] = ++cur_c;
        }
    }
    //check_c(p); OK

    //2^k = 1 << k
    vector<pair<pair<int, int>, int>> c2; // <c1, c2>, p
    for (int k = 0; k <= log2(n); ++k) { // <= вроде < достаточно ?
        c2.clear();
        for (int i = 0; i < n; ++i) { // pairs
            c2.emplace_back(make_pair(make_pair(c[p[i]], c[(p[i] + (1 << k)) % n]), p[i]));
        }
        sort(c2.begin(), c2.end(), compare); // must be ok
        cur_c = 0;
        for (int i = 0; i < c2.size(); ++i) { // do new c & p
            p[i] = c2[i].second;
            if (i == 0) {
                c[p[i]] = cur_c;
            } else {
                if (c2[i].first == c2[i - 1].first) {
                    c[p[i]] = cur_c;
                } else {
                    c[p[i]] = ++cur_c;
                }
            }
        }
        //check_C2(c2);
      /////  c2.clear();
    }
    //suffix_array(p);
    //check_c(c); //last
    int pos;
    bool flag = false;
    string ans = "";
    in >> pos;
    int count_shift = 0;
    cur_c = -1;
    for(int i = 0; i < p.size(); ++i){
        if(c[p[i]] != cur_c){
            count_shift++;
            cur_c = c[p[i]];
        }
        else{
            continue;
        }
        if(count_shift == pos){
            flag = true;
            ans = str.substr(p[i], str.size() - p[i]) + str.substr(0, p[i]);
            out << ans;
            break;
        }
    }
    if(!flag){
        out <<"IMPOSSIBLE";
    }

    return 0;
}