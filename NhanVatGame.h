#ifndef NHAN_VAT_GAME_H
#define NHAN_VAT_GAME_H

#include "HamChung.h"
#include "DoiTuong.h"
#include "BanDoGame.h"
#include "RuongDo.h"
#include "ThuatToan1.h"
#include "AmThanh.h"


#define SO_KHUNG_HINH 16
#define SO_KHUNG_HINH_MOT_HANG 4
#define KICH_THUOC_NHAN_VAT 32

#define VI_TRI_BAN_DAU_X  1216
#define VI_TRI_BAN_DAU_Y  608
#define TOC_DO_PIXEL_MOT_GIAY 160       // tốc độ di chuyển theo pixel/giây (đi 1 ô = KICH_THUOC_KHOI pixel)







struct Input{
    int Trai_;
    int Phai_;
    int Tren_;
    int Duoi_;
    int Nhay_;
};


enum HuongDi{
    KHONG_CO_HUONG = -1,
    HUONG_DUOI = 0,
    HUONG_TRAI = 1,
    HUONG_PHAI = 2,
    HUONG_TREN = 3
};



struct Character{
    DoiTuong AnhCuaNhanVat_;
    SDL_Rect CacVungCatNhanVat_[SO_KHUNG_HINH_MOT_HANG][SO_KHUNG_HINH_MOT_HANG];
    HuongDi Huong_;

    int ViTriNhanVatX_, ViTriNhanVatY_;
    float DoDichChuyenNhanVatX_, DoDichChuyenNhanVatY_;
    
    int KhungHinh_;

    int KichThuocThatNhanVat_;
};




class NhanVatGame{
public:
    NhanVatGame();
    ~NhanVatGame();

    // Tải ảnh 

    bool TaiAnhNhanVat(string DuongDan, SDL_Renderer *ManHinh);


    //  ----------- Xử lý dự liệu đầu vào -----------
    void HanhDongDauVao(SDL_Event SuKien, SDL_Renderer *ManHinh, const BanDoGame& BanDo, const int& SO_LUONG_SACH);

    // ----------- Xử lý dự liệu đầu vào -----------
    void XuLyDiChuyen(const BanDoGame& BanDo);

    void BatDauMotBuoc(int dx, int dy, HuongDi Huong, const BanDoGame& BanDo);
    void TrongQuaTrinhDi(const BanDoGame& BanDo);
    void KetThucBuoc(const BanDoGame& BanDo);

    // ----------- Vẽ nhân vật -----------

    void VeNhanVat(SDL_Renderer *ManHinh);

    // ----------- Túi đồ -------------
    void KhoiTaoDanhSachTenSach(); 
    void ThuThapSach(BanDoGame& BanDo);   // gọi sau khi kết thúc bước

    void VeChu(SDL_Renderer* ManHinh, TTF_Font* Font, const string& NoiDung, int x, int y, SDL_Color Mau);
    void VeTuiDo(SDL_Renderer* ManHinh, TTF_Font* Font,const string& DuongDanLinkSach, const BanDoGame& BanDo, const int& SO_LUONG_SACH);
    
    // ----------- Tương tác với bảng  -------------

    const DoiTuongTuongTac* VaChamDoiTuongTuongTac(const BanDoGame& BanDo);
    void VeBangChiDan(SDL_Renderer *ManHinh, TTF_Font* Font, const BanDoGame& BanDo);

    // ----------- Vẽ mini game rương  -------------
    void VeMiniGameRuong(SDL_Renderer* ManHinh, TTF_Font* Font, const int& SO_LUONG_SACH){ Ruong_.Ve(ManHinh, Font, SO_LUONG_SACH); }
    void VeThongBaoDaHoanThanh(SDL_Renderer* ManHinh, TTF_Font* Font);

    
    // ----------- Hệ THỐNG GAME 1  -------------
    void KhoiTaoHeThongBuocDi(int SoLuongSach, const SDL_Rect& VungTroChoi, const BanDoGame& BanDo);
    void CapNhatBuocDi(const ToaDo& ViTriMoi, const BanDoGame& BanDo);
    void HoanLaiBuocDi(const BanDoGame& BanDo);
    void BatTatGoiY() { DangHienThiGoiY_ = !DangHienThiGoiY_; }
    
