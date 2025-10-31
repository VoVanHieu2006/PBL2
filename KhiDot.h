#ifndef KHI_DOT_H
#define KHI_DOT_H

#include "HamChung.h"
#include "DoiTuong.h"
#include "NVTrongBoss.h"

#define SO_KHUNG_HINH_KHI_DOT 3
#define THOI_GIAN_HOI_SINH 5000
#define TOC_DO_TAN_CONG 0.2


class KhiDot{
public:
KhiDot();
    ~KhiDot();

    bool KhoiTao(SDL_Renderer *ManHinh, const int SO_LUONG_SACH);
    void VeKhiDot(SDL_Renderer *ManHinh, NVTrongBoss* NhanVat);
    void GayThuong();
    bool DaChet() const { return MauHienTai_ <= 0;}

    int GetViTriKhiDotX(){return ViTriKhiDotX_;}
    int GetViTriKhiDotY(){return ViTriKhiDotY_;}
    Uint32 GetThoiGianChet(){return ThoiGianChet_;}

    void CapNhatKhiDot(NVTrongBoss* NhanVat);

    void VeThanhMauKhiDot(SDL_Renderer* ManHinh);

    void HoiSinh();


private:
    int KHI_DOT_DAY_MAU;

    DoiTuong AnhKhiDot_;
    SDL_Rect CacVungCatKhiDot_[SO_KHUNG_HINH_KHI_DOT];
    float ViTriKhiDotX_;
    float ViTriKhiDotY_;
    float VanTocKhiDotX_;
    float VanTocKhiDotY_;
    float ViTriMucTieuX_;
    float ViTriMucTieuY_;
    float DichDenKhiDotX_;
    float DichDenKhiDotY_;

    int MauHienTai_;
    bool DangBiThuong_;
    Uint32 ThoiGianBatDauBiThuong_;
    Uint32 ThoiGianChet_;


    int  KhungHinhHienTai_;
    int ThoiGianBatDauTienHoa_;
    bool DaTienHoa_ ;
    bool DangTanCong_ ;
    int TocDoTanCong_;

}; 

#endif