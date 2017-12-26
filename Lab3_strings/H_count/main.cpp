#include <iostream>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

int n;
int countV = 0; // - vert in bor; +1 because root

class vertex {
public:
    bool flag = false;
    map<char, int> to;
    int p; //parent
    char c;
    int link = -2;
    vector<int> numbers_s;
    int next_term = -2;

    vertex() {}
};

vector<vertex> bor;

int get_to(int v, char c);

void add(string s, int number_str) {
    int v = 0;
    for (int i = 0; i < s.size(); ++i) {
        if (bor[v].to.count(s[i])) {
            v = bor[v].to[s[i]];
        } else {
            bor[v].to[s[i]] = ++countV;
            vertex cur = vertex();
            bor.emplace_back(cur);
            bor[countV].p = v;// поставили parent
            bor[countV].c = s[i];
            v = countV;
        }
    }
    bor[v].flag = true;
    bor[v].numbers_s.emplace_back(number_str);//строки, оканч в этой вершине
}

int link_f(int v) {
    if (bor[v].link == -2) {
        if (v == 0) {
            bor[v].link = -1;

        } else {
            int par_v = bor[v].p;
            bor[v].p == 0 ? bor[v].link = 0 : bor[v].link = get_to(link_f(par_v), bor[v].c);
        }
    }
    return bor[v].link;
}

int get_to(int v, char c) {
    if (!bor[v].to.count(c)) {
        if (v == 0) {
            bor[v].to[c] = 0;
        } else {
            bor[v].to[c] = get_to(link_f(v), c);
        }
    }
    return bor[v].to[c];
}

int get_next_term(int v) {
    if (bor[v].next_term == -2) {//если не посчитана
        if (bor[v].p == 0) {
            bor[v].next_term = -1;
        } else {
            int link_v = link_f(v);
            if (bor[link_v].flag) {
                bor[v].next_term = link_v;
            } else {
                bor[v].next_term = get_next_term(link_v);
            }
        }
    }
    return bor[v].next_term;
}

vector<int> inT; //сколько раз строка встречается в T

int main() {
    std::ios::sync_with_stdio(false);
    ifstream in("search5.in");
    ofstream out("search5.out");
    in >> n;
    bor.resize(1);
    inT.resize(n, 0);
    string cur_str;
    bor[0].p = -1;
    bor[0].link = -1;
    bor[0].next_term = -1;
    for (int i = 0; i < n; ++i) {
        in >> cur_str;
        add(cur_str, i);
    }
    //check
    /* for (int i = 0; i <= countV; ++i) {
         cout << i << ": " << get_next_term(i) << endl;
     }*/
    vector<int> wasHere, ans_count;
    wasHere.resize(countV + 1, 0); //в каких ВЕРШИНАХ были, проходясь по Т
    ans_count.resize(countV + 1, 0); // реально сколько в итоге в каждой вершине "были"(с суф ссылками)

    string T;
    in >> T;
    int i = 0, v = 0;
    while (i != T.size()) {
        v = get_to(v, T[i]);
        wasHere[v]++;
        if (bor[v].flag) {
            ans_count[v]++;
        }
        i++;
    }
    for (int u = 1; u < wasHere.size(); ++u) {
        if (wasHere[u] != 0) {
            int term = u;
            int cur_next_t = get_next_term(term);
            while (cur_next_t != -1) {
                if (bor[term].flag && term != u) {
                    ans_count[term] += wasHere[u];
                }
                term = cur_next_t;
                cur_next_t = get_next_term(term);

            }
            if (bor[term].flag && term != u) {
                ans_count[term] += wasHere[u];
            }
        }
    }
    for (int j = 0; j < ans_count.size(); ++j) {
        if (ans_count[j] != 0) {
            for (auto one : bor[j].numbers_s) {
                inT[one] = ans_count[j];
            }
        }
    }
    for (auto &k: inT) {
        out << k << '\n';
    }
    return 0;
}