#include "ManChoiBoss.h"


ManChoiBoss::ManChoiBoss(){
    DangChoi_ = false;
    DaThang_ = false;
    DaThua_ = false;
    DangHoiThoai_ = false;
    HoiThoaiHoanThanh_ = false;
    NoiDungHoiThoai_ = "Giỏi vậy sao, đến được tận đây cơ à?  GIỜ THÌ ĐI CHẾT ĐI!!!";
}

ManChoiBoss::~ManChoiBoss(){

}






bool ManChoiBoss::KhoiTao(SDL_Renderer* ManHinh, int SO_LUONG_SACH) {

    DangHoiThoai_ = true;  // Bắt đầu với hội thoại
   
    
    if (!AnhNenDauTruong_.TaiAnh("img/dautruong1.png", ManHinh)) {
        cout << "Loi tai anh nen dau truong" << endl;
        return false;
    }
    
    
    if (!AnhNenBoss_.TaiAnh("img/bosstam.png", ManHinh)) {
        cout << "Loi tai anh nen boss" << endl;
        return false;
    }

    if (!NhanVat_.KhoiTao(ManHinh)){
        cout << "Loi tai anh nhan vat" << endl;
        return false;
    }

    if (!Boss_.KhoiTao(ManHinh, SO_LUONG_SACH)){
        cout << "Loi tai anh boss" << endl;
        return false;
    }

    if(!KhiDot_.KhoiTao(ManHinh, SO_LUONG_SACH)){
        cout << "Loi tai anh khi dot" << endl;
        return false;
    }

    for (int i = 0; i < SO_LUONG_PHAP_SU; i++){
        if(!PhapSu_[i].KhoiTao(ManHinh, i, SO_LUONG_SACH)){
        cout << "Loi tai anh phap su" << endl;
        return false;
    }
    }

    if (!AnhManHinhThua_.TaiAnh("img/thua.png", ManHinh)) {
        cout << "Loi tai man hinh thua" << endl;
        return false;
    }

    if (!AnhManHinhThang_.TaiAnh("img/thang.png", ManHinh)) {
        cout << "Loi tai man hinh thang" << endl;
        return false;
    }
    
    TrangThaiGame_ = DANG_CHOI_BINH_THUONG;
    YeuCauQuayVeMenu_ = false;
    cout << "Boss Game khoi tao thanh cong!" << endl;
    return true;
}

void ManChoiBoss::CapNhat() {
    if (DangHoiThoai_ && !HoiThoaiHoanThanh_) {
        return;
    }
    
    if (!DangChoi_) {
        return;
    }

    
    switch (TrangThaiGame_) {
        case DANG_CHOI_BINH_THUONG:{
            // Logic game bình thường
            NhanVat_.XuLyDiChuyen();
            NhanVat_.CapNhatSuriken();
            KiemTraVaChamSurikenVsBoss();
            KiemTraVaChamSurikenVsKhiDot();
            KiemTraVaChamSurikenVsPhapSu();
            KiemTraVeMaThuat();
            KiemTraVaChamSurikenVsMaThuat();
            KiemTraVaChamVoiNhanVat();

            KhiDot_.CapNhatKhiDot(&NhanVat_);
            
            KiemTraVaSinhPhapSuTheoGiaiDoan();

            // Kiểm tra điều kiện thắng/thua
            if (Boss_.DaChet() && !DaThang_) {
                DaThang_ = true;
                BatDauHieuUngBossChet();
            }
            break;
        }
        case HIEU_UNG_NHAN_VAT_CHET:
        case HIEU_UNG_BOSS_CHET:
        {
            CapNhatHieuUngChet();
            break;
        }
            
        case HIEN_THI_MAN_HINH_THUA:
        case HIEN_THI_MAN_HINH_THANG:
        {
            break;
        }
    }

}

