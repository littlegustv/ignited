#include "Background.h"

Background::Background(int z, int x, int y, std::string imageFile)
{
	Background::z = z;
	GameObject::Init(x, y, 0, 0, boundX, boundY);
	Background::image = al_load_bitmap(imageFile.c_str());

	if (image)
		std::cout << "loaded!\n";
	else
		std::cout << "didn't load :(\n";
}

Background::Background(std::string data, ALLEGRO_BITMAP *backbuffer, ALLEGRO_BITMAP *tilesets[10], int tilesetInfo[10][2], int z, int w, int h, int fade, int scroll)
{
	Background::z = z;

	int map[200][200];

	Background::fade = fade;
	Background::scroll = scroll;


	std::istringstream ss(data);
	std::string token;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			std::getline(ss, token, ',');
			map[j][i] = atoi(token.c_str());
//			std::cout << map[j][i] << " ";
		}
//		std::cout << "\n";
	}

	image = al_create_bitmap(w*tileSize, h*tileSize);

	al_set_target_bitmap(Background::image);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (map[j][i] < 0)
				continue;

			int mod = 1;
			int offset = 1;
			int tile = 0;
			for (int k = 0; k < 9; k++)
			{
				if (map[j][i] == 0)
					break;
				
				if (tilesetInfo[k][1] > 0)
				{
					if (map[j][i] >= tilesetInfo[k][1])
					{
						offset = tilesetInfo[k][1];
						mod = tilesetInfo[k][0];
						tile = k;
					}
				}
				else
					break;
					//std::cout << tilesetInfo[k][0] << "\n";
			}
		//	std::cout << offset << "\n";
			int withoffset = map[j][i] > 0 ? map[j][i] - offset : -1;
		//	std::cout << withoffset << " ";
			if (withoffset >= 0)
			{
				int cx = (withoffset % mod) * tileSize;
				int cy = (withoffset / mod) * tileSize;

				al_draw_bitmap_region(tilesets[tile], cx, cy, tileSize, tileSize, j * tileSize, i * tileSize, 0);

			}
		}
	//	std::cout << "\n";
	}

	Background::x = 0;
	Background::y = 0;

	al_set_target_bitmap(backbuffer);
}
void Background::Destroy()
{
	GameObject::Destroy();
}
void Background::Update()
{
	GameObject::Update();
	if (scroll > 0)
		scroll -= 0.1;

}
void Background::Render()
{
	if (scroll <= 0)
		al_draw_bitmap(image, x, y, 0);
	else if (scroll > 0)
	{
		int w = al_get_bitmap_width(image);
		//int wTiles = (1 - scroll / 100) * w;
		float slice = w * 0.001;
		float maxSlice = (100 - scroll) * 10;

		for (int i = 0; i < maxSlice; i++)
		{
			float opacity = (maxSlice - i ) < 30 ? ((maxSlice - i )/ 30) : 1.0;// -(i / wTiles);
			al_draw_tinted_bitmap_region(image, al_map_rgba_f(opacity, opacity, opacity, opacity), i * slice, 0, slice, al_get_bitmap_height(image), i * slice, 0, 0);
			if (maxSlice - i < 30)
				al_draw_pixel((i + 10) * slice, rand() % (al_get_bitmap_height(image) - 64) + 64, al_map_rgb(255, 255, 255));

		}

	}
	GameObject::Render();
}
void Background::Collided()
{}