    // Vẽ UI bước đi và các nút
    bool NamTrongTroChoi1();
    void KiemTraMoCuaTroChoi1(BanDoGame& BanDo);
    void VeThongTinBuocDi(SDL_Renderer* ManHinh, TTF_Font* Font);
    void VeDuongDiGoiY(SDL_Renderer* ManHinh, const BanDoGame& BanDo);
    void VeCacNutDieuKhien(SDL_Renderer* ManHinh, TTF_Font* Font);
    void VeTroChoi1(SDL_Renderer* ManHinh, TTF_Font* Font, const BanDoGame& BanDo);
    
    // Xử lý click nút
    bool XuLyClickNutDieuKhien(int mouseX, int mouseY, const BanDoGame& BanDo);
    void BatDauNhapNhay();
    void CapNhatNhapNhay();


    int ThongTinViTriX() const { return NhanVat_.ViTriNhanVatX_; }
    int ThongTinViTriY() const { return NhanVat_.ViTriNhanVatY_; }
    void ResetViTri() { 
        NhanVat_.ViTriNhanVatX_ = 8 * 32;
        NhanVat_.ViTriNhanVatY_ = 8 * 32;
        DangDiDenO_ = false;
    }

protected:
    Character NhanVat_;

    Input DauVao_; 


    // Cho nhân vật chỉ di chuyển 1 bước 1 ô
    bool DangDiDenO_;

    int XuatPhatX_ ;
    int XuatPhatY_ ;
    int DichDenX_ ;
    int DichDenY_ ;
    
    Uint32 ThoiGianBatDauBuoc_;
    Uint32 ThoiGianCanBuoc_;   // ms = (KICH_THUOC_KHOI / TOC_DO_PIXEL_MOT_GIAY)*1000

    // Animation cũ (không còn dùng kiểu tăng từng tick)
    Uint32 ThoiGianToiThieu_;         // giữ lại nếu bạn muốn
    Uint32 ThoiGianTichLuyAnimation_; // giữ lại nếu muốn tái sử dụng

    // -- túi đồ -- 
    vector<string> DanhSachTenSach_;          // index = localID
    map<int,int> TuiSach_;               // localID -> số lượng
    bool HienThiTuiDo_ ;

    int ChiSoSachDauTien_;
    int SoLuongSachHienThi_;


    // Kiểm trả và chạm với biển báo
    bool HienThiThongBao_; // Kiểm tra trạng thái

    // Mini-game rương 
    RuongDo Ruong_;
    bool YeuCauMoRuong_;
    bool HienThiThongBaoDaHoanThanh_;
    bool DaMoCua_; // Dùng để đánh dấu mở cửa khi xong trò chơi
    


    // ==================== Hệ thống game nhặt sách ====================
    bool DaDongCuaTroChoi1_;
    bool KetThucTroChoi1_;
    

    long long SoBuocDaDi_;              // số bước đã đi
    long long GioiHanBuoc_;             // giới hạn bước cho phép
    vector<pair<ToaDo, int>> LichSuDuongDi_; // lưu lại đường đi để có thể quay lại
    // Tọa độ thì sẽ lưu tọa độ, int sẽ lưu id cục bộ của sách khi bị ăn
    bool DaCapNhat_;
    
    
    // === HỆ THỐNG GỢI Ý ĐƯỜNG ĐI ===
    bool DangHienThiGoiY_;        // có đang hiện gợi ý không
    vector<ToaDo> DuongDiGoiY_;   // đường đi gợi ý

    
    // === VỊ TRÍ ĐIỂM BẮT ĐẦU VÀ KẾT THÚC ===
    ToaDo DiemBatDau_;            // vị trí spawn nhân vật
    ToaDo DiemKetThuc_;           // vị trí cửa/đích
    vector<ToaDo> CacDiemSach_;

    SDL_Rect NutGoiY_;
    SDL_Rect NutHoanLai_;
    SDL_Rect TRO_CHOI_1_;


    // === BỔ SUNG: HIỆU ỨNG NHẤP NHÁY CHO NÚT HOÀN LẠI ===
    bool DangNhapNhay_;
    Uint32 ThoiGianBatDauNhapNhay_;
    Uint32 ThoiGianNhapNhay_;  // thời gian nhấp nháy (ms)
    int SoLanNhapNhay_;       // số lần nhấp nháy
    int DemNhapNhay_;         // đếm số lần đã nhấp nháy


    // ====================================================================================================
    // =============================== THUẬT TOÁN TÌM ĐƯỜNG ===============================================
    // ====================================================================================================

    

    void TimDuongDiOptimal(const BanDoGame& BanDo);
                                    
    // ===========================================================================================




};

#endif