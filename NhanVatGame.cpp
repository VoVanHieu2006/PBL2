#include "NhanVatGame.h"


// =======================
// =======Khởi tạo========
// =======================
NhanVatGame::NhanVatGame(){
    NhanVat_.Huong_ = HUONG_DUOI;
    NhanVat_.DoDichChuyenNhanVatX_ = 0;
    NhanVat_.DoDichChuyenNhanVatY_ = 0;
    NhanVat_.ViTriNhanVatX_ = VI_TRI_BAN_DAU_X;
    NhanVat_.ViTriNhanVatY_ = VI_TRI_BAN_DAU_Y;
    NhanVat_.KhungHinh_ = 0;
    NhanVat_.KichThuocThatNhanVat_ = 16;

    // Kiểm tra chỉ cho di chuyển 1 ô 1 lần
    DangDiDenO_ = false;
    XuatPhatX_ = 0;
    XuatPhatY_ = 0;
    DichDenX_ = 0;
    DichDenY_ = 0;

    ThoiGianBatDauBuoc_ = 0;
    ThoiGianCanBuoc_ = 0;

    ThoiGianToiThieu_ = 50;
    ThoiGianTichLuyAnimation_ = 0;
    KhoiTaoDanhSachTenSach();


    // Túi đồ
    HienThiTuiDo_ = false;
    HienThiThongBao_ = false;
    ChiSoSachDauTien_ = 0;
    SoLuongSachHienThi_ = 10;

    // Sao chép qua rương đồ
    Ruong_.GanThamChieuDuLieu(&TuiSach_, &DanhSachTenSach_);
    YeuCauMoRuong_ = false;
    HienThiThongBaoDaHoanThanh_ = false;
    DaMoCua_ = false;
}

void NhanVatGame::KhoiTaoDanhSachTenSach(){
    // Ví dụ: nếu Book.tsj có 64 tile (0..63)
    DanhSachTenSach_.clear();
    DanhSachTenSach_ = {
        // --- Ma Thuật Nguyên Tố Căn Bản (0-7) ---
        "Sach Lua",             // IDCucBo 0
        "Sach Nuoc",            // 1
        "Sach Gio",             // 2
        "Sach Dat",             // 3
        "Sach Anh Sang",        // 4
        "Sach Bong Toi",        // 5
        "Sach Set",             // 6
        "Sach Bang",            // 7

        // --- Ma Thuật Nguyên Tố Cao Cấp (8-15) ---
        "Sach Dung Nham",       // 8 (Lửa + Đất)
        "Sach Bao To",          // 9 (Gió + Nước)
        "Sach Bui Phat Sang",   // 10 (Ánh Sáng + Đất)
        "Sach Am Tinh",         // 11 (Bóng Tối + Băng)
        "Sach Plasma",          // 12 (Lửa + Sét)
        "Sach Hoi Nuoc",        // 13 (Lửa + Nước)
        "Sach Tinh The",        // 14 (Đất + Băng)
        "Sach Quang Phong",     // 15 (Gió + Ánh Sáng)

        // --- Ma Thuật Thiên Nhiên (16-23) ---
        "Sach Moc",             // 16 (Sự sống, cây cối)
        "Sach Thu",             // 17 (Triệu hồi, giao tiếp với động vật)
        "Sach Thoi Tiet",       // 18 (Điều khiển thời tiết)
        "Sach Doc To",          // 19
        "Sach Soi Mon",         // 20 (Axit, phân rã)
        "Sach Nam",             // 21 (Bào tử,菌類)
        "Sach Sa Mac",          // 22
        "Sach Nui Rung",        // 23

        // --- Ma Thuật Không Gian & Thời Gian (24-31) ---
        "Sach Thoi Gian",       // 24
        "Sach Khong Gian",      // 25
        "Sach Dich Chuyen",     // 26
        "Sach Trong Luc",       // 27
        "Sach Tuong Lai",       // 28 (Tiên tri)
        "Sach Qua Khu",         // 29 (Nhìn lại quá khứ)
        "Sach Chieu Khong Gian",// 30
        "Sach Lo Den",          // 31

        // --- Ma Thuật Tinh Thần & Linh Hồn (32-39) ---
        "Sach Thao Tung Tam Tri",// 32
        "Sach Ao Anh",          // 33
        "Sach Goi Hon",         // 34
        "Sach Tinh Than",       // 35 (Sức mạnh ý chí)
        "Sach Cam Xuc",         // 36 (Tạo ra hoặc điều khiển cảm xúc)
        "Sach Giac Mo",         // 37
        "Sach Linh Hon",        // 38
        "Sach Tru Ta",          // 39 (Chống lại ma quỷ, linh hồn)

        // --- Ma Thuật Biến Hình & Cường Hóa (40-47) ---
        "Sach Bien Hinh",       // 40
        "Sach Cuong Hoa Co The",// 41
        "Sach Gia Kim",         // 42 (Biến đổi vật chất)
        "Sach Tang Luc",        // 43
        "Sach Phong Thu",       // 44
        "Sach Toc Do",          // 45
        "Sach Vo Hinh",         // 46
        "Sach Khong Lo",        // 47

        // --- Ma Thuật Cấm & Hỗn Mang (48-55) ---
        "Sach Huy Diet",        // 48
        "Sach Hon Mang",        // 49
        "Sach Map",          // 50
        "Sach Hut Mau",         // 51
        "Sach Nguyen Rua",      // 52
        "Sach Benh Dich",       // 53
        "Sach Hoi Sinh",        // 54 (Cấm thuật)
        "Sach Goi Quy",         // 55

        // --- Các Sách Huyền Thoại & Cổ Đại (56-63) ---
        "Sach Thien Co",        // 56 (Sách về các định luật vũ trụ)
        "Sach So Sang",         // 57 (Sách tạo ra vạn vật)
        "Sach Tan The",         // 58
        "Sach Co Ngu",          // 59 (Cổ ngữ ma thuật)
        "Sach Phong An",        // 60
        "Sach Tinh Hoa Nguyen To",// 61
        "Sach Thien Su",        // 62
        "Sach Ac Quy Vuong"   // 63
    };
}

