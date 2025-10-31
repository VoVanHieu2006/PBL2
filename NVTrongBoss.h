#ifndef NHAN_VAT_GAME_TRONG_BOSS
#define NHAN_VAT_GAME_TRONG_BOSS

#include "HamChung.h"
#include "NhanVatGame.h"


#define VI_TRI_BOSS_X int(CHIEU_RONG_MAN_HINH / 2 )
#define VI_TRI_BOSS_Y int(CHIEU_CAO_MAN_HINH / 2 + KICH_THUOC_KHOI)

#define SURIKEN_COUNT 3
#define SURIKEN_COOLDOWN 3000
#define SURIKEN_SPEED 0.1
#define SURIKEN_R 50
#define SURIKEN_FLY_SPEED 0.5
#define SURIKEN_ROAST 5

#define THOI_GIAN_BI_THUONG 300
#define THOI_GIAN_BAT_TU 1000
#define NHAN_VAT_DAY_MAU 10



struct Suriken {
    float x, y;
    float VanTocX_, VanTocY_;
    int KhungHinh_;
    float GocViTri_;
    bool DangBay_;
    bool KichHoat_;
    
    float GocQuay_;        
    float TocDoQuay_;    
};


class NVTrongBoss : public NhanVatGame
{
private:


    
    int MauHienTai_;
    bool DangBiThuong_;
    Uint32 ThoiGianBatDauBiThuong_;
    
    bool DaKichHoatMaThuat_;

    // =============== SURIKENN ========================
    DoiTuong AnhSuriken_;
    Suriken DanhSachSuriken_[SURIKEN_COUNT];
    float GocQuaySuriken_;

    int SoSurikenChuaPhong_;
    int SoSurikenDaPhong_;

    Uint32 LanSuDungSurikenCuoi_;
     

public:
    NVTrongBoss();
    ~NVTrongBoss();

    bool KhoiTao(SDL_Renderer* ManHinh);

    // =================== SURIKEN ==============================
    // quay
    bool TaiAnhSuriken(string DuongDan, SDL_Renderer* ManHinh);
    void VeSurikens(SDL_Renderer* renderer);
    void CapNhatSuriken();
    // bay
    void PhongSuriken(int mouseX, int mouseY);
    // Hồi
    void HoiLaiSuriken();
    // get set
    Suriken GetSuriken(int i){return DanhSachSuriken_[i];}
    void SetSuriken(int i, Suriken suriken){DanhSachSuriken_[i] = suriken;}

    // ==================================================================

    int GetViTriX(){return NhanVat_.ViTriNhanVatX_;}
    int GetViTriY(){return NhanVat_.ViTriNhanVatY_;}
    void SetViTriXY(float x, float y){NhanVat_.ViTriNhanVatX_ = x; NhanVat_.ViTriNhanVatY_ = y;}
    SDL_Texture* GetThongTinDoiTuong(){return NhanVat_.AnhCuaNhanVat_.ThongTinDoiTuong();}

    void HanhDongDauVao(SDL_Event SuKien);

    // ----------- Xử lý dự liệu đầu vào -----------
    void XuLyDiChuyen();

    void BatDauMotBuoc(int dx, int dy, HuongDi Huong);
    void TrongQuaTrinhDi();
    void KetThucBuoc();


    void VeNhanVat(SDL_Renderer *ManHinh);
    void VeThanhMauNhanVat(SDL_Renderer* ManHinh, TTF_Font* font);
    void VeChu(SDL_Renderer* ManHinh, TTF_Font* Font, const string& NoiDung, int x, int y, SDL_Color Mau);


    bool DaChet(){return MauHienTai_ <= 0;}
    void GayThuong();

    bool DangVaChamVoiVsMaThuat(int x, int y);
    void SetDaKichHoatMaThuat(bool b) { DaKichHoatMaThuat_ = b; }


    
};




#endif