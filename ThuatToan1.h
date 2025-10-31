#ifndef THUAT_TOAN_1_H
#define THUAT_TOAN_1_H

#include "HamChung.h"

using namespace std;

// ======== CẤU HÌNH TOÀN CỤC ========
constexpr long long VO_CUNG = numeric_limits<long long>::max() / 4; // an toàn tránh overflow
constexpr array<int,4> HUONG_HANG = {-1, 1, 0, 0};
constexpr array<int,4> HUONG_COT  = {0, 0, -1, 1};

// RNG seed một lần cho toàn chương trình
static std::mt19937_64 RNG((uint64_t)chrono::steady_clock::now().time_since_epoch().count());

// Struct lưu toạ độ (hàng, cột)
struct ToaDo {
    int x, y;
    ToaDo(int _x = 0, int _y = 0)  : x(_x), y(_y) {}
    bool operator==(const ToaDo& other) const { return x == other.x && y == other.y; }
};

inline int idx_flat(int y, int x, int so_cot) {
    return y * so_cot + x;  // y=hang, x=cot
}

// ==================================================================================
// CLASS THUẬT TOÁN 1 - WRAPPER CHO GAME
// ==================================================================================
class ThuatToan1 {

public:
    ThuatToan1() = default;
    ~ThuatToan1() = default;

    // Hàm tính chi phí tối ưu
    pair<long long, vector<ToaDo>> GiaiToanBaiToan(const vector<string>& luoi_input, 
                                                   const ToaDo& diem_bat_dau_game, 
                                                   const ToaDo& diem_ket_thuc_game,
                                                   const vector<ToaDo>& cac_tram_game);
};

// Khai báo các hàm được tách ra từ main()
vector<int> TimDuongBFS_1D(const ToaDo& diem_bat_dau, int so_hang, int so_cot, const vector<string>& luoi);
void InDuongDi(const vector<int>& thu_tu_tham, const ToaDo& diem_xuat_phat, const ToaDo& diem_dich, const vector<ToaDo>& cac_diem_quan_trong);
vector<int> TruyVetDuongDiDP(int so_tram, const vector<vector<long long>>& dp, const vector<vector<int>>& ban_do_khoang_cach, const vector<int>& khoang_cach_den_dich);
long long GiaiBangQuyHoachDongBitmask(int so_tram, const vector<vector<int>>& ban_do_khoang_cach, const vector<int>& khoang_cach_den_dich, vector<vector<long long>>& dp);
long long TinhChiPhiLoTrinh(const vector<int>& lo_trinh, const vector<vector<int>>& ban_do_khoang_cach, const vector<int>& khoang_cach_den_dich);
vector<int> KhoiTaoBangChenDiemXaNhat(int so_diem, const vector<vector<int>>& ban_do_khoang_cach);
void TimKiemCucBo2Opt(vector<int>& lo_trinh, const vector<vector<int>>& ban_do_khoang_cach);
void PhaRoiLoTrinh(vector<int>& lo_trinh);
pair<vector<int>, long long> GiaiBangTimKiemDiaPhuongLap(int so_diem, const vector<vector<int>>& ban_do_khoang_cach, const vector<int>& khoang_cach_den_dich, int so_lan_lap_toi_da);
vector<ToaDo> TimDuongDiChiTiet(const ToaDo& diem_bat_dau, const ToaDo& diem_ket_thuc, 
                                int so_hang, int so_cot, const vector<string>& luoi);


#endif