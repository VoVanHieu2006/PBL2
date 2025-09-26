#ifndef NHAN_VAT_GAME_H
#define NHAN_VAT_GAME_H

#include "HamChung.h"
#include "DoiTuong.h"
#include "BanDoGame.h"

#include "RuongDo.h"

#define SO_KHUNG_HINH 16
#define SO_KHUNG_HINH_MOT_HANG 4
#define KICH_THUOC_NHAN_VAT 32
#define TOC_DO_NHAN_VAT 1

#define VI_TRI_BAN_DAU_X 1216;
#define VI_TRI_BAN_DAU_Y 608;

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
    HUONG_TREN = 1,
    HUONG_TRAI = 2,
    HUONG_PHAI = 3
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


class NhanVatGame : public DoiTuong{
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
    void KetThucBuoc();

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

    

private:
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
    

};




#endif