void ManChoiBoss::KiemTraVaSinhPhapSuTheoGiaiDoan() {
    int SoPhapSuMoi = Boss_.LuongMauConLai();
    
    if (SoPhapSuMoi > SoPhapSuHienTai_) {
        cout << "CHUYEN SANG GIAI DOAN " << SoPhapSuMoi << endl;
        
        for (int i = 0; i < SoPhapSuMoi; i++) {
            PhapSu_[i].HoiSinh(); 
        }
        
        if (KhiDot_.DaChet()) {
            KhiDot_.HoiSinh();
            cout << "SINH LAI KHI DOT" << endl;
        }
        SoPhapSuHienTai_ = SoPhapSuMoi;
        return;
    }
    
    bool TatCaPhapSuChet = true;
    Uint32 ThoiGianPhapSuChet = 0;
    
    for (int i = 0; i < SoPhapSuHienTai_; i++) {  
        PhapSu_[i].CapNhatPhapSu(&NhanVat_);

        if (!PhapSu_[i].DaChet()) {
            TatCaPhapSuChet = false;
        } else {
            ThoiGianPhapSuChet = max(ThoiGianPhapSuChet, PhapSu_[i].GetThoiGianChet());
        }
    }
    
    if (KhiDot_.DaChet() && TatCaPhapSuChet) {
        if (SDL_GetTicks() - max(KhiDot_.GetThoiGianChet(), ThoiGianPhapSuChet) > 5000) {
            KhiDot_.HoiSinh();
            for (int i = 0; i < SoPhapSuHienTai_; i++) {
                PhapSu_[i].HoiSinh();
            }
        }
    }
}

void ManChoiBoss::XuLyInput(SDL_Event &SuKien){
    if (SuKien.type == SDL_KEYDOWN){
        SDL_Keycode key = SuKien.key.keysym.sym;
        
        if (TrangThaiGame_ == HIEN_THI_MAN_HINH_THUA) {
            if (key == SDLK_r) {
                DangChoi_ = false;
                DaThua_ = true;;
                
            }
            return;
        }
        
        if (TrangThaiGame_ == HIEN_THI_MAN_HINH_THANG) {
            if (key == SDLK_RETURN) {
                YeuCauQuayVeMenu_ = true;
            }
            return;
        }
        
        if (DangHoiThoai_ && !HoiThoaiHoanThanh_ && key == SDLK_RETURN){
            HoiThoaiHoanThanh_ = true;
            DangHoiThoai_ = false;
            DangChoi_ = true;
        }
    }

    if (TrangThaiGame_ == DANG_CHOI_BINH_THUONG) {
        NhanVat_.HanhDongDauVao(SuKien);
    }
}


void ManChoiBoss::BatDauHoiThoaiBoss() {
    cout << "Bat dau hoi thoai boss" << endl;
    DangHoiThoai_ = true;
    HoiThoaiHoanThanh_ = false;
    DangChoi_ = false;  // CHƯA BẮT ĐẦU CHƠI
    DaThang_ = false;
    DaThua_ = false;
}



void ManChoiBoss::VeHoiThoai(SDL_Renderer* ManHinh, TTF_Font* font) {
    // VẼ NỀN BOSS TRƯỚC TIÊN
    SDL_Rect NenBoss = {0, 0, CHIEU_RONG_MAN_HINH, CHIEU_CAO_MAN_HINH};
    SDL_RenderCopy(ManHinh, AnhNenBoss_.ThongTinDoiTuong(), nullptr, &NenBoss);
        
    // Dialog box
    SDL_Rect HopDoiThoai = {
        CHIEU_RONG_MAN_HINH / 4,
        CHIEU_CAO_MAN_HINH * 3 / 4,
        CHIEU_RONG_MAN_HINH / 2,
        CHIEU_CAO_MAN_HINH / 4
    };
    
    SDL_SetRenderDrawColor(ManHinh, 255, 255, 255, 200);
    SDL_RenderFillRect(ManHinh, &HopDoiThoai);
    SDL_SetRenderDrawColor(ManHinh, 0, 0, 0, 255);
    SDL_RenderDrawRect(ManHinh, &HopDoiThoai);
    
    // Text
    VeChu(ManHinh, font, NoiDungHoiThoai_, 
        HopDoiThoai.x + 100, HopDoiThoai.y + HopDoiThoai.h / 2 - 10, {0, 0, 0, 255});
    
    VeChu(ManHinh, font, "Nhấn phím ENTER để tiếp tục...", 
        HopDoiThoai.x + 20, HopDoiThoai.y + HopDoiThoai.h - 40, {100, 100, 100, 255});
}

void ManChoiBoss::VeHieuUngBossChet(SDL_Renderer* ManHinh, TTF_Font* font) {
    SDL_Rect NenDauTruong = {0, 0, CHIEU_RONG_MAN_HINH, CHIEU_CAO_MAN_HINH};
    SDL_RenderCopy(ManHinh, AnhNenDauTruong_.ThongTinDoiTuong(), nullptr, &NenDauTruong);
    
    // Vẽ nhân vật bình thường
    NhanVat_.VeNhanVat(ManHinh);
    
    SDL_Rect CatTrongAnh = {0, 0, 16, 16}; // Frame của boss
    SDL_Rect ViTriTrenManHinh = {
        Boss_.GetViTriBossX(),
        Boss_.GetViTriBossY(),
        KICH_THUOC_KHOI,
        KICH_THUOC_KHOI
    };
    
    SDL_RenderCopyEx(ManHinh, Boss_.GetThongTinDoiTuongAnhBoss(), 
                     &CatTrongAnh, &ViTriTrenManHinh,
                     GocXoayBoss_, nullptr, SDL_FLIP_NONE);
    
}


