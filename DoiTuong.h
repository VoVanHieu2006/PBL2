#ifndef DOI_TUONG_H
#define DOI_TUONG_H


#include "HamChung.h"

class DoiTuong
{
public:

    DoiTuong();

    // Hàm hủy (destructor)
    ~DoiTuong();

    // Đặt vị trí cho đối tượng (tọa độ x, y)
    void DatViTri(const int& x, const int& y) { HinhChuNhat_.x = x; HinhChuNhat_.y = y; }

    // Lấy về thông tin hình chữ nhật (tọa độ & kích thước) của đối tượng
    SDL_Rect ThongTinHCN() const { return HinhChuNhat_; }

    // Lấy về texture (ảnh) của đối tượng
    SDL_Texture* ThongTinDoiTuong() const { return DoiTuong_; }

    virtual bool TaiAnh(std::string DuongLink, SDL_Renderer* ManHinh);

    void Ve(SDL_Renderer* ManHinh, const SDL_Rect* GioiHan = NULL);

    // Giải phóng
    void GiaiPhong();
protected:
    SDL_Texture* DoiTuong_; // Con trỏ tới texture (ảnh) của đối tượng
    SDL_Rect HinhChuNhat_;         // Hình chữ nhật mô tả vị trí/kích thước đối tượng
};

#endif