#include "BanDoGame.h"


BanDoGame::BanDoGame(){
    BanDo_.BatDauX_ = 0;
    BanDo_.BatDauY_ = 0;
    BanDo_.ToiDaX_ = 0;
    BanDo_.ToiDaY_ = 0;

    BanDo_.GidBatDauCuaTuong_ = -1;
    BanDo_.GidKetThucCuatTuong_ = -1;

    BanDo_.GidBatDauCuaSach_ = -1;
    BanDo_.GidKetThucCuatSach_ = -1;

    BanDo_.GidBatDauCuaKhoa_ = -1;   // Door0.tsj (khóa)
    BanDo_.GidKetThucCuaKhoa_= -1;
    BanDo_.GidBatDauCuaMo_ = -1;   // Door1.tsj (mở)
    BanDo_.GidKetThucCuaMo_ = -1;

    BanDo_.GidBatDauCuaBoss_ = -1;
    BanDo_.GidKetThucCuaBoss_ = -1;
}

BanDoGame::~BanDoGame(){
    for (BoKhoi* XoaBoKhoi: DanhSachCacBoKhoi_){
        delete XoaBoKhoi;
    }
    DanhSachCacBoKhoi_.clear();
}



// Hàm chính để tải mọi thứ
void BanDoGame::TaiBanDoVaBoKhoi(char* DuongDanJson, SDL_Renderer* ManHinh) {
    ifstream f(DuongDanJson);
    if (!f.is_open()) {
        cout << "Loi: Khong the mo file JSON cua Tiled: " << DuongDanJson << endl;
        return;
    }

    json DuLieuJson = json::parse(f); // nhận dữ liệu từ file json
    f.close();

    // Bước 1: Đọc dữ liệu các lớp (layer) vào mảng BanDo_.Khoi_


    TaiDuLieuBanDo(DuLieuJson);
    BanDo_.TenFile_ = DuongDanJson;

    // Bước 2: Đọc thông tin các bộ khối (tilesets), tải ảnh và lưu các vùng cắt
    TaiCacBoKhoi(DuLieuJson, ManHinh);



}


void BanDoGame::TaiDuLieuBanDo(const json& DuLieuJson) {

    int Rong = DuLieuJson["width"].get<int>();
    int Cao = DuLieuJson["height"].get<int>();
    
    BanDo_.ToiDaX_ = Rong * KICH_THUOC_KHOI;
    BanDo_.ToiDaY_ = Cao * KICH_THUOC_KHOI;
    

    // Đọc dữ liệu của lớp đầu tiên (layer 0)
    // Tiled lưu dưới dạng mảng 1D, chúng ta chuyển nó thành mảng 2D

    

    for (const auto& CacLop : DuLieuJson["layers"]){

        // Tải type = tilelayer

        if (CacLop["type"] == "tilelayer"){
            const auto& DuLieuLop = CacLop["data"];
            
            LopBanDo LopTam;
            LopTam.TenLop_ = CacLop["name"];
            cout << LopTam.TenLop_ << endl;
            for (int y = 0; y < Cao; y++) {
                for (int x = 0; x < Rong; x++) {
                    int chi_so = y * Rong + x;
                    LopTam.Khoi_[y][x] = DuLieuLop[chi_so];
                }
            }
            BanDo_.DanhSachLopBanDo_.push_back(LopTam);
        }


        // Tải type = objectgroup

        else if (CacLop["type"] == "objectgroup"){
            for (const auto& ThongTin : CacLop["objects"]){

                // Va chạm với BẢNG THÔNG BÁO
               

                    DoiTuongTuongTac DoiTuongMoi;
                    DoiTuongMoi.Loai_ = ThongTin["type"];

                    DoiTuongMoi.VungVaCham_.x = ThongTin["x"].get<int>() * 2;
                    DoiTuongMoi.VungVaCham_.y = ThongTin["y"].get<int>() * 2;
                    DoiTuongMoi.VungVaCham_.w = ThongTin["width"].get<int>() ;
                    DoiTuongMoi.VungVaCham_.h = ThongTin["height"].get<int>() ;
                     
                    // Chỉ có bảng thông báo mới có thông tin chi tiết
                    if (ThongTin.contains("properties")) {
                        for (const auto& ThongTinChiTiet : ThongTin["properties"]) {
                            DoiTuongMoi.NoiDung_ = ThongTinChiTiet["value"];
                        }
                    }
                    BanDo_.DanhSachDoiTuongTuongTac_.push_back(DoiTuongMoi);
               



            }

        }

    }

}


