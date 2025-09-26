#include "DoiTuong.h"

DoiTuong::DoiTuong(){
    DoiTuong_ = NULL;
    HinhChuNhat_.x = 0;
    HinhChuNhat_.y = 0;
    HinhChuNhat_.h = 0;
    HinhChuNhat_.w = 0;
}

DoiTuong::~DoiTuong(){
    GiaiPhong();
}


bool DoiTuong::TaiAnh(std::string DuongDan, SDL_Renderer* ManHinh){
    GiaiPhong();
    SDL_Texture* DoiTuongTam = NULL;

    SDL_Surface* TaiNen = IMG_Load(DuongDan.c_str());
    if (TaiNen != NULL){
        SDL_SetColorKey(TaiNen, SDL_TRUE, SDL_MapRGB(TaiNen->format, SAC_MAU_DO, SAC_MAU_XANH_LA, SAC_MAU_XANH_DUONG));

        DoiTuongTam = SDL_CreateTextureFromSurface(ManHinh, TaiNen);
        if (DoiTuongTam != NULL){
            HinhChuNhat_.w = TaiNen->w; 
            HinhChuNhat_.h = TaiNen->h;
        }

        SDL_FreeSurface(TaiNen);
    }
    else{
        cout << "Loi tai nen " << endl;
    }
    DoiTuong_ = DoiTuongTam;
    return DoiTuong_ != NULL;
}


void DoiTuong::Ve(SDL_Renderer* ManHinh, const SDL_Rect* GioiHan /*= NULL*/ ){
    SDL_Rect ThongTinTrenManHinh {HinhChuNhat_ .x, HinhChuNhat_.y, HinhChuNhat_.w, HinhChuNhat_.h};

    SDL_RenderCopy(ManHinh, DoiTuong_, GioiHan, &ThongTinTrenManHinh);
    // GioiHan x, y, weidth, height la cat trong anh (nhan vat)
    // ThongTinTrenManHinh x, y, weidth, height la thong tin in tren man hinh
}


void DoiTuong::GiaiPhong(){
    if (DoiTuong_ != NULL){
        SDL_DestroyTexture(DoiTuong_);
        DoiTuong_ = NULL;
        HinhChuNhat_.h = 0;
        HinhChuNhat_.w = 0;
    }
}

