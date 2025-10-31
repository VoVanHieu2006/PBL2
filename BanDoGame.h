#ifndef BAN_DO_GAME_H
#define BAN_DO_GAME_H

#include "HamChung.h"
#include "DoiTuong.h"



#define TOI_DA_MAP_X 40
#define TOI_DA_MAP_Y 20


using json = nlohmann::json;

struct LopBanDo{
    unsigned int Khoi_[TOI_DA_MAP_Y + 5][TOI_DA_MAP_X + 5];
    string TenLop_;
};



struct DoiTuongTuongTac{
    SDL_Rect VungVaCham_;
    string NoiDung_;
    string Loai_;
    int SoLuong_;
};

struct Map{
    int BatDauX_;
    int BatDauY_;

    int ToiDaX_;
    int ToiDaY_;

    vector<LopBanDo> DanhSachLopBanDo_;
    vector<DoiTuongTuongTac> DanhSachDoiTuongTuongTac_;


    char* TenFile_;

    // Nhận biết khối tường
    int GidBatDauCuaTuong_;
    int GidKetThucCuatTuong_;

    // Nhận biết khối sách
    int GidBatDauCuaSach_;
    int GidKetThucCuatSach_;

    // Nhận biết khối cửa
    int GidBatDauCuaKhoa_;   // Door0.tsj (khóa)
    int GidKetThucCuaKhoa_;
    int GidBatDauCuaMo_;   // Door1.tsj (mở)
    int GidKetThucCuaMo_;
    
    int GidBatDauCuaBoss_;
    int GidKetThucCuaBoss_;
};




struct BoKhoi {
    int firstgid_; // GID (Global ID) của khối đầu tiên trong bộ này
    DoiTuong AnhCuaBoKhoi_; // Dùng lớp DoiTuong để tải và lưu giữ ảnh spritesheet

    map<int, SDL_Rect> CacVungCatKhoi_; // int là id, Rect là thông tin cắt
    int KichThuocKhoiThucTe_;
    string DiaChiDanBoKhoi_;

    
};




class BanDoGame{
public:
    BanDoGame();
    ~BanDoGame();

    // tải các thứ
    void TaiBanDoVaBoKhoi(char* DuongDanJson, SDL_Renderer* ManHinh);

    void TaiDuLieuBanDo(const json& DuLieuJson);

    void TaiCacBoKhoi(const json& DuLieuJson, SDL_Renderer* ManHinh);

    // vẽ bản đồ
    void VeBanDo(SDL_Renderer* ManHinh);


    // Lấy thông tin
     
    const Map& ThongTinBanDo() const { return BanDo_; }
    void DatLaiBanDo(Map BanDo){BanDo_ = BanDo;}

    vector<BoKhoi*> ThongTinDanhSachCacBoKhoi() const {return DanhSachCacBoKhoi_;}
    void DatLaiDanhSachCacBoKhoi(vector<BoKhoi*> DanhSachBoKhoi){ DanhSachCacBoKhoi_ = DanhSachBoKhoi; }

    // Kiểm tra va chạm 
    
    bool LaKhoiTuong(const int& Gid) const;

    bool LaKhoiSach(const int& Gid) const;

    bool LaCuaKhoa(const int& Gid) const;
    bool LaCuaMo(const int& Gid) const;
    //================== BOSSS==================
    bool LaKhoiBoss(const int& Gid) const;
    bool KiemTraVaChamVoiBoss(int tileX, int tileY) const ;

    void KhoaCuaTroChoi1(SDL_Rect TRO_CHOI_1);
    void MoCuaTroChoi1(SDL_Rect TRO_CHOI_1);

    void MoCuaTroChoi2(); // mở cửa



    // Lấy Gid và khối của lớp 
    Uint32 LayGid(int KhoiX, int KhoiY, int Lop) const;
    void XoaKhoi(int KhoiX, int KhoiY, int Lop);

    void SinhSachNgauNhien(const string& TenLopSach,const string& DiaChiDanBoKhoi, int KhoiBatDauX, int KhoiBatDauY, int ChieuRong, int ChieuCao, const int& SO_LUONG_SACH);
   

    // GAME 1:
    void HoanLaiSach(int x, int y, int IDCucBo);
private:

    Map BanDo_; // Vẫn giữ lại để chứa mảng 2D các con số (GID)
    
    // THAY ĐỔI LỚN: Bỏ mảng Khoi cũ, thay bằng một vector chứa các Bộ Khối
    vector<BoKhoi*> DanhSachCacBoKhoi_;
};

#endif