void BanDoGame::TaiCacBoKhoi(const json& DuLieuJson, SDL_Renderer* ManHinh) {
    
    string ThuMucMap = "map/"; 
    
    // Vòng lặp này duyệt qua mảng "tilesets" trong file map chính (.tmj)
    for (const auto& ThongTinBoKhoiTrongMap : DuLieuJson["tilesets"]) {


        BoKhoi* BoKhoiMoi = new BoKhoi();  // LỖI CHỖ NÀY, PHẢI GỌI CON TRỎ NẾU KO THÌ SẼ KO ĐÚNG VÌ TEXTURE MỖI LẦN BỊ HỦY
        // LƯU LÀ PHẢI LƯU CON TRÒ VÌ MỖI LẦN CHẠY TEXTURE BỊ HỦY 
        BoKhoiMoi->firstgid_ = ThongTinBoKhoiTrongMap["firstgid"];
        

        // Lấy tên file nguồn (ví dụ: "Wall.tsj") từ file map
        string TenFileNguon = ThongTinBoKhoiTrongMap["source"];
        // Ghép tên thư mục với tên file để có đường dẫn đầy đủ
        string DuongDanDayDuToiTsj = ThuMucMap + TenFileNguon;
        cout << DuongDanDayDuToiTsj << endl;
        // Mở và đọc file .tsj (file tileset)


        ifstream f_boKhoi(DuongDanDayDuToiTsj);
        if (!f_boKhoi.is_open()) {
            cout << "Loi: Khong the mo file tileset .tsj: " << DuongDanDayDuToiTsj << endl;
            continue; // Nếu không mở được, bỏ qua bộ khối này và tiếp tục
        }
        // Phân tích file .tsj thành một đối tượng JSON khác
        BoKhoiMoi->DiaChiDanBoKhoi_ = DuongDanDayDuToiTsj;
        json DuLieuBoKhoiJson = json::parse(f_boKhoi);
        f_boKhoi.close();


        // Xử lý nếu là TƯỜNG:
        if (DuongDanDayDuToiTsj == "map/Tuong.tsj"){
            BanDo_.GidBatDauCuaTuong_ = BoKhoiMoi->firstgid_;
            int SoLuongKhoiTrongBoKhoi = DuLieuBoKhoiJson["tilecount"];
            BanDo_.GidKetThucCuatTuong_ = BanDo_.GidBatDauCuaTuong_ + SoLuongKhoiTrongBoKhoi - 1;
            cout << "PHAT HIEN TUONG TU GID: " << BanDo_.GidBatDauCuaTuong_ << " DEN " << BanDo_.GidKetThucCuatTuong_ << endl;
        }

        // Xử lý nếu là SÁCH
        if (DuongDanDayDuToiTsj == "map/Book.tsj"){
            BanDo_.GidBatDauCuaSach_ = BoKhoiMoi->firstgid_;
            int SoLuongKhoiTrongBoKhoi = DuLieuBoKhoiJson["tilecount"];
            BanDo_.GidKetThucCuatSach_ = BanDo_.GidBatDauCuaSach_ + SoLuongKhoiTrongBoKhoi - 1;
            cout << "PHAT HIEN TUONG TU GID: " << BanDo_.GidBatDauCuaSach_ << " DEN " << BanDo_.GidKetThucCuatSach_ << endl;
        }

        // Xử lý cửa
        if (DuongDanDayDuToiTsj == "map/Door0.tsj"){
            BanDo_.GidBatDauCuaKhoa_ = BoKhoiMoi->firstgid_;
            int SoLuongKhoiTrongBoKhoi = DuLieuBoKhoiJson["tilecount"];
            BanDo_.GidKetThucCuaKhoa_ = BoKhoiMoi->firstgid_ + SoLuongKhoiTrongBoKhoi - 1;
        } else if (DuongDanDayDuToiTsj == "map/Door1.tsj"){
            BanDo_.GidBatDauCuaMo_ = BoKhoiMoi->firstgid_;
            int SoLuongKhoiTrongBoKhoi = DuLieuBoKhoiJson["tilecount"];
            BanDo_.GidKetThucCuaMo_ = BoKhoiMoi->firstgid_ + SoLuongKhoiTrongBoKhoi - 1;
        }

        if (DuongDanDayDuToiTsj == "map/Rogue.tsj"){
        BanDo_.GidBatDauCuaBoss_ = BoKhoiMoi->firstgid_;
        int SoLuongKhoiTrongBoKhoi = DuLieuBoKhoiJson["tilecount"];
        BanDo_.GidKetThucCuaBoss_ = BoKhoiMoi->firstgid_ + SoLuongKhoiTrongBoKhoi - 1;
        }



        // Bây giờ, chúng ta lấy thông tin từ file .tsj đã đọc
        
        // Lấy tên file ảnh (ví dụ "Wall.png") từ DuLieuBoKhoiJson
        string TenFileAnh = DuLieuBoKhoiJson["image"];
        string DuongDanAnh = "map/" + TenFileAnh; // Ghép với thư mục chứa ảnh
        cout << DuongDanAnh << endl;

        // Tải ảnh ảnh bộ khối lớn

        if (!BoKhoiMoi->AnhCuaBoKhoi_.TaiAnh(DuongDanAnh, ManHinh)){
            cout << "Tai anh that bai" << endl;
        }
        else cout << "Tai anh thanh cong" << endl;

        // Cắt khối ra, từng ảnh nhỏ
       
        BoKhoiMoi->KichThuocKhoiThucTe_ = DuLieuBoKhoiJson["tileheight"].get<int>();
        
        int BKM_KTKTT = BoKhoiMoi->KichThuocKhoiThucTe_; // tạo biến tạm viết cho ngắn
        int SoKhoiChieuRong = DuLieuBoKhoiJson["imagewidth"].get<int>() / BKM_KTKTT; // gán theo kiểu json
        int SoKhoiChieuCao = DuLieuBoKhoiJson["imageheight"].get<int>() / BKM_KTKTT;
        for (int y = 0; y < SoKhoiChieuCao; y++){
            for (int x = 0; x < SoKhoiChieuRong; x++){
                int IDCucBo = y * SoKhoiChieuRong + x; //  IDCucbo nó sẽ bằng gid - FirstGid nên bắt đầu từ 0
                SDL_Rect CatKhoi;
                CatKhoi.x = x * BKM_KTKTT;
                CatKhoi.y = y * BKM_KTKTT;
                CatKhoi.w = BKM_KTKTT;
                CatKhoi.h = BKM_KTKTT;
                BoKhoiMoi->CacVungCatKhoi_.insert(make_pair(IDCucBo, CatKhoi));
            }
        }

        DanhSachCacBoKhoi_.push_back(BoKhoiMoi);      
                                                    
                                                        
    }       

}


