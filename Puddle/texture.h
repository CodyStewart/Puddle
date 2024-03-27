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
	void renderScaledEx(SDL_Renderer* renderer, int x, int y, int width, int height, float rotation, SDL_Rect* clip = NULL);

	void setTexture(SDL_Texture* texture);

	SDL_Texture* getTexture();
	int getWidth();
	int getHeight();

protected:
	SDL_Texture* _texture;

	int _width;
	int _height;
};

struct TextTexture : public Texture {
	TextTexture();
	TextTexture(SDL_Renderer* renderer, std::string text, TTF_Font* font, SDL_Color color);

	~TextTexture();

	void setFlashingText(bool value);
	void setFlashingPeriod(int period);

	SDL_Color getTextColor();
	int getWidth();
	int getHeight();
	std::string getText();
	TTF_Font* getFont();

	void setTextColor(SDL_Color color);

	bool loadFromRenderedText(SDL_Renderer* renderer, std::string text, TTF_Font* font, SDL_Color textColor);

	//void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip);

	void free();

private:
	std::string _text;
	SDL_Color _textColor;
	TTF_Font* _font;

	bool _isFlashing;
	Uint32 _flashTime;
};