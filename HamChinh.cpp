#include "HamChung.h"
#include "DoiTuong.h"
#include "BanDoGame.h"
#include "NhanVatGame.h"
#include "MenuGame.h"
#include "ManChoiBoss.h"
#include "AmThanh.h"


DoiTuong PhongNen;
TTF_Font* FontChinh = nullptr;
TTF_Font* FontTieuDe = nullptr;
// DoiTuong PhongNen2;


void close(){
    PhongNen.GiaiPhong();
    SDL_DestroyRenderer(ManHinhChinh);
    ManHinhChinh = NULL;

    SDL_DestroyWindow(CuaSoChinh);
    CuaSoChinh = NULL;
    TTF_CloseFont(FontChinh);
    TTF_CloseFont(FontTieuDe);
    if (AmThanhGame) {
        delete AmThanhGame;
        AmThanhGame = nullptr;
    }
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

    FontTieuDe = TTF_OpenFont("phongchu/Poller_One/PollerOne-Regular.ttf", 48);
    if (!FontTieuDe) {
        cout << "Loi mo font tieu de: " << TTF_GetError() << endl;
        return false;
    }
    
    AmThanhGame = new QuanLyAmThanh();
    if (!AmThanhGame->KhoiTao()){
        cout << "KHONG THE KHOI TAO AM THANH" << endl;
    }


    return ThanhCong;
}



void TaiTatCaAmThanh() {
    if (!AmThanhGame) return;
    
    // Tải nhạc nền
    AmThanhGame->TaiNhacNen(NHAC_NEN_MENU, "audio/music/nhacnenmenu.mp3");
    AmThanhGame->TaiNhacNen(NHAC_NEN_GAME, "audio/music/nhacnengame.mp3");
    AmThanhGame->TaiNhacNen(NHAC_NEN_BOSS, "audio/music/nhacnendanhboss.mp3");
    
    // Tải hiệu ứng âm thanh
    AmThanhGame->TaiHieuUngAmThanh(SFX_DI_CHUYEN, "audio/effect/nhanvatdichuyen.ogg");
    AmThanhGame->TaiHieuUngAmThanh(SFX_NHAT_SACH, "audio/effect/nhatsach.ogg");
    AmThanhGame->TaiHieuUngAmThanh(SFX_MO_TUI_DO, "audio/effect/motuido.wav");
    AmThanhGame->TaiHieuUngAmThanh(SFX_MO_RUONG, "audio/effect/moruong.ogg");
    AmThanhGame->TaiHieuUngAmThanh(SFX_PHONG_SURIKEN, "audio/effect/suriken.wav");
    AmThanhGame->TaiHieuUngAmThanh(SFX_MO_CUA, "audio/effect/mocua.ogg");
    AmThanhGame->TaiHieuUngAmThanh(SFX_DONG_CUA, "audio/effect/dongcua.ogg");
    AmThanhGame->TaiHieuUngAmThanh(SFX_NHAN_VAT_BI_THUONG, "audio/effect/connguoibithuong.wav");
    AmThanhGame->TaiHieuUngAmThanh(SFX_BOSS_BI_THUONG, "audio/effect/bossbithuong.wav");
    // AmThanhGame->TaiHieuUngAmThanh(SFX_THANG, "audio/victory.wav");
    // AmThanhGame->TaiHieuUngAmThanh(SFX_THUA, "audio/defeat.wav");
    AmThanhGame->TaiHieuUngAmThanh(SFX_NUT_MENU, "audio/effect/chonmenu.wav");

    AmThanhGame->TaiHieuUngAmThanh(SFX_KHI_DOT, "audio/effect/khidot.wav");
    AmThanhGame->TaiHieuUngAmThanh(SFX_KHI_DOT_BI_THUONG, "audio/effect/khidotbithuong.wav");

    AmThanhGame->TaiHieuUngAmThanh(SFX_PHAP_SU, "audio/effect/phapsu.wav");
    AmThanhGame->TaiHieuUngAmThanh(SFX_PHAP_SU_BI_THUONG, "audio/effect/phapsubithuong.wav");


}



