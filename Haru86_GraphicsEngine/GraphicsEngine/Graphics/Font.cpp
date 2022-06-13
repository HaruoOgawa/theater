#include "Font.h"
#include "./Texture.h"

Font::Font() {

}

Font::~Font() {
	UnLoad();
}

bool Font::Load(std::string fontPass) {
	return true;
}

void Font::UnLoad() {
}
