#include "AmThanh.h"
#include <iostream>


QuanLyAmThanh* AmThanhGame = nullptr;


QuanLyAmThanh::QuanLyAmThanh() {
    AmLuongNhacNen_ = 50;    
    AmLuongHieuUng_ = 96;    
    DaKhoiTao_ = false;
}

QuanLyAmThanh::~QuanLyAmThanh() {
    DongAmThanh();
}

bool QuanLyAmThanh::KhoiTao() {
    // Khởi tạo SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { // Tần số, định hướng mẫu âm thanh, số kênh, kích thước buffer (độ trễ);
        return false;
    }
    DaKhoiTao_ = true;
    cout << "SDL_mixer khoi tao thanh cong!" << endl;
    return true;
}

void QuanLyAmThanh::DongAmThanh() {
    if (!DaKhoiTao_) return;
    
    // Dừng tất cả âm thanh
    DungNhacNen();
    Mix_HaltChannel(-1); // Dừng hiệu ứng âm thanh
    
    // Giải phóng nhạc nền
    for (auto& pair : DanhSachNhacNen_) {
        if (pair.second) {
            Mix_FreeMusic(pair.second);
        }
    }
    DanhSachNhacNen_.clear();
    
    // Giải phóng hiệu ứng âm thanh
    for (auto& pair : DanhSachHieuUng_) {
        if (pair.second) {
            Mix_FreeChunk(pair.second);
        }
    }
    DanhSachHieuUng_.clear();
    
    Mix_CloseAudio();
    DaKhoiTao_ = false;
}

bool QuanLyAmThanh::TaiNhacNen(LoaiAmThanh Loai, const string& DuongDan) {
    Mix_Music* Nhac = Mix_LoadMUS(DuongDan.c_str());
    if (!Nhac) {
        cout << "Loi tai nhac: " << DuongDan << endl;
        return false;
    }
    
    // Xóa nhạc cũ nếu có
    if (DanhSachNhacNen_[Loai]) {
        Mix_FreeMusic(DanhSachNhacNen_[Loai]);
    }
    
    DanhSachNhacNen_[Loai] = Nhac;
    cout << "Tai thanh cong nhac: " << DuongDan << endl;
    return true;
}

bool QuanLyAmThanh::TaiHieuUngAmThanh(LoaiAmThanh Loai, const string& DuongDan) {
    Mix_Chunk* hieuUng = Mix_LoadWAV(DuongDan.c_str());
    if (!hieuUng) {
        return false;
    }
    
    // Xóa hiệu ứng cũ nếu có
    if (DanhSachHieuUng_[Loai]) {
        Mix_FreeChunk(DanhSachHieuUng_[Loai]);
    }
    
    DanhSachHieuUng_[Loai] = hieuUng;
    cout << "Tai hieu ung am thanh thanh cong: " << DuongDan << endl;
    return true;
}

void QuanLyAmThanh::PhatNhacNen(LoaiAmThanh Loai, int SoLanLap) {
    if (DanhSachNhacNen_.find(Loai) == DanhSachNhacNen_.end()) return;
    
    Mix_VolumeMusic(AmLuongNhacNen_);
    Mix_PlayMusic(DanhSachNhacNen_[Loai], SoLanLap);
}


void QuanLyAmThanh::PhatHieuUngAmThanh(LoaiAmThanh Loai) {
    if (DanhSachHieuUng_.find(Loai) == DanhSachHieuUng_.end()) return;
    
    Mix_VolumeChunk(DanhSachHieuUng_[Loai], AmLuongHieuUng_);
    Mix_PlayChannel(-1, DanhSachHieuUng_[Loai], 0);
}

void QuanLyAmThanh::DungNhacNen() {
    Mix_HaltMusic(); // Hàm tắt nhạc nền
}

void QuanLyAmThanh::DatAmLuongNhacNen(int amLuong) {
    AmLuongNhacNen_ = max(0, min(128, amLuong));
    Mix_VolumeMusic(AmLuongNhacNen_);
}

void QuanLyAmThanh::DatAmLuongHieuUng(int amLuong) {
    AmLuongHieuUng_ = max(0, min(128, amLuong));
}

bool QuanLyAmThanh::DangPhatNhacNen() const {
    return Mix_PlayingMusic();
}