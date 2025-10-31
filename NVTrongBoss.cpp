#include "NVTrongBoss.h"


NVTrongBoss::NVTrongBoss(){

}

NVTrongBoss::~NVTrongBoss(){
}


bool NVTrongBoss::KhoiTao(SDL_Renderer* ManHinh){
    TaiAnhNhanVat("img/Warrior.png", ManHinh);
    TaiAnhSuriken("img/suriken.png", ManHinh);
    for (int i = 0; i < SURIKEN_COUNT; i++) {
        DanhSachSuriken_[i].x = 0;
        DanhSachSuriken_[i].y = 0;
        DanhSachSuriken_[i].VanTocX_ = 0;
        DanhSachSuriken_[i].VanTocY_ = 0;
        DanhSachSuriken_[i].KhungHinh_ = i;
        DanhSachSuriken_[i].GocViTri_ = (360 / SURIKEN_COUNT) * i;
        DanhSachSuriken_[i].DangBay_ = false;
        DanhSachSuriken_[i].KichHoat_ = true;

        DanhSachSuriken_[i].GocQuay_ = 0;    
        DanhSachSuriken_[i].TocDoQuay_ = SURIKEN_ROAST;   
    }
    NhanVat_.ViTriNhanVatX_ = CHIEU_RONG_MAN_HINH / 2;
    NhanVat_.ViTriNhanVatY_ = KICH_THUOC_KHOI * 3;
    SoSurikenChuaPhong_ = SURIKEN_COUNT;
    SoSurikenDaPhong_ = 0;

    MauHienTai_ = NHAN_VAT_DAY_MAU;
    DangBiThuong_ = false;
    ThoiGianBatDauBiThuong_ = 0;

    return true;

}



void NVTrongBoss::HanhDongDauVao(SDL_Event SuKien){

   

    if (SuKien.type == SDL_KEYDOWN){


        switch(SuKien.key.keysym.sym){

            case SDLK_RIGHT:
            {
                DauVao_.Phai_ = 1;
                DauVao_.Trai_ = 0;
                DauVao_.Tren_ = 0;
                DauVao_.Duoi_ = 0;
                
            }
            break;
            
            case SDLK_LEFT:
            {
                DauVao_.Phai_ = 0;
                DauVao_.Trai_ = 1;
                DauVao_.Tren_ = 0;
                DauVao_.Duoi_ = 0;
                
            }
            break;

            case SDLK_UP:
            {
                DauVao_.Phai_ = 0;
                DauVao_.Trai_ = 0;
                DauVao_.Tren_ = 1;
                DauVao_.Duoi_ = 0;
            }
            break;
            
            case SDLK_DOWN:
            {
                DauVao_.Phai_ = 0;
                DauVao_.Trai_ = 0;
                DauVao_.Tren_ = 0;
                DauVao_.Duoi_ = 1;
            }
            break;

        }
    }
    else if (SuKien.type == SDL_KEYUP){
        
        switch(SuKien.key.keysym.sym){

            case SDLK_RIGHT:
            {
                DauVao_.Phai_ = 0;
            }
            break;
            
            case SDLK_LEFT:
            {
                DauVao_.Trai_ = 0;
            }
            break;

            case SDLK_UP:
            {
                DauVao_.Tren_ = 0;
            }
            break;

            case SDLK_DOWN:
            {
                DauVao_.Duoi_ = 0;
            }
            break;
        }
    }
    
     if (SuKien.type == SDL_MOUSEBUTTONDOWN && SuKien.button.button == SDL_BUTTON_LEFT) {
        PhongSuriken(SuKien.button.x, SuKien.button.y);
    }


}

// ============== DI CHUYỂN ======================

void NVTrongBoss::XuLyDiChuyen(){
    if (DangDiDenO_){
        TrongQuaTrinhDi();
    }
    else{
       
        if (DauVao_.Trai_) BatDauMotBuoc(-1, 0, HUONG_TRAI);
        else if (DauVao_.Phai_) BatDauMotBuoc(1, 0, HUONG_PHAI);
        else if (DauVao_.Tren_) BatDauMotBuoc(0, -1, HUONG_TREN);
        else if (DauVao_.Duoi_) BatDauMotBuoc(0, 1, HUONG_DUOI);
    }
    
}

