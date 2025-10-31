#ifndef RUONG_DO_H
#define RUONG_DO_H

#include "HamChung.h"
#include "BanDoGame.h"
#include "ThuatToan2.h"

#define SO_LUONG_SACH_GOI_Y_TOI_DA 5
#define THOI_GIAN_CHO_MOI_CUON_SACH 3


class RuongDo{
public:

    RuongDo();
    ~RuongDo();

    // 1) Gắn tham chiếu dữ liệu bên ngoài (không sở hữu)
    //    - TuiSach: map<localID, soLuong>
    //    - dsTenSach: vector tên sách (index = localID)
    void GanThamChieuDuLieu(map<int,int>* TuiSach,
                             const vector<string>* DSTenSach);

    // 2) Mở mini-game khi va chạm rương (đọc số lượng cần nhập từ ObjChest.SoLuong_)
    void Mo(const DoiTuongTuongTac& DoiTuongRuong);

    // 3) Đóng mini-game (thanhCong = true nếu hoàn tất đúng thứ tự)
    void Dong(bool ThanhCong);

    // 4) Xử lý sự kiện nhập (SDL_TEXTINPUT, Enter, Backspace, Esc)
    void XuLySuKien(const SDL_Event& e, const int& SO_LUONG_SACH);
    void RandomSachCanNhap();

    // 5) Vẽ UI hộp nhập
    void VeGameOver(SDL_Renderer* ManHinh, TTF_Font* Font);
    void Ve(SDL_Renderer* ManHinh, TTF_Font* Font, const int& SO_LUONG_SACH);

    // 6) Trạng thái
    bool DangMo() const { return DangNhapTenSach_; }
    bool HoanThanh() const { return HoanThanhThuTu_; }

    // Tiện ích: vẽ chữ
    void VeChu(SDL_Renderer* ManHinh, TTF_Font* Font,
               const string& nd, int x, int y, SDL_Color Mau);
    // Chuẩn hoá chuỗi để so sánh tên (bỏ khoảng trắng đầu/cuối, lower)

    string ChuanHoa(const string& SauChuaChuanHoa) const;

    int TimLocalIDTheoTenSach(const string& TenSachChuaChuanHoa) const;

    // ================== HỆ THỐNG GỢI Ý ===============
    void CapNhatGoiY();
    void CapNhatThoiGian();

private:
    // Dữ liệu bên ngoài (KHÔNG sở hữu)
    map<int,int>* TuiSach_ ;
    map<int, int> TuiSachTam_;
    const vector<string>* DanhSachTenSach_ ;

    // Trạng thái mini-game
    bool DangNhapTenSach_ ;

    string BufferNhap_;

    vector<int> ThuTuYeuCau_; // dãy localID cần nhập 
    int ViTriDangCan_;

    string ThuTuHienThi_;     // chuỗi hiển thị dãy cần nhập
    string ThongBaoLoi_;      // lỗi hiển thị dưới ô nhập

    bool HoanThanhThuTu_ ;

    // ================== HỆ THỐNG GỢI Ý ===============
    bool DangHienThiGameOver_;
    bool DangNhapSoGoiY_;
    // THÊM: Hệ thống gợi ý và bộ đếm thời gian
    TrieGoiY* Cay_;
    int ChiSoGoiYDangChon_;
    vector<string> CacGoiY_;
    
    // Bộ đếm thời gian
    Uint32 ThoiGianBatDau_;
    Uint32 ThoiGianConLai_;  // tính bằng giây
    Uint32 ThoiGianBanDau_;
    bool DaHetGio_;
        


};

#endif