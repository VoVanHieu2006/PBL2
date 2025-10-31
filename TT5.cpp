#include <bits/stdc++.h>
using namespace std;

// ======== CẤU HÌNH TOÀN CỤC ========
constexpr long long VO_CUNG = numeric_limits<long long>::max() / 4; // tránh overflow
constexpr array<int,4> HUONG_HANG = {-1, 1, 0, 0};
constexpr array<int,4> HUONG_COT  = {0, 0, -1, 1};

// Ngưỡng: nếu số trạm < NGUONG_DP thì dùng DP bitmask, ngược lại dùng ILS
constexpr int NGUONG_DP = 20;

// RNG seed một lần cho toàn chương trình (dùng trong ILS/pha rối)
static std::mt19937_64 RNG((uint64_t)chrono::steady_clock::now().time_since_epoch().count());

// Struct lưu toạ độ (hàng, cột)
struct ToaDo {
    int hang;
    int cot;
};

inline int idx_flat(int hang, int cot, int so_cot) {
    return hang * so_cot + cot;
}

// ==================================================================================
// BFS (1D representation): trả về vector<int> kích thước R*C chứa khoảng cách, -1 nếu không tới được
// ==================================================================================
vector<int> TimDuongBFS_1D(const ToaDo& diem_bat_dau, int so_hang, int so_cot, const vector<string>& luoi) {
    auto hop_le = [&](int r, int c){
        if (r < 0 || r >= so_hang || c < 0 || c >= so_cot) return false;
        return luoi[r][c] != '#'; // '#' là tường/không đi được
    };

    vector<int> ma_tran_khoang_cach(so_hang * so_cot, -1);
    if (!hop_le(diem_bat_dau.hang, diem_bat_dau.cot)) return ma_tran_khoang_cach;

    deque<int> hang_doi;
    int start = idx_flat(diem_bat_dau.hang, diem_bat_dau.cot, so_cot);
    ma_tran_khoang_cach[start] = 0;
    hang_doi.push_back(start);

    while (!hang_doi.empty()) {
        int cur = hang_doi.front(); hang_doi.pop_front();
        int r = cur / so_cot;
        int c = cur % so_cot;
        int dcur = ma_tran_khoang_cach[cur];
        for (int k = 0; k < 4; ++k) {
            int nr = r + HUONG_HANG[k];
            int nc = c + HUONG_COT[k];
            if (!hop_le(nr, nc)) continue;
            int ni = idx_flat(nr, nc, so_cot);
            if (ma_tran_khoang_cach[ni] == -1) {
                ma_tran_khoang_cach[ni] = dcur + 1;
                hang_doi.push_back(ni);
            }
        }
    }
    return ma_tran_khoang_cach;
}

// ==================================================================================
// In đường đi (hiển thị theo thứ tự các điểm quan trọng)
// - thu_tu_tham: vector chứa chỉ số các điểm (theo chỉ số trong cac_diem_quan_trong)
// ==================================================================================
void InDuongDi(const vector<int>& thu_tu_tham, const ToaDo& diem_xuat_phat, const ToaDo& diem_dich,
               const vector<ToaDo>& cac_diem_quan_trong) {
    cout << "Xuat phat (" << diem_xuat_phat.hang << "," << diem_xuat_phat.cot << ")";
    for (int chi_so : thu_tu_tham) {
        if (chi_so >= 0 && chi_so < (int)cac_diem_quan_trong.size()) {
            cout << " -> Tram " << chi_so << " (" 
                 << cac_diem_quan_trong[chi_so].hang << "," 
                 << cac_diem_quan_trong[chi_so].cot << ")";
        }
    }
    cout << " -> Dich (" << diem_dich.hang << "," << diem_dich.cot << ")\n";
}