void NVTrongBoss::BatDauMotBuoc(int dx, int dy, HuongDi Huong){
    if (DangDiDenO_) return;
    AmThanhGame->DatAmLuongHieuUng(20);
    AmThanhGame->PhatHieuUngAmThanh(SFX_DI_CHUYEN);
    AmThanhGame->DatAmLuongHieuUng(96);
    XuatPhatX_ = NhanVat_.ViTriNhanVatX_;
    XuatPhatY_ = NhanVat_.ViTriNhanVatY_;

    DichDenX_ = NhanVat_.ViTriNhanVatX_ + dx * KICH_THUOC_KHOI;
    DichDenY_ = NhanVat_.ViTriNhanVatY_ + dy * KICH_THUOC_KHOI;

    if (DichDenX_ < TRO_CHOI_CUOI.x || DichDenX_ +  KICH_THUOC_KHOI >  TRO_CHOI_CUOI.x + TRO_CHOI_CUOI.w
    ||  DichDenY_ < TRO_CHOI_CUOI.y || DichDenY_ +  KICH_THUOC_KHOI > TRO_CHOI_CUOI.y + TRO_CHOI_CUOI.h)  return;
    
    if(DangVaChamVoiVsMaThuat(DichDenX_, DichDenY_)){
        return;
    }

    float ThoiGianGiayTam = (float) KICH_THUOC_KHOI / (float) TOC_DO_PIXEL_MOT_GIAY;
    ThoiGianCanBuoc_ = (Uint32)(ThoiGianGiayTam * 1000.0);

    ThoiGianBatDauBuoc_ = SDL_GetTicks();
    NhanVat_.Huong_ = Huong;
    DangDiDenO_ = true;
}


void NVTrongBoss::TrongQuaTrinhDi(){
    if (!DangDiDenO_) return;

    Uint32 ThoiGianDaTroiQua = SDL_GetTicks() - ThoiGianBatDauBuoc_;
    float TienDo = (float) ThoiGianDaTroiQua / (float) ThoiGianCanBuoc_;
    // tính xem đã bao nhiêu % thòi gian đã trôi qua

    int DentaX = DichDenX_ - XuatPhatX_;
    int DentaY = DichDenY_ - XuatPhatY_;
    NhanVat_.ViTriNhanVatX_ = XuatPhatX_ + (int) (DentaX * TienDo);
    NhanVat_.ViTriNhanVatY_ = XuatPhatY_ + (int) (DentaY * TienDo);

    int KhungHinhHienTai = (int) (TienDo * SO_KHUNG_HINH_MOT_HANG);
    if (KhungHinhHienTai > SO_KHUNG_HINH_MOT_HANG) KhungHinhHienTai = SO_KHUNG_HINH_MOT_HANG - 1;
    NhanVat_.KhungHinh_ = KhungHinhHienTai;
    if(TienDo > 1.0){
        KetThucBuoc();
    }
    
}


void NVTrongBoss::KetThucBuoc(){
    
    NhanVat_.ViTriNhanVatX_ = DichDenX_;
    NhanVat_.ViTriNhanVatY_ = DichDenY_;
    DangDiDenO_ = false;
    NhanVat_.KhungHinh_ = 0;
}

//=================================================================


// ============== SURIKEN ======================



bool NVTrongBoss::TaiAnhSuriken(string DuongDan, SDL_Renderer* ManHinh){
    if (!AnhSuriken_.TaiAnh(DuongDan, ManHinh)) {
        cout << "Loi tai anh suriken" << endl;
        return false;
    }
    return true;
}

void NVTrongBoss::CapNhatSuriken(){
    HoiLaiSuriken();
    // TÂM nhân vật tính bằng pixel
    float TamNhanVatX = NhanVat_.ViTriNhanVatX_ + (KICH_THUOC_NHAN_VAT / 2.0f);
    float TamNhanVatY = NhanVat_.ViTriNhanVatY_ + (KICH_THUOC_NHAN_VAT / 2.0f);
    
    // Cập nhật góc quay chung (chung cho 3 cái )
    GocQuaySuriken_ += SURIKEN_SPEED;
    if (GocQuaySuriken_ >= 360.0f) {
        GocQuaySuriken_ -= 360.0f;
    }
    
    for (auto& suriken : DanhSachSuriken_) {
        if (!suriken.KichHoat_) continue;

        if (suriken.DangBay_){
            suriken.x += suriken.VanTocX_;
            suriken.y += suriken.VanTocY_;

            suriken.GocQuay_ += suriken.TocDoQuay_; // Quay xung quanh trục
            if (suriken.GocQuay_ >= 360.0f) {
                suriken.GocQuay_ -= 360.0f;
            }  

            if (suriken.x < 0 || suriken.x > CHIEU_RONG_MAN_HINH 
            ||  suriken.y < 0 || suriken.y > CHIEU_CAO_MAN_HINH){
                suriken.KichHoat_ = false;
            }
        }
        else{
            float GocHienTai = GocQuaySuriken_ + suriken.GocViTri_;
            
            // Chuyển đổi từ độ sang radian
            float GocRadian = GocHienTai * M_PI / 180.0f;
        

            suriken.x = TamNhanVatX + SURIKEN_R * cos(GocRadian);
            suriken.y = TamNhanVatY + SURIKEN_R * sin(GocRadian);


            
        }
       
    }
}