void ManChoiBoss::VeHieuUngNhanVatChet(SDL_Renderer* ManHinh, TTF_Font* font) {
    // Vẽ nền đấu trường
    SDL_Rect NenDauTruong = {0, 0, CHIEU_RONG_MAN_HINH, CHIEU_CAO_MAN_HINH};
    SDL_RenderCopy(ManHinh, AnhNenDauTruong_.ThongTinDoiTuong(), nullptr, &NenDauTruong);
    Boss_.VeBoss(ManHinh, &NhanVat_);

    SDL_Rect CatTrongAnh = {0, 0, 16, 16}; // Giả sử nhân vật có frame 16x16
    SDL_Rect ViTriTrenManHinh = {
        NhanVat_.GetViTriX(),
        NhanVat_.GetViTriY(),
        KICH_THUOC_KHOI,
        KICH_THUOC_KHOI
    };
    
    SDL_RenderCopyEx(ManHinh, NhanVat_.GetThongTinDoiTuong(), 
                     &CatTrongAnh, &ViTriTrenManHinh,
                     GocXoayNhanVat_, nullptr, SDL_FLIP_NONE);
    
                     
}


void ManChoiBoss::VeManHinhThang(SDL_Renderer* ManHinh, TTF_Font* font) {
    SDL_Rect ToanManHinh = {0, 0, CHIEU_RONG_MAN_HINH, CHIEU_CAO_MAN_HINH};
    SDL_RenderCopy(ManHinh, AnhManHinhThang_.ThongTinDoiTuong(), nullptr, &ToanManHinh);
}

void ManChoiBoss::VeManHinhThua(SDL_Renderer* ManHinh, TTF_Font* font) {
    SDL_Rect ToanManHinh = {0, 0, CHIEU_RONG_MAN_HINH, CHIEU_CAO_MAN_HINH};
    SDL_RenderCopy(ManHinh, AnhManHinhThua_.ThongTinDoiTuong(), nullptr, &ToanManHinh);
}



void ManChoiBoss::VeChu(SDL_Renderer* ManHinh, TTF_Font* font, const string& text, 
                     int x, int y, SDL_Color color) {
    if (!font) return;
    
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) return;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ManHinh, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    
    if (texture) {
        SDL_RenderCopy(ManHinh, texture, nullptr, &rect);
        SDL_DestroyTexture(texture);
    }
}



void ManChoiBoss::Ve(SDL_Renderer* ManHinh, TTF_Font* font) {
    if (DangHoiThoai_ && !HoiThoaiHoanThanh_) {
        VeHoiThoai(ManHinh, font);
        return;
    }

    if (!DangChoi_) return;
    
    switch (TrangThaiGame_) {
        case DANG_CHOI_BINH_THUONG:{
            SDL_Rect NenDauTruong = {0, 0, CHIEU_RONG_MAN_HINH, CHIEU_CAO_MAN_HINH};
            SDL_RenderCopy(ManHinh, AnhNenDauTruong_.ThongTinDoiTuong(), nullptr, &NenDauTruong);
            Boss_.VeMaThuat(ManHinh);

            NhanVat_.VeNhanVat(ManHinh);
            NhanVat_.VeThanhMauNhanVat(ManHinh, font);
            NhanVat_.VeSurikens(ManHinh);


            KhiDot_.VeKhiDot(ManHinh, &NhanVat_);
            KhiDot_.VeThanhMauKhiDot(ManHinh);

            for (int i = 0; i < SoPhapSuHienTai_; i++){
                PhapSu_[i].VePhapSu(ManHinh, &NhanVat_);
                PhapSu_[i].VeThanhMauPhapSu(ManHinh);
                PhapSu_[i].VePhepThuat(ManHinh);
            }

            Boss_.VeBoss(ManHinh, &NhanVat_);
            Boss_.VeThanhMauBoss(ManHinh, font);
            break;
        }   
        case HIEU_UNG_NHAN_VAT_CHET:{
            VeHieuUngNhanVatChet(ManHinh, font);
            break;
        }    
        case HIEU_UNG_BOSS_CHET:{
            VeHieuUngBossChet(ManHinh, font);
            break;
        }   
        case HIEN_THI_MAN_HINH_THUA:{
            VeManHinhThua(ManHinh, font);
            break;
        }    
        case HIEN_THI_MAN_HINH_THANG:{
            VeManHinhThang(ManHinh, font);
        }   break;
    }
}