// ==================================================================================
// PHƯƠNG PHÁP 1: QUY HOẠCH ĐỘNG (BITMASK)
// ==================================================================================
vector<int> TruyVetDuongDiDP(int so_tram, 
                              const vector<vector<long long>>& dp,
                              const vector<vector<int>>& ban_do_khoang_cach,
                              const vector<int>& khoang_cach_den_dich) {
    vector<int> thu_tu_tham;
    if (so_tram == 0) return thu_tu_tham;

    int mask_cuoi_cung = (1 << so_tram) - 1;

    int tram_ket_thuc = -1;
    long long chi_phi_tot_nhat = VO_CUNG;
    for (int i = 0; i < so_tram; ++i) {
        if (dp[mask_cuoi_cung][i] != VO_CUNG) {
            long long chi_phi_tong = dp[mask_cuoi_cung][i] + (long long)khoang_cach_den_dich[i + 1];
            if (chi_phi_tong < chi_phi_tot_nhat) {
                chi_phi_tot_nhat = chi_phi_tong;
                tram_ket_thuc = i;
            }
        }
    }

    if (tram_ket_thuc == -1) return thu_tu_tham;

    int mask_hien_tai = mask_cuoi_cung;
    int tram_hien_tai = tram_ket_thuc;

    while (mask_hien_tai > 0) {
        // Lưu chỉ số tương ứng trong cac_diem_quan_trong (0 = start, 1.. = tram)
        thu_tu_tham.push_back(tram_hien_tai + 1);

        if ((__builtin_popcount(mask_hien_tai)) == 1) break;

        int mask_truoc = mask_hien_tai ^ (1 << tram_hien_tai);
        int tram_truoc = -1;

        for (int j = 0; j < so_tram; ++j) {
            if ((mask_truoc >> j) & 1) {
                long long chi_phi = dp[mask_truoc][j] + (long long)ban_do_khoang_cach[j + 1][tram_hien_tai + 1];
                if (chi_phi == dp[mask_hien_tai][tram_hien_tai]) {
                    tram_truoc = j;
                    break;
                }
            }
        }

        if (tram_truoc == -1) break;
        mask_hien_tai = mask_truoc;
        tram_hien_tai = tram_truoc;
    }

    reverse(thu_tu_tham.begin(), thu_tu_tham.end());
    return thu_tu_tham;
}

long long GiaiBangQuyHoachDongBitmask(int so_tram, 
                                       const vector<vector<int>>& ban_do_khoang_cach, 
                                       const vector<int>& khoang_cach_den_dich,
                                       vector<vector<long long>>& dp) {
    if (so_tram == 0) return (long long)khoang_cach_den_dich[0];

    dp.assign(1 << so_tram, vector<long long>(so_tram, VO_CUNG));

    for (int i = 0; i < so_tram; ++i) {
        dp[1 << i][i] = (long long)ban_do_khoang_cach[0][i + 1];
    }

    for (int mask = 1; mask < (1 << so_tram); ++mask) {
        for (int i = 0; i < so_tram; ++i) {
            if ((mask >> i) & 1) {
                int mask_truoc = mask ^ (1 << i);
                if (mask_truoc == 0) continue;
                for (int j = 0; j < so_tram; ++j) {
                    if ((mask_truoc >> j) & 1) {
                        if (dp[mask_truoc][j] != VO_CUNG) {
                            long long can_nhap = dp[mask_truoc][j] + (long long)ban_do_khoang_cach[j + 1][i + 1];
                            if (can_nhap < dp[mask][i]) dp[mask][i] = can_nhap;
                        }
                    }
                }
            }
        }
    }

    long long chi_phi_toi_uu = VO_CUNG;
    int mask_cuoi_cung = (1 << so_tram) - 1;
    for (int i = 0; i < so_tram; ++i) {
        if (dp[mask_cuoi_cung][i] != VO_CUNG) {
            chi_phi_toi_uu = min(chi_phi_toi_uu, dp[mask_cuoi_cung][i] + (long long)khoang_cach_den_dich[i + 1]);
        }
    }
    return chi_phi_toi_uu;
}

// ==================================================================================
// PHƯƠNG PHÁP 2: HEURISTIC ILS (Iterated Local Search) với 2-opt và một số thao tác pha rối
// ==================================================================================
long long TinhChiPhiLoTrinh(const vector<int>& lo_trinh, 
                             const vector<vector<int>>& ban_do_khoang_cach, 
                             const vector<int>& khoang_cach_den_dich) {
    if (lo_trinh.empty()) return (long long)khoang_cach_den_dich[0];

    long long cost = 0;
    cost += (long long)ban_do_khoang_cach[0][lo_trinh[0]];
    for (size_t i = 0; i + 1 < lo_trinh.size(); ++i) {
        cost += (long long)ban_do_khoang_cach[lo_trinh[i]][lo_trinh[i + 1]];
    }
    cost += (long long)khoang_cach_den_dich[lo_trinh.back()];
    return cost;
}

