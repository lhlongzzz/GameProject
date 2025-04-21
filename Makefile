all:
	g++ -Isrc/include -Lsrc/lib -o main main.cpp Player.cpp Bullet.cpp Enemy.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image