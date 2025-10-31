#include "ThuatToan2.h"



TrieGoiY::TrieGoiY() {
    Goc_ = new TrieNode();
}

TrieGoiY::~TrieGoiY() {
    if (Goc_) {
        delete Goc_;
        Goc_ = nullptr;
    }
}

void TrieGoiY::XoaToanBo() {
    if (Goc_) {
        delete Goc_;
        Goc_ = nullptr;
    }
    Goc_ = new TrieNode(); // Luôn tạo root mới
}

string TrieGoiY::ChuanHoa(const string& chuoi) const {
    string chuanHoa = "";
    for (char kiTu : chuoi) {
        if (isalpha(kiTu)) {
            chuanHoa.push_back(tolower(kiTu));
        }
    }
    return chuanHoa;
}

void TrieGoiY::ThemSach(const string& TenSach) {
    string sachChuanHoa = ChuanHoa(TenSach);
    if (sachChuanHoa.empty() || !Goc_) return;
    
    TrieNode* node = Goc_;
    for (char c : sachChuanHoa) {
        if (node->Con_.find(c) == node->Con_.end()) {
            node->Con_[c] = new TrieNode();            ///////////////////////////////////////////////////////////////
        }
        node = node->Con_[c];
    }
    node->DaKetThuc_ = true;
    node->TenBanDau_ = TenSach;
}

void TrieGoiY::XayDungTuDanhSach(const vector<string>& DanhSachTenSach, 
                                 const map<int,int>& tuiSach) {
    XoaToanBo(); // Xóa dữ liệu cũ
    
    // Chỉ thêm các sách đang có trong túi (số lượng > 0)
    for (const auto& Sach : tuiSach) {
        if (Sach.second > 0 && Sach.first >= 0 && Sach.first < (int)DanhSachTenSach.size()) {
            ThemSach(DanhSachTenSach[Sach.first]);
        }
    }
}

TrieNode* TrieGoiY::LayNodePrefix(const string& prefix) {
    string prefixChuanHoa = ChuanHoa(prefix);
    TrieNode* node = Goc_;
    for (char c : prefixChuanHoa){
        if (!node->Con_[c]) {
            return nullptr; // Không tìm được luôn
        }
        node = node->Con_[c];
        if (!node) return nullptr; 
    }
    return node;
}

void TrieGoiY::DFS(TrieNode* node, string duongDan, vector<string>& ketQua, int& dem, int SoLuongToiDa) {
    if (dem >= SoLuongToiDa) return; // Dừng khi đủ số lượng
    
    if (node->DaKetThuc_) {
        ketQua.push_back(node->TenBanDau_);
        dem++;
    }
    
    if (dem >= SoLuongToiDa) return;
    
    for (const auto& Nhanh : node->Con_) {
        if(Nhanh.second){
            DFS(Nhanh.second, duongDan + Nhanh.first, ketQua, dem, SoLuongToiDa);
            if (dem >= SoLuongToiDa) break;
        }  
    }
}

vector<string> TrieGoiY::LayGoiY(const string& prefix, int SoLuongToiDa) {
    vector<string> KetQua;
    string prefixChuanHoa = ChuanHoa(prefix);
    
    if (prefixChuanHoa.empty()) return KetQua;
    
    TrieNode* NodePrefix = LayNodePrefix(prefixChuanHoa);
    if (!NodePrefix) return KetQua;
    
    int dem = 0;
    DFS(NodePrefix, prefixChuanHoa, KetQua, dem, SoLuongToiDa);
    return KetQua;
}