#include "texture.h"
Texture::Texture() {
	_texture = nullptr;
	_width = 0;
	_height = 0;
}

Texture::Texture(Texture* other) { // copy constructor
	_texture = other->getTexture();
	_width = other->getWidth();
	_height = other->getHeight();

}

Texture::Texture(SDL_Renderer* renderer, char* buffer, uint32 size) {
	_texture = nullptr;
	SDL_RWops* ops = SDL_RWFromMem(buffer, size);
	printf("%d", IMG_isPNG(ops));
	printf("%s\n", SDL_GetError());
	printf("%s\n", IMG_GetError());
	_texture = IMG_LoadTexture_RW(renderer, ops, 0);
	printf("%s\n", SDL_GetError());
	printf("%s\n", IMG_GetError());

	if (_texture != NULL) {
		SDL_QueryTexture(_texture, NULL, NULL, &_width, &_height);
	}
	else {
		_width = 0;
		_height = 0;
	}
}

Texture::~Texture() {
	free();
}

bool Texture::loadTextureFromFile(SDL_Renderer* renderer, std::string path) {
	SDL_Texture* txt = IMG_LoadTexture(renderer, path.c_str());
	_texture = txt;
	if (_texture != NULL) {
		SDL_QueryTexture(_texture, NULL, NULL, &_width, &_height);
		return true;
	}
	else {
		return false;
	}
}

void Texture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip) {
	if (_texture != NULL) {
		SDL_Rect renderQuad = { x, y, _width, _height };

		if (clip != NULL) {
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		SDL_RenderCopy(renderer, _texture, clip, &renderQuad);
	}
 }

void Texture::renderScaled(SDL_Renderer* renderer, int x, int y, int width, int height, SDL_Rect* clip) {
	if (_texture != NULL) {
		SDL_Rect renderQuad = { x, y, width, height };

		if (clip != NULL) {
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		SDL_RenderCopy(renderer, _texture, clip, &renderQuad);
	}
}

void Texture::setTexture(SDL_Texture* texture) {
	_texture = texture;
}

void Texture::free() {
	if (_texture != NULL)
	{
		SDL_DestroyTexture(_texture);
		_texture = NULL;
		_width = 0;
		_height = 0;
	}
}

SDL_Texture* Texture::getTexture() { return _texture; }
int Texture::getWidth() { return _width; }
int Texture::getHeight() { return _height; }