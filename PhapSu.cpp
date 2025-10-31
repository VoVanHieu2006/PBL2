#include "PhapSu.h"

PhapSu::PhapSu(){
    
}

PhapSu::~PhapSu(){;}

bool PhapSu::KhoiTao(SDL_Renderer* ManHinh, int GocTuong, const int SO_LUONG_SACH){
    
    if(!AnhPhapSu_.TaiAnh("img/bocap.png", ManHinh)){
        return false;
    }

    if(!AnhPhepThuat_.TaiAnh("img/phep.png", ManHinh)){
        return false;
    }
    PHAP_SU_DAY_MAU = int(float(SO_LUONG_SACH) * 40.0f / 100.0f);

    GocTuong_ = GocTuong;
    
    switch(GocTuong_) {
        case 0: // Góc trên trái
            ViTriPhapSuX_ = TRO_CHOI_CUOI.x - KICH_THUOC_KHOI;
            ViTriPhapSuY_ = TRO_CHOI_CUOI.y - KICH_THUOC_KHOI;
            break;
            
        case 1: // Góc trên phải  
            ViTriPhapSuX_ = TRO_CHOI_CUOI.x + TRO_CHOI_CUOI.w;
            ViTriPhapSuY_ = TRO_CHOI_CUOI.y - KICH_THUOC_KHOI;
            break;
            
        case 2: // Góc dưới trái
            ViTriPhapSuX_ = TRO_CHOI_CUOI.x - KICH_THUOC_KHOI;
            ViTriPhapSuY_ = TRO_CHOI_CUOI.y + TRO_CHOI_CUOI.h;
            break;
            
        case 3: // Góc dưới phải
            ViTriPhapSuX_ = TRO_CHOI_CUOI.x + TRO_CHOI_CUOI.w ;
            ViTriPhapSuY_ = TRO_CHOI_CUOI.y + TRO_CHOI_CUOI.h ;
            break;
            
            
        default:
            ViTriPhapSuX_ = TRO_CHOI_CUOI.x - KICH_THUOC_KHOI;
            ViTriPhapSuY_ = TRO_CHOI_CUOI.y - KICH_THUOC_KHOI;
            break;
    }
    
    MauHienTai_ = PHAP_SU_DAY_MAU;
    DangBiThuong_ = false;
    ThoiGianBatDauBiThuong_ = 0;
    
    KhungHinhHienTai_ = 0;
    ThoiGianBatDauTienHoa_ = SDL_GetTicks();
    DaTienHoa_ = false;
    DangTanCong_ = false;
    
    for (int i = 0; i < SO_KHUNG_HINH_PHAP_SU; i++){
        SDL_Rect CatKhungHinh;
        CatKhungHinh.x = i * 16; // Mỗi khung hình rộng 16 pixel
        CatKhungHinh.y = 0;
        CatKhungHinh.w = 16;
        CatKhungHinh.h = 16;
        CacVungCatPhapSu_[i] = CatKhungHinh;
    }


    PhepThuat_.ViTriPhepThuatX_ = ViTriPhapSuX_;
    PhepThuat_.ViTriPhepThuatY_ = ViTriPhapSuY_;
    PhepThuat_.VanTocPhepThuatX_ = 0;
    PhepThuat_.VanTocPhepThuatY_ = 0;


    
    return true;
}

void PhapSu::CapNhatPhapSu(NVTrongBoss* NhanVat) {
    if (DangTanCong_){
        PhepThuat_.ViTriPhepThuatX_ += PhepThuat_.VanTocPhepThuatX_;
        PhepThuat_.ViTriPhepThuatY_ += PhepThuat_.VanTocPhepThuatY_;

        if (PhepThuat_.ViTriPhepThuatX_ < 0 || PhepThuat_.ViTriPhepThuatX_ + KICH_THUOC_KHOI > CHIEU_RONG_MAN_HINH ||
            PhepThuat_.ViTriPhepThuatY_ < 0 || PhepThuat_.ViTriPhepThuatY_ + KICH_THUOC_KHOI > CHIEU_CAO_MAN_HINH){
            PhepThuat_.ViTriPhepThuatX_ = ViTriPhapSuX_;
            PhepThuat_.ViTriPhepThuatY_ = ViTriPhapSuY_;
            PhepThuat_.VanTocPhepThuatX_ = 0;
            PhepThuat_.VanTocPhepThuatY_ = 0;
            KhungHinhHienTai_ = 0;
            ThoiGianBatDauTienHoa_ = SDL_GetTicks();
            DaTienHoa_ = false;
            DangTanCong_ = false;
        }
    }

    if (DaChet()) return;
    
    
    if (!DaTienHoa_) {
        Uint32 ThoiGianTienHoa = SDL_GetTicks() - ThoiGianBatDauTienHoa_;
        
        if (ThoiGianTienHoa < 1000) {
            KhungHinhHienTai_ = 0; // Giai đoạn 1 (yếu nhất)
        } 
        else if (ThoiGianTienHoa < 2000) {
            KhungHinhHienTai_ = 1; // Giai đoạn 2
        } 
        else if (ThoiGianTienHoa < 3000) {
            KhungHinhHienTai_ = 2; // Giai đoạn 3
        } 
        else if (ThoiGianTienHoa < 4000) {
            KhungHinhHienTai_ = 3; // Giai đoạn 4 (mạnh nhất)
        }
        else {
            DaTienHoa_ = true;
            DangTanCong_ = true;
            
            PhongPhepThuat(NhanVat);
        }
    }

    
}

