#include "texture.h"
Texture::Texture() {
	_texture = nullptr;
	_width = 0;
	_height = 0;
}

Texture::Texture(SDL_Texture* texture) {
	if (texture != NULL) {
		SDL_QueryTexture(texture, NULL, NULL, &_width, &_height);
		_texture = texture;
	}
	else {
		_width = 0;
		_height = 0;
		_texture = nullptr;
	}
}

Texture::Texture(Texture* other) { // copy constructor
	_texture = other->getTexture();
	_width = other->getWidth();
	_height = other->getHeight();
}

Texture::Texture(SDL_Renderer* renderer, char* buffer, uint32 size) {
	_texture = nullptr;
	SDL_RWops* ops = SDL_RWFromMem(buffer, size);
	printf("%s\n", IMG_GetError());
	_texture = IMG_LoadTexture_RW(renderer, ops, 0);
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

void Texture::renderScaledEx(SDL_Renderer* renderer, int x, int y, int width, int height, float rotation, SDL_Rect* clip) {
	if (_texture != NULL) {
		SDL_Rect renderQuad = { x, y, width, height };

		if (clip != NULL) {
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		SDL_RenderCopyEx(renderer, _texture, clip, &renderQuad, (double)rotation, NULL, SDL_FLIP_NONE);
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

TextTexture::TextTexture() {
	_texture = nullptr;
	_font = nullptr;
	_textColor = {};
	_text = "";
	_width = 0;
	_height = 0;

	_isFlashing = false;
	_flashTime = 0;
}

TextTexture::TextTexture(SDL_Renderer* renderer, std::string tex, TTF_Font* font, SDL_Color color) {
	_text = tex;
	_font = font;
	_textColor = color;

	loadFromRenderedText(renderer, _text, font, _textColor);

	_isFlashing = false;
	_flashTime = 0;
}

TextTexture::~TextTexture() {

}

void TextTexture::setFlashingText(bool value) {
	_isFlashing = value;
}

void TextTexture::setFlashingPeriod(int period) {
	_flashTime = (Uint32)period;
}

void TextTexture::setTextColor(SDL_Color color) {
	_textColor = color;
}

SDL_Color TextTexture::getTextColor() {
	return _textColor;
}

int TextTexture::getWidth() { return _width; }
int TextTexture::getHeight() { return _height; }
std::string TextTexture::getText() { return _text; }
TTF_Font* TextTexture::getFont() { return _font; }

bool TextTexture::loadFromRenderedText(SDL_Renderer* renderer, std::string tex, TTF_Font* fon, SDL_Color color) {
	free();

	_text = tex;
	_textColor = color;
	_font = fon;

	if (_isFlashing) {
		static int flashValue = _textColor.a;	// NOTE: this seems like a bug. All instances of text with flashing will have this?
		if (flashValue == 0) {
			flashValue = 255;
			_textColor.a = flashValue;
		}
		else if (flashValue > 2 && flashValue <= 255) {
			flashValue -= 2;
			_textColor.a = flashValue;
		}
		else if (flashValue < 2 && flashValue > -255) {
			flashValue -= 2;
			_textColor.a = -1 * flashValue;
		}
		else {
			flashValue *= -1;
			flashValue -= 2;
			_textColor.a = flashValue;
		}
	}

	SDL_Surface* surface = TTF_RenderText_Blended(_font, _text.c_str(), _textColor);
	if (surface == NULL) {
		printf("Was unable to render text surface! SDL_TTF ERROR: %s\n", TTF_GetError());
		return false;
	}
	else {
		_texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (_texture == NULL) {
			printf("Was unable to create texture from rendered text! SDL_TTF ERROR: %s\n", TTF_GetError());
		}
		else {
			_width = surface->w;
			_height = surface->h;
		}

		SDL_FreeSurface(surface);

		return true;
	}
}

//void TextTexture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip) {
//	if (texture != NULL) {
//		SDL_Rect renderQuad = { x, y, width, height };
//
//		if (clip != NULL) {
//			renderQuad.w = clip->w;
//			renderQuad.h = clip->h;
//		}
//
//		SDL_RenderCopy(renderer, texture, clip, &renderQuad);
//	}
//}

void TextTexture::free() {
	if (_texture != nullptr)
	{
		SDL_DestroyTexture(_texture);
		_texture = nullptr;
		_font = nullptr;
		_width = 0;
		_height = 0;
	}
}