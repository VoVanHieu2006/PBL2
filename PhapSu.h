#ifndef PHAP_SU_H
#define PHAP_SU_H

#include "HamChung.h"
#include "DoiTuong.h"
#include "NVTrongBoss.h"

#define THOI_GIAN_HOI_SINH 5000
#define SO_KHUNG_HINH_PHAP_SU 4
#define TOC_DO_PHEP_THUAT 0.2
#define THOI_GIAN



struct PhepThuat{
    float ViTriPhepThuatX_, ViTriPhepThuatY_;
    float VanTocPhepThuatX_, VanTocPhepThuatY_;
    HuongDi Huong_;
};


class PhapSu{
public:
PhapSu();
    ~PhapSu();

    bool KhoiTao(SDL_Renderer *ManHinh, int GocTuong, const int SO_LUONG_SACH);
    void VePhapSu(SDL_Renderer *ManHinh, NVTrongBoss* NhanVat);
    void GayThuong();
    bool DaChet() const { return MauHienTai_ <= 0;}

    int GetViTriPhapSuX(){return ViTriPhapSuX_;}
    int GetViTriPhapSuY(){return ViTriPhapSuY_;}
    Uint32 GetThoiGianChet(){return ThoiGianChet_;}
    PhepThuat GetPhepThuat(){return PhepThuat_;}

    void CapNhatPhapSu(NVTrongBoss* NhanVat);

    void VeThanhMauPhapSu(SDL_Renderer* ManHinh);

    void PhongPhepThuat(NVTrongBoss *NhanVat);
    void VePhepThuat(SDL_Renderer* ManHinh);
    
    void HoiSinh();

private:
    int PHAP_SU_DAY_MAU;
    DoiTuong AnhPhapSu_;
    SDL_Rect CacVungCatPhapSu_[SO_KHUNG_HINH_MOT_HANG];
    float ViTriPhapSuX_;
    float ViTriPhapSuY_;



    int MauHienTai_;
    bool DangBiThuong_;
    Uint32 ThoiGianBatDauBiThuong_;
    Uint32 ThoiGianChet_;


    int  KhungHinhHienTai_;
    int ThoiGianBatDauTienHoa_;
    bool DaTienHoa_ ;
    bool DangTanCong_ ;

    int GocTuong_;


    DoiTuong AnhPhepThuat_;
    PhepThuat PhepThuat_;


}; 

#endif