void BanDoGame::VeBanDo(SDL_Renderer* ManHinh) {
    int x1 = BanDo_.BatDauX_;
    int x2 = BanDo_.ToiDaX_;
    int y1 = BanDo_.BatDauY_;
    int y2 = BanDo_.ToiDaY_;

    // một khối có thể xoay nên đây là cái xử lý
    // có 32 bit, 3 bit đầu để kí hiệu lật hay ko, những bit sau thì để lưu số
    const uint32_t FLIP_H = 0x80000000;   // lật theo chiều ngang, bấm x
    const uint32_t FLIP_V = 0x40000000;   // lật theo chiều dọc, bấm y
    const uint32_t FLIP_D = 0x20000000;   // hỗ trợ khi bấm z
    const uint32_t GID_MASK = 0x1FFFFFFF; // che 3 bit đầu để còn lại những bit sau thôi
    
    for (LopBanDo LopHienTai : BanDo_.DanhSachLopBanDo_){
        int MapX = 0, MapY = 0;

        for (int y = y1; y < y2; y+= KICH_THUOC_KHOI){
            MapX = 0;
            for (int x = x1; x < x2; x += KICH_THUOC_KHOI){
                unsigned int GidBanDau = LopHienTai.Khoi_[MapY][MapX];
                //cout << gid << " ";
                if (GidBanDau == 0){
                    MapX++;
                    continue;
                }

                // Kiểm tra có xoay hay lật gì ko 
                bool flipH = (GidBanDau & FLIP_H) != 0;
                bool flipV = (GidBanDau & FLIP_V) != 0;
                bool flipD = (GidBanDau & FLIP_D) != 0; 

                unsigned int GidDaXuLy = GidBanDau & GID_MASK;



                BoKhoi* BoKhoiMucTieu = nullptr;
                for (int TimBoKhoi = DanhSachCacBoKhoi_.size() - 1; TimBoKhoi >= 0; TimBoKhoi--){
                    if (GidDaXuLy >= DanhSachCacBoKhoi_[TimBoKhoi]->firstgid_) {
                        BoKhoiMucTieu = DanhSachCacBoKhoi_[TimBoKhoi];
                        break;
                    }
                }
                if (BoKhoiMucTieu == nullptr){
                    cout << "Loi khong tim duoc bo khoi phu hop" << endl;
                }
                int IDCucBo = GidDaXuLy - BoKhoiMucTieu->firstgid_;

                // Xác định bộ khối cần vẽ 
                SDL_Texture *BoKhoiCanVe = BoKhoiMucTieu->AnhCuaBoKhoi_.ThongTinDoiTuong();
            
                // Cắt trong bộ khối ra khối cần tìm
                SDL_Rect CatKhoiHienTai;
                auto TimKiem = BoKhoiMucTieu->CacVungCatKhoi_.find(IDCucBo);
                if (TimKiem != BoKhoiMucTieu->CacVungCatKhoi_.end()){ // tìm thấy
                    CatKhoiHienTai = TimKiem->second;
                }
                else continue;

                // Set vị trí trên màn hình + kích thước mỗi khối
                SDL_Rect ViTriTrenManHinh;
                ViTriTrenManHinh.x = x;
                ViTriTrenManHinh.y = y;
                ViTriTrenManHinh.w = KICH_THUOC_KHOI;       
                ViTriTrenManHinh.h = KICH_THUOC_KHOI;



                // SDL_RenderCopy(ManHinh,
                //             BoKhoiCanVe,
                //             &CatKhoiHienTai,
                //             &ViTriTrenManHinh);
                SDL_RendererFlip CheDoLat = SDL_FLIP_NONE;
                if (flipD) {
                    double Do = 0.0;
    
                    if (flipH && flipV) {
                        // H + V + D = xoay 270 độ
                        Do = 270.0;
                    } else if (flipH) {
                        // H + D = xoay 90 độ
                        Do = 90.0;
                    } else if (flipV) {
                        // V + D = xoay 270 độ + flip horizontal
                        Do = 270.0;
                        CheDoLat = SDL_FLIP_HORIZONTAL;
                    } else {
                        // chỉ D = xoay 90 độ + flip vertical
                        Do = 90.0;
                        CheDoLat = SDL_FLIP_VERTICAL;
                    }
                }   
                else {
                    if (flipH && flipV) {
                        // H+V = xoay 180 độ (SDL không có trực tiếp) -> có thể dùng SDL_RenderCopyEx + angle 180
                        // tạm: xử lý riêng
                        SDL_RenderCopyEx(ManHinh, BoKhoiCanVe,
                                        &CatKhoiHienTai, &ViTriTrenManHinh, 180.0, nullptr, SDL_FLIP_NONE);
                        MapX++;
                        continue;
                    } else {
                        if (flipH) CheDoLat = (SDL_RendererFlip)(CheDoLat | SDL_FLIP_HORIZONTAL);
                        if (flipV) CheDoLat = (SDL_RendererFlip)(CheDoLat | SDL_FLIP_VERTICAL);
                    }
                }

               SDL_RenderCopyEx(ManHinh, BoKhoiCanVe,
                    &CatKhoiHienTai, &ViTriTrenManHinh, 0.0, nullptr, CheDoLat);
                            


                MapX++;
            }
            MapY++;
        }
    }

}



