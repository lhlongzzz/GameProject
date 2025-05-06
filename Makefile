all:
	g++ -Isrc/include -Lsrc/lib -o ChikenInvader main.cpp Player.cpp Bullet.cpp Enemy.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer