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

// Hằng số
const int VO_CUNG = 1e9;
const int NGUONG_CHUYEN_DOI_THUAT_TOAN = 20; // Ngưỡng số checkpoint để chuyển từ DP sang ILS

// Struct để lưu tọa độ cho dễ đọc
struct ToaDo {
    int hang, cot;
};

// ==================================================================================
// 1) NỀN TẢNG (BFS - TẠO BẢN ĐỒ KHOẢNG CÁCH) - DÙNG CHUNG CHO CẢ 2 THUẬT TOÁN
// ==================================================================================
int huong_hang[] = {-1, 1, 0, 0};
int huong_cot[] = {0, 0, -1, 1};

vector<vector<int>> TimDuongBFS(const ToaDo& diem_bat_dau, int so_hang, int so_cot, const vector<string>& luoi) {
    vector<vector<int>> ma_tran_khoang_cach(so_hang, vector<int>(so_cot, -1));
    queue<ToaDo> hang_doi;
    
    ma_tran_khoang_cach[diem_bat_dau.hang][diem_bat_dau.cot] = 0;
    hang_doi.push(diem_bat_dau);
    
    while (!hang_doi.empty()) {
        ToaDo diem_hien_tai = hang_doi.front();
        hang_doi.pop();
        
        for (int i = 0; i < 4; ++i) {
            int hang_moi = diem_hien_tai.hang + huong_hang[i];
            int cot_moi = diem_hien_tai.cot + huong_cot[i];
            
            if (hang_moi >= 0 && hang_moi < so_hang && 
                cot_moi >= 0 && cot_moi < so_cot && 
                ma_tran_khoang_cach[hang_moi][cot_moi] == -1) {
                
                ma_tran_khoang_cach[hang_moi][cot_moi] = 
                    ma_tran_khoang_cach[diem_hien_tai.hang][diem_hien_tai.cot] + 1;
                hang_doi.push({hang_moi, cot_moi});
            }
        }
    }
    
    return ma_tran_khoang_cach;
}

// ==================================================================================
// HÀM IN ĐƯỜNG ĐI
// ==================================================================================
void InDuongDi(const vector<int>& thu_tu_tham, const ToaDo& diem_xuat_phat, const ToaDo& diem_dich,
               const vector<ToaDo>& cac_diem_quan_trong) {
    cout << "\n=========================================" << endl;
    cout << "THU TU THAM CUA DUONG DI:" << endl;
    cout << "=========================================" << endl;
    
    cout << "Xuat phat (" << diem_xuat_phat.hang << "," << diem_xuat_phat.cot << ")";
    
    for (int chi_so : thu_tu_tham) {
        cout << " -> Tram " << chi_so << " (" 
             << cac_diem_quan_trong[chi_so].hang << "," 
             << cac_diem_quan_trong[chi_so].cot << ")";
    }
    
    cout << " -> Dich (" << diem_dich.hang << "," << diem_dich.cot << ")" << endl;
    cout << "=========================================" << endl;
}

// ==================================================================================
// PHƯƠNG PHÁP 1: QUY HOẠCH ĐỘNG BITMASK (CHO SỐ CHECKPOINT NHỎ)
// ==================================================================================
long long GiaiBangQuyHoachDongBitmask(int so_tram, 
                                       const vector<vector<int>>& ban_do_khoang_cach, 
                                       const vector<int>& khoang_cach_den_dich) {
    // dp[mask][i]: chi phí nhỏ nhất để đi từ Xuất phát, thăm các trạm trong `mask`, kết thúc tại trạm `i`
    // Lưu ý: trạm `i` (0 <= i < so_tram) tương ứng với chỉ số `i+1` trong ban_do_khoang_cach
    vector<vector<long long>> dp(1 << so_tram, vector<long long>(so_tram, VO_CUNG));

    // Khởi tạo: đi từ Xuất phát (chỉ số 0) đến trạm i đầu tiên
    for (int i = 0; i < so_tram; ++i) {
        dp[1 << i][i] = ban_do_khoang_cach[0][i + 1];
    }

    // Vòng lặp chính của DP
    for (int mask = 1; mask < (1 << so_tram); ++mask) {
        for (int i = 0; i < so_tram; ++i) {
            if ((mask >> i) & 1) { // Nếu trạm i có trong mask
                for (int j = 0; j < so_tram; ++j) {
                    if (i != j && ((mask >> j) & 1)) { // Thử đi đến i từ trạm j
                        int mask_truoc = mask ^ (1 << i);
                        if (dp[mask_truoc][j] != VO_CUNG) {
                            dp[mask][i] = min(dp[mask][i], 
                                            dp[mask_truoc][j] + ban_do_khoang_cach[j + 1][i + 1]);
                        }
                    }
                }
            }
        }
    }

    // Tính kết quả cuối cùng
    long long chi_phi_toi_uu = VO_CUNG;
    int mask_cuoi_cung = (1 << so_tram) - 1;

    for (int i = 0; i < so_tram; ++i) {
        if (dp[mask_cuoi_cung][i] != VO_CUNG) {
            chi_phi_toi_uu = min(chi_phi_toi_uu, 
                                dp[mask_cuoi_cung][i] + khoang_cach_den_dich[i + 1]);
        }
    }
    
    return chi_phi_toi_uu;
}

// ==================================================================================
// PHƯƠNG PHÁP 2: HEURISTIC ILS (CHO SỐ CHECKPOINT LỚN)
// ==================================================================================

// --- Khai báo trước các hàm con cho ILS ---
long long TinhChiPhiLoTrinh(const vector<int>& lo_trinh, 
                             const vector<vector<int>>& ban_do_khoang_cach, 
                             const vector<int>& khoang_cach_den_dich);
vector<int> KhoiTaoBangChenDiemXaNhat(int so_diem, const vector<vector<int>>& ban_do_khoang_cach);
void TimKiemCucBo2Opt(vector<int>& lo_trinh, const vector<vector<int>>& ban_do_khoang_cach);
void PhaRoiLoTrinh(vector<int>& lo_trinh, mt19937& bo_sinh_so_ngau_nhien);

// Hàm ILS chính
pair<vector<int>, long long> GiaiBangTimKiemDiaPhuongLap(int so_diem, 
                                                           const vector<vector<int>>& ban_do_khoang_cach, 
                                                           const vector<int>& khoang_cach_den_dich, 
                                                           int so_lan_lap_toi_da) {
    random_device thiet_bi_ngau_nhien;
    mt19937 bo_sinh_so_ngau_nhien(thiet_bi_ngau_nhien());
    
    vector<int> lo_trinh_hien_tai = KhoiTaoBangChenDiemXaNhat(so_diem, ban_do_khoang_cach);
    TimKiemCucBo2Opt(lo_trinh_hien_tai, ban_do_khoang_cach);
    
    vector<int> lo_trinh_tot_nhat = lo_trinh_hien_tai;
    long long chi_phi_tot_nhat = TinhChiPhiLoTrinh(lo_trinh_tot_nhat, 
                                                     ban_do_khoang_cach, 
                                                     khoang_cach_den_dich);
    
    cout << "Chi phi khoi tao (sau 2-Opt): " << chi_phi_tot_nhat << endl;
    
    for (int i = 0; i < so_lan_lap_toi_da; ++i) {
        vector<int> lo_trinh_bi_pha_roi = lo_trinh_hien_tai;
        PhaRoiLoTrinh(lo_trinh_bi_pha_roi, bo_sinh_so_ngau_nhien);
        TimKiemCucBo2Opt(lo_trinh_bi_pha_roi, ban_do_khoang_cach);
        
        long long chi_phi_moi = TinhChiPhiLoTrinh(lo_trinh_bi_pha_roi, 
                                                   ban_do_khoang_cach, 
                                                   khoang_cach_den_dich);
        
        if (chi_phi_moi < chi_phi_tot_nhat) {
            lo_trinh_tot_nhat = lo_trinh_bi_pha_roi;
            chi_phi_tot_nhat = chi_phi_moi;
            cout << "Tim thay giai phap tot hon o lan lap " << i + 1 
                 << ": " << chi_phi_tot_nhat << endl;
        }
        
        lo_trinh_hien_tai = lo_trinh_tot_nhat; 
    }
    
    return {lo_trinh_tot_nhat, chi_phi_tot_nhat};
}

