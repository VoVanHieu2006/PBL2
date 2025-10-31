#include "RuongDo.h"

RuongDo::RuongDo(){
    TuiSach_ = nullptr;
    DanhSachTenSach_ = nullptr;

    // Trạng thái mini-game
    DangNhapTenSach_ = false;
    BufferNhap_ = "";
    ViTriDangCan_ = 0;
    ThuTuHienThi_ = "";     // chuỗi hiển thị dãy cần nhập
    ThongBaoLoi_ = "";      // lỗi hiển thị dưới ô nhập
    HoanThanhThuTu_ = false;
    // Trạng thái gợi ý 
    
    Cay_ = new TrieGoiY();
    ChiSoGoiYDangChon_ = -1;
    ThoiGianBatDau_ = 0;
    ThoiGianConLai_ = 0;
    DaHetGio_ = false;
    DangHienThiGameOver_ = false;
    DangNhapSoGoiY_ = false;
}


RuongDo::~RuongDo(){
    delete Cay_;
}

void RuongDo::GanThamChieuDuLieu(map<int,int>* TuiSach,
                                  const vector<string>* DSTenSach){
    TuiSach_ = TuiSach;
    DanhSachTenSach_ = DSTenSach;
}



string RuongDo::ChuanHoa(const string& SauChuaChuanHoa) const{
    string SauDuocChuanHoa = "";
    for (char KiTu : SauChuaChuanHoa){
        if (isalpha(KiTu)){
            SauDuocChuanHoa.push_back(tolower(KiTu));
        }
    }
    return SauDuocChuanHoa;
}

int RuongDo::TimLocalIDTheoTenSach(const string& TenSachChuaChuanHoa) const{
    if (!DanhSachTenSach_) return -1;
    string TenSachDaChuanHoa = ChuanHoa(TenSachChuaChuanHoa);
    for (int i = 0; i < (int)DanhSachTenSach_->size(); ++i){
        if (ChuanHoa((*DanhSachTenSach_)[i]) == TenSachDaChuanHoa) return i;
    }
    cout << "KHONG TIM DUOC ID THEO TEN" << endl;
    return -1;
}

void RuongDo::Mo(const DoiTuongTuongTac& DoiTuongRuong){
    if (HoanThanhThuTu_) return;
    
    if (!TuiSach_ || !DanhSachTenSach_){
        ThongBaoLoi_ = "Tham chiếu lỗi, không tìm thấy túi sách!";
        DangNhapTenSach_ = true;
        SDL_StartTextInput();
        return;
    }
    TuiSachTam_ = *TuiSach_;
    // Lấy danh sách loại sách đang có (số lượng > 0)
    RandomSachCanNhap();
    
    Cay_->XayDungTuDanhSach(*DanhSachTenSach_, *TuiSach_);
    CacGoiY_.clear();
    ChiSoGoiYDangChon_ = -1;
    
    // Khởi tạo bộ đếm thời gian (1 giây mỗi sách)
    int TongSoSach = 0;
    for (auto it : *TuiSach_) {
        TongSoSach += it.second;
    }
    cout << "TONG SO SACH: " << endl;
    ThoiGianConLai_ = TongSoSach * THOI_GIAN_CHO_MOI_CUON_SACH; 
    ThoiGianBatDau_ = SDL_GetTicks() / 1000;
    ThoiGianBanDau_ = TongSoSach * THOI_GIAN_CHO_MOI_CUON_SACH; 
    DaHetGio_ = false;

    DangHienThiGameOver_ = false;
    HoanThanhThuTu_ = false;
    DangNhapTenSach_ = true;
    SDL_StartTextInput();
}

void RuongDo::RandomSachCanNhap(){
    int SoLuongSachDangCo = 0;
    ThuTuYeuCau_.clear();
    ThuTuHienThi_.clear();
    for (auto& p : *TuiSach_) if (p.second > 0){
        ThuTuYeuCau_.push_back(p.first);  // đọc sách trong túi sách
        SoLuongSachDangCo += p.second;
    }
    if (ThuTuYeuCau_.empty()){
        ThongBaoLoi_ = "Bạn không có sách nào trong túi!";
        ThuTuHienThi_ = "Không có thứ tự (Túi trống).";
        DangNhapTenSach_ = true;
        SDL_StartTextInput();
        return;
    }

    // Số lượng cần nhập (mặc định 3 nếu không có property hoặc =0)
    int SoLuongSachKhauNhau = (int)ThuTuYeuCau_.size();
    

    // Random dãy cần nhập
    random_device rd;
    mt19937 rng(rd());
    shuffle(ThuTuYeuCau_.begin(), ThuTuYeuCau_.end(), rng);
    ThuTuHienThi_.clear();
    if (!ThuTuYeuCau_.empty()){
        int ID = ThuTuYeuCau_[0];
        ThuTuHienThi_ = (*DanhSachTenSach_)[ID];
    }
}



