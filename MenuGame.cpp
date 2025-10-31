#include "MenuGame.h"

MenuGame::MenuGame() {
    // Khởi tạo vị trí các nút (giữa màn hình)
    int ChieuRongNut = 300;
    int ChieuCaoNut = 60;
    int KhoangCachNut = 80;
    
    int ToaDoX = (CHIEU_RONG_MAN_HINH - ChieuRongNut) / 2;
    int ToaDoYBatDau = 350;
    
    NutBatDau_ = {ToaDoX, ToaDoYBatDau, ChieuRongNut, ChieuCaoNut};
    NutCaiDat_ = {ToaDoX, ToaDoYBatDau + KhoangCachNut, ChieuRongNut, ChieuCaoNut};
    NutThoat_ = {ToaDoX, ToaDoYBatDau + KhoangCachNut * 2, ChieuRongNut, ChieuCaoNut};
    
    NutDangChon_ = MENU_KHONG_CHON;

    // === KHỞI TẠO MENU ĐỘ KHÓ ===
    TrangThaiHienTai_ = MENU_CHINH;
    DoKhoDaChon_ = DO_KHO_DE; // mặc định
    
    // Vị trí các nút độ khó
    int ToaDoYDoKho = 280;
    NutDe_ = {ToaDoX, ToaDoYDoKho, ChieuRongNut, ChieuCaoNut};
    NutTrungBinh_ = {ToaDoX, ToaDoYDoKho + KhoangCachNut, ChieuRongNut, ChieuCaoNut};
    NutKho_ = {ToaDoX, ToaDoYDoKho + KhoangCachNut * 2, ChieuRongNut, ChieuCaoNut};
    NutQuayLai_ = {ToaDoX, ToaDoYDoKho + KhoangCachNut * 3, ChieuRongNut, ChieuCaoNut};
    
    NutDoKhoDangChon_ = MENU_KHONG_CHON;
}

MenuGame::~MenuGame() {
}

bool MenuGame::TaiHinhNen(const string& DuongDan, SDL_Renderer* ManHinh) {
    return HinhNen_.TaiAnh(DuongDan, ManHinh);
}


bool MenuGame::KiemTraChuotTrongNut(int mouseX, int mouseY, SDL_Rect Nut) {
    return (mouseX >= Nut.x && mouseX <= Nut.x + Nut.w &&
            mouseY >= Nut.y && mouseY <= Nut.y + Nut.h);
}

TrangThaiMenu MenuGame::XuLySuKien(SDL_Event& e) {
    if (TrangThaiHienTai_ == MENU_CHINH) {
        // === XỬ LÝ MENU CHÍNH  ===
        if (e.type == SDL_MOUSEMOTION) {
            int mouseX = e.motion.x;
            int mouseY = e.motion.y;
            
            if (KiemTraChuotTrongNut(mouseX, mouseY, NutBatDau_)) {
                NutDangChon_ = MENU_BAT_DAU;
            } else if (KiemTraChuotTrongNut(mouseX, mouseY, NutCaiDat_)) {
                NutDangChon_ = MENU_CAI_DAT;
            } else if (KiemTraChuotTrongNut(mouseX, mouseY, NutThoat_)) {
                NutDangChon_ = MENU_THOAT;
            } else {
                NutDangChon_ = MENU_KHONG_CHON;
            }
        }
        
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            if (NutDangChon_ == MENU_BAT_DAU) {
                AmThanhGame->PhatHieuUngAmThanh(SFX_NUT_MENU);
                TrangThaiHienTai_ = MENU_CHON_DO_KHO; // Chuyển sang menu độ khó
                return MENU_KHONG_CHON;
            }
            return NutDangChon_;
        }
    } 
    else if (TrangThaiHienTai_ == MENU_CHON_DO_KHO) {
        // === XỬ LÝ MENU ĐỘ KHÓ ===
        if (e.type == SDL_MOUSEMOTION) {
            int mouseX = e.motion.x;
            int mouseY = e.motion.y;
            
            if (KiemTraChuotTrongNut(mouseX, mouseY, NutDe_)) {
                NutDoKhoDangChon_ = MENU_BAT_DAU; // dùng lại enum
            } else if (KiemTraChuotTrongNut(mouseX, mouseY, NutTrungBinh_)) {
                NutDoKhoDangChon_ = MENU_CAI_DAT;
            } else if (KiemTraChuotTrongNut(mouseX, mouseY, NutKho_)) {
                NutDoKhoDangChon_ = MENU_THOAT;
            } else if (KiemTraChuotTrongNut(mouseX, mouseY, NutQuayLai_)) {
                NutDoKhoDangChon_ = (TrangThaiMenu)999; // giá trị đặc biệt
            } else {
                NutDoKhoDangChon_ = MENU_KHONG_CHON;
            }
        }
        
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            if (NutDoKhoDangChon_ == MENU_BAT_DAU) { // Dễ
                DoKhoDaChon_ = DO_KHO_DE;
                return MENU_BAT_DAU; // Bắt đầu game
            } else if (NutDoKhoDangChon_ == MENU_CAI_DAT) { // Trung bình
                DoKhoDaChon_ = DO_KHO_TRUNG_BINH;
                return MENU_BAT_DAU;
            } else if (NutDoKhoDangChon_ == MENU_THOAT) { // Khó
                DoKhoDaChon_ = DO_KHO_KHO;
                return MENU_BAT_DAU;
            } else if (NutDoKhoDangChon_ == (TrangThaiMenu)999) { // Quay lại
                TrangThaiHienTai_ = MENU_CHINH;
                return MENU_KHONG_CHON;
            }
        }
    }
    
    return MENU_KHONG_CHON;
}