void NVTrongBoss::VeSurikens(SDL_Renderer* ManHinh) {
    for (const auto& suriken : DanhSachSuriken_) {
        if (!suriken.KichHoat_) continue;
        SDL_Rect CatSurikenTrenHinh = {suriken.KhungHinh_ * 16, 0, 16, 16};
        SDL_Rect ViTriTrenManHinh = {
            (int)(suriken.x - KICH_THUOC_KHOI / 2),  // chỉnh cho nó trúng xung quanh
            (int)(suriken.y - KICH_THUOC_KHOI / 2),  
            KICH_THUOC_KHOI,
            KICH_THUOC_KHOI
        };
        SDL_RenderCopyEx(ManHinh, AnhSuriken_.ThongTinDoiTuong(), 
                           &CatSurikenTrenHinh, &ViTriTrenManHinh,
                           suriken.GocQuay_, 
                           nullptr,           // Quay quanh center
                           SDL_FLIP_NONE);
        
       
    }
}


void NVTrongBoss::PhongSuriken(int mouseX, int mouseY) {
    if (SoSurikenChuaPhong_ <= 0) return;
    AmThanhGame->PhatHieuUngAmThanh(SFX_PHONG_SURIKEN);
    Suriken &SurikenMoi = DanhSachSuriken_[SoSurikenDaPhong_++];
    SurikenMoi.DangBay_ = true;
    
    // Tính hướng bay đến vị trí chuột
    float KhoangCachTheoX = mouseX - SurikenMoi.x;
    float KhoangCachTheoY = mouseY - SurikenMoi.y;
    float KhoangCach = sqrt(KhoangCachTheoX * KhoangCachTheoX + KhoangCachTheoY * KhoangCachTheoY);
    
    if (KhoangCach > 0) {
        SurikenMoi.VanTocX_ = (KhoangCachTheoX / KhoangCach) * SURIKEN_FLY_SPEED;
        SurikenMoi.VanTocY_ = (KhoangCachTheoY / KhoangCach) * SURIKEN_FLY_SPEED;
    }
    
    // Giảm số suriken khả dụng
    SoSurikenChuaPhong_--;
    LanSuDungSurikenCuoi_ = SDL_GetTicks();
}

void NVTrongBoss::HoiLaiSuriken(){
    if (SoSurikenChuaPhong_ > 0) return;
    Uint32 ThoiGianHienTai = SDL_GetTicks();
    if (ThoiGianHienTai - LanSuDungSurikenCuoi_ < SURIKEN_COOLDOWN) return;
    for (int i = 0; i < SURIKEN_COUNT; i++) {
        DanhSachSuriken_[i].x = 0;
        DanhSachSuriken_[i].y = 0;
        DanhSachSuriken_[i].VanTocX_ = 0;
        DanhSachSuriken_[i].VanTocY_ = 0;
        DanhSachSuriken_[i].KhungHinh_ = i;
        DanhSachSuriken_[i].GocViTri_ = (360 / SURIKEN_COUNT) * i;
        DanhSachSuriken_[i].DangBay_ = false;
        DanhSachSuriken_[i].KichHoat_ = true;

        DanhSachSuriken_[i].GocQuay_ = 0;    
        DanhSachSuriken_[i].TocDoQuay_ = SURIKEN_ROAST;  
    }
    SoSurikenDaPhong_ = 0;
    SoSurikenChuaPhong_ = SURIKEN_COUNT;
}

// ============================================



