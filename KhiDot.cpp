#include "KhiDot.h"

KhiDot::KhiDot(){
    // KHỞI TẠO CÁC GIÁ TRỊ MẶC ĐỊNH
    ViTriKhiDotX_ = 0;
    ViTriKhiDotY_ = 0;
    VanTocKhiDotX_ = 0;
    VanTocKhiDotY_ = 0;
    MauHienTai_ = KHI_DOT_DAY_MAU;
    DangBiThuong_ = false;
    ThoiGianBatDauBiThuong_ = 0;
    
    //Khởi tạo hệ thống tiến hóa và tấn công
    KhungHinhHienTai_ = 0;
    ThoiGianBatDauTienHoa_ = 0;
    DaTienHoa_ = false;
    DangTanCong_ = false;
    
}


KhiDot::~KhiDot(){;}


bool KhiDot::KhoiTao(SDL_Renderer* ManHinh, const int SO_LUONG_SACH){
    // TẢI ẢNH KHI DOT
    if(!AnhKhiDot_.TaiAnh("img/khi.png", ManHinh)){
        return false;
    }
    KHI_DOT_DAY_MAU = int(float(SO_LUONG_SACH) * 40.0f / 100.0f);
    ViTriKhiDotX_ = CHIEU_RONG_MAN_HINH / 2 + 100; // Bên phải boss
    ViTriKhiDotY_ = CHIEU_CAO_MAN_HINH / 2 + KICH_THUOC_KHOI;
    
    MauHienTai_ = KHI_DOT_DAY_MAU;
    DangBiThuong_ = false;
    ThoiGianBatDauBiThuong_ = 0;

    VanTocKhiDotX_ = 0;
    VanTocKhiDotY_ = 0;
    
    KhungHinhHienTai_ = 0;
    ThoiGianBatDauTienHoa_ = SDL_GetTicks();
    DaTienHoa_ = false;
    DangTanCong_ = false;
    TocDoTanCong_ = 0.8;
    
    for (int i = 0; i < SO_KHUNG_HINH_KHI_DOT; i++){
        SDL_Rect CatKhungHinh;
        CatKhungHinh.x = i * 16; // Mỗi khung hình rộng 16 pixel
        CatKhungHinh.y = 0;
        CatKhungHinh.w = 16;
        CatKhungHinh.h = 16;
        CacVungCatKhiDot_[i] = CatKhungHinh;
    }
    
    return true;
}

void KhiDot::CapNhatKhiDot(NVTrongBoss* NhanVat) {
    if (DaChet()) return;
    
    
    if (!DaTienHoa_) {
        Uint32 ThoiGianTienHoa = SDL_GetTicks() - ThoiGianBatDauTienHoa_;
        
        if (ThoiGianTienHoa < 1500) {
            KhungHinhHienTai_ = 0; // Khung hình 1 (yếu nhất)
        } 
        else if (ThoiGianTienHoa < 3000) {
            KhungHinhHienTai_ = 1; // Khung hình 2 (trung bình)
        } 
        else if (ThoiGianTienHoa < 4500) {
            KhungHinhHienTai_ = 2; // Khung hình 3 (mạnh nhất)
        } 
        else {
            AmThanhGame->PhatHieuUngAmThanh(SFX_KHI_DOT);
            DaTienHoa_ = true;
            DangTanCong_ = true;
            
            ViTriMucTieuX_ = NhanVat->GetViTriX();
            ViTriMucTieuY_ = NhanVat->GetViTriY();
            
            float KhoangCachX = ViTriMucTieuX_ - ViTriKhiDotX_;
            float KhoangCachY = ViTriMucTieuY_ - ViTriKhiDotY_;
            float KhoangCach = sqrt(KhoangCachX * KhoangCachX + KhoangCachY * KhoangCachY);
            
            if (KhoangCach > 0) {
                VanTocKhiDotX_ = (KhoangCachX / KhoangCach) * TOC_DO_TAN_CONG * min(2, (KHI_DOT_DAY_MAU / MauHienTai_));
                VanTocKhiDotY_ = (KhoangCachY / KhoangCach) * TOC_DO_TAN_CONG * min(2, (KHI_DOT_DAY_MAU / MauHienTai_));
            }
            
        }
    }
    else if (DangTanCong_) {
        
        DichDenKhiDotX_ = ViTriKhiDotX_ + VanTocKhiDotX_;
        DichDenKhiDotY_ = ViTriKhiDotY_ + VanTocKhiDotY_;

        if (DichDenKhiDotX_ < TRO_CHOI_CUOI.x || DichDenKhiDotX_ + KICH_THUOC_KHOI >= TRO_CHOI_CUOI.x + TRO_CHOI_CUOI.w ||
            DichDenKhiDotY_ <= TRO_CHOI_CUOI.y || DichDenKhiDotY_ + KICH_THUOC_KHOI >= TRO_CHOI_CUOI.y + TRO_CHOI_CUOI.h ) {
            
            VanTocKhiDotX_ = VanTocKhiDotY_ = 0;
            KhungHinhHienTai_ = 0;
            DaTienHoa_ = false;
            DangTanCong_ = false;
            ThoiGianBatDauTienHoa_ = SDL_GetTicks();
            return;
        }
    }

    ViTriKhiDotX_ += VanTocKhiDotX_;
    ViTriKhiDotY_ += VanTocKhiDotY_;
}