void MenuGame::VeMenuChinh(SDL_Renderer* ManHinh, TTF_Font* FontTieuDe, TTF_Font* FontNut) {
    // 1. Vẽ hình nền
    HinhNen_.Ve(ManHinh, NULL);
    
    // 2. Vẽ tiêu đề "HITACHI"
    SDL_Color MauTrang = {255, 255, 255, 255};
    SDL_Color MauVang = {255, 215, 0, 255};
    SDL_Color MauVangKem = {255, 235, 153, 255};       // vàng nhẹ, bắt mắt

    // Đo kích thước chữ để căn giữa
    int ChieuRongTieuDe = 0, ChieuCaoTieuDe = 0;
    TTF_SizeUTF8(FontTieuDe, "HITACHI", &ChieuRongTieuDe, &ChieuCaoTieuDe);
    
    int ToaDoTieuDeX = (CHIEU_RONG_MAN_HINH - ChieuRongTieuDe) / 2;
    int ToaDoTieuDeY = 250;
    
    VeChu(ManHinh, FontTieuDe, "HITACHI", ToaDoTieuDeX, ToaDoTieuDeY, MauVangKem);
    
    // 3. Vẽ các nút
    VeNut(ManHinh, FontNut, "BẮT ĐẦU", NutBatDau_, (NutDangChon_ == MENU_BAT_DAU));
    VeNut(ManHinh, FontNut, "CÀI ĐẶT ÂM THANH", NutCaiDat_, (NutDangChon_ == MENU_CAI_DAT));
    VeNut(ManHinh, FontNut, "THOÁT GAME", NutThoat_, (NutDangChon_ == MENU_THOAT));
}



void MenuGame::VeMenuDoKho(SDL_Renderer* ManHinh, TTF_Font* FontTieuDe, TTF_Font* FontNut) {
    // Vẽ nền
    HinhNen_.Ve(ManHinh, NULL);
    
    // Vẽ tiêu đề
    SDL_Color MauVangKem = {255, 235, 153, 255};
    int ChieuRongTieuDe = 0, ChieuCaoTieuDe = 0;
    TTF_SizeUTF8(FontTieuDe, "HITACHI", &ChieuRongTieuDe, &ChieuCaoTieuDe);
    int ToaDoTieuDeX = (CHIEU_RONG_MAN_HINH - ChieuRongTieuDe) / 2;
    int ToaDoTieuDeY = 180;
    VeChu(ManHinh, FontTieuDe, "HITACHI", ToaDoTieuDeX, ToaDoTieuDeY, MauVangKem);
    
    // Vẽ các nút
    VeNut(ManHinh, FontNut, "DỄ (10 sách)", NutDe_, (NutDoKhoDangChon_ == MENU_BAT_DAU));
    VeNut(ManHinh, FontNut, "TRUNG BÌNH (20 sách)", NutTrungBinh_, (NutDoKhoDangChon_ == MENU_CAI_DAT));
    VeNut(ManHinh, FontNut, "KHÓ (50 sách)", NutKho_, (NutDoKhoDangChon_ == MENU_THOAT));
    VeNut(ManHinh, FontNut, "QUAY LẠI", NutQuayLai_, (NutDoKhoDangChon_ == (TrangThaiMenu)999));
}

void MenuGame::VeChu(SDL_Renderer* ManHinh, TTF_Font* Font, const string& NoiDung,
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

void MenuGame::VeNut(SDL_Renderer* ManHinh, TTF_Font* Font, const string& NoiDung,
                     SDL_Rect ViTri, bool DangChon) {
    
    if (DangChon) {
        SDL_SetRenderDrawColor(ManHinh, 255, 215, 0, 200); // Vàng khi hover
    } else {
        SDL_SetRenderDrawColor(ManHinh, 50, 50, 50, 180); // Xám đậm bình thường
    }
    
    SDL_RenderFillRect(ManHinh, &ViTri);
    
    // Vẽ viền nút
    SDL_SetRenderDrawColor(ManHinh, 255, 255, 255, 255);
    SDL_RenderDrawRect(ManHinh, &ViTri);
    
    // Vẽ chữ trong nút (căn giữa)
    int ChieuRongChu = 0, ChieuCaoChu = 0;
    TTF_SizeUTF8(Font, NoiDung.c_str(), &ChieuRongChu, &ChieuCaoChu);
    
    int ToaDoChuX = ViTri.x + (ViTri.w - ChieuRongChu) / 2;
    int ToaDoChuY = ViTri.y + (ViTri.h - ChieuCaoChu) / 2;
    
    SDL_Color MauChu = DangChon ? SDL_Color{0, 0, 0, 255} : SDL_Color{255, 255, 255, 255};
    VeChu(ManHinh, Font, NoiDung, ToaDoChuX, ToaDoChuY, MauChu);
}

void MenuGame::Ve(SDL_Renderer* ManHinh, TTF_Font* FontTieuDe, TTF_Font* FontNut) {
    if (TrangThaiHienTai_ == MENU_CHINH) {
        VeMenuChinh(ManHinh, FontTieuDe, FontNut);
    } else if (TrangThaiHienTai_ == MENU_CHON_DO_KHO) {
        VeMenuDoKho(ManHinh, FontTieuDe, FontNut);
    }
}