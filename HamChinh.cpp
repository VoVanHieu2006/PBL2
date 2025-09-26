#include "HamChung.h"
#include "DoiTuong.h"
#include "BanDoGame.h"
#include "NhanVatGame.h"


DoiTuong PhongNen;
TTF_Font* FontChinh = nullptr;
// DoiTuong PhongNen2;

void close(){
    PhongNen.GiaiPhong();
    SDL_DestroyRenderer(ManHinhChinh);
    ManHinhChinh = NULL;

    SDL_DestroyWindow(CuaSoChinh);
    CuaSoChinh = NULL;

    IMG_Quit();
    SDL_Quit();
}



bool KhoiTao(){
    bool ThanhCong = true;   // Báo kết quả xem có khởi tạo thành công không 
    int KtraKhoiTaoVideo = SDL_Init(SDL_INIT_VIDEO);  // Bật khởi tạo video 
    if (KtraKhoiTaoVideo < 0) 
        return false;  // Nếu không bật được thì hủy luôn
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); 
    // Bật bộ lọc lên cho mượt, 0 là ko lọc, 1 là lọc mượt hơn, nếu ko lọc thì đồ họa như minecraft

    // Tạo cửa sổ 
    CuaSoChinh = SDL_CreateWindow("Game c++ SDL 2.0",             
                                SDL_WINDOWPOS_UNDEFINED, // tự quyết định hệ thông x y 
                                SDL_WINDOWPOS_UNDEFINED, 
                                CHIEU_RONG_MAN_HINH, CHIEU_CAO_MAN_HINH,  // chiều rộng, chiều cao 
                                SDL_WINDOW_SHOWN); // cửa sổ được hiển thi sau khi tạo

    if (CuaSoChinh == NULL){    
        ThanhCong = false; // Nếu tạo thất bại thì tạo ko thành công
    }
    else{
        ManHinhChinh = SDL_CreateRenderer(CuaSoChinh, -1, SDL_RENDERER_ACCELERATED); 
        // tạo cây ManHinhChinh cọ vẽ để vẽ lên CuaSoChinh, -1 là driver mặc định, dùng GPU đển vẽ, nhanh hơn CPU
        if (ManHinhChinh == NULL){ // nếu vẽ lỗi thì báo thất bại
            ThanhCong = false;
        }
        else{
            SDL_SetRenderDrawColor(ManHinhChinh, SAC_DO_TOI_DA, SAC_DO_TOI_DA, SAC_DO_TOI_DA, SAC_DO_TOI_DA); // xét màu
            // Render theo sắc màu, lần lượt là: đỏ, xanh lá, xanh dương, độ trong suốt tất cả nằm trong 0-255
            // 0 là tắt hẳn, 255 là đục hoàn toàn, ở đây tất cả là RENDER_DRAW_COLOR = 255, có nghĩa là trắng tinh, hoàn toàn đục
            int LoaiHinhAnh = IMG_INIT_JPG | IMG_INIT_PNG; // Con số để thể hiện ảnh loại gì (là con sô trong bit)
            // IMG_Init: xem thử ảnh này có hỗ trợ ko, nếu có thì kết quả giữa nguyên
            if ( !(IMG_Init(LoaiHinhAnh) & LoaiHinhAnh) ) // đây là phép AND nếu như mà nó ko không giống nhau (ảnh ko đc hỗ trợ)
                ThanhCong = false; // Trả về lỗi nếu không có bit nào là bit 1

        }
    }

    if (TTF_Init() == -1) {
        cout << "Loi TTF_Init: " << TTF_GetError() << endl;
        return false;
    }

    FontChinh = TTF_OpenFont("phongchu/Oswald/static/Oswald-SemiBold.ttf", 18); // bạn tạo thư mục font/
    if (!FontChinh) {
        cout << "Loi mo font: " << TTF_GetError() << endl;
        return false;
    }
    return ThanhCong;
}

bool TaiPhongNen(){
    bool KtraTaiAnh = PhongNen.TaiAnh("img/hoa.jpg" ,ManHinhChinh);
    if (KtraTaiAnh == false) 
        return false;
    return true;    
}

// bool TaiPhongNen2(){
//     bool KtraTaiAnh = PhongNen2.TaiAnh("map/Floor.png" ,ManHinhChinh);
//     if (KtraTaiAnh == false) 
//         return false;
//     return true;    
// }

BanDoGame BanDoGameChinh;
NhanVatGame NhanVatChinh;




int main(int argc, char* argv[]) {

    if (KhoiTao() == false){ // khởi tạo SDL
        return -1;
    }
    
    if (TaiPhongNen() == false) return -1;

    // if (TaiPhongNen2() == false) return -1;
    SDL_Rect TRO_CHOI_1 = {14, 1, 22, 18};
    BanDoGameChinh.TaiBanDoVaBoKhoi((char*)"map/BanDo1.tmj", ManHinhChinh);
    int SO_LUONG_SACH = 8;
    BanDoGameChinh.SinhSachNgauNhien("Sach", "map/Book.tsj", TRO_CHOI_1.x, TRO_CHOI_1.y, TRO_CHOI_1.w, TRO_CHOI_1.h, SO_LUONG_SACH);
    
    NhanVatChinh.TaiAnhNhanVat("img/Warrior.png", ManHinhChinh);

    bool Thoat = false;

    while(!Thoat){
       
        while(SDL_PollEvent(&SuKienChinh) != 0){
            if (SuKienChinh.type == SDL_QUIT) Thoat = true;

                NhanVatChinh.HanhDongDauVao(SuKienChinh, ManHinhChinh, BanDoGameChinh, SO_LUONG_SACH);
 
        }
        
        SDL_RenderClear(ManHinhChinh); // xóa màn hình theo màu trên (lúc khởi tạo)
        //PhongNen.Ve(ManHinhChinh, NULL); // vẽ g_background.p_object_ lên ManHinhChinh 
        // PhongNen2.Ve(ManHinhChinh, NULL);


        
        
        BanDoGameChinh.VeBanDo(ManHinhChinh);
        

        NhanVatChinh.XuLyDiChuyen(BanDoGameChinh);
        NhanVatChinh.VeNhanVat(ManHinhChinh);
        
        NhanVatChinh.VeTuiDo(ManHinhChinh, FontChinh, "map/Book.tsj", BanDoGameChinh, SO_LUONG_SACH);
        NhanVatChinh.VeBangChiDan(ManHinhChinh, FontChinh, BanDoGameChinh);
        NhanVatChinh.VeMiniGameRuong(ManHinhChinh, FontChinh, SO_LUONG_SACH); // VẼ HỘP NHẬP RƯƠNG
        NhanVatChinh.VeThongBaoDaHoanThanh(ManHinhChinh, FontChinh);
        SDL_RenderPresent(ManHinhChinh); // trình diễn nhừng gì vừa vẽ từ ManHinhChinh lên CuaSoChinh


    }


    close();
    return 0;
}
