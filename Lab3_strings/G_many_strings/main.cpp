#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <queue>
#include <set>

using namespace std;

int const maxSizeStr = 1000001; // ?
int n;
int countV = 0; // - vert in bor; +1 because root
//vector<bool> used_term;

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
    //bor[v].next_term = v; //листья-терминалы
    //bor[v].number_str = number_str;
}
int link_f(int v) {
    if (bor[v].link == -2) {
        if (v == 0) {
            bor[v].link = -1;

        } else {
            int par_v = bor[v].p;
            bor[v].p == 0 ? bor[v].link = 0 : bor[v].link = get_to(link_f(par_v), bor[v].c);
            //return bor[v].link;
        }
    }
    return bor[v].link;
    /*  if (bor[v].p == 0) {
          bor[v].link = 0;
          return bor[v].link;
      } else {
          int par_v = bor[v].p;
          bor[v].link = get_to(link_f(par_v), bor[v].c);
          return bor[v].link;
      }*/
}

int get_to(int v, char c) {
   /* if (v == -1) {
        v = 0;
    }*/
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
        if(bor[v].p == 0){
            bor[v].next_term = -1;
        }
        else {
            int link_v = link_f(v);
            /* if (v == 0) {
                 bor[v].next_term = -1;
             } else if (bor[v].flag && get_next_term(link_f(v)) == -1) { //тогда останется она сама
                 bor[v].next_term = v;*/
            if (bor[link_v].flag) {
                bor[v].next_term = link_v;
            } else {
                bor[v].next_term = get_next_term(link_v);
            }
        }
    }
   /* if (bor[v].next_term == -1 && bor[v].flag) {
        bor[v].next_term = v;
    }*/
    return bor[v].next_term;
}

vector<char> inT; //есть ли строка в T

void add_vert_to_inT(vector<int> termin, int number_term) { // нашли терминальные
        for (auto &k: termin) {
            inT[k] = true;
        }
    }


int main() {
    std::ios::sync_with_stdio(false);
    ifstream in("search4.in");
    ofstream out("search4.out");
    in >> n;
    // bor.resize(maxSizeStr + 1); // i don't like it
    bor.resize(1);
    inT.resize(n);
    string cur_str;
    bor[0].p = -1;//?
    bor[0].link = -1;
    bor[0].next_term = -1;
    for (int i = 0; i < n; ++i) {
        in >> cur_str;
        add(cur_str, i);
    }
    //used_term.assign(countV + 1, false);
    //cout << get_next_term(10);
    //  make_links();
    //cout << countV << endl;

    //used.resize(countV + 1);
   /* for (int i = 0; i <= countV; ++i) {
        cout << i << ": " << get_next_term(i) << endl;
    }*/
    string T;
    in >> T;
    int i = 0, v = 0;
    while (i != T.size()) {
       /* if (v == -1) { //если нет начальной
            i++;
            v = 0;
        }*/
        v = get_to(v, T[i]);
        if (bor[v].flag && inT[bor[v].numbers_s[0]]) {
            i++;
            continue;
        }
        int term = v;//cur_next_t = ;
        int cur_next_t = get_next_term(term);
        while (cur_next_t != -1 /*&& term != get_next_term(term)*/) {
            if (bor[term].flag){
                if(inT[bor[term].numbers_s[0]]){
                    break;
                }
                else{
                    add_vert_to_inT(bor[term].numbers_s, term);
                }
            }
           /* if (bor[term].flag && inT[bor[term].numbers_s[0]]) {
                break;
            }
            if (bor[term].flag && !inT[bor[term].numbers_s[0]]) {
                //mark
                add_vert_to_inT(bor[term].numbers_s, term);
            }*/
            term = cur_next_t;
            cur_next_t = get_next_term(term);
        }
        if(bor[term].flag){
            add_vert_to_inT(bor[term].numbers_s, term);
        }
       /* if (term == get_next_term(term)) {
            add_vert_to_inT(bor[term].numbers_s, term);
        }*/
        i++;
    }

    for (auto &k: inT) {
        out << (k ? "YES\n" : "NO\n");
        //cout << k << " ";
    }
    return 0;
}