#ifndef MENU_GAME_H
#define MENU_GAME_H

#include "HamChung.h"
#include "DoiTuong.h"
#include "AmThanh.h"

enum TrangThaiMenu {
    MENU_BAT_DAU,
    MENU_CAI_DAT,
    MENU_THOAT,
    MENU_KHONG_CHON
};  

enum TrangThaiMenuChinh{
    MENU_CHINH,
    MENU_CHON_DO_KHO
};

enum DoKho {
    DO_KHO_DE = 10,      // 10 sách
    DO_KHO_TRUNG_BINH = 20, // 20 sách  
    DO_KHO_KHO = 50      // 50 sách
};

class MenuGame {
public:
    MenuGame();
    ~MenuGame();

    // Tải ảnh nền và khởi tạo
    bool TaiHinhNen(const string& DuongDan, SDL_Renderer* ManHinh);
    
    // Xử lý sự kiện chuột/phím
    TrangThaiMenu XuLySuKien(SDL_Event& e);
    
    // Vẽ menu
    void VeMenuChinh(SDL_Renderer* ManHinh, TTF_Font* FontTieuDe, TTF_Font* FontNut);

    // Hàm phụ trợ
    void VeChu(SDL_Renderer* ManHinh, TTF_Font* Font, const string& NoiDung, int x, int y, SDL_Color Mau);
    void VeNut(SDL_Renderer* ManHinh, TTF_Font* Font, const string& NoiDung, SDL_Rect ViTri, bool DangChon);
    bool KiemTraChuotTrongNut(int mouseX, int mouseY, SDL_Rect Nut);

    // Menu độ khó
    DoKho LayDoKhoDaChon() const { return DoKhoDaChon_; }
    void VeMenuDoKho(SDL_Renderer* ManHinh, TTF_Font* FontTieuDe, TTF_Font* FontNut);

    // Vẽ tổng quát
    void Ve(SDL_Renderer* ManHinh, TTF_Font* FontTieuDe, TTF_Font* FontNut);





    

private:
    DoiTuong HinhNen_;
    
    // Vị trí các nút
    SDL_Rect NutBatDau_;
    SDL_Rect NutCaiDat_;
    SDL_Rect NutThoat_;
    
    TrangThaiMenu NutDangChon_; // nút đang được hover
    
    // === THÊM CÁC BIẾN MỚI ===
    TrangThaiMenuChinh TrangThaiHienTai_;

    DoKho DoKhoDaChon_;
    
    // Vị trí các nút độ khó
    SDL_Rect NutDe_;
    SDL_Rect NutTrungBinh_;
    SDL_Rect NutKho_;
    SDL_Rect NutQuayLai_;  // nút quay lại menu chính
    
    TrangThaiMenu NutDoKhoDangChon_;
    
};

#endif