void RuongDo::Dong(bool ThanhCong){
    ViTriDangCan_ = 0;
    DangNhapTenSach_ = false;
    SDL_StopTextInput();
    if (ThanhCong) HoanThanhThuTu_ = true;
}

void RuongDo::XuLySuKien(const SDL_Event& e, const int& SO_LUONG_SACH){
    if (!DangNhapTenSach_) return;
    if (HoanThanhThuTu_) return;

    if (DangHienThiGameOver_) {
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
            DangHienThiGameOver_ = false;
            Dong(false);
        }
        return;
    }


    // Kiểm tra hết giờ
    CapNhatThoiGian();
    if (e.type == SDL_TEXTINPUT){  
        if (DangNhapSoGoiY_){
            DangNhapSoGoiY_ = false;
            return;
        }      
        BufferNhap_ += e.text.text;
        // Cập nhật gợi ý khi gõ
        CapNhatGoiY();
    } 
    else if (e.type == SDL_KEYDOWN){
        SDL_Keycode key = e.key.keysym.sym;
        
        if (key >= SDLK_1 && key <= SDLK_5) {
            DangNhapSoGoiY_ = true;
            int ChiSo = (int)(key - SDLK_1); // 0..4
            if (ChiSo < (int)CacGoiY_.size()) {
                ChiSoGoiYDangChon_ = ChiSo;
                BufferNhap_ = CacGoiY_[ChiSo];
            }
            return;
        }
        // Xử lý phím mũi tên
        

        if (key == SDLK_UP) {
            if (!CacGoiY_.empty()) {
                ChiSoGoiYDangChon_--;
                if (ChiSoGoiYDangChon_ < -1) ChiSoGoiYDangChon_ = (int)CacGoiY_.size() - 1;
            }
            return;
        }
        else if (key == SDLK_DOWN) {
            if (!CacGoiY_.empty()) {
                ChiSoGoiYDangChon_++;
                if (ChiSoGoiYDangChon_ >= (int)CacGoiY_.size()) ChiSoGoiYDangChon_ = -1;
            }
            return;
        }

        else if (key == SDLK_BACKSPACE){
            if (!BufferNhap_.empty()) {
                BufferNhap_.pop_back();
                // Cập nhật gợi ý khi xóa
                CapNhatGoiY();
                return;
            }
        } 
        else if (key == SDLK_ESCAPE){
            //  SỬA: Không cho thoát nữa
            ThongBaoLoi_ = "Không thể thoát! Phải hoàn thành!";
            return;
        } 
        else if (key == SDLK_RETURN){
            string TextCanNhap = BufferNhap_;
            
            //   Nếu có gợi ý được chọn, dùng gợi ý đó
            if (ChiSoGoiYDangChon_ >= 0 && ChiSoGoiYDangChon_ < (int)CacGoiY_.size()) {
                TextCanNhap = CacGoiY_[ChiSoGoiYDangChon_];
            }
            
            int IDNhap = TimLocalIDTheoTenSach(TextCanNhap);
            if (IDNhap < 0){
                ThongBaoLoi_ = "Không tìm thấy tên sách!";
                return;
            }

            auto CapChuaID = TuiSach_->find(IDNhap);
            if (CapChuaID == TuiSach_->end() || CapChuaID->second <= 0){
                ThongBaoLoi_ = "Túi đồ không có sách này!";
                return;
            }

            if (ChuanHoa(TextCanNhap) != ChuanHoa(ThuTuHienThi_)){
                ThongBaoLoi_ = "Sai thứ tự rồi bạn ơi!";
                return;
            }

            // Hợp lệ → trừ sách và chuyển bước
            ThongBaoLoi_ = "Nhập thành công tên sách!";
            CapChuaID->second--;
            
            BufferNhap_.clear();
            ChiSoGoiYDangChon_ = -1; //  THÊM: Reset chọn gợi ý
            CacGoiY_.clear();        //  THÊM: Xóa gợi ý cũ
            
            if (CapChuaID->second == 0){
                TuiSach_->erase(CapChuaID->first);
            }
            
            ViTriDangCan_++;
            if (ViTriDangCan_ >= SO_LUONG_SACH){
                Dong(true);
                return;
            }
            
            RandomSachCanNhap();
            //  THÊM: Cập nhật Trie sau khi thay đổi túi sách
            Cay_->XayDungTuDanhSach(*DanhSachTenSach_, *TuiSach_);
        }   
    }
}