bool BanDoGame::LaKhoiTuong(const int& Gid) const {
    if (BanDo_.GidBatDauCuaTuong_ == -1) {
        cout << "KHONG CO KHOI TUONG" << endl;
        return false;
    }

    return (Gid >= BanDo_.GidBatDauCuaTuong_ && Gid <= BanDo_.GidKetThucCuatTuong_);
}

bool BanDoGame::LaKhoiSach(const int& Gid) const{
    if (BanDo_.GidBatDauCuaSach_ == -1){
        cout << "KHONG CO KHOI SACH" << endl;
        return false;
    }
    return (Gid >= BanDo_.GidBatDauCuaSach_&& Gid <= BanDo_.GidKetThucCuatSach_);
}


bool BanDoGame::LaCuaKhoa(const int& Gid) const{
    if (BanDo_.GidBatDauCuaKhoa_ == -1) return false;
    return (Gid >= BanDo_.GidBatDauCuaKhoa_ && Gid <= BanDo_.GidKetThucCuaKhoa_);
}

bool BanDoGame::LaCuaMo(const int& Gid) const{
    if (BanDo_.GidBatDauCuaMo_ == -1){
        cout << "KHONG CO CUA MO NAO CA" << endl;
        return false;
    }
    return (Gid >= BanDo_.GidBatDauCuaMo_ && Gid <= BanDo_.GidKetThucCuaMo_);
}