void PhapSu::PhongPhepThuat(NVTrongBoss *NhanVat){
    AmThanhGame->PhatHieuUngAmThanh(SFX_PHAP_SU);
    float NguoiChoiX = NhanVat->GetViTriX();
    float NguoiChoiY = NhanVat->GetViTriY();
    
    float KhoangCachX = NguoiChoiX - ViTriPhapSuX_;
    float KhoangCachY = NguoiChoiY - ViTriPhapSuY_;
    float KhoangCach = sqrt(KhoangCachX * KhoangCachX + KhoangCachY * KhoangCachY);
    
    if (KhoangCach > 0) {
        // Đặt vị trí xuất phát của phép thuật tại pháp sư
        
        PhepThuat_.VanTocPhepThuatX_ = (KhoangCachX / KhoangCach * TOC_DO_PHEP_THUAT) * min(2, (PHAP_SU_DAY_MAU / MauHienTai_));
        PhepThuat_.VanTocPhepThuatY_ = (KhoangCachY / KhoangCach * TOC_DO_PHEP_THUAT) * min(2, (PHAP_SU_DAY_MAU / MauHienTai_));
        
        if (PhepThuat_.ViTriPhepThuatX_ < NguoiChoiX) PhepThuat_.Huong_ = HUONG_TRAI;
        else PhepThuat_.Huong_ = HUONG_PHAI;
    }


}




void PhapSu::VePhepThuat(SDL_Renderer* ManHinh){
    if (!DangTanCong_) return;

    

    
    SDL_Rect CatPhepThuat = {
        0,
        0,
        16,
        16
    };
    
    SDL_Rect ViTriPhepThuat = {
        int(PhepThuat_.ViTriPhepThuatX_), 
        int(PhepThuat_.ViTriPhepThuatY_),
        KICH_THUOC_KHOI , 
        KICH_THUOC_KHOI
    };
    
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if(PhepThuat_.Huong_ == HUONG_TRAI){
        flip = SDL_FLIP_HORIZONTAL; 
    }
    else{
        flip = SDL_FLIP_NONE; 
    }
    
    SDL_SetTextureColorMod(AnhPhepThuat_.ThongTinDoiTuong(), 
                          200 + (sin(SDL_GetTicks() * 0.01f) * 55), // Đỏ nhấp nháy
                          100 + (cos(SDL_GetTicks() * 0.01f) * 100), // Xanh lá nhấp nháy  
                          255); // Xanh dương cố định
    
    SDL_RenderCopyEx(ManHinh, AnhPhepThuat_.ThongTinDoiTuong(),
                    &CatPhepThuat, &ViTriPhepThuat,
                    0, 
                    nullptr, 
                    flip);
    
    
    // Reset alpha về bình thường
    SDL_SetTextureAlphaMod(AnhPhepThuat_.ThongTinDoiTuong(), 255);
}


void PhapSu::GayThuong() {
    if (MauHienTai_ > 0) {
        AmThanhGame->PhatHieuUngAmThanh(SFX_PHAP_SU_BI_THUONG);
        MauHienTai_ -= 1;
        if (MauHienTai_ < 0) {
            MauHienTai_ = 0;
        }
        
        if (MauHienTai_ == 0){
            ViTriPhapSuX_ = -20;
            ViTriPhapSuY_ = -20;
            ThoiGianChet_ = SDL_GetTicks();
        }
        
        DangBiThuong_ = true;
        ThoiGianBatDauBiThuong_ = SDL_GetTicks();

    }
}

void PhapSu::VePhapSu(SDL_Renderer *ManHinh, NVTrongBoss *NhanVat){
    if (DaChet()) return; 
    
    SDL_Rect CatTrongAnh = CacVungCatPhapSu_[KhungHinhHienTai_];

    SDL_Rect ViTriTrenManHinh = {
        (int)ViTriPhapSuX_, 
        (int)ViTriPhapSuY_,
        KICH_THUOC_KHOI,
        KICH_THUOC_KHOI
    };

    if (DangBiThuong_) {
        SDL_SetTextureColorMod(AnhPhapSu_.ThongTinDoiTuong(), 255, 100, 100); // Đỏ khi bị thương
        if (SDL_GetTicks() - ThoiGianBatDauBiThuong_ > THOI_GIAN_BI_THUONG) {
            DangBiThuong_ = false;
        }
    }
    else{
        SDL_SetTextureColorMod(AnhPhapSu_.ThongTinDoiTuong(), 255, 255, 255); // Màu bình thường
    }

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    float TrungTamX = CHIEU_RONG_MAN_HINH / 2;
    
    if(ViTriPhapSuX_ > TrungTamX){
        flip = SDL_FLIP_HORIZONTAL; // Góc trái nhìn trais
    }
    else{
        flip = SDL_FLIP_NONE; // Góc phải nhìn phải
    }

    
    SDL_RenderCopyEx(ManHinh, AnhPhapSu_.ThongTinDoiTuong(), 
                        &CatTrongAnh, &ViTriTrenManHinh, 0, nullptr, flip);
    
}

