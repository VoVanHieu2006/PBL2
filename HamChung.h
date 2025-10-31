#ifndef HAM_CHUNG_H
#define HAM_CHUNG_H

#include <windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "json.hpp" 
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>  // để chia xuống dòng 
#include <random> // Dùng để random
#include <ctime>
#include <queue>
#include <algorithm>
#include <random>
#include <chrono>
#include <numeric>
#include <cmath>  // cho sqrt, cos, sin, atan2

using namespace std;

#define KICH_THUOC_KHOI 32


class QuanLyAmThanh;

enum LoaiAmThanh {
    NHAC_NEN_MENU,
    NHAC_NEN_GAME,
    NHAC_NEN_BOSS,
    
    SFX_DI_CHUYEN,
    
    SFX_MO_TUI_DO,
    SFX_MO_RUONG,
    SFX_MO_CUA,
    SFX_DONG_CUA,
    SFX_NHAT_SACH,
    SFX_PHONG_SURIKEN,
    SFX_TRUNG_MUCTIEU,
    SFX_NHAN_VAT_BI_THUONG,
    SFX_BOSS_BI_THUONG,
    SFX_THANG,
    SFX_THUA,
    SFX_NUT_MENU,

    SFX_KHI_DOT,
    SFX_KHI_DOT_BI_THUONG,
    SFX_PHAP_SU,
    SFX_PHAP_SU_BI_THUONG,
};

extern QuanLyAmThanh* AmThanhGame;




static SDL_Window* CuaSoChinh = NULL;  
static SDL_Renderer* ManHinhChinh = NULL; 
static SDL_Event SuKienChinh;


// Screen
const int SO_FRAME_TREN_GIAY = 24;  // fps


const int CHIEU_RONG_MAN_HINH = 1280;
const int CHIEU_CAO_MAN_HINH = 640;

const int SAC_MAU_DO = 167;
const int SAC_MAU_XANH_LA = 175;
const int SAC_MAU_XANH_DUONG = 180;

const int SAC_DO_TOI_DA = 0xff;

const int LE_GAME = KICH_THUOC_KHOI * 2;
const SDL_Rect TRO_CHOI_CUOI = {
    LE_GAME,
    LE_GAME,
    CHIEU_RONG_MAN_HINH - 2 * LE_GAME,
    CHIEU_CAO_MAN_HINH - 2 * LE_GAME
};



// Bản đồ 




// Nhân vật game






#endif