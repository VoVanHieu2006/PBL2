#ifndef HAM_CHUNG_H
#define HAM_CHUNG_H

#include <windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "json.hpp" 
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>  // để chia xuống dòng 
// Dùng để random
#include <random>
#include <ctime>


using namespace std;

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



// Bản đồ 





// Nhân vật game






#endif