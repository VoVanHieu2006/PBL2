all:
	g++ \
	-I src/include/SDL2 -L src/lib \
	-o HamChinh \
	HamChinh.cpp DoiTuong.cpp BanDoGame.cpp NhanVatGame.cpp RuongDo.cpp MenuGame.cpp AmThanh.cpp \
	ThuatToan1.cpp ThuatToan2.cpp ManChoiBoss.cpp NVTrongBoss.cpp Boss.cpp KhiDot.cpp PhapSu.cpp \
	-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

