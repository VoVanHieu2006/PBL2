#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <chrono>

using namespace std;

// Hằng số (dùng long long để tránh tràn khi cộng dồn)
const long long VO_CUNG = (long long)9e15;
const int NGUONG_CHUYEN_DOI_THUAT_TOAN = 20; // Ngưỡng số checkpoint để chuyển từ DP sang ILS

// Struct để lưu tọa độ
struct ToaDo {
    int hang, cot;
};

int huong_hang[] = {-1, 1, 0, 0};
int huong_cot[] = {0, 0, -1, 1};

// ==================================================================================
// BFS: tạo bản đồ khoảng cách (trả về -1 nếu không thể tới ô nào đó)
// ==================================================================================
vector<vector<int>> TimDuongBFS(const ToaDo& diem_bat_dau, int so_hang, int so_cot, const vector<string>& luoi) {
    vector<vector<int>> ma_tran_khoang_cach(so_hang, vector<int>(so_cot, -1));
    auto hop_le = [&](int r, int c){
        if (r < 0 || r >= so_hang || c < 0 || c >= so_cot) return false;
        // Xem ô có phải chướng ngại hay không. Giả sử ký tự '#' là chướng ngại; mọi ký tự khác được coi là đi được.
        return luoi[r][c] != '#';
    };

    if (!hop_le(diem_bat_dau.hang, diem_bat_dau.cot)) return ma_tran_khoang_cach;

    queue<ToaDo> hang_doi;
    ma_tran_khoang_cach[diem_bat_dau.hang][diem_bat_dau.cot] = 0;
    hang_doi.push(diem_bat_dau);

    while (!hang_doi.empty()) {
        ToaDo cur = hang_doi.front(); hang_doi.pop();
        for (int k = 0; k < 4; ++k) {
            int nr = cur.hang + huong_hang[k];
            int nc = cur.cot + huong_cot[k];
            if (hop_le(nr, nc) && ma_tran_khoang_cach[nr][nc] == -1) {
                ma_tran_khoang_cach[nr][nc] = ma_tran_khoang_cach[cur.hang][cur.cot] + 1;
                hang_doi.push({nr, nc});
            }
        }
    }
    return ma_tran_khoang_cach;
}

// ==================================================================================
// Hàm in đường đi
// ==================================================================================
void InDuongDi(const vector<int>& thu_tu_tham, const ToaDo& diem_xuat_phat, const ToaDo& diem_dich,
               const vector<ToaDo>& cac_diem_quan_trong) {
    cout << "\n=========================================\n";
    cout << "THU TU THAM CUA DUONG DI:\n";
    cout << "=========================================\n";

    cout << "Xuat phat (" << diem_xuat_phat.hang << "," << diem_xuat_phat.cot << ")";
    for (int chi_so : thu_tu_tham) {
        if (chi_so >= 0 && chi_so < (int)cac_diem_quan_trong.size()) {
            cout << " -> Tram " << chi_so << " (" 
                 << cac_diem_quan_trong[chi_so].hang << "," 
                 << cac_diem_quan_trong[chi_so].cot << ")";
        }
    }
    cout << " -> Dich (" << diem_dich.hang << "," << diem_dich.cot << ")\n";
    cout << "=========================================\n";
}