vector<int> KhoiTaoBangChenDiemXaNhat(int so_diem, const vector<vector<int>>& ban_do_khoang_cach) {
    if (so_diem <= 1) return {};
    if (so_diem == 2) return {1};

    vector<int> tour;
    tour.reserve(so_diem - 1);
    tour.push_back(1);
    vector<char> da_tham(so_diem, false);
    da_tham[0] = true;
    da_tham[1] = true;

    while ((int)tour.size() < so_diem - 1) {
        int diem_xa_nhat = -1;
        int khoang_cach_xa_nhat = -1;
        for (int k = 1; k < so_diem; ++k) {
            if (!da_tham[k]) {
                int kc_ngan_nhat_den_tour = INT_MAX;
                for (int node : tour) {
                    kc_ngan_nhat_den_tour = min(kc_ngan_nhat_den_tour, ban_do_khoang_cach[k][node]);
                }
                if (kc_ngan_nhat_den_tour > khoang_cach_xa_nhat) {
                    khoang_cach_xa_nhat = kc_ngan_nhat_den_tour;
                    diem_xa_nhat = k;
                }
            }
        }

        int vi_tri_chen_tot_nhat = 0;
        long long chi_phi_tang_min = VO_CUNG;

        long long chi_phi_dau = (long long)ban_do_khoang_cach[0][diem_xa_nhat] + ban_do_khoang_cach[diem_xa_nhat][tour[0]] - ban_do_khoang_cach[0][tour[0]];
        chi_phi_tang_min = chi_phi_dau;
        vi_tri_chen_tot_nhat = 0;

        for (size_t i = 0; i + 1 < tour.size(); ++i) {
            long long chi_phi_tang = (long long)ban_do_khoang_cach[tour[i]][diem_xa_nhat] + ban_do_khoang_cach[diem_xa_nhat][tour[i + 1]] - ban_do_khoang_cach[tour[i]][tour[i + 1]];
            if (chi_phi_tang < chi_phi_tang_min) {
                chi_phi_tang_min = chi_phi_tang;
                vi_tri_chen_tot_nhat = (int)i + 1;
            }
        }

        tour.insert(tour.begin() + vi_tri_chen_tot_nhat, diem_xa_nhat);
        da_tham[diem_xa_nhat] = true;
    }

    return tour;
}

void TimKiemCucBo2Opt(vector<int>& lo_trinh, const vector<vector<int>>& ban_do_khoang_cach) {
    if (lo_trinh.empty()) return;

    vector<int> a;
    a.reserve(lo_trinh.size() + 1);
    a.push_back(0);
    a.insert(a.end(), lo_trinh.begin(), lo_trinh.end());
    int n = (int)a.size();
    bool improved = true;

    while (improved) {
        improved = false;
        for (int i = 0; i + 1 < n - 1; ++i) {
            for (int j = i + 1; j + 1 < n; ++j) {
                int d1 = a[i], d2 = a[i + 1], d3 = a[j], d4 = a[j + 1];
                long long gain = (long long)ban_do_khoang_cach[d1][d3] + ban_do_khoang_cach[d2][d4]
                                 - ((long long)ban_do_khoang_cach[d1][d2] + ban_do_khoang_cach[d3][d4]);
                if (gain < 0) {
                    reverse(a.begin() + i + 1, a.begin() + j + 1);
                    improved = true;
                }
            }
        }
    }

    lo_trinh.assign(a.begin() + 1, a.end());
}

