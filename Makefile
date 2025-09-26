all:
	g++ \
	-I src/include/SDL2 -L src/lib \
	-o HamChinh \
	HamChinh.cpp DoiTuong.cpp BanDoGame.cpp NhanVatGame.cpp RuongDo.cpp \
	-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

# 	g++ \
# 	-I src/include/SDL2 -L src/lib \
# 	-o zmain \
# 	zTamThoi.cpp \
# 	-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf