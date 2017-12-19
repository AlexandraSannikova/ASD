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
ifstream in("array.in");
ofstream out("array.out");

//output of answers:
void suffix_array(vector<int> p) {
    for (int i = 1; i < p.size(); ++i) {
        out << p[i] + 1 << " ";
    }
    out << endl;
}

void lcp_out(vector<int> lcp) {
    for (int i = 2; i < lcp.size(); ++i) {
        out << lcp[i] << " ";
    }
}

//end of output
bool compare(pair<pair<int, int>, int> x, pair<pair<int, int>, int> y) {
    if (x.first < y.first) {
        return true;
    }
    return false;
}

int main() {
    string str;
    vector<int> p; //перестановка
    vector<int> c; // класс эквив-ти
    vector<pair<char, int>> start;
    in >> str;
    str += "$";
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

    /*multimap<pair<int, int>, int> multi;
    vector<pair<int, int>> c_pair;
    c_pair.resize(n);*/
    /*for(int k = 0; k < log(n); ++k){ // k = 1, log, <= ?
        for(int i = 0; i < n; ++i){ // pairs
            multi.insert( make_pair(make_pair(c[p[i]], c[(p[i] + (1>>k))%n]), p[i]) );
            //multi[make_pair(c[p[i]], c[(p[i] + (1>>k))%n])] =
            //c_pair.emplace_back(make_pair(c[p[i]], c[(p[i] + (1>>k))%n]));
        }
        //sort(c_pair.begin(), c_pair.end()); //долго
        count = 0, cur_c = -1;
        pair<int, int> cur_p = make_pair(-1, -1);
        for(auto it = multi.begin(); it != multi.end(); ++it){
            p[count] = it->second;
            if(it->first == cur_p){
                c[it->second] = cur_c;
            }
            else{
                c[it->second] = ++cur_c;
                cur_p = it->first;
            }
        }

    }*/
    //2^k = 1 << k
    vector<pair<pair<int, int>, int>> c2; // <c1, c2>, p
    for (int k = 0; k <= log2(n); ++k) { // <= вроде < достаточно ?
        //cout << k << ": " << endl;
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
        //check_c(c);
        c2.clear();
        //cout << c2.size()<< endl;
        // check_C2(c2);
    }
    suffix_array(p);
    //lcp
    vector<int> indexP;
    vector<int> lcp;
    indexP.resize(p.size());
    lcp.resize(n);
    for (int i = 0; i < indexP.size(); ++i) {
        indexP[p[i]] = i;
    }
    int count = 0; // count the same symbols in prev step
    for (auto &i: indexP) {
        if (i == 0) {
            continue;
        }
        count = max(count - 1, 0);
        while (str[p[i] + count] == str[p[i - 1] + count]) {
            count++;
        }
        lcp[i] = count;
    }
    lcp_out(lcp);

    return 0;
}