#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };

struct Point
{
	int x, y;
} a[4], b[4];
// a is the array storing the position of tetris
// b is for checking
// later you will see b[i] = a[i], b as a back up of a
// if the checking fail, a[i] will restore to b[i]

int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

bool check()
{
	for (int i = 0; i < 4; i++)
	{
		if (a[i].x < 0 || a[i].x > N || a[i].y >= M)
			return false; // The tetris can't go out of boundary
		else if (field[a[i].y][a[i].x]) 
			return false;
	}
	return true;
}

int main() {

	// Window
	RenderWindow window(VideoMode(320, 480), "Tetris");

	Texture t;
	t.loadFromFile("../images/tiles.png");

	Sprite s(t);
	s.setTextureRect(IntRect(0, 0, 18, 18));

	int dx = 0; bool rotate = false; int colorNum = 1;
	float timer = 0, delay = 0.3;

	Clock clock;

	// Game loop
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		// Event polling
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed)
				window.close();
			switch (e.type) {
				case Event::Closed:
					window.close();
					break;
				case Event::KeyPressed:
					if (e.key.code == Keyboard::Up) rotate = true;
					else if (e.key.code == Keyboard::Left) dx = -1;
					else if (e.key.code == Keyboard::Right) dx = 1;
					break;
			}
		}
		// Update

		//// <- Move -> ///
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].x += dx;
		}
		if (!check())
			for (int i = 0; i < 4; i++)
				a[i] = b[i];

		//// <- Rotate -> ///
		if (rotate)
		{
			Point p = a[1]; // center of rotation
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!check())
				for (int i = 0; i < 4; i++)
					a[i] = b[i];
		}

		//// <- Tick -> ///
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				a[i].y += 1;
			}
			if (!check())
			{
				for (int i = 0; i < 4; i++)
					field[b[i].y][b[i].x] = colorNum;

				colorNum = 1 + rand() % 7;
				int n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}
			timer = 0;
		}

		dx = 0; rotate = false; // reset move and rotate

		// Render
		window.clear(Color::White); // Clear Old frame

		// Draw your game
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0) continue;
				s.setPosition(j * 18, i * 18);
				window.draw(s);
			}
		for (int i = 0; i < 4; i++)
		{
			s.setPosition(a[i].x * 18, a[i].y * 18);
			window.draw(s);
		}

		window.display(); // Tell app that window is done drawing
	}

	return 0;
}