// --- Định nghĩa các hàm con của ILS ---

long long TinhChiPhiLoTrinh(const vector<int>& lo_trinh, 
                             const vector<vector<int>>& ban_do_khoang_cach, 
                             const vector<int>& khoang_cach_den_dich) {
    if (lo_trinh.empty()) {
        return khoang_cach_den_dich[0];
    }
    
    long long chi_phi = ban_do_khoang_cach[0][lo_trinh[0]]; 
    
    for (size_t i = 0; i < lo_trinh.size() - 1; ++i) {
        chi_phi += ban_do_khoang_cach[lo_trinh[i]][lo_trinh[i + 1]];
    }
    
    chi_phi += khoang_cach_den_dich[lo_trinh.back()]; 
    
    return chi_phi;
}

vector<int> KhoiTaoBangChenDiemXaNhat(int so_diem, const vector<vector<int>>& ban_do_khoang_cach) {
    if (so_diem <= 1) return {};
    
    vector<int> tour = {1};
    vector<bool> da_tham(so_diem, false);
    da_tham[0] = true; 
    da_tham[1] = true;
    
    while (tour.size() < so_diem - 1) {
        int diem_xa_nhat = -1;
        int khoang_cach_xa_nhat = -1;
        
        for (int k = 1; k < so_diem; ++k) {
            if (!da_tham[k]) {
                int kc_ngan_nhat_den_tour = VO_CUNG;
                for (int diem_trong_tour : tour) {
                    kc_ngan_nhat_den_tour = min(kc_ngan_nhat_den_tour, 
                                                 ban_do_khoang_cach[k][diem_trong_tour]);
                }
                
                if (kc_ngan_nhat_den_tour > khoang_cach_xa_nhat) {
                    khoang_cach_xa_nhat = kc_ngan_nhat_den_tour;
                    diem_xa_nhat = k;
                }
            }
        }
        
        int vi_tri_chen_tot_nhat = -1;
        long long chi_phi_tang_min = -1;
        
        long long chi_phi_tang_dau_tien = (long long)ban_do_khoang_cach[0][diem_xa_nhat] + 
                                          ban_do_khoang_cach[diem_xa_nhat][tour[0]] - 
                                          ban_do_khoang_cach[0][tour[0]];
        chi_phi_tang_min = chi_phi_tang_dau_tien;
        vi_tri_chen_tot_nhat = 0;
        
        for (size_t i = 0; i < tour.size() - 1; ++i) {
            long long chi_phi_tang = (long long)ban_do_khoang_cach[tour[i]][diem_xa_nhat] + 
                                     ban_do_khoang_cach[diem_xa_nhat][tour[i + 1]] - 
                                     ban_do_khoang_cach[tour[i]][tour[i + 1]];
            
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
    if (lo_trinh.size() < 2) return;
    
    vector<int> lo_trinh_day_du = {0};
    lo_trinh_day_du.insert(lo_trinh_day_du.end(), lo_trinh.begin(), lo_trinh.end());
    
    int n = lo_trinh_day_du.size();
    bool da_cai_thien = true;
    
    while (da_cai_thien) {
        da_cai_thien = false;
        
        for (int i = 0; i < n - 1; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int d1 = lo_trinh_day_du[i];
                int d2 = lo_trinh_day_du[(i + 1) % n];
                int d3 = lo_trinh_day_du[j];
                int d4 = lo_trinh_day_du[(j + 1) % n];
                
                if (d2 == d3 || d1 == d4) continue;
                
                if ((long long)ban_do_khoang_cach[d1][d3] + ban_do_khoang_cach[d2][d4] < 
                    (long long)ban_do_khoang_cach[d1][d2] + ban_do_khoang_cach[d3][d4]) {
                    
                    reverse(lo_trinh_day_du.begin() + i + 1, lo_trinh_day_du.begin() + j + 1);
                    da_cai_thien = true;
                }
            }
        }
    }
    
    lo_trinh.assign(lo_trinh_day_du.begin() + 1, lo_trinh_day_du.end());
}

void PhaRoiLoTrinh(vector<int>& lo_trinh, mt19937& bo_sinh_so_ngau_nhien) {
    if (lo_trinh.size() < 4) return; 
    
    uniform_int_distribution<int> phan_phoi(0, lo_trinh.size() - 1);
    vector<int> vi_tri(4);
    
    for (int i = 0; i < 4; ++i) {
        vi_tri[i] = phan_phoi(bo_sinh_so_ngau_nhien);
    }
    
    sort(vi_tri.begin(), vi_tri.end());
    
    if (vi_tri[0] == vi_tri[1] || vi_tri[1] == vi_tri[2] || vi_tri[2] == vi_tri[3]) {
        return;
    }
    
    vector<int> lo_trinh_moi;
    lo_trinh_moi.insert(lo_trinh_moi.end(), 
                        lo_trinh.begin(), 
                        lo_trinh.begin() + vi_tri[0] + 1);
    lo_trinh_moi.insert(lo_trinh_moi.end(), 
                        lo_trinh.begin() + vi_tri[2] + 1, 
                        lo_trinh.begin() + vi_tri[3] + 1);
    lo_trinh_moi.insert(lo_trinh_moi.end(), 
                        lo_trinh.begin() + vi_tri[1] + 1, 
                        lo_trinh.begin() + vi_tri[2] + 1);
    lo_trinh_moi.insert(lo_trinh_moi.end(), 
                        lo_trinh.begin() + vi_tri[0] + 1, 
                        lo_trinh.begin() + vi_tri[1] + 1);
    
    lo_trinh = lo_trinh_moi;
}

// ==================================================================================
// HÀM MAIN - BỘ ĐIỀU KHIỂN CHÍNH
// ==================================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto thoi_gian_bat_dau = chrono::high_resolution_clock::now();
    
    // --- ĐỌC DỮ LIỆU ĐỘNG ---
    int so_hang, so_cot;
    cout << "Nhap so hang va so cot: ";
    cin >> so_hang >> so_cot;
    
    vector<string> luoi(so_hang);
    cout << "Nhap ma tran (su dung 'D' cho diem dau/cuoi, '1' cho tram dung chan, '0' cho o trong):" << endl;
    for (int i = 0; i < so_hang; ++i) {
        cin >> luoi[i];
    }

    ToaDo diem_xuat_phat, diem_dich;
    vector<ToaDo> cac_tram_dung_chan;
    vector<ToaDo> diem_D;
    
    for (int i = 0; i < so_hang; ++i) {
        for (int j = 0; j < so_cot; ++j) {
            if (luoi[i][j] == 'D') {
                diem_D.push_back({i, j});
            } else if (luoi[i][j] == '1') {
                cac_tram_dung_chan.push_back({i, j});
            }
        }
    }
    
    if (diem_D.size() != 2) {
        cerr << "Loi: Phai co dung 2 diem 'D' la diem xuat phat va diem dich." << endl;
        return 1;
    }
    
    diem_xuat_phat = diem_D[0];
    diem_dich = diem_D[1];

    // --- XỬ LÝ TRƯỜNG HỢP KHÔNG CÓ TRẠM DỪNG CHÂN ---
    int so_tram = cac_tram_dung_chan.size();
    
    if (so_tram == 0) {
        cout << "\nKhong co tram dung chan nao. Tim duong di truc tiep..." << endl;
        vector<vector<int>> kc = TimDuongBFS(diem_xuat_phat, so_hang, so_cot, luoi);
        int chi_phi = kc[diem_dich.hang][diem_dich.cot];
        
        if (chi_phi == -1) {
            cout << "Khong ton tai duong di." << endl;
        } else {
            cout << "Duong di ngan nhat: " << chi_phi << endl;
        }
        
        return 0;
    }

    // --- TỔ CHỨC CÁC ĐIỂM QUAN TRỌNG VÀ TÍNH KHOẢNG CÁCH (BƯỚC CHUNG) ---
    vector<ToaDo> cac_diem_quan_trong = {diem_xuat_phat};
    cac_diem_quan_trong.insert(cac_diem_quan_trong.end(), 
                                cac_tram_dung_chan.begin(), 
                                cac_tram_dung_chan.end());
    
    int so_diem_xu_ly = cac_diem_quan_trong.size();
    
    vector<vector<int>> ban_do_khoang_cach(so_diem_xu_ly, vector<int>(so_diem_xu_ly));
    vector<int> khoang_cach_den_dich(so_diem_xu_ly);
    
    for (int i = 0; i < so_diem_xu_ly; ++i) {
        vector<vector<int>> kc_tu_i = TimDuongBFS(cac_diem_quan_trong[i], so_hang, so_cot, luoi);
        
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
    
    // =========================================================================
    // == BỘ ĐIỀU KHIỂN: TỰ ĐỘNG CHỌN THUẬT TOÁN DỰA TRÊN SỐ LƯỢNG CHECKPOINT ==
    // =========================================================================
    
    if (so_tram < NGUONG_CHUYEN_DOI_THUAT_TOAN + 1000) {
        cout << "==> So luong tram (" << so_tram 
             << ") nho. Su dung Quy Hoach Dong Bitmask de tim KET QUA TOI UU." << endl;
        
        long long ket_qua = GiaiBangQuyHoachDongBitmask(so_tram, 
                                                         ban_do_khoang_cach, 
                                                         khoang_cach_den_dich);
        
        cout << "\n=========================================" << endl;
        cout << "KET QUA TOI UU:" << endl;
        
        if (ket_qua >= VO_CUNG) {
            cout << "Khong ton tai duong di thoa man." << endl;
        } else {
            cout << "Duong di ngan nhat tim duoc: " << ket_qua << endl;
        }

    } else {
        cout << "==> So luong tram (" << so_tram 
             << ") lon. Su dung Heuristic ILS de tim KET QUA GAN TOI UU." << endl;
        
        int so_lan_lap_ils = 1000;
        cout << "Bat dau chay Tim Kiem Dia Phuong Lap voi " 
             << so_lan_lap_ils << " lan lap..." << endl;
        
        pair<vector<int>, long long> ket_qua = GiaiBangTimKiemDiaPhuongLap(so_diem_xu_ly, 
                                                                             ban_do_khoang_cach, 
                                                                             khoang_cach_den_dich, 
                                                                             so_lan_lap_ils);
        
        cout << "\n=========================================" << endl;
        cout << "KET QUA GAN TOI UU:" << endl;
        cout << "Duong di ngan nhat tim duoc: " << ket_qua.second << endl;
        
        // Gọi hàm InDuongDi
        InDuongDi(ket_qua.first, diem_xuat_phat, diem_dich, cac_diem_quan_trong);
    }

    auto thoi_gian_ket_thuc = chrono::high_resolution_clock::now();
    chrono::duration<double> thoi_gian_troi_qua = thoi_gian_ket_thuc - thoi_gian_bat_dau;
    
    cout << "-----------------------------------------" << endl;
    cout << "Tong thoi gian thuc thi: " << thoi_gian_troi_qua.count() << " giay" << endl;
    cout << "=========================================" << endl;

    return 0;
}