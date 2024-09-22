#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <hb.h>
#include <hb-ft.h>

#include <glm/glm.hpp>

#include <iostream>
#include <string>

#include "data_path.hpp"
#include "GL.hpp"

struct Text {
    std::string text;
    float line_height;
    float line_length; // max length of a line
    glm::vec2 start_pos; // start at start_pos.x and start_pos.y

    Text(std::string text, float line_height, float line_length, glm::vec2 start_pos) : text(text), line_height(line_height), line_length(line_length), start_pos(start_pos){}
    Text(){}
};

class Font {
public:
    Font(std::string const &font_path, int font_size, unsigned int width, unsigned int height);
    ~Font();

    void gen_texture(unsigned int& texture, std::vector<std::shared_ptr<Text> > const &texts);

    std::vector<std::string> wrapText(const std::string& text, size_t line_length = 85);

    FT_Library ft_library;
	FT_Face ft_face;
    hb_font_t *hb_font;
    hb_buffer_t *hb_buffer;
    int font_size;
    unsigned int width;
    unsigned int height;
};