void NVTrongBoss::VeChu(SDL_Renderer* ManHinh, TTF_Font* Font, const string& NoiDung,
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

void NVTrongBoss::GayThuong() {
    if (SDL_GetTicks() - ThoiGianBatDauBiThuong_ < THOI_GIAN_BAT_TU) return;
    if (MauHienTai_ > 0) {
        AmThanhGame->PhatHieuUngAmThanh(SFX_NHAN_VAT_BI_THUONG);
        MauHienTai_ -= 1;
        if (MauHienTai_ < 0) {
            MauHienTai_ = 0;
        }
        
        DangBiThuong_ = true;
        ThoiGianBatDauBiThuong_ = SDL_GetTicks();

    }
}

void NVTrongBoss::VeNhanVat(SDL_Renderer *ManHinh){
    SDL_Texture *AnhNhanVatCanVe = NhanVat_.AnhCuaNhanVat_.ThongTinDoiTuong();
    if (AnhNhanVatCanVe == NULL){
        cout << "Loi anh nhan vat can ve" << endl;
    }
    
    int Cot = 0;
    switch (NhanVat_.Huong_) {
        case HUONG_DUOI: Cot = 0; break;
        case HUONG_TRAI: Cot = 1; break;
        case HUONG_PHAI: Cot = 2; break;
        case HUONG_TREN: Cot = 3; break;
    }


    SDL_Rect KhungHinhHienTai = NhanVat_.CacVungCatNhanVat_[Cot][NhanVat_.KhungHinh_];

    SDL_Rect ViTriTrenManHinh = {NhanVat_.ViTriNhanVatX_, NhanVat_.ViTriNhanVatY_, KICH_THUOC_NHAN_VAT, KICH_THUOC_NHAN_VAT};

    if (DangBiThuong_) {
        SDL_SetTextureColorMod(NhanVat_.AnhCuaNhanVat_.ThongTinDoiTuong(), 255, 100, 100); // Đỏ khi bị thương
        if (SDL_GetTicks() - ThoiGianBatDauBiThuong_ > THOI_GIAN_BI_THUONG) {
            DangBiThuong_ = false;
        }
    }
    else{
        SDL_SetTextureColorMod(NhanVat_.AnhCuaNhanVat_.ThongTinDoiTuong(), 255, 255, 255); // Màu bình thường
    }

    SDL_RenderCopy(ManHinh, NhanVat_.AnhCuaNhanVat_.ThongTinDoiTuong(), &KhungHinhHienTai, &ViTriTrenManHinh);

}


void NVTrongBoss::VeThanhMauNhanVat(SDL_Renderer* ManHinh, TTF_Font* font) {
    if (MauHienTai_ <= 0) return; 
    
    int ThanhMauX = (int)NhanVat_.ViTriNhanVatX_ + 5; 
    int ThanhMauY = (int)NhanVat_.ViTriNhanVatY_ + KICH_THUOC_KHOI + 2; 
    int ChieuRongThanh = KICH_THUOC_KHOI - 10; 
    int ChieuCaoThanh = 3; 
    
    SDL_Rect VienThanh = {ThanhMauX - 1, ThanhMauY - 1, ChieuRongThanh + 2, ChieuCaoThanh + 2};
    SDL_SetRenderDrawColor(ManHinh, 255, 255, 255, 255); // Trắng
    SDL_RenderFillRect(ManHinh, &VienThanh);
    
    SDL_Rect NenThanh = {ThanhMauX, ThanhMauY, ChieuRongThanh, ChieuCaoThanh};
    SDL_SetRenderDrawColor(ManHinh, 0, 0, 0, 255); // Đen
    SDL_RenderFillRect(ManHinh, &NenThanh);
    
    // Tính tỷ lệ máu còn lại
    float TyLeMau = (float)MauHienTai_ / (float)NHAN_VAT_DAY_MAU;
    int ChieuRongMau = (int)(ChieuRongThanh * TyLeMau);
    
    // Chọn màu thanh máu dựa trên tỷ lệ máu
    if (TyLeMau > 0.6f) {
        SDL_SetRenderDrawColor(ManHinh, 0, 255, 0, 255); // Xanh lá (nhiều máu)
    } else if (TyLeMau > 0.3f) {
        SDL_SetRenderDrawColor(ManHinh, 255, 255, 0, 255); // Vàng (máu trung bình)
    } else {
        SDL_SetRenderDrawColor(ManHinh, 255, 0, 0, 255); // Đỏ (ít máu)
    }
    
    // Vẽ thanh máu
    if (ChieuRongMau > 0) {
        SDL_Rect ThanhMau = {ThanhMauX, ThanhMauY, ChieuRongMau, ChieuCaoThanh};
        SDL_RenderFillRect(ManHinh, &ThanhMau);
    }
}



bool NVTrongBoss::DangVaChamVoiVsMaThuat(int x, int y) {
    
    // KIỂM TRA VÙNG MA THUẬT (3x3 ô xung quanh boss)
    SDL_Rect VungMaThuat = {
        VI_TRI_BOSS_X - KICH_THUOC_KHOI,
        VI_TRI_BOSS_Y - KICH_THUOC_KHOI,
        KICH_THUOC_KHOI * 3,
        KICH_THUOC_KHOI * 3
    };
    
    SDL_Rect VungNhanVat = {
        x, y,
        KICH_THUOC_KHOI,
        KICH_THUOC_KHOI
    };
    
    // CHỈ CHẶN KHI MA THUẬT ĐANG KÍCH HOẠT
    if (DaKichHoatMaThuat_) {
        return SDL_HasIntersection(&VungNhanVat, &VungMaThuat);
    }
    
    return false;
}