// ============================= GỢI Ý ==========================
void RuongDo::CapNhatGoiY() {

    if (!Cay_ || !Cay_->Goc_) {
        cout << "LOI: Cay_ hoac Goc_ bi NULL trong CapNhatGoiY!" << endl;
        CacGoiY_.clear();
        return;
    }

    if (BufferNhap_.empty()) {
        CacGoiY_.clear();
        ChiSoGoiYDangChon_ = -1;
        return;
    }

    CacGoiY_ = Cay_->LayGoiY(BufferNhap_, SO_LUONG_SACH_GOI_Y_TOI_DA);
    ChiSoGoiYDangChon_ = -1; // Reset lựa chọn
}

// Hàm cập nhật thời gian
void RuongDo::CapNhatThoiGian() {
    if (DaHetGio_) return;
    
    Uint32 ThoiGianHienTai = SDL_GetTicks() / 1000;
    Uint32 ThoiGianDaTroi = (ThoiGianHienTai - ThoiGianBatDau_);
    if (ThoiGianConLai_ <= 0) {
        DaHetGio_ = true;
        ThoiGianConLai_ = 0;
        *TuiSach_ = TuiSachTam_;
        DangHienThiGameOver_ = true;
        BufferNhap_.clear();
        ThongBaoLoi_ = "Hết thời gian! Game over!";
    } else {
        ThoiGianConLai_ = ThoiGianBanDau_ - ThoiGianDaTroi; // Làm cho 2 biến này luôn nhỏ để tránh tràn số, tạo hiệu ứng đếm ngược
        // ThoiGianBatDau_ = ThoiGianHienTai; // Reset để tính lại
    }
}

void RuongDo::VeGameOver(SDL_Renderer* ManHinh, TTF_Font* Font) {
    // Overlay đen mờ
    SDL_SetRenderDrawBlendMode(ManHinh, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ManHinh, 0, 0, 0, 150);
    SDL_Rect ToanManHinh = {0, 0, CHIEU_RONG_MAN_HINH, CHIEU_CAO_MAN_HINH};
    SDL_RenderFillRect(ManHinh, &ToanManHinh);

    // Panel Game Over
    int Rong = 500, Cao = 300;
    int ToaDoX = (CHIEU_RONG_MAN_HINH - Rong) / 2;
    int ToaDoY = (CHIEU_CAO_MAN_HINH - Cao) / 2;
    SDL_Rect KhungGameOver = {ToaDoX, ToaDoY, Rong, Cao};
    
    SDL_SetRenderDrawColor(ManHinh, 255, 255, 255, 240);
    SDL_RenderFillRect(ManHinh, &KhungGameOver);
    SDL_SetRenderDrawColor(ManHinh, 255, 0, 0, 255);
    SDL_RenderDrawRect(ManHinh, &KhungGameOver);

    // Text Game Over
    SDL_Color MauDo = {255, 0, 0, 255};
    SDL_Color MauDen = {0, 0, 0, 255};
    
    string TieuDe = "GAME OVER!";
    int ChieuRongTieuDe = 0, ChieuCaoTieuDe = 0;
    TTF_SizeUTF8(Font, TieuDe.c_str(), &ChieuRongTieuDe, &ChieuCaoTieuDe);
    int ToaDoTieuDeX = ToaDoX + (Rong - ChieuRongTieuDe) / 2;
    int ToaDoTieuDeY = ToaDoY + 50;
    VeChu(ManHinh, Font, TieuDe, ToaDoTieuDeX, ToaDoTieuDeY, MauDo);
    
    string ThongBao = "Hết thời gian! Các sách đã được trả lại túi.";
    VeChu(ManHinh, Font, ThongBao, ToaDoX + 20, ToaDoTieuDeY + 50, MauDen);
    
    string HuongDan = "Nhấn ESC để đóng";
    VeChu(ManHinh, Font, HuongDan, ToaDoX + 20, ToaDoTieuDeY + 100, MauDen);
}