bool BanDoGame::LaKhoiBoss(const int& Gid) const {
    if (BanDo_.GidBatDauCuaBoss_ == -1) return false;
    return (Gid >= BanDo_.GidBatDauCuaBoss_ && Gid <= BanDo_.GidKetThucCuaBoss_);
}


void BanDoGame::MoCuaTroChoi2(){
    if (BanDo_.GidBatDauCuaKhoa_ == -1 || BanDo_.GidBatDauCuaMo_ == -1) {
        cout << "Chua co tileset cua Door0/Door1" << endl;;
        return;
    }
    int GioiHanTroChoi2X = 10;
    const uint32_t GID_MASK = 0x1FFFFFFF; // đề phòng các khối xoay
    for (auto& Lop : BanDo_.DanhSachLopBanDo_){
        for (int y = 0; y < TOI_DA_MAP_Y; ++y){
            for (int x = 0; x < GioiHanTroChoi2X; ++x){
                unsigned int GidBanDau = Lop.Khoi_[y][x];
                if (GidBanDau == 0) continue;
                int GidDaXuLy = (GidBanDau & GID_MASK);
                if (LaCuaKhoa(GidDaXuLy)){
                    int IDCucBo = GidDaXuLy - BanDo_.GidBatDauCuaKhoa_;
                    int GidCuaCuaMo = BanDo_.GidBatDauCuaMo_ + IDCucBo;
                    Lop.Khoi_[y][x] = (unsigned int)GidCuaCuaMo; // không set flip
                }
            }
        }
    }
}