NhanVatGame::~NhanVatGame(){

}

// =======================
// ========Tải ảnh========
// =======================
bool NhanVatGame::TaiAnhNhanVat(string DuongDan, SDL_Renderer *ManHinh){
    bool Ktra = NhanVat_.AnhCuaNhanVat_.TaiAnh(DuongDan, ManHinh);
    if (!Ktra) cout << " Tai anh cua nhan vat that bai" << endl;

    for (int i = 0; i < SO_KHUNG_HINH_MOT_HANG; i++){
        for (int j = 0; j < SO_KHUNG_HINH_MOT_HANG; j++){
            SDL_Rect CatKhungHinh;
            CatKhungHinh.x = j * NhanVat_.KichThuocThatNhanVat_;
            CatKhungHinh.y = i * NhanVat_.KichThuocThatNhanVat_;
            CatKhungHinh.w = NhanVat_.KichThuocThatNhanVat_;
            CatKhungHinh.h = NhanVat_.KichThuocThatNhanVat_;
            NhanVat_.CacVungCatNhanVat_[i][j] = CatKhungHinh;
            // i = 0 duoi, 1 trai, 2 phai, 3 tren
        }
    }
    return Ktra;
}

// =======================
// ========Đầu vào========
// =======================

void NhanVatGame::HanhDongDauVao(SDL_Event SuKien, SDL_Renderer *ManHinh, const BanDoGame& BanDo, const int& SO_LUONG_SACH){

    if (Ruong_.DangMo()){
        if (SuKien.type == SDL_TEXTINPUT || SuKien.type == SDL_KEYDOWN){
            Ruong_.XuLySuKien(SuKien, SO_LUONG_SACH);
        }
        return; // chặn mọi hành động khác (di chuyển, mở túi...)
    }

    if (HienThiTuiDo_){
        int SoLuongSachDangCo = (int)TuiSach_.size();
        if (SuKien.type == SDL_KEYDOWN){
            if (SuKien.key.keysym.sym == SDLK_ESCAPE || SuKien.key.keysym.sym == SDLK_i){
                HienThiTuiDo_ = false;
                return;
            }
            if (SuKien.key.keysym.sym == SDLK_UP){
                if (ChiSoSachDauTien_ > 0) ChiSoSachDauTien_--;
                return;
            }
            if (SuKien.key.keysym.sym == SDLK_DOWN){
                if (SoLuongSachDangCo > SoLuongSachHienThi_ && 
                    ChiSoSachDauTien_ < SoLuongSachDangCo - SoLuongSachHienThi_) ChiSoSachDauTien_++;                
                return;
            }     
        }       
    }



    if (SuKien.type == SDL_KEYDOWN){

        if (SuKien.key.keysym.sym == SDLK_e && SuKien.key.repeat == 0){
            DauVao_.Phai_ = 0;
            DauVao_.Trai_ = 0;
            DauVao_.Tren_ = 0;
            DauVao_.Duoi_ = 0;
            YeuCauMoRuong_ = true;
            
        }

        if (SuKien.key.keysym.sym == SDLK_i){ // Toggle túi đồ
            HienThiTuiDo_ = !HienThiTuiDo_; // chuyển trạng thái túi đồ
        }

        

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
        YeuCauMoRuong_ = false;
        
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
}

// =========================
// ========Di chuyển========
// =========================

void NhanVatGame::XuLyDiChuyen(const BanDoGame& BanDo){
    BanDoGame& BanDoGameTam = const_cast<BanDoGame&>(BanDo);
    if (DangDiDenO_){
        TrongQuaTrinhDi(BanDo);  

        
        ThuThapSach(BanDoGameTam);

    }
    else{
        if (DauVao_.Trai_) BatDauMotBuoc(-1, 0, HUONG_TRAI, BanDo);
        else if (DauVao_.Phai_) BatDauMotBuoc(1, 0, HUONG_PHAI, BanDo);
        else if (DauVao_.Tren_) BatDauMotBuoc(0, -1, HUONG_TREN, BanDo);
        else if (DauVao_.Duoi_) BatDauMotBuoc(0, 1, HUONG_DUOI, BanDo);
    }

    const DoiTuongTuongTac* DoiTuong = VaChamDoiTuongTuongTac(BanDo);


    if (DoiTuong != nullptr){
        if (DoiTuong->Loai_ == "Ruong"){
            if (Ruong_.HoanThanh()){
                HienThiThongBaoDaHoanThanh_ = true;
                if (!DaMoCua_){
                    BanDoGameTam.MoTatCaCuaTrenTatCaLop();
                    DaMoCua_ = true;
                }
            }


            else if (YeuCauMoRuong_){
                HienThiThongBao_ = false;
                Ruong_.Mo(*DoiTuong);
                YeuCauMoRuong_ = false;
            }
        }
        else if (DoiTuong->Loai_ == "BienBao"){
            HienThiThongBao_ = true;
        }
    }
    else{
        HienThiThongBao_ = false;
        HienThiThongBaoDaHoanThanh_ = false;
    }
}


void NhanVatGame::BatDauMotBuoc(int dx, int dy, HuongDi Huong, const BanDoGame& BanDo){
    if (DangDiDenO_) return;
    
    XuatPhatX_ = NhanVat_.ViTriNhanVatX_;
    XuatPhatY_ = NhanVat_.ViTriNhanVatY_;

    DichDenX_ = NhanVat_.ViTriNhanVatX_ + dx * KICH_THUOC_KHOI;
    DichDenY_ = NhanVat_.ViTriNhanVatY_ + dy * KICH_THUOC_KHOI;

    if (DichDenX_ < 0 || DichDenX_ + KICH_THUOC_NHAN_VAT> CHIEU_RONG_MAN_HINH
    ||  DichDenY_ < 0 || DichDenY_ + KICH_THUOC_NHAN_VAT> CHIEU_CAO_MAN_HINH)  return;



    // Xử lý khi va chạm với tường
    int ODichX = DichDenX_ / KICH_THUOC_KHOI;
    int ODichY = DichDenY_ / KICH_THUOC_KHOI;

    bool CoChan = false;
    for (const auto& Lop : BanDo.ThongTinBanDo().DanhSachLopBanDo_){
        int GidTaiODich = Lop.Khoi_[ODichY][ODichX];
        if (BanDo.LaKhoiTuong(GidTaiODich) || BanDo.LaCuaKhoa(GidTaiODich)){
            CoChan = true;
            break;
        }
    }
    if (CoChan) return;




    float ThoiGianGiayTam = (float) KICH_THUOC_KHOI / (float) TOC_DO_PIXEL_MOT_GIAY;
    ThoiGianCanBuoc_ = (Uint32)(ThoiGianGiayTam * 1000.0);

    ThoiGianBatDauBuoc_ = SDL_GetTicks();
    

    NhanVat_.Huong_ = Huong;
    DangDiDenO_ = true;
    
}


void NhanVatGame::TrongQuaTrinhDi(const BanDoGame& BanDo){
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


void NhanVatGame::KetThucBuoc(){
    NhanVat_.ViTriNhanVatX_ = DichDenX_;
    NhanVat_.ViTriNhanVatY_ = DichDenY_;
    DangDiDenO_ = false;
    NhanVat_.KhungHinh_ = 0;
}

// ========================
// ========Thu sách========
// ========================

void NhanVatGame::ThuThapSach(BanDoGame& BanDoTam){
    int KhoiX = NhanVat_.ViTriNhanVatX_ / KICH_THUOC_KHOI;
    int KhoiY = NhanVat_.ViTriNhanVatY_ / KICH_THUOC_KHOI;
    Map ThongTinBanDoTam = BanDoTam.ThongTinBanDo();
    int SoLop = (int)ThongTinBanDoTam.DanhSachLopBanDo_.size();

    for (int Lop = 0; Lop < SoLop; Lop++){
        Uint32 GidBanDau = BanDoTam.LayGid(KhoiX, KhoiY, Lop);
        if (GidBanDau == 0) continue;
        
        Uint32 Gid = GidBanDau & 0x1FFFFFFF;
        if (BanDoTam.LaKhoiSach(Gid)){
            int IDCucBo = Gid - ThongTinBanDoTam.GidBatDauCuaSach_;
            if (IDCucBo >= 0){
                TuiSach_[IDCucBo]++;   // tăng số lượng
                BanDoTam.XoaKhoi(KhoiX, KhoiY, Lop); // xóa tile
                cout << "DA AN SACH" << endl;
            }
        }
    }
}

const DoiTuongTuongTac* NhanVatGame::VaChamDoiTuongTuongTac(const BanDoGame& BanDo){
    SDL_Rect VungNhanVat ={
        NhanVat_.ViTriNhanVatX_,
        NhanVat_.ViTriNhanVatY_,
        KICH_THUOC_NHAN_VAT,
        KICH_THUOC_NHAN_VAT
    };

    const auto& DanhSachDoiTuongTuongTac = BanDo.ThongTinBanDo().DanhSachDoiTuongTuongTac_;

    for (const DoiTuongTuongTac& DoiTuongTrongDanhSach : DanhSachDoiTuongTuongTac){
        if (SDL_HasIntersection(&VungNhanVat, &DoiTuongTrongDanhSach.VungVaCham_)){
            return &DoiTuongTrongDanhSach;
        }
    }

    return nullptr;
}




// =========================
// ========Vẽ các thứ=======
// =========================


void NhanVatGame::VeNhanVat(SDL_Renderer *ManHinh){
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

    SDL_RenderCopy(ManHinh, AnhNhanVatCanVe, &KhungHinhHienTai, &ViTriTrenManHinh);
    
}

void NhanVatGame::VeChu(SDL_Renderer* ManHinh, TTF_Font* Font, const string& NoiDung, int x, int y, SDL_Color Mau){
    if (!Font){
        cout << "KHONG TIM THAY FONT" << endl;
        return;
    }
    SDL_Surface* ChuNen = TTF_RenderUTF8_Blended(Font, NoiDung.c_str(), Mau);
    if (!ChuNen) return;
    SDL_Texture* ChuThat = SDL_CreateTextureFromSurface(ManHinh, ChuNen);
    SDL_Rect ViTriTrenManHinh { x, y, ChuNen->w, ChuNen->h };
    SDL_FreeSurface(ChuNen);
    if (ChuThat != NULL){
        SDL_RenderCopy(ManHinh, ChuThat, nullptr, &ViTriTrenManHinh);
        SDL_DestroyTexture(ChuThat);
    }
    else{
        cout << "KHONG TAI DUOC CHU LEN" << endl;
    }
}

void NhanVatGame::VeTuiDo(SDL_Renderer *ManHinh, TTF_Font* Font,const string& DuongDanLinkSach, const BanDoGame& BanDo, const int& SO_LUONG_SACH){
    if (!HienThiTuiDo_) return;

    int ChieuRongTui, ChieuCaoTui;
    ChieuRongTui = 420;
    ChieuCaoTui = 100 + 26 *  SoLuongSachHienThi_; // đủ chứa 8 hàng
 

    int ToaDoTuiDoX = (CHIEU_RONG_MAN_HINH - ChieuRongTui)/2;
    int ToaDoTuiDoY = (CHIEU_CAO_MAN_HINH - ChieuCaoTui)/2;

    // Overlay mờ toàn màn hình (tùy chọn cho đẹp)
    SDL_SetRenderDrawBlendMode(ManHinh, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ManHinh, 0, 0, 0, 100);
    SDL_Rect ToanManHinh {0, 0, CHIEU_RONG_MAN_HINH, CHIEU_CAO_MAN_HINH};
    SDL_RenderFillRect(ManHinh, &ToanManHinh);

    // Nền túi đồ
    SDL_Rect KichThuocTuiDo { ToaDoTuiDoX, ToaDoTuiDoY, ChieuRongTui, ChieuCaoTui };
    SDL_SetRenderDrawColor(ManHinh, 255,255,255,235); // trắng hơi đục
    SDL_RenderFillRect(ManHinh, &KichThuocTuiDo); // Tô màu lên màn hình
    SDL_SetRenderDrawColor(ManHinh, 0,0,0,255);
    SDL_RenderDrawRect(ManHinh, &KichThuocTuiDo);



    // Bố cục chi tiết
    int Le = 12;
    
    // Canh giữa chữ "TÚI ĐỒ"
    string TieuDe = "TÚI ĐỒ";
    // (Có thể đo bằng TTF_SizeUTF8(Font, TieuDe.c_str(), &w,&h))
    int ChieuRongTieuDe = 0, ChieuCaoTieuDe = 0;
    if (Font) TTF_SizeUTF8(Font, TieuDe.c_str(), &ChieuRongTieuDe, &ChieuCaoTieuDe); // lấy ra chiều rộng và cao của tiêu đề
    int ToaDoTieuDeY = ToaDoTuiDoY + Le;
    int ToaDoTuiDeX = ToaDoTuiDoX + (ChieuRongTui - ChieuRongTieuDe)/2;
    SDL_Color MauDen = {0,0,0,255};
    VeChu(ManHinh, Font, TieuDe,ToaDoTuiDeX , ToaDoTieuDeY, MauDen);

    // Cột
    int ChieuRongNoiDung = ChieuRongTui - 2*Le; // căn 2 cái lề 2 bên
    int ChieuRongCotTenSach = (int)(ChieuRongNoiDung * 0.55f);
    int ChieuRongCotSoLuong  = (int)(ChieuRongNoiDung * 0.20f);
    int ChieuRongCotIcon = ChieuRongNoiDung - ChieuRongCotTenSach - ChieuRongCotSoLuong; // phần còn lại ~25%

    
    
    // Xét tọa độ của các cột để in ra 
    int ToaDoCotTenSachX = ToaDoTuiDoX + Le;
    int ToaDoCotTenSachY = ToaDoTieuDeY + ChieuCaoTieuDe + 10;

    int ToaDoCotSoLuongX  = ToaDoCotTenSachX + ChieuRongCotTenSach;
    int ToaDoCotIconX = ToaDoCotSoLuongX + ChieuRongCotSoLuong;

    // Tiêu đề cột
    VeChu(ManHinh, Font, "Tên sách", ToaDoCotTenSachX, ToaDoCotTenSachY, {10,10,10,255});
    VeChu(ManHinh, Font, "Số lượng", ToaDoCotSoLuongX, ToaDoCotTenSachY, {10,10,10,255});
    VeChu(ManHinh, Font, "Icon", ToaDoCotIconX, ToaDoCotTenSachY, {10,10,10,255});


    
    // Đường kẻ dưới các mục chính 
    SDL_SetRenderDrawColor(ManHinh, 0,0,0,255);
    SDL_RenderDrawLine(ManHinh, ToaDoTuiDoX + Le, ToaDoCotTenSachY + 25, ToaDoTuiDoX + ChieuRongTui-Le, ToaDoCotTenSachY + 25);


    // thông số 1 dòng
    int ChieuCaoMoiDong = 26;
    int SoHangToiDa = SO_LUONG_SACH;

    // Thông số nội dung
    int ToaDoNoiDungX = ToaDoTuiDoX + Le;
    int ToaDoNoiDungY = ToaDoCotTenSachY + 30;

    // tìm bộ khối sách 
    
    BoKhoi* BoKhoiMucTieu = nullptr;
    const vector<BoKhoi*>& DanhSachCacBoKhoiTam = BanDo.ThongTinDanhSachCacBoKhoi();
    for (BoKhoi* TimBoKhoiSach : DanhSachCacBoKhoiTam){
        if (TimBoKhoiSach->DiaChiDanBoKhoi_ == DuongDanLinkSach){
            BoKhoiMucTieu = TimBoKhoiSach;
            break;
        }
    }

    if (BoKhoiMucTieu == nullptr){
        cout << "KHONG TIM DUOC BO KHOI SACH DE CO THE IN ICON" << endl;
    }

    // Duyệt túi đồ -> hiển thị tối đa SO_LUONG_SACH
    int Hang = 0;
    for (auto &p : TuiSach_){
        if (Hang >= SoHangToiDa){
            cout << "VUOT MUC PICKER BALL SO HANG" << endl;
            break;
        }
        int IDCucBo = p.first;
        int SoLuong = p.second;
        string TenSach = (IDCucBo >= 0 && IDCucBo < (int)DanhSachTenSach_.size()) ? DanhSachTenSach_[IDCucBo] : ("Loi! KHON TIM THAY TEN SACH" + to_string(IDCucBo));

        int ToaDoMoiDongY = ToaDoNoiDungY + Hang * ChieuCaoMoiDong;

        // Nền mỗi dòng (thêm màu cho dễ đọc)
        if (Hang % 2 != 0){ // nếu là hàm chẵn
            SDL_Rect KichThuocNenCuaDong {ToaDoNoiDungX, ToaDoMoiDongY - 2, ChieuRongNoiDung, ChieuCaoMoiDong };
            SDL_SetRenderDrawColor(ManHinh, 200,200,200,255); // màu xám xám
            SDL_RenderFillRect(ManHinh, &KichThuocNenCuaDong);
            
        }

        // vẽ chữ đè lên
        VeChu(ManHinh, Font, TenSach, ToaDoNoiDungX, ToaDoMoiDongY - 2, MauDen); // màu đen
        VeChu(ManHinh, Font, to_string(SoLuong), ToaDoCotSoLuongX, ToaDoMoiDongY - 2, MauDen);


        // Icon: vẽ ô vuông màu
        SDL_Texture *BoKhoiCanVe = BoKhoiMucTieu->AnhCuaBoKhoi_.ThongTinDoiTuong();

        if (BoKhoiCanVe == NULL){
            cout << "KHONG TIM THAY BO KHOI CAN VE ICON" << endl;
        }
        
        SDL_Rect CatKhoiHienTai;
        auto TimKiem = BoKhoiMucTieu->CacVungCatKhoi_.find(IDCucBo);
        if (TimKiem != BoKhoiMucTieu->CacVungCatKhoi_.end()){ // tìm thấy
            CatKhoiHienTai = TimKiem->second;
        }
        else{
            cout << "KHONG TIM THAY ID CUC BO" << endl;
            continue;
        }
        
        int KichThuocIcon = (ChieuCaoMoiDong - 6);
        SDL_Rect ViTriIconTrenManHinh { ToaDoCotIconX, ToaDoMoiDongY + 2, KichThuocIcon, KichThuocIcon };

        SDL_RenderCopy(ManHinh, BoKhoiCanVe, &CatKhoiHienTai, &ViTriIconTrenManHinh);
        

        Hang++;
    }
}

void NhanVatGame::VeBangChiDan(SDL_Renderer *ManHinh, TTF_Font* Font, const BanDoGame& BanDo){
    if (!HienThiThongBao_) return;
    
    const DoiTuongTuongTac* DoiTuongDangVaCham = VaChamDoiTuongTuongTac(BanDo);
    if (DoiTuongDangVaCham == nullptr) return;
    string NoiDungThongBao = DoiTuongDangVaCham->NoiDung_;

    if (!Font) {
        cout << "KHONG THE VE BANG CHI DAN VI KHONG CO FONT" << endl;
        return;
    }

    int ChieuRongBang, ChieuCaoBang;
    ChieuRongBang = 520;
    ChieuCaoBang = 300; 

    int ToaDoBangX = (CHIEU_RONG_MAN_HINH - ChieuRongBang)/2;
    int ToaDoBangY = (CHIEU_CAO_MAN_HINH - ChieuCaoBang)/2;

    // Overlay mờ toàn màn hình (tùy chọn cho đẹp)
    SDL_SetRenderDrawBlendMode(ManHinh, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ManHinh, 0, 0, 0, 100);
    SDL_Rect ToanManHinh {0, 0, CHIEU_RONG_MAN_HINH, CHIEU_CAO_MAN_HINH};
    SDL_RenderFillRect(ManHinh, &ToanManHinh);

    // Nền túi đồ
    SDL_Rect KichThuocTuiDo {ToaDoBangX, ToaDoBangY, ChieuRongBang, ChieuCaoBang };
    SDL_SetRenderDrawColor(ManHinh, 255,255,255,235); // trắng hơi đục
    SDL_RenderFillRect(ManHinh, &KichThuocTuiDo); // Tô màu lên màn hình
    SDL_SetRenderDrawColor(ManHinh, 0,0,0,255);
    SDL_RenderDrawRect(ManHinh, &KichThuocTuiDo);



    // Bố cục chi tiết
    int Le = 12;
    
    string TieuDe = "BẢNG HƯỚNG DẪN";
    // (Có thể đo bằng TTF_SizeUTF8(Font, TieuDe.c_str(), &w,&h))
    int ChieuRongTieuDe = 0, ChieuCaoTieuDe = 0;
    if (Font) TTF_SizeUTF8(Font, TieuDe.c_str(), &ChieuRongTieuDe, &ChieuCaoTieuDe); // lấy ra chiều rộng và cao của tiêu đề
    int ToaDoTieuDeY = ToaDoBangY + Le;
    int ToaDoTieuDeX = ToaDoBangX + (ChieuRongBang - ChieuRongTieuDe)/2;
    SDL_Color MauDen = {0,0,0,255};
    VeChu(ManHinh, Font, TieuDe, ToaDoTieuDeX , ToaDoTieuDeY, MauDen);

    
    // Đường kẻ dưới các mục chính 
    SDL_SetRenderDrawColor(ManHinh, 0,0,0,255);
    SDL_RenderDrawLine(ManHinh, ToaDoBangX + Le, ToaDoTieuDeY + 25, ToaDoBangX + ChieuRongBang - Le, ToaDoTieuDeY + 25);

    //Nội dung 
    
    int ToaDoNoiDungX = ToaDoBangX + Le, ToaDoNoiDungY;
    int ChieuCaoMoiHang = 26;
    int Hang = 0;
    
    vector<string> CacDong;
    stringstream ss(NoiDungThongBao);

    string Dong;
    while(getline(ss, Dong, '\n')){
        CacDong.push_back(Dong);
    }

    for (string NoiDung : CacDong){
        ToaDoNoiDungY = ToaDoTieuDeY + 30 + ChieuCaoMoiHang * Hang;
        VeChu(ManHinh, Font, NoiDung, ToaDoNoiDungX, ToaDoNoiDungY, MauDen);
        Hang++;
    }

    // "Ấn di chuyển để tiếp tục" gọi là kết

    string Ket = "DI CHUYỂN ĐỂ TIẾP TỤC";
    int ChieuRongKet = 0, ChieuCaoKet = 0;
    if (Font) TTF_SizeUTF8(Font, Ket.c_str(), &ChieuRongKet, &ChieuCaoKet); // lấy ra chiều rộng và cao của tiêu đề
    int ToaDoKetY = ToaDoBangY + ChieuCaoBang - ChieuCaoKet - Le;
    int ToaDoKetX = ToaDoBangX + (ChieuRongBang - ChieuRongKet)/2;
    VeChu(ManHinh, Font, Ket, ToaDoKetX, ToaDoKetY, {80,80,80,255});

}

void NhanVatGame::VeThongBaoDaHoanThanh(SDL_Renderer* ManHinh, TTF_Font* Font){
    if (!HienThiThongBaoDaHoanThanh_) return;
    
    // Overlay
    SDL_SetRenderDrawBlendMode(ManHinh, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ManHinh, 0,0,0,110);
    SDL_Rect ToanManHInh = {0,0,CHIEU_RONG_MAN_HINH, CHIEU_CAO_MAN_HINH};
    SDL_RenderFillRect(ManHinh, &ToanManHInh);

    // Panel trung tâm
    int Rong = 700, Cao = 200;
    int ToaDoTBRuongX = (CHIEU_RONG_MAN_HINH - Rong)/2;
    int ToaDoTBRuongY = (CHIEU_CAO_MAN_HINH - Cao)/2;
    SDL_Rect KichThucTBRuong {ToaDoTBRuongX, ToaDoTBRuongY, Rong, Cao};
    SDL_SetRenderDrawColor(ManHinh, 255,255,255,240);
    SDL_RenderFillRect(ManHinh, &KichThucTBRuong); // fill
    SDL_SetRenderDrawColor(ManHinh, 0,0,0,255);
    SDL_RenderDrawRect(ManHinh, &KichThucTBRuong); //viền đen


    int Le = 16;
    SDL_Color MauDo{255,0,0,255};
    string NoiDung = "CHÚC MỪNG ĐÃ HOÀN THÀNH NHIỆM VỤ 2!";
    int ChieuRongNoiDung = 0, ChieuCaoNoiDung = 0;
    if (Font) TTF_SizeUTF8(Font, NoiDung.c_str(), &ChieuRongNoiDung, &ChieuCaoNoiDung);
    int ToaDoHoanThanhX = ToaDoTBRuongX + (Rong - ChieuRongNoiDung)/2;
    int ToaDoHoanThanhY = ToaDoTBRuongY + Cao/2 - 10;
    VeChu(ManHinh, Font, NoiDung, ToaDoHoanThanhX, ToaDoHoanThanhY, MauDo); // vẽ tiếu đề

    string Ket = "DI CHUYỂN ĐỂ TIẾP TỤC";
    int ChieuRongKet = 0, ChieuCaoKet = 0;
    if (Font) TTF_SizeUTF8(Font, Ket.c_str(), &ChieuRongKet, &ChieuCaoKet); // lấy ra chiều rộng và cao của tiêu đề
    int ToaDoKetX = ToaDoTBRuongX + (Rong - ChieuRongKet)/2;
    int ToaDoKetY = ToaDoTBRuongY + Cao - ChieuCaoKet - Le;
    VeChu(ManHinh, Font, Ket, ToaDoKetX, ToaDoKetY, {80,80,80,255});
}