void PhaRoiLoTrinh(vector<int>& lo_trinh) {
    int m = (int)lo_trinh.size();
    if (m < 4) return;

    vector<int> idx(m);
    iota(idx.begin(), idx.end(), 0);
    shuffle(idx.begin(), idx.end(), RNG);
    int a = idx[0], b = idx[1], c = idx[2], d = idx[3];
    array<int,4> arr = {a,b,c,d};
    sort(arr.begin(), arr.end());
    a = arr[0]; b = arr[1]; c = arr[2]; d = arr[3];

    vector<int> t;
    t.reserve(m);
    t.insert(t.end(), lo_trinh.begin(), lo_trinh.begin() + a + 1);
    t.insert(t.end(), lo_trinh.begin() + c + 1, lo_trinh.begin() + d + 1);
    t.insert(t.end(), lo_trinh.begin() + b + 1, lo_trinh.begin() + c + 1);
    t.insert(t.end(), lo_trinh.begin() + a + 1, lo_trinh.begin() + b + 1);
    lo_trinh.swap(t);
}

pair<vector<int>, long long> GiaiBangTimKiemDiaPhuongLap(int so_diem, 
                                                           const vector<vector<int>>& ban_do_khoang_cach, 
                                                           const vector<int>& khoang_cach_den_dich, 
                                                           int so_lan_lap_toi_da) {
    vector<int> lo_trinh_hien_tai = KhoiTaoBangChenDiemXaNhat(so_diem, ban_do_khoang_cach);
    TimKiemCucBo2Opt(lo_trinh_hien_tai, ban_do_khoang_cach);

    vector<int> lo_trinh_tot_nhat = lo_trinh_hien_tai;
    long long chi_phi_tot_nhat = TinhChiPhiLoTrinh(lo_trinh_tot_nhat, ban_do_khoang_cach, khoang_cach_den_dich);

    for (int iter = 0; iter < so_lan_lap_toi_da; ++iter) {
        vector<int> lo_trinh_bi_pha = lo_trinh_hien_tai;
        PhaRoiLoTrinh(lo_trinh_bi_pha);
        TimKiemCucBo2Opt(lo_trinh_bi_pha, ban_do_khoang_cach);
        long long chi_phi_moi = TinhChiPhiLoTrinh(lo_trinh_bi_pha, ban_do_khoang_cach, khoang_cach_den_dich);
        if (chi_phi_moi < chi_phi_tot_nhat) {
            lo_trinh_tot_nhat = lo_trinh_bi_pha;
            chi_phi_tot_nhat = chi_phi_moi;
        }
        lo_trinh_hien_tai = lo_trinh_tot_nhat;
    }

    return {lo_trinh_tot_nhat, chi_phi_tot_nhat};
}

