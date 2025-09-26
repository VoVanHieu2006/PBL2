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

    // Lấy danh sách loại sách đang có (số lượng > 0)
    RandomSachCanNhap();
    
    
    HoanThanhThuTu_ = false;
    DangNhapTenSach_ = true;
    SDL_StartTextInput();
}

void RuongDo::RandomSachCanNhap(){
    ThuTuYeuCau_.clear();
    ThuTuHienThi_.clear();
    for (auto& p : *TuiSach_) if (p.second > 0) ThuTuYeuCau_.push_back(p.first);  // đọc sách trong túi sách

    if (ThuTuYeuCau_.empty()){
        ThongBaoLoi_ = "Bạn không có sách nào trong túi!";
        ThuTuHienThi_ = "Không có thứ tự (Túi trống).";
        DangNhapTenSach_ = true;
        SDL_StartTextInput();
        return;
    }

    // Số lượng cần nhập (mặc định 3 nếu không có property hoặc =0)
    int SoLuong = (int)ThuTuYeuCau_.size();
    cout << "so luong " << SoLuong << endl;

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
    DangNhapTenSach_ = false;
    SDL_StopTextInput();
    if (ThanhCong) HoanThanhThuTu_ = true;
}

void RuongDo::XuLySuKien(const SDL_Event& e, const int& SO_LUONG_SACH){
    if (!DangNhapTenSach_) return;
    if (HoanThanhThuTu_) return;


    if (e.type == SDL_TEXTINPUT){
        BufferNhap_ += e.text.text; 
    } 
    else if (e.type == SDL_KEYDOWN){
        SDL_Keycode key = e.key.keysym.sym;
        if (key == SDLK_BACKSPACE){
            if (!BufferNhap_.empty()) BufferNhap_.pop_back();
        } 
        else if (key == SDLK_ESCAPE){
            Dong(false);
        } 
        else if (key == SDLK_RETURN){ // phím enter
            int IDNhap = TimLocalIDTheoTenSach(BufferNhap_); // xử lý theo ID
            if (IDNhap < 0){
                ThongBaoLoi_ = "Không tìm thấy tên sách!";  // bấm bậy
                return;
            }

            
            auto CapChuaID = TuiSach_->find(IDNhap); // tìm cặp pair
            if (CapChuaID == TuiSach_->end() || CapChuaID->second <= 0){
                ThongBaoLoi_ = "Túi đồ không có sách này!";
                return;
            }

            if (ChuanHoa(BufferNhap_) != ChuanHoa(ThuTuHienThi_)){
                ThongBaoLoi_ = "Sai thứ tự rồi bạn ơi!";  // bấm bậy
                return;
            }


            // Hợp lệ → trừ sách và chuyển bước
            ThongBaoLoi_ = "Nhập thành công tên sách!";
            CapChuaID->second -= 1;
            
            BufferNhap_.clear();
            if (CapChuaID->second == 0){
                TuiSach_->erase(CapChuaID->first);
            }
            
            ViTriDangCan_++;
            cout << "vi tri dang van " << ViTriDangCan_ << endl; 
            if (ViTriDangCan_ >= SO_LUONG_SACH){
                
                
                Dong(true);
                return;
            }
            RandomSachCanNhap();
        }   
    }
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
    if (!DangNhapTenSach_) return;

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
    SDL_Color MauDen{0,0,0,255};
    VeChu(ManHinh, Font, "NHẬP TÊN SÁCH THEO CHỈ DẪN:", ToaDoTBRuongX + Le, ToaDoTBRuongY + 12, MauDen); // vẽ tiếu đề
    VeChu(ManHinh, Font, ThuTuHienThi_, ToaDoTBRuongX + Le, ToaDoTBRuongY + 42, MauDen); // vẽ thông báo thứ tự hiện tại

    // Hộp nhập
    int ToaDoHopNhapX = ToaDoTBRuongX + Le;
    int ToaDoHopNhapY = ToaDoTBRuongY + 80;
    int HopNhapRong = Rong - Le * 2;
    int HopNhapCao = 40;

    SDL_Rect box {ToaDoHopNhapX, ToaDoHopNhapY, HopNhapRong, HopNhapCao};
    SDL_SetRenderDrawColor(ManHinh, 245,245,245,255);
    SDL_RenderFillRect(ManHinh, &box);
    SDL_SetRenderDrawColor(ManHinh, 0,0,0,255);
    SDL_RenderDrawRect(ManHinh, &box);


    int LeSoVoiHopNhap = 8;
    // Vẽ chữ đã nhập
    VeChu(ManHinh, Font, BufferNhap_, ToaDoHopNhapX + LeSoVoiHopNhap, ToaDoHopNhapY + LeSoVoiHopNhap, MauDen);

    // Con trỏ nháy
    Uint32 ticks = SDL_GetTicks();  // trả về thời gian mili giây 
    bool ThoiGianNhapNhay = ((ticks / 500) % 2) == 0;
    
    if (ThoiGianNhapNhay){
        int ChieuRongChu = 0, ChieuCaoChu = 0;
        TTF_SizeUTF8(Font, BufferNhap_.c_str(), &ChieuRongChu, &ChieuCaoChu);

        SDL_Rect ConTro { ToaDoHopNhapX + LeSoVoiHopNhap + ChieuRongChu + 2, ToaDoHopNhapY + LeSoVoiHopNhap, 10, ChieuCaoChu };
        SDL_SetRenderDrawColor(ManHinh, 0,0,0,255);
        SDL_RenderFillRect(ManHinh, &ConTro);
    }

    // vẽ thông báo lỗi
    if (!ThongBaoLoi_.empty()){
        VeChu(ManHinh, Font, ThongBaoLoi_, ToaDoHopNhapX + LeSoVoiHopNhap, ToaDoHopNhapY + HopNhapCao + 12, {200,0,0,255});
    } // phía dưới hộp nhập

    VeChu(ManHinh, Font, "[Enter] Xác nhận   [Backspace] Xóa   [Esc] Thoát",
    ToaDoTBRuongX + 16, ToaDoTBRuongY + Cao - Le - 20, {80,80,80,255});
}