#ifndef THUAT_TOAN_2_H
#define THUAT_TOAN_2_H

#include "HamChung.h"

struct TrieNode {
    bool DaKetThuc_;
    map<char, TrieNode*> Con_;
    string TenBanDau_;

    TrieNode() : DaKetThuc_(false) {}
    ~TrieNode() {
        for (auto& child : Con_) {
            delete child.second;
        }
    }
};

class TrieGoiY {
public:
    TrieNode* Goc_;
    
    TrieGoiY();
    ~TrieGoiY();
    
    void XoaToanBo();
    void ThemSach(const string& tenSach);
    void XayDungTuDanhSach(const vector<string>& danhSachTenSach, 
                           const map<int,int>& tuiSach);
    
    vector<string> LayGoiY(const string& prefix, int soLuongToiDa = 5);
    
    string ChuanHoa(const string& chuoi) const;
    TrieNode* LayNodePrefix(const string& prefix);
    void DFS(TrieNode* node, string duongDan, vector<string>& ketQua, int& dem, int soLuongToiDa);
};

#endif