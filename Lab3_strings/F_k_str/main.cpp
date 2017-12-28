#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
vector<long long> p_step;

class Strings {
public:
    string str;
    vector<long long> h; //hashs of prefics of str
    Strings(string s) : str(s) {}
};

vector<Strings> strs;

long long hash_ij(int l, int r, int num_str) { // hash_ij(i, j) - include both; строка тут считается с 1
    return (strs[num_str].h[r] - strs[num_str].h[l - 1] * p_step[r - l + 1]);
}

void check_hashs(int k) { //h of strs
    for (int j = 0; j < k; ++j) {
        for (int i = 0; i <= strs[j].str.size(); ++i) {
            cout << strs[j].h[i] << " ";
        }
        cout << endl;
    }
}

int main() {
    ifstream in("substr3.in");
    ofstream out("substr3.out");
    int k, minLen = 10000, maxLen, id = 0; //k - count of strs; id - i of min(size) str
    maxLen = 0;
    string cur;
    in >> k;
    for (int i = 0; i < k; ++i) {
        in >> cur;
        Strings curS = Strings(cur);
        strs.push_back(curS);
        if (cur.size() < minLen) {
            minLen = cur.size();
            id = i;
        }
        if (cur.size() > maxLen) {
            maxLen = cur.size();
        }
    }
    long long const p = 31;

    p_step.assign(maxLen + 1, 1);
    for (int i = 1; i <= maxLen; ++i) {
        p_step[i] = (p * p_step[i - 1]);
    }
    for (int j = 0; j < k; ++j) {
        strs[j].h.assign(strs[j].str.size() + 1, 0);
        for (int i = 1; i <= strs[j].str.size(); ++i) {
            strs[j].h[i] = (p * strs[j].h[i - 1] + strs[j].str[i - 1]);
        }
    }
    //check_hashs(k);
    string minStr = strs[id].str; //минимальная строка
    int mid = 0, l = -1, r = minStr.size() + 1;
    int count = 0;
    string ans_s = "", curr;
    while (l < r - 1) {
        mid = (l + r) / 2;
        for (int i = 0; i <= minStr.size() - mid; ++i) {// для всех подстрок minStr размера mid
            count = 0;
            curr = minStr.substr(i, mid);
            for (int k_ = 0; k_ < k; ++k_) { //для каждой строки из strs
                if (k_ == id) {
                    continue;
                }
                bool yes = false; //если есть такая подстрока в k_
                for (int y = 0; y <= strs[k_].str.size() - mid; ++y) { // все подстроки строки k_ размера mid
                    if (hash_ij(i + 1, i + mid, id) == hash_ij(y + 1, y + mid, k_)) {
                        yes = true;
                        break;//если уже нашли такую подстроку в k_
                    }
                }
                if (yes) {
                    count++;
                } else {
                    count = 0;
                    break;
                }
            }
            if (count == k - 1) { //во всех строках есть такая подстрока
                ans_s = curr;
                break;
            }
        }
        if (count == k - 1) {
            ans_s = curr; //можно и убрать
            l = mid;
        } else {
            r = mid;
        }
    }
    out << ans_s;
    return 0;
}