void RuongDo::VeChu(SDL_Renderer* ManHinh, TTF_Font* Font, const string& NoiDung, int x, int y, SDL_Color Mau){
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


void RuongDo::Ve(SDL_Renderer* ManHinh, TTF_Font* Font, const int& SO_LUONG_SACH){

     if (DangHienThiGameOver_) {
        VeGameOver(ManHinh, Font);
        return;
    }

    if (!DangNhapTenSach_) return;

    // Cập nhật thời gian
    CapNhatThoiGian();

    // Overlay
    SDL_SetRenderDrawBlendMode(ManHinh, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ManHinh, 0,0,0,110);
    SDL_Rect ToanManHInh = {0,0,CHIEU_RONG_MAN_HINH, CHIEU_CAO_MAN_HINH};
    SDL_RenderFillRect(ManHinh, &ToanManHInh);

    // Panel trung tâm, Mở rộng chiều cao để chứa gợi ý
    int Rong = 700, Cao = 400; // Tăng từ 200 lên 400
    int ToaDoTBRuongX = (CHIEU_RONG_MAN_HINH - Rong)/2;
    int ToaDoTBRuongY = (CHIEU_CAO_MAN_HINH - Cao)/2;
    SDL_Rect KichThucTBRuong {ToaDoTBRuongX, ToaDoTBRuongY, Rong, Cao};
    SDL_SetRenderDrawColor(ManHinh, 255,255,255,240);
    SDL_RenderFillRect(ManHinh, &KichThucTBRuong);
    SDL_SetRenderDrawColor(ManHinh, 0,0,0,255);
    SDL_RenderDrawRect(ManHinh, &KichThucTBRuong);

    int Le = 16;
    SDL_Color MauDen{0,0,0,255};
    SDL_Color MauDo{255,0,0,255};
    
    //  Hiển thị thời gian còn lại
    string TextThoiGian = "Thời gian còn lại: " + to_string(ThoiGianConLai_) + "s";
    SDL_Color mauThoiGian = (ThoiGianConLai_ <= 10) ? MauDo : MauDen;
    VeChu(ManHinh, Font, TextThoiGian, ToaDoTBRuongX + Le, ToaDoTBRuongY + 12, mauThoiGian);
    
    VeChu(ManHinh, Font, "NHẬP TÊN SÁCH THEO CHỈ DẪN:", ToaDoTBRuongX + Le, ToaDoTBRuongY + 42, MauDen);
    VeChu(ManHinh, Font, ThuTuHienThi_, ToaDoTBRuongX + Le, ToaDoTBRuongY + 72, MauDen);

    // Hộp nhập
    int ToaDoHopNhapX = ToaDoTBRuongX + Le;
    int ToaDoHopNhapY = ToaDoTBRuongY + 110;
    int HopNhapRong = Rong - Le * 2;
    int HopNhapCao = 40;

    SDL_Rect box {ToaDoHopNhapX, ToaDoHopNhapY, HopNhapRong, HopNhapCao};
    SDL_SetRenderDrawColor(ManHinh, 245,245,245,255);
    SDL_RenderFillRect(ManHinh, &box);
    SDL_SetRenderDrawColor(ManHinh, 0,0,0,255);
    SDL_RenderDrawRect(ManHinh, &box);

    int LeSoVoiHopNhap = 8; // Lề so với hộp nhập

    VeChu(ManHinh, Font, BufferNhap_, ToaDoHopNhapX + LeSoVoiHopNhap, ToaDoHopNhapY + LeSoVoiHopNhap, MauDen);

    // Con trỏ nháy
    Uint32 ticks = SDL_GetTicks();
    bool ThoiGianNhapNhay = ((ticks / 500) % 2) == 0;
    
    if (ThoiGianNhapNhay){
        int ChieuRongChu = 0, ChieuCaoChu = 0;
        TTF_SizeUTF8(Font, BufferNhap_.c_str(), &ChieuRongChu, &ChieuCaoChu);
        SDL_Rect ConTro { ToaDoHopNhapX + LeSoVoiHopNhap + ChieuRongChu + 2, ToaDoHopNhapY + LeSoVoiHopNhap, 10, ChieuCaoChu };
        SDL_SetRenderDrawColor(ManHinh, 0,0,0,255);
        SDL_RenderFillRect(ManHinh, &ConTro);
    }

    // Vẽ gợi ý
    if (!CacGoiY_.empty()) {
        VeChu(ManHinh, Font, "Gợi ý:", ToaDoHopNhapX, ToaDoHopNhapY + HopNhapCao + 15, MauDen);
        
        for (int i = 0; i < (int)CacGoiY_.size(); ++i) {
            SDL_Color mauGoiY = (i == ChiSoGoiYDangChon_) ? 
                SDL_Color {255,0,0,255} :  // Đỏ nếu được chọn
                SDL_Color{100, 100, 100, 255}; // Xám nếu không được chọn
                
            VeChu(ManHinh, Font, 
                  to_string(i + 1) + ". " + CacGoiY_[i], 
                  ToaDoHopNhapX + 20, 
                  ToaDoHopNhapY + HopNhapCao + 40 + (i * 25), 
                  mauGoiY);
        }
    }

    // Vẽ thông báo lỗi
    if (!ThongBaoLoi_.empty()){
        int ThongBaoLoiY = ToaDoHopNhapY + HopNhapCao + 40 + (SO_LUONG_SACH_GOI_Y_TOI_DA * 25) + 15;
        VeChu(ManHinh, Font, ThongBaoLoi_, ToaDoHopNhapX + LeSoVoiHopNhap, ThongBaoLoiY, MauDo);
    }

    //  Cập nhật hướng dẫn
    string huongDan = "[Enter] Xác nhận   [Mũi tên] Chọn gợi ý   [Backspace] Xóa";
    VeChu(ManHinh, Font, huongDan,
          ToaDoTBRuongX + 16, ToaDoTBRuongY + Cao - Le - 20, {80,80,80,255});
}