void ManChoiBoss::KiemTraVaChamSurikenVsBoss(){
    int BossX = Boss_.GetViTriBossX();
    int BossY = Boss_.GetViTriBossY();
    SDL_Rect KhungHinhBoss = {
        BossX,
        BossY,
        KICH_THUOC_KHOI,
        KICH_THUOC_KHOI
    };

    for (int i = 0; i < SURIKEN_COUNT; i++){
        Suriken s  = NhanVat_.GetSuriken(i);
        if (!s.DangBay_) continue;
        SDL_Rect KhungHinhSuriken = {
            (int)s.x,
            (int)s.y,
            KICH_THUOC_KHOI,
            KICH_THUOC_KHOI
        };
        if (SDL_HasIntersection(&KhungHinhBoss, &KhungHinhSuriken)){
            Boss_.GayThuong();
            s.KichHoat_ = false;
            s.x = 0;
            s.y = 0;
            NhanVat_.SetSuriken(i, s);
        }
    }
}





void ManChoiBoss::KiemTraVaChamSurikenVsKhiDot(){
    int KhiDotX = (int)KhiDot_.GetViTriKhiDotX();
    int KhiDotY = (int)KhiDot_.GetViTriKhiDotY();
    SDL_Rect KhungHinhBoss = {
        KhiDotX,
        KhiDotY,
        KICH_THUOC_KHOI,
        KICH_THUOC_KHOI
    };

    for (int i = 0; i < SURIKEN_COUNT; i++){
        Suriken s  = NhanVat_.GetSuriken(i);
        if (!s.DangBay_) continue;
        SDL_Rect KhungHinhSuriken = {
            (int)s.x,
            (int)s.y,
            KICH_THUOC_KHOI,
            KICH_THUOC_KHOI
        };
        if (SDL_HasIntersection(&KhungHinhBoss, &KhungHinhSuriken)){
            KhiDot_.GayThuong();
            s.KichHoat_ = false;
            s.x = 0;
            s.y = 0;
            NhanVat_.SetSuriken(i, s);
        }
    }
}


void ManChoiBoss::KiemTraVeMaThuat(){
    bool TatCaPhapSuChet = true;
    for (int i = 0; i < SoPhapSuHienTai_; i++){
        if (!PhapSu_[i].DaChet()){
            TatCaPhapSuChet = false;
        }
    }

    if(KhiDot_.DaChet() && TatCaPhapSuChet){
        Boss_.SetDangKichHoatMaThuat(false);
        NhanVat_.SetDaKichHoatMaThuat(false);
    }
    else{
        Boss_.SetDangKichHoatMaThuat(true);
        NhanVat_.SetDaKichHoatMaThuat(true);
    }
}

void ManChoiBoss::KiemTraVaChamSurikenVsMaThuat(){
    if (!Boss_.GetDangKichHoatMaThuat()) return;
    SDL_Rect KhungHinhMaThuat = {
        VI_TRI_BOSS_X - KICH_THUOC_KHOI,
        VI_TRI_BOSS_Y - KICH_THUOC_KHOI,
        KICH_THUOC_KHOI * 3,
        KICH_THUOC_KHOI * 3
    };
    for (int i = 0; i < SURIKEN_COUNT; i++){
        Suriken s  = NhanVat_.GetSuriken(i);
        if (!s.DangBay_) continue;
        SDL_Rect KhungHinhSuriken = {
            (int)s.x,
            (int)s.y,
            KICH_THUOC_KHOI,
            KICH_THUOC_KHOI
        };
       
        if (SDL_HasIntersection(&KhungHinhMaThuat, &KhungHinhSuriken)){
            s.KichHoat_ = false;
            s.x = 0;
            s.y = 0;
            NhanVat_.SetSuriken(i, s);
        }
    }
}

