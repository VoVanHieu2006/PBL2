#ifndef AM_THANH_H
#define AM_THANH_H

#include "HamChung.h"



class QuanLyAmThanh {
public:
    QuanLyAmThanh();
    ~QuanLyAmThanh();
    
    bool KhoiTao();
    void DongAmThanh();
    
    // Tải âm thanh
    bool TaiNhacNen(LoaiAmThanh loai, const string& duongDan);
    bool TaiHieuUngAmThanh(LoaiAmThanh loai, const string& duongDan);
    
    // Phát âm thanh
    void PhatNhacNen(LoaiAmThanh loai, int soLanLap = -1); // -1 = lặp vô hạn
    void DungNhacNen();
    void PhatHieuUngAmThanh(LoaiAmThanh loai);
    
    // Điều chỉnh âm lượng
    void DatAmLuongNhacNen(int amLuong); // 0-128
    void DatAmLuongHieuUng(int amLuong); // 0-128
    
    // Kiểm tra trạng thái
    bool DangPhatNhacNen() const;
    
private:
    map<LoaiAmThanh, Mix_Music*> DanhSachNhacNen_;
    map<LoaiAmThanh, Mix_Chunk*> DanhSachHieuUng_;
    
    int AmLuongNhacNen_;
    int AmLuongHieuUng_;
    bool DaKhoiTao_;
};

#endif