void BanDoGame::KhoaCuaTroChoi1(SDL_Rect TRO_CHOI_1){
    if (BanDo_.GidBatDauCuaKhoa_ == -1 || BanDo_.GidBatDauCuaMo_ == -1) {
        cout << "Chua co tileset cua Door0/Door1" << endl;;
        return;
    }
    const uint32_t GID_MASK = 0x1FFFFFFF; // đề phòng các khối xoay
    for (auto& Lop : BanDo_.DanhSachLopBanDo_){
        for (int y = TRO_CHOI_1.y; y < TRO_CHOI_1.y + TRO_CHOI_1.h; ++y){
                unsigned int GidBanDau1 = Lop.Khoi_[y][TRO_CHOI_1.x - 1];
                unsigned int GidBanDau2 = Lop.Khoi_[y][TRO_CHOI_1.x + TRO_CHOI_1.w];
                int GidDaXuLy1 = (GidBanDau1 & GID_MASK);
                int GidDaXuLy2 = (GidBanDau2 & GID_MASK);
                if (LaCuaMo(GidDaXuLy1)){
                    int IDCucBo = GidDaXuLy1 - BanDo_.GidBatDauCuaMo_;
                    int GidCuaCuaKhoa = BanDo_.GidBatDauCuaKhoa_ + IDCucBo;
                    Lop.Khoi_[y][TRO_CHOI_1.x - 1] = (unsigned int)GidCuaCuaKhoa; // không set flip
                }
                if (LaCuaMo(GidDaXuLy2)){
                    int IDCucBo = GidDaXuLy2 - BanDo_.GidBatDauCuaMo_;
                    int GidCuaCuaKhoa = BanDo_.GidBatDauCuaKhoa_ + IDCucBo;
                    Lop.Khoi_[y][TRO_CHOI_1.x + TRO_CHOI_1.w] = (unsigned int)GidCuaCuaKhoa; // không set flip
                }
        }
    }
}


void BanDoGame::MoCuaTroChoi1(SDL_Rect TRO_CHOI_1){
    if (BanDo_.GidBatDauCuaKhoa_ == -1 || BanDo_.GidBatDauCuaMo_ == -1) {
        cout << "Chua co tileset cua Door0/Door1" << endl;;
        return;
    }
   
    const uint32_t GID_MASK = 0x1FFFFFFF; // đề phòng các khối xoay
    for (auto& Lop : BanDo_.DanhSachLopBanDo_){
        for (int y = TRO_CHOI_1.y; y < TRO_CHOI_1.y + TRO_CHOI_1.h; ++y){
                unsigned int GidBanDau1 = Lop.Khoi_[y][TRO_CHOI_1.x - 1];
                unsigned int GidBanDau2 = Lop.Khoi_[y][TRO_CHOI_1.x + TRO_CHOI_1.w];
                int GidDaXuLy1 = (GidBanDau1 & GID_MASK);
                int GidDaXuLy2 = (GidBanDau2 & GID_MASK);
                if (LaCuaKhoa(GidDaXuLy1)){
                    int IDCucBo = GidDaXuLy1 - BanDo_.GidBatDauCuaKhoa_;
                    int GidCuaCuaMo = BanDo_.GidBatDauCuaMo_ + IDCucBo;
                    Lop.Khoi_[y][TRO_CHOI_1.x - 1] = (unsigned int)GidCuaCuaMo; // không set flip
                }
                if (LaCuaKhoa(GidDaXuLy2)){
                    int IDCucBo = GidDaXuLy2 - BanDo_.GidBatDauCuaKhoa_;
                    int GidCuaCuaMo = BanDo_.GidBatDauCuaMo_ + IDCucBo;
                    Lop.Khoi_[y][TRO_CHOI_1.x + TRO_CHOI_1.w] = (unsigned int)GidCuaCuaMo; // không set flip
                }
        }
    }
}


// Phục vụ cho là khối sách 
Uint32 BanDoGame::LayGid(int KhoiX, int KhoiY, int Lop) const{
    if (KhoiX < 0 || KhoiX > TOI_DA_MAP_X ||
        KhoiY < 0 || KhoiY > TOI_DA_MAP_Y){
            cout << "LAY GID LOI" << endl;
            return 0;
    }

    if (Lop < 0 || Lop >= (int)BanDo_.DanhSachLopBanDo_.size()){
        cout << "LAY GID LOI" << endl;
        return 0;
    }

    return BanDo_.DanhSachLopBanDo_[Lop].Khoi_[KhoiY][KhoiX];
    
}
void BanDoGame::XoaKhoi(int KhoiX, int KhoiY, int Lop){
    if (KhoiX < 0 || KhoiX > TOI_DA_MAP_X ||
        KhoiY < 0 || KhoiY > TOI_DA_MAP_Y){
            cout << "XOA KHOI LOI" << endl;
            return;
    }

    if (Lop < 0 || Lop >= (int)BanDo_.DanhSachLopBanDo_.size()){
        cout << "XOA KHOI LOI" << endl;
        return;
    }
    BanDo_.DanhSachLopBanDo_[Lop].Khoi_[KhoiY][KhoiX] = 0;
}