void ManChoiBoss::KiemTraVaChamSurikenVsPhapSu(){
    

    for (int i = 0; i < SURIKEN_COUNT; i++){
        Suriken s  = NhanVat_.GetSuriken(i);
        if (!s.DangBay_) continue;
        SDL_Rect KhungHinhSuriken = {
            (int)s.x,
            (int)s.y,
            KICH_THUOC_KHOI,
            KICH_THUOC_KHOI
        };

        for (int j = 0; j < SoPhapSuHienTai_; j++){
            if (PhapSu_[j].DaChet()) continue; 
            int PhapSuX = (int)PhapSu_[j].GetViTriPhapSuX();
            int PhapSuY = (int)PhapSu_[j].GetViTriPhapSuY();
            SDL_Rect KhungHinhPhapSu = {
                PhapSuX,
                PhapSuY,
                KICH_THUOC_KHOI,
                KICH_THUOC_KHOI
            };

            if (SDL_HasIntersection(&KhungHinhPhapSu, &KhungHinhSuriken)){
                PhapSu_[j].GayThuong();
                s.KichHoat_ = false;
                s.x = 0;
                s.y = 0;
                NhanVat_.SetSuriken(i, s);
            }
        }

    }
}

void ManChoiBoss::KiemTraVaChamVoiNhanVat() {
    SDL_Rect KhungHinhNhanVat = {
        NhanVat_.GetViTriX(),
        NhanVat_.GetViTriY(),
        KICH_THUOC_KHOI,
        KICH_THUOC_KHOI
    };
    
    if (!KhiDot_.DaChet()) {
        SDL_Rect KhungHinhKhiDot = {
            KhiDot_.GetViTriKhiDotX(),
            KhiDot_.GetViTriKhiDotY(),
            KICH_THUOC_KHOI,
            KICH_THUOC_KHOI
        };
        

        if (SDL_HasIntersection(&KhungHinhNhanVat, &KhungHinhKhiDot)) {
            NhanVat_.GayThuong();
            if (NhanVat_.DaChet()){
                BatDauHieuUngNhanVatChet();
                return;
            }
            
            
        }
    }
    
    for (int i = 0; i < SoPhapSuHienTai_; i++) {
        if (!PhapSu_[i].DaChet()) {
            PhepThuat phepthuat = PhapSu_[i].GetPhepThuat();

            SDL_Rect KhungHinhPhepThuat = {
                (int)phepthuat.ViTriPhepThuatX_,
                (int)phepthuat.ViTriPhepThuatY_,
                KICH_THUOC_KHOI,
                KICH_THUOC_KHOI
            };
            
            if (SDL_HasIntersection(&KhungHinhNhanVat, &KhungHinhPhepThuat)) {
                NhanVat_.GayThuong();
                if (NhanVat_.DaChet()){
                    BatDauHieuUngNhanVatChet();
                    return;
                }
            }
        }
    }

    
    
}



void ManChoiBoss::BatDauHieuUngNhanVatChet() {
    TrangThaiGame_ = HIEU_UNG_NHAN_VAT_CHET;
    ThoiGianBatDauHieuUng_ = SDL_GetTicks();
    GocXoayNhanVat_ = 0.0f;
}

void ManChoiBoss::BatDauHieuUngBossChet() {
    TrangThaiGame_ = HIEU_UNG_BOSS_CHET;
    ThoiGianBatDauHieuUng_ = SDL_GetTicks();
    GocXoayBoss_ = 0.0f;
}


void ManChoiBoss::CapNhatHieuUngChet() {
    Uint32 ThoiGianHienTai = SDL_GetTicks();
    Uint32 ThoiGianTroi = ThoiGianHienTai - ThoiGianBatDauHieuUng_;
    float tienDo = (float)ThoiGianTroi / (float)THOI_GIAN_HIEU_UNG;
    
    if (TrangThaiGame_ == HIEU_UNG_NHAN_VAT_CHET) {
        GocXoayNhanVat_ = tienDo * 90.0f; // Xoay 90 độ (nằm xuống)
        if (GocXoayNhanVat_ > 90.0f) GocXoayNhanVat_ = 90.0f;
        
        if (ThoiGianTroi >= THOI_GIAN_HIEU_UNG) {
            SDL_Delay(1000);
            TrangThaiGame_ = HIEN_THI_MAN_HINH_THUA;
        }
    } else if (TrangThaiGame_ == HIEU_UNG_BOSS_CHET) {
        GocXoayBoss_ = tienDo * 90.0f; // Xoay 90 độ (nằm xuống)
        if (GocXoayBoss_ > 90.0f) GocXoayBoss_ = 90.0f;
        
        if (ThoiGianTroi >= THOI_GIAN_HIEU_UNG) {
            SDL_Delay(1000);
            TrangThaiGame_ = HIEN_THI_MAN_HINH_THANG;
        }
    }
}














