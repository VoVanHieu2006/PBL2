#include "Boss.h"


Boss::Boss(){;}

Boss::~Boss(){;}




bool Boss::KhoiTao(SDL_Renderer* ManHinh, const int SO_LUONG_SACH){

    if(!AnhBoss_.TaiAnh("img/Rogue.png", ManHinh)){
        return false;
    }
    BOSS_DAY_MAU = SO_LUONG_SACH;

    ViTriBossX_ = VI_TRI_BOSS_X;
    ViTriBossY_ = VI_TRI_BOSS_Y;
    Huong_ = KHONG_CO_HUONG;

    for (int i = 0; i < SO_KHUNG_HINH_MOT_HANG; i++){
        for (int j = 0; j < SO_KHUNG_HINH_MOT_HANG; j++){
            SDL_Rect CatKhungHinh;
            CatKhungHinh.x = j * 16;
            CatKhungHinh.y = i * 16;
            CatKhungHinh.w = 16;
            CatKhungHinh.h = 16;
            CacVungCatNhanVat_[i][j] = CatKhungHinh;
        }
    }

    MauHienTai_ = BOSS_DAY_MAU;
    DangBiThuong_ = false;
    ThoiGianBatDauBiThuong_ = 0;


    if(!AnhMaThuat_.TaiAnh("img/mathuat.png", ManHinh)){
        return false;
    }

    for (int i = 0; i < SO_LUONG_MA_THUAT; i++){
        DanhSachMaThuat_[i].TrongAnhX_ = (i % 4) * 16;
        DanhSachMaThuat_[i].TrongAnhY_ = 0;
    }


    DangKichHoatMaThuat_ = true;

    return true;
    
}


void Boss::VeBoss(SDL_Renderer *ManHinh, NVTrongBoss *NhanVat){
    int ViTriNhanVatX = NhanVat->GetViTriX();
    SDL_Rect CatTrongAnh;
    if(ViTriNhanVatX < CHIEU_RONG_MAN_HINH / 2){
        Huong_ = HUONG_TRAI;
    }
    else{
        Huong_ = HUONG_PHAI;
    }
    
    CatTrongAnh = {
            0, 
            Huong_ * 16,
            16,
            16
        };

    SDL_Rect ViTriTrenManHinh = {
        ViTriBossX_, 
        ViTriBossY_,
        KICH_THUOC_KHOI,
        KICH_THUOC_KHOI
    };

    if (DangBiThuong_) {
        SDL_SetTextureColorMod(AnhBoss_.ThongTinDoiTuong(), 255, 100, 100); // Màu đỏ khi bị thương
        
        // Kiểm tra thời gian hiệu ứng bị thương
        if (SDL_GetTicks() - ThoiGianBatDauBiThuong_ > THOI_GIAN_BI_THUONG) { // 0.5 giây
            DangBiThuong_ = false;
        }
    } 
    else {
        SDL_SetTextureColorMod(AnhBoss_.ThongTinDoiTuong(), 255, 255, 255); // Màu bình thường
    }

    SDL_RenderCopy(ManHinh, AnhBoss_.ThongTinDoiTuong(), &CatTrongAnh, &ViTriTrenManHinh);

}




void Boss::GayThuong() {
    if (MauHienTai_ > 0) {
        AmThanhGame->PhatHieuUngAmThanh(SFX_BOSS_BI_THUONG);
        MauHienTai_ -= 1;
        if (MauHienTai_ < 0) {
            MauHienTai_ = 0;
        }
        
        DangBiThuong_ = true;
        ThoiGianBatDauBiThuong_ = SDL_GetTicks();
    }
}