void AnimationManChe_ThuNho(SDL_Renderer* ManHinh, int ThoiGianMs = 500) {
    Uint32 BatDau = SDL_GetTicks();
    
    while (true) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return;
        }
        
        float tienDo = (float)(SDL_GetTicks() - BatDau) / (float)ThoiGianMs;
        if (tienDo >= 1.0f) break;
        
        if (tienDo < 0.5f) {
            // Thu nhỏ từ full màn hình về giữa
            float PhongNho = 1.0f - (tienDo); // từ 1 đến 0,5
            int Rong = (int)(CHIEU_RONG_MAN_HINH * PhongNho);
            int Cao = (int)(CHIEU_CAO_MAN_HINH * PhongNho);
            int x = (CHIEU_RONG_MAN_HINH - Rong) / 2;
            int y = (CHIEU_CAO_MAN_HINH - Cao) / 2;

            
            SDL_SetRenderDrawColor(ManHinh, 0, 0, 0, 255);
            SDL_RenderClear(ManHinh);
        
            // Để lại lỗ ở giữa (không tô)
            SDL_SetRenderDrawColor(ManHinh, 50, 50, 50, 255);
            SDL_Rect Lo {x, y, Rong, Cao};
            SDL_RenderFillRect(ManHinh, &Lo);
        } else {
            // Phóng to từ giữa ra full
            float PhongTo = (tienDo - 0.5f) * 2;  // từ 0 cho đến 1 (làm trễ)
            int rong = (int)(CHIEU_RONG_MAN_HINH * PhongTo);
            int cao = (int)(CHIEU_CAO_MAN_HINH * PhongTo);
            int x = (CHIEU_RONG_MAN_HINH - rong) / 2;
            int y = (CHIEU_CAO_MAN_HINH - cao) / 2;
            
            SDL_SetRenderDrawColor(ManHinh, 50, 50, 50, 255);
            SDL_Rect Lo {x, y, rong, cao};
            SDL_RenderFillRect(ManHinh, &Lo);
        }
        
        SDL_RenderPresent(ManHinh);
    }
}

