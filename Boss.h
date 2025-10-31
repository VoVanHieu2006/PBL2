#ifndef BOSS_H
#define BOSS_H

#include "HamChung.h"
#include "DoiTuong.h"
#include "NVTrongBoss.h"

#define SO_LUONG_MA_THUAT 8


const int dx[8] = { 0,  1,  1,  1,  0, -1, -1, -1 };
const int dy[8] = {-1, -1,  0,  1,  1,  1,  0, -1 };


struct MaThuat {
    int TrongAnhX_, TrongAnhY_;
    int KhungHinh_;
};



class Boss{
public:
Boss();
    ~Boss();

    bool KhoiTao(SDL_Renderer *ManHinh, const int SO_LUONG_SACH);
    void VeBoss(SDL_Renderer *ManHinh, NVTrongBoss* NhanVat);
    void GayThuong();
    bool DaChet() const { return MauHienTai_ <= 0;}

    int GetViTriBossX(){return ViTriBossX_;}
    int GetViTriBossY(){return ViTriBossY_;}
    MaThuat GetMaThuat(int i){return DanhSachMaThuat_[i];}
    SDL_Texture* GetThongTinDoiTuongAnhBoss(){return AnhBoss_.ThongTinDoiTuong();}

    void VeThanhMauBoss(SDL_Renderer* ManHinh, TTF_Font* font);
    void VeChu(SDL_Renderer* ManHinh, TTF_Font* Font, const string& NoiDung, int x, int y, SDL_Color Mau);

    void VeMaThuat(SDL_Renderer* ManHinh);
    bool GetDangKichHoatMaThuat(){return DangKichHoatMaThuat_;}
    void SetDangKichHoatMaThuat(bool b){DangKichHoatMaThuat_ = b;}

    int LuongMauConLai(){
    float TyLeMau = (float)MauHienTai_ / (float)BOSS_DAY_MAU; 
    if (TyLeMau > 0.9f) return 1;
    else if (TyLeMau > 0.7f) return 2;
    else if (TyLeMau > 0.5f) return 3;
    else return 4;
}


private:

    // BOSS
    int BOSS_DAY_MAU;
    DoiTuong AnhBoss_;
    SDL_Rect CacVungCatNhanVat_[SO_KHUNG_HINH_MOT_HANG][SO_KHUNG_HINH_MOT_HANG];
    int ViTriBossX_;
    int ViTriBossY_;
    int Huong_;



    int MauHienTai_;
    bool DangBiThuong_;
    Uint32 ThoiGianBatDauBiThuong_;



    // MaThuat
    DoiTuong AnhMaThuat_;
    MaThuat DanhSachMaThuat_[SO_LUONG_MA_THUAT];
    bool DangKichHoatMaThuat_;

};

#endif