// ==================================================================================
// MAIN
// ==================================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using clk = chrono::steady_clock;
    auto t0 = clk::now();

    int so_hang, so_cot;
    if (!(cin >> so_hang >> so_cot)) return 0;

    vector<string> luoi(so_hang);
    for (int i = 0; i < so_hang; ++i) cin >> luoi[i];

    for (int i = 0; i < so_hang; ++i) {
        if ((int)luoi[i].size() != so_cot) {
            cerr << "Loi: kich thuoc hang " << i << " khong phu hop voi so_cot." << endl;
            return 1;
        }
    }

    ToaDo diem_xuat_phat{-1, -1}, diem_dich{-1, -1};
    vector<ToaDo> cac_tram_dung_chan;
    vector<ToaDo> diem_D;

    for (int i = 0; i < so_hang; ++i) {
        for (int j = 0; j < so_cot; ++j) {
            if (luoi[i][j] == 'D') diem_D.push_back({i, j});
            else if (luoi[i][j] == '1') cac_tram_dung_chan.push_back({i, j});
        }
    }

    if (diem_D.size() != 2) {
        cerr << "Loi: Phai co dung 2 diem 'D' la diem xuat phat va diem dich." << endl;
        return 1;
    }
    diem_xuat_phat = diem_D[0];
    diem_dich = diem_D[1];

    int so_tram = (int)cac_tram_dung_chan.size();
    if (so_tram == 0) {
        auto kc = TimDuongBFS_1D(diem_xuat_phat, so_hang, so_cot, luoi);
        int chi_phi = kc[idx_flat(diem_dich.hang, diem_dich.cot, so_cot)];
        if (chi_phi == -1) cout << "Khong ton tai duong di.\n";
        else cout << "Duong di ngan nhat: " << chi_phi << "\n";
        return 0;
    }

    vector<ToaDo> cac_diem_quan_trong;
    cac_diem_quan_trong.reserve(1 + so_tram);
    cac_diem_quan_trong.push_back(diem_xuat_phat);
    cac_diem_quan_trong.insert(cac_diem_quan_trong.end(), cac_tram_dung_chan.begin(), cac_tram_dung_chan.end());

    int so_diem_xu_ly = (int)cac_diem_quan_trong.size();

    vector<vector<int>> ban_do_khoang_cach(so_diem_xu_ly, vector<int>(so_diem_xu_ly, -1));
    vector<int> khoang_cach_den_dich(so_diem_xu_ly, -1);

    for (int i = 0; i < so_diem_xu_ly; ++i) {
        auto kc_tu_i = TimDuongBFS_1D(cac_diem_quan_trong[i], so_hang, so_cot, luoi);
        for (int j = 0; j < so_diem_xu_ly; ++j) {
            int val = kc_tu_i[idx_flat(cac_diem_quan_trong[j].hang, cac_diem_quan_trong[j].cot, so_cot)];
            ban_do_khoang_cach[i][j] = val;
            if (val == -1) {
                cerr << "Loi: Khong tim thay duong di giua hai diem quan trong." << endl;
                cout << "Khong ton tai duong di.\n";
                return 1;
            }
        }
        int val2 = kc_tu_i[idx_flat(diem_dich.hang, diem_dich.cot, so_cot)];
        khoang_cach_den_dich[i] = val2;
        if (val2 == -1) {
            cerr << "Loi: Khong tim thay duong di tu mot diem quan trong den Dich." << endl;
            cout << "Khong ton tai duong di.\n";
            return 1;
        }
    }

    cout << "Da tinh xong ban do khoang cach giua " << so_diem_xu_ly << " diem.\n";

    // Sử dụng ngưỡng NGUONG_DP để quyết định phương pháp
    if (so_tram < NGUONG_DP) {
        cout << "==> So luong tram (" << so_tram << ") nho (< " << NGUONG_DP << "). Su dung Quy Hoach Dong Bitmask de tim KET QUA TOI UU." << endl;
        vector<vector<long long>> dp;
        long long ket_qua = GiaiBangQuyHoachDongBitmask(so_tram, ban_do_khoang_cach, khoang_cach_den_dich, dp);
        cout << "\n=========================================\nKET QUA TOI UU:\n";
        if (ket_qua >= VO_CUNG) cout << "Khong ton tai duong di thoa man.\n";
        else {
            cout << "Duong di ngan nhat tim duoc: " << ket_qua << "\n";
            auto thu_tu_tham = TruyVetDuongDiDP(so_tram, dp, ban_do_khoang_cach, khoang_cach_den_dich);
            InDuongDi(thu_tu_tham, diem_xuat_phat, diem_dich, cac_diem_quan_trong);
        }
    } else {
        cout << "==> So luong tram (" << so_tram << ") lon (>= " << NGUONG_DP << "). Su dung Heuristic ILS de tim KET QUA GAN TOI UU." << endl;
        int so_lan_lap_ils = 1000; 
        cout << "Bat dau chay Tim Kiem Dia Phuong Lap voi " << so_lan_lap_ils << " lan lap..." << endl;
        auto ket_qua = GiaiBangTimKiemDiaPhuongLap(so_diem_xu_ly, ban_do_khoang_cach, khoang_cach_den_dich, so_lan_lap_ils);
        cout << "\n=========================================\nKET QUA GAN TOI UU:\n";
        cout << "Duong di ngan nhat tim duoc: " << ket_qua.second << "\n";
        InDuongDi(ket_qua.first, diem_xuat_phat, diem_dich, cac_diem_quan_trong);
    }

    auto t1 = clk::now();
    chrono::duration<double> dt = t1 - t0;
    cout << "-----------------------------------------\n";
    cout << "Tong thoi gian thuc thi: " << dt.count() << " giay\n";
    cout << "=========================================" << endl;
    return 0;
}