// ==================================================================================
// PHƯƠNG PHÁP 1: QUY HOẠCH ĐỘNG BITMASK
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
        // Lưu chỉ số tương ứng trong cac_diem_quan_trong (bên ngoài: 0 = start, 1.. = tram)
        thu_tu_tham.push_back(tram_hien_tai + 1);

        if ((__builtin_popcount(mask_hien_tai)) == 1) break;

        int mask_truoc = mask_hien_tai ^ (1 << tram_hien_tai);
        int tram_truoc = -1;

        for (int j = 0; j < so_tram; ++j) {
            if ((mask_truoc >> j) & 1) {
                long long chi_phi = dp[mask_truoc][j] + (long long)ban_do_khoang_cach[j + 1][tram_hien_tai + 1];
                if (chi_phi == dp[mask_hien_tai][tram_hien_tai]) { // so sánh nguyên xác
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
                            dp[mask][i] = min(dp[mask][i], dp[mask_truoc][j] + (long long)ban_do_khoang_cach[j + 1][i + 1]);
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
// PHƯƠNG PHÁP 2: HEURISTIC ILS
// ==================================================================================

long long TinhChiPhiLoTrinh(const vector<int>& lo_trinh, 
                             const vector<vector<int>>& ban_do_khoang_cach, 
                             const vector<int>& khoang_cach_den_dich) {
    // lo_trinh chứa chỉ số trong bản đồ: 1..(n-1). 0 là start.
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
    // so_diem: tổng số điểm quan trọng (0=start, 1..=trams)
    if (so_diem <= 1) return {};
    if (so_diem == 2) return {1};

    vector<int> tour = {1};
    vector<bool> da_tham(so_diem, false);
    da_tham[0] = true; // start
    da_tham[1] = true; // đã thêm node 1

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

        // Tìm vị trí chèn tối ưu (tính thêm chi phí nhỏ nhất)
        int vi_tri_chen_tot_nhat = 0;
        long long chi_phi_tang_min = VO_CUNG;

        // chèn trước tour[0]
        long long chi_phi_dau = (long long)ban_do_khoang_cach[0][diem_xa_nhat] + ban_do_khoang_cach[diem_xa_nhat][tour[0]] - ban_do_khoang_cach[0][tour[0]];
        chi_phi_tang_min = chi_phi_dau;
        vi_tri_chen_tot_nhat = 0;

        for (size_t i = 0; i + 1 < tour.size(); ++i) {
            long long chi_phi_tang = (long long)ban_do_khoang_cach[tour[i]][diem_xa_nhat] + ban_do_khoang_cach[diem_xa_nhat][tour[i + 1]] - ban_do_khoang_cach[tour[i]][tour[i + 1]];
            if (chi_phi_tang < chi_phi_tang_min) {
                chi_phi_tang_min = chi_phi_tang;
                vi_tri_chen_tot_nhat = i + 1;
            }
        }

        tour.insert(tour.begin() + vi_tri_chen_tot_nhat, diem_xa_nhat);
        da_tham[diem_xa_nhat] = true;
    }

    return tour;
}

void TimKiemCucBo2Opt(vector<int>& lo_trinh, const vector<vector<int>>& ban_do_khoang_cach) {
    // Phiên bản 2-opt cho đường hướng tuyến (0 là start cố định, lo_trinh chứa 1..)
    if (lo_trinh.empty()) return;

    vector<int> a;
    a.push_back(0);
    a.insert(a.end(), lo_trinh.begin(), lo_trinh.end());
    int n = (int)a.size();
    bool improved = true;

    while (improved) {
        improved = false;
        for (int i = 0; i + 1 < n - 1; ++i) {
            for (int j = i + 1; j + 1 < n; ++j) {
                int d1 = a[i], d2 = a[i + 1], d3 = a[j], d4 = a[j + 1];
                if ((long long)ban_do_khoang_cach[d1][d3] + ban_do_khoang_cach[d2][d4] <
                    (long long)ban_do_khoang_cach[d1][d2] + ban_do_khoang_cach[d3][d4]) {
                    reverse(a.begin() + i + 1, a.begin() + j + 1);
                    improved = true;
                }
            }
        }
    }

    lo_trinh.assign(a.begin() + 1, a.end());
}

void PhaRoiLoTrinh(vector<int>& lo_trinh, mt19937& rng) {
    int m = (int)lo_trinh.size();
    if (m < 4) return;
    vector<int> idx(m);
    iota(idx.begin(), idx.end(), 0);
    shuffle(idx.begin(), idx.end(), rng);
    sort(idx.begin(), idx.begin() + 4);
    int a = idx[0], b = idx[1], c = idx[2], d = idx[3];
    if (!(a < b && b < c && c < d)) return; // an toàn

    vector<int> t;
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
    random_device rd;
    mt19937 rng(rd());

    vector<int> lo_trinh_hien_tai = KhoiTaoBangChenDiemXaNhat(so_diem, ban_do_khoang_cach);
    TimKiemCucBo2Opt(lo_trinh_hien_tai, ban_do_khoang_cach);

    vector<int> lo_trinh_tot_nhat = lo_trinh_hien_tai;
    long long chi_phi_tot_nhat = TinhChiPhiLoTrinh(lo_trinh_tot_nhat, ban_do_khoang_cach, khoang_cach_den_dich);

    for (int iter = 0; iter < so_lan_lap_toi_da; ++iter) {
        vector<int> lo_trinh_bi_pha = lo_trinh_hien_tai;
        PhaRoiLoTrinh(lo_trinh_bi_pha, rng);
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

    auto t0 = chrono::high_resolution_clock::now();

    int so_hang, so_cot;
    cout << "Nhap so hang va so cot: ";
    if (!(cin >> so_hang >> so_cot)) return 0;

    vector<string> luoi(so_hang);
    cout << "Nhap ma tran (su dung 'D' cho diem dau/cuoi, '1' cho tram dung chan, '0' cho o trong, '#' la tuong):\n";
    for (int i = 0; i < so_hang; ++i) cin >> luoi[i];

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
        cout << "\nKhong co tram dung chan nao. Tim duong di truc tiep...\n";
        auto kc = TimDuongBFS(diem_xuat_phat, so_hang, so_cot, luoi);
        int chi_phi = kc[diem_dich.hang][diem_dich.cot];
        if (chi_phi == -1) cout << "Khong ton tai duong di.\n";
        else cout << "Duong di ngan nhat: " << chi_phi << "\n";
        return 0;
    }

    vector<ToaDo> cac_diem_quan_trong = {diem_xuat_phat};
    cac_diem_quan_trong.insert(cac_diem_quan_trong.end(), cac_tram_dung_chan.begin(), cac_tram_dung_chan.end());
    int so_diem_xu_ly = (int)cac_diem_quan_trong.size();

    vector<vector<int>> ban_do_khoang_cach(so_diem_xu_ly, vector<int>(so_diem_xu_ly, -1));
    vector<int> khoang_cach_den_dich(so_diem_xu_ly, -1);

    for (int i = 0; i < so_diem_xu_ly; ++i) {
        auto kc_tu_i = TimDuongBFS(cac_diem_quan_trong[i], so_hang, so_cot, luoi);
        for (int j = 0; j < so_diem_xu_ly; ++j) {
            ban_do_khoang_cach[i][j] = kc_tu_i[cac_diem_quan_trong[j].hang][cac_diem_quan_trong[j].cot];
            if (ban_do_khoang_cach[i][j] == -1) {
                cerr << "Loi: Khong tim thay duong di giua hai diem quan trong." << endl;
                return 1;
            }
        }
        khoang_cach_den_dich[i] = kc_tu_i[diem_dich.hang][diem_dich.cot];
        if (khoang_cach_den_dich[i] == -1) {
            cerr << "Loi: Khong tim thay duong di tu mot diem quan trong den Dich." << endl;
            return 1;
        }
    }

    cout << "\nDa tinh xong ban do khoang cach giua " << so_diem_xu_ly << " diem." << endl;

    if (so_tram < NGUONG_CHUYEN_DOI_THUAT_TOAN) {
        cout << "==> So luong tram (" << so_tram << ") nho. Su dung Quy Hoach Dong Bitmask de tim KET QUA TOI UU." << endl;
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
        cout << "==> So luong tram (" << so_tram << ") lon. Su dung Heuristic ILS de tim KET QUA GAN TOI UU." << endl;
        int so_lan_lap_ils = 1000;
        cout << "Bat dau chay Tim Kiem Dia Phuong Lap voi " << so_lan_lap_ils << " lan lap..." << endl;
        auto ket_qua = GiaiBangTimKiemDiaPhuongLap(so_diem_xu_ly, ban_do_khoang_cach, khoang_cach_den_dich, so_lan_lap_ils);
        cout << "\n=========================================\nKET QUA GAN TOI UU:\n";
        cout << "Duong di ngan nhat tim duoc: " << ket_qua.second << "\n";
        InDuongDi(ket_qua.first, diem_xuat_phat, diem_dich, cac_diem_quan_trong);
    }

    auto t1 = chrono::high_resolution_clock::now();
    chrono::duration<double> dt = t1 - t0;
    cout << "-----------------------------------------\n";
    cout << "Tong thoi gian thuc thi: " << dt.count() << " giay\n";
    cout << "=========================================\n";
    return 0;
}