void PhapSu::VeThanhMauPhapSu(SDL_Renderer* ManHinh) {
    if (DaChet()) return;
    
    int ThanhMauX = (int)ViTriPhapSuX_ + 5;
    int ThanhMauY = (int)ViTriPhapSuY_ + KICH_THUOC_KHOI + 5;
    int ChieuRongThanh = KICH_THUOC_KHOI - 10;
    int ChieuCaoThanh = 4; // Thanh hơi dày hơn
    
    // Viền vàng cho pháp sư
    SDL_Rect VienThanh = {ThanhMauX - 1, ThanhMauY - 1, ChieuRongThanh + 2, ChieuCaoThanh + 2};
    SDL_SetRenderDrawColor(ManHinh, 255, 215, 0, 255); // Vàng
    SDL_RenderFillRect(ManHinh, &VienThanh);
    
    // Nền đen
    SDL_Rect NenThanh = {ThanhMauX, ThanhMauY, ChieuRongThanh, ChieuCaoThanh};
    SDL_SetRenderDrawColor(ManHinh, 0, 0, 0, 255);
    SDL_RenderFillRect(ManHinh, &NenThanh);
    
    // Tính tỷ lệ máu
    float TyKICH_THUOC_KHOIMau = (float)MauHienTai_ / (float)PHAP_SU_DAY_MAU;
    int ChieuRongMau = (int)(ChieuRongThanh * TyKICH_THUOC_KHOIMau);
    
    // Màu thanh máu pháp sư (tím -> xanh dương -> tím đậm)
    if (TyKICH_THUOC_KHOIMau > 0.6f) {
        SDL_SetRenderDrawColor(ManHinh, 138, 43, 226, 255);  // Tím đậm (nhiều máu)
    } else if (TyKICH_THUOC_KHOIMau > 0.3f) {
        SDL_SetRenderDrawColor(ManHinh, 75, 0, 130, 255);    // Tím navy (máu trung bình)
    } else {
        SDL_SetRenderDrawColor(ManHinh, 255, 20, 147, 255);  // Hồng đậm (ít máu)
    }
    
    // Vẽ thanh máu
    if (ChieuRongMau > 0) {
        SDL_Rect ThanhMau = {ThanhMauX, ThanhMauY, ChieuRongMau, ChieuCaoThanh};
        SDL_RenderFillRect(ManHinh, &ThanhMau);
    }
    
    // Hiệu ứng lấp lánh khi đã tiến hóa xong
    if (DaTienHoa_) {
        static Uint32 thoiGianLapLanh = 0;
        if (SDL_GetTicks() - thoiGianLapLanh > 500) { // Lấp lánh mỗi 0.5 giây
            // Vẽ viền lấp lánh
            SDL_SetRenderDrawColor(ManHinh, 255, 255, 255, 255); // Trắng
            SDL_RenderDrawRect(ManHinh, &VienThanh);
            thoiGianLapLanh = SDL_GetTicks();
        }
    }
}


void PhapSu::HoiSinh(){
    switch(GocTuong_) {
        case 0: // Góc trên trái
            ViTriPhapSuX_ = TRO_CHOI_CUOI.x - KICH_THUOC_KHOI;
            ViTriPhapSuY_ = TRO_CHOI_CUOI.y - KICH_THUOC_KHOI;
            break;
            
        case 1: // Góc trên phải  
            ViTriPhapSuX_ = TRO_CHOI_CUOI.x + TRO_CHOI_CUOI.w;
            ViTriPhapSuY_ = TRO_CHOI_CUOI.y - KICH_THUOC_KHOI;
            break;
            
        case 2: // Góc dưới trái
            ViTriPhapSuX_ = TRO_CHOI_CUOI.x - KICH_THUOC_KHOI;
            ViTriPhapSuY_ = TRO_CHOI_CUOI.y + TRO_CHOI_CUOI.h;
            break;
            
        case 3: // Góc dưới phải
            ViTriPhapSuX_ = TRO_CHOI_CUOI.x + TRO_CHOI_CUOI.w ;
            ViTriPhapSuY_ = TRO_CHOI_CUOI.y + TRO_CHOI_CUOI.h ;
            break;
            
            
        default:
            ViTriPhapSuX_ = TRO_CHOI_CUOI.x - KICH_THUOC_KHOI;
            ViTriPhapSuY_ = TRO_CHOI_CUOI.y - KICH_THUOC_KHOI;
            break;
    }
    MauHienTai_ = PHAP_SU_DAY_MAU;
    DangBiThuong_ = false;
    ThoiGianBatDauBiThuong_ = 0;
    
    KhungHinhHienTai_ = 0;
    ThoiGianBatDauTienHoa_ = SDL_GetTicks();
    DaTienHoa_ = false;
}