#pragma once
#include<string>
#include<memory>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>

#include "commonTypedefs.h"

struct Texture
{
	Texture();
	Texture(Texture* other); // copy constructor
	Texture(SDL_Renderer* renderer, char* buffer, uint32 size);

	~Texture();

	bool loadTextureFromFile(SDL_Renderer* renderer, std::string path);

	void free();

	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL);
	void renderScaled(SDL_Renderer* renderer, int x, int y, int width, int height, SDL_Rect* clip = NULL);

	void setTexture(SDL_Texture* texture);

	SDL_Texture* getTexture();
	int getWidth();
	int getHeight();

protected:
	SDL_Texture* _texture;

	int _width;
	int _height;
};