void Boss::VeThanhMauBoss(SDL_Renderer* ManHinh, TTF_Font* font) {
    if (DangKichHoatMaThuat_) return;
    // Vẽ thanh máu boss ở góc trên phải
    int ThanhMauX = CHIEU_RONG_MAN_HINH - 250;
    int ThanhMauY = 10;
    int ChieuRongThanh = 200;
    int ChieuCaoThanh = 20;
    
    // Vẽ viền thanh máu
    SDL_Rect VienThanh = {ThanhMauX - 2, ThanhMauY - 2, ChieuRongThanh + 4, ChieuCaoThanh + 4};
    SDL_SetRenderDrawColor(ManHinh, 255, 255, 255, 255); // Màu trắng
    SDL_RenderFillRect(ManHinh, &VienThanh);
    
    // Vẽ nền thanh máu (màu đen)
    SDL_Rect NenThanh = {ThanhMauX, ThanhMauY, ChieuRongThanh, ChieuCaoThanh};
    SDL_SetRenderDrawColor(ManHinh, 0, 0, 0, 255); // Màu đen
    SDL_RenderFillRect(ManHinh, &NenThanh);
    
    // Tính tỷ lệ máu còn lại
    float TyLeMau = (float)MauHienTai_ / (float)BOSS_DAY_MAU;
    int ChieuRongMau = (int)(ChieuRongThanh * TyLeMau);
    
    // Chọn màu thanh máu dựa trên tỷ lệ máu
    SDL_Color MauThanh;
    if (TyLeMau > 0.6f) {
        MauThanh = {0, 255, 0, 255}; // Xanh lá
    } else if (TyLeMau > 0.3f) {
        MauThanh = {255, 255, 0, 255}; // Vàng
    } else {
        MauThanh = {255, 0, 0, 255}; // Đỏ
    }
    
    // Vẽ thanh máu
    if (ChieuRongMau > 0) {
        SDL_Rect ThanhMau = {ThanhMauX, ThanhMauY, ChieuRongMau, ChieuCaoThanh};
        SDL_SetRenderDrawColor(ManHinh, MauThanh.r, MauThanh.g, MauThanh.b, MauThanh.a);
        SDL_RenderFillRect(ManHinh, &ThanhMau);
    }
    
    // Vẽ text hiển thị số máu
    string TextMau = "Boss HP: " + to_string(MauHienTai_) + "/" + to_string(BOSS_DAY_MAU);
    VeChu(ManHinh, font, TextMau, ThanhMauX, ThanhMauY + ChieuCaoThanh + 5, {255, 255, 255, 255});
}

void Boss::VeChu(SDL_Renderer* ManHinh, TTF_Font* Font, const string& NoiDung,
                     int x, int y, SDL_Color Mau) {
    if (!Font) return;
    
    SDL_Surface* ChuNen = TTF_RenderUTF8_Blended(Font, NoiDung.c_str(), Mau);
    if (!ChuNen) return;
    
    SDL_Texture* ChuThat = SDL_CreateTextureFromSurface(ManHinh, ChuNen);
    SDL_Rect ViTri = {x, y, ChuNen->w, ChuNen->h};
    SDL_FreeSurface(ChuNen);
    
    if (ChuThat) {
        SDL_RenderCopy(ManHinh, ChuThat, nullptr, &ViTri);
        SDL_DestroyTexture(ChuThat);
    }
}



void Boss::VeMaThuat(SDL_Renderer* ManHinh){
    if (!DangKichHoatMaThuat_) return;
    for (int i = 0; i < SO_LUONG_MA_THUAT; i++){
        SDL_Rect CatTrongAnh = {
            DanhSachMaThuat_[i].TrongAnhX_,
            DanhSachMaThuat_[i].TrongAnhY_,
            16,
            16
        };
        SDL_Rect ViTriTrenManHinh = {
            VI_TRI_BOSS_X - KICH_THUOC_KHOI * dx[i],
            VI_TRI_BOSS_Y - KICH_THUOC_KHOI * dy[i],
            32,
            32
        };
        SDL_RenderCopy(ManHinh, AnhMaThuat_.ThongTinDoiTuong(), &CatTrongAnh, &ViTriTrenManHinh);
    }
}