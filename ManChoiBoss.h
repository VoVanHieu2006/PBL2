#ifndef MAN_CHOI_BOSS_H
#define MAN_CHOI_BOSS_H
#include "HamChung.h"
#include "DoiTuong.h" 
#include "NVTrongBoss.h"
#include "Boss.h"
#include "KhiDot.h"
#include "PhapSu.h"

#define SO_LUONG_PHAP_SU 4


enum TrangThaiBossGame {
    DANG_CHOI_BINH_THUONG,
    HIEU_UNG_NHAN_VAT_CHET,    
    HIEU_UNG_BOSS_CHET,       
    HIEN_THI_MAN_HINH_THUA,   
    HIEN_THI_MAN_HINH_THANG   
};


class ManChoiBoss {
public:
    ManChoiBoss();
    ~ManChoiBoss();
    
    // Khởi tạo
    bool KhoiTao(SDL_Renderer* ManHinh,const int SO_LUONG_SACH);
    
    // Cập nhật game
    void CapNhat();
    
    // Xử lý input
    void XuLyInput(SDL_Event& SuKien);
    
    // Vẽ
    void Ve(SDL_Renderer* ManHinh, TTF_Font* font);
    
    // Kiểm tra trạng thái
    bool DangChoi() const { return DangChoi_; }
    bool DaThang() const { return DaThang_; }
    bool DaThua() const { return DaThua_; }
    
    // Bắt đầu cuộc hội thoại boss
    void VeHoiThoai(SDL_Renderer* ManHinh, TTF_Font* font);
    void BatDauHoiThoaiBoss();
    void VeChu(SDL_Renderer* ManHinh, TTF_Font* font, const string& text, 
               int x, int y, SDL_Color color);


    // Kiem tra va cham
    void KiemTraVaChamSurikenVsBoss();
    void KiemTraVaChamSurikenVsKhiDot();
    void KiemTraVaChamSurikenVsMaThuat();
    void KiemTraVaChamSurikenVsPhapSu();
    void KiemTraVeMaThuat();
    void KiemTraVaChamVoiNhanVat();



    // Các hàm xử lý hiệu ứng

    void BatDauHieuUngNhanVatChet();
    void BatDauHieuUngBossChet();
    void CapNhatHieuUngChet();
    void VeHieuUngNhanVatChet(SDL_Renderer* ManHinh, TTF_Font* font);
    void VeHieuUngBossChet(SDL_Renderer* ManHinh, TTF_Font* font);
    void VeManHinhThua(SDL_Renderer* ManHinh, TTF_Font* font);
    void VeManHinhThang(SDL_Renderer* ManHinh, TTF_Font* font);

    bool GetYeuCauQuayVeMenu(){return YeuCauQuayVeMenu_;}
    void SetYeuCauQuayVeMenu(bool b){YeuCauQuayVeMenu_ = b;}

    void KiemTraVaSinhPhapSuTheoGiaiDoan();

    


private:
    DoiTuong AnhNenBoss_;
    DoiTuong AnhNenDauTruong_;
    NVTrongBoss NhanVat_;
    Boss Boss_;
    KhiDot KhiDot_;
    PhapSu PhapSu_[4];
    
    // Trạng thái game
    bool DangChoi_;
    bool DaThang_;
    bool DaThua_;

    // Hội thoại
    bool DangHoiThoai_;
    bool HoiThoaiHoanThanh_;
    string NoiDungHoiThoai_;


    //Hệ thống hiệu ứng và màn hình kết thúc
    TrangThaiBossGame TrangThaiGame_;
    
    // Hiệu ứng chết
    float GocXoayNhanVat_;
    float GocXoayBoss_;

    Uint32 ThoiGianBatDauHieuUng_;
    const float TOC_DO_XOAY = 180.0f;  // 180 độ/giây
    const Uint32 THOI_GIAN_HIEU_UNG = 2000; // 2 giây
    
    // Màn hình kết thúc
    DoiTuong AnhManHinhThua_;
    DoiTuong AnhManHinhThang_;
    bool DaTaiAnhKetThuc_;
    
    // Yêu cầu quay về menu
    bool YeuCauQuayVeMenu_;

    int SoPhapSuHienTai_;
    
    
   
};

#endif