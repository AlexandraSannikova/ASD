#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <queue>

using namespace std;

class ans {
public:
    long long LEN, KOL;
    int begin_i;

    ans(long long k, long long len, int i) : KOL(k), LEN(len), begin_i(i) {}

    void make_else_ans(long long k, long long len, int i) {
        LEN = len;
        KOL = k;
        begin_i = i;

    }
};

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
ifstream in("refrain.in");
ofstream out("refrain.out");

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
    int n, m, ch;
    in >> n >> m;
    string str = "";
    string all_str = "";
    for (int i = 0; i < n; ++i) {
        in >> ch;
        all_str += to_string(ch);
        all_str += " ";
        ch--;
        str += to_string(ch);
    }
    vector<int> p; //перестановка
    vector<int> c; // класс эквив-ти
    vector<pair<char, int>> start;
    str += "$";
    n++;
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
    //suffix_array(p);
    //lcp
    vector<int> indexP;
    vector<int> lcp;
    indexP.resize(p.size());
    lcp.resize(n + 1, 0);//lcp.resize(n);
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
    // lcp_out(lcp);
    n--;
    long long cur_k = 1;
    deque<pair<pair<long long, long long>, int>> q;
    ans answer(1, n, 1); // k, len, i
    ans cur(-1, -1, -1);
    for (int i = 1; i <= n; ++i) {
        if (q.size() == 0 or (lcp[i + 1] > q.back().first.second)) {
            q.push_back(make_pair(make_pair(cur_k, lcp[i + 1]), i));
            continue;
        }
        while (q.size() != 0 and q.back().first.second >= lcp[i + 1]) {
            cur.make_else_ans(q.back().first.first, q.back().first.second, q.back().second);
            q.pop_back();
            cur_k = cur_k + cur.KOL;
            if (answer.KOL * answer.LEN < cur.LEN * cur_k) {
                answer.make_else_ans(cur_k, cur.LEN, cur.begin_i);
            }
        }
        if (q.size() == 0 or q.back().first.second < lcp[i + 1]) {
            q.push_back({{cur_k, lcp[i + 1]}, i});
        }
        cur_k = 1;
    }
    long long a1 = answer.KOL * answer.LEN;
    out << a1 << "\n" << answer.LEN << "\n";
    //cout << answer.KOL<< " " << answer.LEN << " "<< answer.begin_i<< endl;
    if (answer.LEN == n) {
        out << all_str;
        exit(0);
    }
    for (int j = p[answer.begin_i]; j < answer.LEN + p[answer.begin_i]; ++j) {
        char a = str[j];
        if (a == '9') {
            out << "10 ";
        } else {
            out << (char) (a + 1) << " ";
        }
    }

    return 0;
}