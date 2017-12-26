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
    int left = -1, right = -1, size = -1; // глубина вершины

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
            bor[countV].p = v;
            bor[countV].c = s[i];
            bor[countV].size = i + 1;
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

int main() {
    std::ios::sync_with_stdio(false);
    ifstream in("search6.in");
    ofstream out("search6.out");
    in >> n;
    bor.resize(1);
    string cur_str;
    bor[0].p = -1;
    bor[0].link = -1;
    bor[0].next_term = -1;
    for (int i = 0; i < n; ++i) {
        in >> cur_str;
        add(cur_str, i);
    }
    string T;
    in >> T;
    int i = 0, v = 0;
    vector<bool> use;
    use.resize(countV + 1, false); //если была в проходе по Т
    while (i != T.size()) {
        v = get_to(v, T[i]);
        use[v] = true;
        if (bor[v].left == -1) {
            bor[v].left = i;
        }
        bor[v].right = i; //правее всегда больше
        i++;
    }
    //проверка вхождений(не конечных)
    /* for (int l = 1; l <= countV; ++l) {
      cout << bor[l].left << " " << bor[l].right << endl;
    }*/
    for (int u = 1; u <= countV; ++u) {
        if (use[u]) {
            int term = u;
            int cur_next_t = get_next_term(term);
            while (cur_next_t != -1) {
                if (bor[term].flag && term != u) {//change
                    if (bor[term].left > bor[u].left || bor[term].left == -1) {
                        bor[term].left = bor[u].left;
                    }
                    if (bor[term].right < bor[u].right || bor[term].left == -1) {
                        bor[term].right = bor[u].right;
                    }
                }
                term = cur_next_t;
                cur_next_t = get_next_term(term);
            }
            if (bor[term].flag && term != u) {
                if (bor[term].left > bor[u].left || bor[term].left == -1) {
                    bor[term].left = bor[u].left;
                }
                if (bor[term].right < bor[u].right || bor[term].right == -1) {
                    bor[term].right = bor[u].right;
                }
            }
        }
    }
    vector<pair<int, int>> ans;
    ans.resize(n, {-1, -1});
    for (int j = 1; j <= countV; ++j) {
        if (bor[j].left == -1 && bor[j].right == -1) {
            continue;
        }
        pair<int, int> cur = make_pair(bor[j].left - bor[j].size + 1, bor[j].right - bor[j].size + 1);
        for (auto one : bor[j].numbers_s) {
            ans[one].first = cur.first;
            ans[one].second = cur.second;
        }
    }

    for (auto &k: ans) {
        out << k.first << " " << k.second << '\n';
    }
    return 0;
}