void BanDoGame::SinhSachNgauNhien(const string& TenLopSach, const string& DiaChiDanBoKhoi, int KhoiBatDauX, 
                        int KhoiBatDauY, int ChieuRong , int ChieuCao, const int& SO_LUONG_SACH){
    // Tạo bộ sinh số ngẫu nhiên


    random_device rd;	
	mt19937 rng(rd());
    
    
    // Tìm lớp sách theo tên
    int LopSach = -1;
    for (int i = 0; i < (int)BanDo_.DanhSachLopBanDo_.size(); i++){
        if (BanDo_.DanhSachLopBanDo_[i].TenLop_ == TenLopSach){
            LopSach = i;
            break;
        }
    }
    if (LopSach == -1){
        cout << "KHONG TIM THAY LOP SACH" << endl;
        return;
    }
    // Diện tích vùng nhỏ
    int DienTich = ChieuRong * ChieuCao;

    int GidBatDauKhoiSach = -1;
    int SoSachTrongKhoi = 63;
    // Danh sách khối có thể sinh
    for (auto i : DanhSachCacBoKhoi_){
        if (i->DiaChiDanBoKhoi_ == DiaChiDanBoKhoi){
            GidBatDauKhoiSach = i->firstgid_;
            break;
        }
    }
    if (GidBatDauKhoiSach == -1){
        cout << "KHONG TIM THAY BO KHOI SACH" << endl;
        return;
    }


    int DanhSachSachDuocSinh[SO_LUONG_SACH];
    int ChiSoDanhSach = 0;
    for (int i = 0; i < SO_LUONG_SACH; i++){
        int GidNgauNhien = rng() % SoSachTrongKhoi + GidBatDauKhoiSach + 1;
        DanhSachSachDuocSinh[ChiSoDanhSach++] = GidNgauNhien;
    }
    


    // Bắt đầu sinh sách vị trí ngẫu nhiên

    vector<pair<int, int>> ToaDoViTriDaCoSach;
    int SoLuongDaSinh = 0;
    while (SoLuongDaSinh < SO_LUONG_SACH){
        int ToaDoSinhSach2D = rng() % DienTich;
        int ToaDoX = KhoiBatDauX + ToaDoSinhSach2D % ChieuRong;
        int ToaDoY = KhoiBatDauY + ToaDoSinhSach2D / ChieuRong;
        auto it = find(ToaDoViTriDaCoSach.begin(), ToaDoViTriDaCoSach.end(), pair(ToaDoX, ToaDoY));
        if (it != ToaDoViTriDaCoSach.end()) continue;

        BanDo_.DanhSachLopBanDo_[LopSach].Khoi_[ToaDoY][ToaDoX] = DanhSachSachDuocSinh[SoLuongDaSinh];
        ToaDoViTriDaCoSach.push_back({ToaDoX, ToaDoY});
        SoLuongDaSinh++;
    }
}



void BanDoGame::HoanLaiSach(int x, int y, int IDCucBo){
    for (auto &Lop : BanDo_.DanhSachLopBanDo_){
        if (Lop.TenLop_ == "Sach"){
            Lop.Khoi_[y][x] = IDCucBo;
            break;
        }   
    }
}


bool BanDoGame::KiemTraVaChamVoiBoss(int tileX, int tileY) const {
    
    for (const auto& Lop : BanDo_.DanhSachLopBanDo_) {
        Uint32 gid = Lop.Khoi_[tileY][tileX];
        if (gid != 0) {
            Uint32 gidDaXuLy = gid & 0x1FFFFFFF; // Bỏ flip bits
            if (LaKhoiBoss(gidDaXuLy)) {
                cout << "Tim thay boss tile! GID: " << gidDaXuLy << endl;
                return true;
            }
        }
    }
    return false;
}