int main(int argc, char* argv[]){

    if (KhoiTao() == false){ // khởi tạo SDL
        return -1;
    }
    
    
    TaiTatCaAmThanh();


    AmThanhGame->PhatNhacNen(NHAC_NEN_MENU, -1);



    BatDauLai:
    BanDoGame BanDoGameChinh;
    NhanVatGame NhanVatChinh;
    ManChoiBoss BossGameChinh;
    bool DangChoi_BossGame = false;


   
    // ===== PHẦN MENU =====
    SDL_Rect TRO_CHOI_1 = {14, 1, 22, 18};
    int SO_LUONG_SACH;

    MenuGame Menu;
    if (!Menu.TaiHinhNen("img/background.png", ManHinhChinh)){
        cout << "Loi tai hinh nen menu!" << endl;
        return -1;
    }

    
    bool DangOMenu = true;
    bool BatDauGame = false;
    
    
    while (DangOMenu) {


        while (SDL_PollEvent(&SuKienChinh) != 0){
            if (SuKienChinh.type == SDL_QUIT){
                DangOMenu = false;
                close();
                return 0; // Thoát toàn bộ
            }
            
            TrangThaiMenu LuaChon = Menu.XuLySuKien(SuKienChinh);
            
            if (LuaChon == MENU_BAT_DAU){
                AmThanhGame->DungNhacNen();
                AmThanhGame->PhatNhacNen(NHAC_NEN_GAME, -1);

                DangOMenu = false;
                BatDauGame = true;

                
                BanDoGameChinh.TaiBanDoVaBoKhoi((char*)"map/BanDo1.tmj", ManHinhChinh);
                SO_LUONG_SACH = (int)Menu.LayDoKhoDaChon();
                BanDoGameChinh.SinhSachNgauNhien("Sach", "map/Book.tsj", TRO_CHOI_1.x, TRO_CHOI_1.y, TRO_CHOI_1.w, TRO_CHOI_1.h, SO_LUONG_SACH);
                
                NhanVatChinh.TaiAnhNhanVat("img/Warrior.png", ManHinhChinh);
                NhanVatChinh.KhoiTaoHeThongBuocDi(SO_LUONG_SACH, TRO_CHOI_1, BanDoGameChinh);


                AnimationManChe_ThuNho(ManHinhChinh, 600);

            } else if (LuaChon == MENU_THOAT){
                DangOMenu = false;
                close();
                return 0;
            }
        }
        
        if (DangOMenu){
            SDL_RenderClear(ManHinhChinh);
            Menu.Ve(ManHinhChinh, FontTieuDe, FontChinh);
            SDL_RenderPresent(ManHinhChinh);
        }
    }
    
    if (!BatDauGame){
        close();
        return 0;
    };

    // ===== PHẦN GAME CHÍNH =====
    

    bool Thoat = false;

    while(!Thoat){
        

        while(SDL_PollEvent(&SuKienChinh) != 0){
            if (DangChoi_BossGame) {
                if (SuKienChinh.type == SDL_QUIT) {
                    Thoat = true;
                    break;
                }
                
                // XỬ LÝ INPUT CHO BOSS GAME
                BossGameChinh.XuLyInput(SuKienChinh);
            }
            else if (SuKienChinh.type == SDL_QUIT) {
                Thoat = true;
            }
            else {
                NhanVatChinh.HanhDongDauVao(SuKienChinh, ManHinhChinh, BanDoGameChinh, SO_LUONG_SACH);
            }
        }

        SDL_SetRenderDrawColor(ManHinhChinh, 0, 0, 0, 255);
        SDL_RenderClear(ManHinhChinh);

        if (DangChoi_BossGame) {
            // CẬP NHẬT BOSS GAME
            BossGameChinh.CapNhat();
            
            // CHỈ VẼ BOSS GAME - KHÔNG VẼ GAME THÔNG THƯỜNG
            BossGameChinh.Ve(ManHinhChinh, FontChinh);
            
            //KIỂM TRA KẾT THÚC
            if (BossGameChinh.DaThang() && BossGameChinh.GetYeuCauQuayVeMenu()) {
                DangChoi_BossGame = false;
                goto BatDauLai;
            } 
            else if (BossGameChinh.DaThua()) {
                DangChoi_BossGame = false;
                NhanVatChinh.ResetViTri();
            }
        }
        else {
            
            // KIỂM TRA VA CHẠM VỚI BOSS AREA
            int playerTileX = NhanVatChinh.ThongTinViTriX() / KICH_THUOC_KHOI;
            int playerTileY = NhanVatChinh.ThongTinViTriY() / KICH_THUOC_KHOI;
            
            if (BanDoGameChinh.KiemTraVaChamVoiBoss(playerTileX, playerTileY)) {
                AmThanhGame->DungNhacNen();
                AmThanhGame->PhatNhacNen(NHAC_NEN_BOSS);
                cout << "VA CHAM VOI BOSS AREA!" << endl;
                DangChoi_BossGame = true;
                
                if (BossGameChinh.KhoiTao(ManHinhChinh, SO_LUONG_SACH)) {
                    
                    BossGameChinh.BatDauHoiThoaiBoss();
                    cout << "Khoi tao boss game thanh cong!" << endl;
                } else {
                    cout << "Loi khoi tao Boss Game!" << endl;
                    DangChoi_BossGame = false;
                }
            }
          

            // CHỈ VẼ GAME THÔNG THƯỜNG KHI KHÔNG CHƠI BOSS
            BanDoGameChinh.VeBanDo(ManHinhChinh);
            NhanVatChinh.XuLyDiChuyen(BanDoGameChinh);
            NhanVatChinh.VeNhanVat(ManHinhChinh);
            NhanVatChinh.VeTroChoi1(ManHinhChinh, FontChinh, BanDoGameChinh);
            NhanVatChinh.VeTuiDo(ManHinhChinh, FontChinh, "map/Book.tsj", BanDoGameChinh, SO_LUONG_SACH);
            NhanVatChinh.VeBangChiDan(ManHinhChinh, FontChinh, BanDoGameChinh);
            NhanVatChinh.VeMiniGameRuong(ManHinhChinh, FontChinh, SO_LUONG_SACH);
            NhanVatChinh.VeThongBaoDaHoanThanh(ManHinhChinh, FontChinh);
        }

        SDL_RenderPresent(ManHinhChinh);
    }
    close();
    return 0;
}