void KhiDot::GayThuong() {
    if (MauHienTai_ > 0) {
        AmThanhGame->PhatHieuUngAmThanh(SFX_KHI_DOT_BI_THUONG);
        MauHienTai_ -= 1;
        if (MauHienTai_ < 0) {
            MauHienTai_ = 0;
        }
        if (MauHienTai_ == 0){
            ViTriKhiDotX_ = -20;
            ViTriKhiDotY_ = -20;
            ThoiGianChet_ = SDL_GetTicks();
        }
        
        DangBiThuong_ = true;
        ThoiGianBatDauBiThuong_ = SDL_GetTicks();

    }
}


void KhiDot::VeKhiDot(SDL_Renderer *ManHinh, NVTrongBoss *NhanVat){
    if (DaChet()) return; 
    
    
    SDL_Rect CatTrongAnh = CacVungCatKhiDot_[KhungHinhHienTai_];

    SDL_Rect ViTriTrenManHinh = {
        (int)ViTriKhiDotX_, 
        (int)ViTriKhiDotY_,
        KICH_THUOC_KHOI,
        KICH_THUOC_KHOI
    };

    

    // Đổi màu khi bị thương 
    if (DangBiThuong_) {
        SDL_SetTextureColorMod(AnhKhiDot_.ThongTinDoiTuong(), 255, 100, 100); // Màu đỏ khi bị thương
        if (SDL_GetTicks() - ThoiGianBatDauBiThuong_ > THOI_GIAN_BI_THUONG) { \
            DangBiThuong_ = false;
        }
    } 
    else {
        SDL_SetTextureColorMod(AnhKhiDot_.ThongTinDoiTuong(), 255, 255, 255); // Màu bình thường
    }

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if(NhanVat->GetViTriX() > ViTriKhiDotX_){
        flip = SDL_FLIP_HORIZONTAL; //Lật khi nhìn phải 
    }
    else{
        flip = SDL_FLIP_NONE; // Không lật khi nhìn trái
    }


    SDL_RenderCopyEx(ManHinh, AnhKhiDot_.ThongTinDoiTuong(), 
                     &CatTrongAnh, &ViTriTrenManHinh,
                     0, nullptr, flip);
}


void KhiDot::VeThanhMauKhiDot(SDL_Renderer* ManHinh) {
    if (MauHienTai_ <= 0) return; 
    
    int ThanhMauX = (int)ViTriKhiDotX_ + 5; // Lùi vào 5 pixel từ mép trái
    int ThanhMauY = (int)ViTriKhiDotY_ + KICH_THUOC_KHOI + 2; // Dưới chân KhiDot + 2 pixel
    int ChieuRongThanh = KICH_THUOC_KHOI - 10; // Nhỏ hơn KhiDot 10 pixel (5 mỗi bên)
    int ChieuCaoThanh = 3; // Thanh rất mỏng chỉ 3 pixel
    
    SDL_Rect VienThanh = {ThanhMauX - 1, ThanhMauY - 1, ChieuRongThanh + 2, ChieuCaoThanh + 2};
    SDL_SetRenderDrawColor(ManHinh, 255, 255, 255, 255); // Trắng
    SDL_RenderFillRect(ManHinh, &VienThanh);
    
    SDL_Rect NenThanh = {ThanhMauX, ThanhMauY, ChieuRongThanh, ChieuCaoThanh};
    SDL_SetRenderDrawColor(ManHinh, 0, 0, 0, 255); // Đen
    SDL_RenderFillRect(ManHinh, &NenThanh);
    
    // Tính tỷ lệ máu còn lại
    float TyLeMau = (float)MauHienTai_ / (float)KHI_DOT_DAY_MAU;
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


void KhiDot::HoiSinh(){
    ViTriKhiDotX_ = CHIEU_RONG_MAN_HINH / 2 + 100; // Bên phải boss
    ViTriKhiDotY_ = CHIEU_CAO_MAN_HINH / 2 + KICH_THUOC_KHOI;
    
    MauHienTai_ = KHI_DOT_DAY_MAU;
    DangBiThuong_ = false;
    ThoiGianBatDauBiThuong_ = 0;

    VanTocKhiDotX_ = 0;
    VanTocKhiDotY_ = 0;
    
    KhungHinhHienTai_ = 0;
    ThoiGianBatDauTienHoa_ = SDL_GetTicks();
    DaTienHoa_ = false;
    DangTanCong_ = false;
}