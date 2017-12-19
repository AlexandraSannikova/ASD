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
ifstream in("common.in");
ofstream out("common.out");

//output of answers:
void suffix_array(vector<int> p) {
    for (int i = 2; i < p.size(); ++i) {
        out << p[i] + 1 << " ";
    }
    out << endl;
}

void lcp_out(vector<int> lcp) {
    for (int i = 2; i < lcp.size() - 1; ++i) {
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

int n1, n2;

int number_str(int p) {
    if (p <= n1) {
        return 1;
    }
    return 2;
}

int main() {
    string s1, s2, str;
    vector<int> p; //перестановка
    vector<int> c; // класс эквив-ти
    vector<pair<char, int>> start;
    in >> s1 >> s2;
    n1 = s1.size(), n2 = s2.size();
    str = "";
    str = s1;
    str += "#";
    str += s2;
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
    //suffix_array(p); //without p[0] & p[1]
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
        lcp[i - 1] = count;
    }
    //lcp_out(lcp);
    int common = 0;
    string com_str = "";
    for (int i = 2; i < lcp.size() - 1; ++i) {
        if (lcp[i] > common && number_str(p[i]) != number_str(p[i + 1])) {
            common = lcp[i];
            com_str = str.substr(p[i], lcp[i]);
        }
    }
    out << com_str;
    //count substr
    /*long long k = 0;
    n--; //without $
    for(int i = 1; i <= n; ++i){ //attention! there is p[] include 0..n - 1
        k += n - p[i];//= длина суффикса = кол-во подстрок
        if(i > 1){
            k-= lcp[i - 1]; //кол-во новых подстрок
        }
    }
    out << k;*/

    return 0;
}