#include <ft2build.h>
#include FT_FREETYPE_H

#include <hb.h>
#include <hb-ft.h>

#include <iostream>
#include <string>

#include "data_path.hpp"

//This file exists to check that programs that use freetype / harfbuzz link properly in this base code.
//You probably shouldn't be looking here to learn to use either library.

#define FONT_SIZE 36
#define MARGIN (FONT_SIZE * .5)

int main(int argc, char **argv) {
	// FT_Library library;
	// FT_Init_FreeType( &library );

	// hb_buffer_t *buf = hb_buffer_create();
	// hb_buffer_destroy(buf);

	// std::cout << "It worked?" << std::endl;

	// Reference: https://github.com/harfbuzz/harfbuzz-tutorial/blob/master/hello-harfbuzz-freetype.c
	
	std::string path = data_path("dist/Open_Sans/OpenSans-VariableFont_wdth,wght.ttf");
	const char *fontfile = path.c_str();
	const char *text = "engineer. One day, you receive a dreaded notice from your boss: you're required to return to the office five days a week.";

  	/* Initialize FreeType and create FreeType font face. */
	FT_Library ft_library;
	FT_Face ft_face;
	FT_Error ft_error;

	if ((ft_error = FT_Init_FreeType (&ft_library))) {
		std::cerr << "Error initializing FreeType library" << std::endl;
		abort();
	}
	if ((ft_error = FT_New_Face (ft_library, fontfile, 0, &ft_face))) {
		std::cerr << "Error loading font file: " << path << std::endl;
		abort();
	}
	if ((ft_error = FT_Set_Char_Size (ft_face, FONT_SIZE*64, FONT_SIZE*64, 0, 0))) {
		std::cerr << "Error setting character size" << std::endl;
		abort();
	}


	/* Create hb-ft font. */
	hb_font_t *hb_font = hb_ft_font_create (ft_face, NULL);

	/* Create hb-buffer and populate. */
	hb_buffer_t *hb_buffer = hb_buffer_create ();
	hb_buffer_add_utf8 (hb_buffer, text, -1, 0, -1);
	hb_buffer_guess_segment_properties (hb_buffer);
	// hb_buffer_set_direction(hb_buffer, HB_DIRECTION_LTR);
	// hb_buffer_set_script(hb_buffer, HB_SCRIPT_COMMON);
	// hb_buffer_set_language(hb_buffer, hb_language_from_string("en", -1));
	// hb_font_set_scale(hb_font, FONT_SIZE * 64, FONT_SIZE * 64);

	/* Shape it! */
	hb_shape (hb_font, hb_buffer, NULL, 0);

	/* Get glyph information and positions out of the buffer. */
	unsigned int len = hb_buffer_get_length (hb_buffer);
	hb_glyph_info_t *info = hb_buffer_get_glyph_infos (hb_buffer, NULL);
	hb_glyph_position_t *pos = hb_buffer_get_glyph_positions (hb_buffer, NULL);

	/* Print them out as is. */
	printf ("Raw buffer contents:\n");
	for (unsigned int i = 0; i < len; i++)
	{
		hb_codepoint_t gid   = info[i].codepoint;
		unsigned int cluster = info[i].cluster;
		double x_advance = pos[i].x_advance / 64.;
		double y_advance = pos[i].y_advance / 64.;
		double x_offset  = pos[i].x_offset / 64.;
		double y_offset  = pos[i].y_offset / 64.;

		char glyphname[32];
		hb_font_get_glyph_name (hb_font, gid, glyphname, sizeof (glyphname));

		printf ("glyph='%s'	cluster=%d	advance=(%g,%g)	offset=(%g,%g)\n",
            glyphname, cluster, x_advance, y_advance, x_offset, y_offset);
	}

	printf ("Converted to absolute positions:\n");
	/* And converted to absolute positions. */
	{
		double current_x = 0;
		double current_y = 0;
		for (unsigned int i = 0; i < len; i++)
		{
		hb_codepoint_t gid   = info[i].codepoint;
		unsigned int cluster = info[i].cluster;
		double x_position = current_x + pos[i].x_offset / 64.;
		double y_position = current_y + pos[i].y_offset / 64.;


		char glyphname[32];
		hb_font_get_glyph_name (hb_font, gid, glyphname, sizeof (glyphname));

		printf ("glyph='%s'	cluster=%d	position=(%g,%g)\n",
			glyphname, cluster, x_position, y_position);

		current_x += pos[i].x_advance / 64.;
		current_y += pos[i].y_advance / 64.;
		}
	}

	hb_buffer_destroy (hb_buffer);
	hb_font_destroy (hb_font);

	FT_Done_Face (ft_face);
	FT_Done_FreeType (ft_library);
}
