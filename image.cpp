/*
 *  image.cpp
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 26/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "image.h"
#include "array.h"
#include "str.h"

#include <png.h>
#include <stdio.h>

namespace wings {
	Image::Image() : _data(NULL), _w(0), _h(0) {}
	
	Image::Image(size_t w, size_t h) : _w(w), _h(h) {
		_data = new Pixel[_w*_h];
	}
	
	Image::Image(const Image& other) : _w(other._w), _h(other._h) {
		_data = new Pixel[_w*_h];
		Array<Pixel>::copy_range(_data, other._data, _w*_h);
	}
	
	Image::~Image() {
		delete [] _data;
	}
	
	void Image::chromify(const Color& c) {
		byte r = (byte)(c.r * 255);
		byte g = (byte)(c.g * 255);
		byte b = (byte)(c.b * 255);
		for (size_t i = 0; i < _w*_h; ++i) {
			Pixel& p = _data[i];
			p.r = r;
			p.g = g;
			p.b = b;
		}
	}
	
	void Image::paste(const Image& src, size_t src_x, size_t src_y, size_t w, size_t h, size_t dst_x, size_t dst_y) {
		ASSERT((src_x + w) * (src_y + h) <= (src._h * src._w)); // invalid source rect
		ASSERT((dst_x + w) * (dst_y + h) <= (_h * _w)); // invalid destination rect
		
		for (size_t i = 0; i < h; ++i) {
			size_t dst_row_index = dst_x + (dst_y + i) * _w;
			size_t src_row_index = src_x + (src_y + i) * src._w;
			Array<Pixel>::copy_range(_data + dst_row_index, src._data + src_row_index, w);
		}
	}
	
	void Image::load_png(Image& result, const String& path) {
		delete [] result._data;
		
		FILE* fp = fopen(path.c_str(), "rb");
		ASSERT(fp);
		byte header[8];
		fread(header, 1, 8, fp);
		ASSERT(!png_sig_cmp(header, 0, 8));
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL /* error */, NULL /* error func */, NULL /* warn func */);
		ASSERT(png_ptr);
		
		png_infop info = png_create_info_struct(png_ptr);
		if (setjmp(png_jmpbuf(png_ptr))) {
			ASSERT(false);
		}
		
		png_init_io(png_ptr, fp);
		png_set_sig_bytes(png_ptr, 8);
		png_read_info(png_ptr, info);
		
		png_set_interlace_handling(png_ptr);
		
		int color_type, bit_depth;
		png_get_IHDR(png_ptr, info, &result._w, &result._h, &bit_depth, &color_type, NULL, NULL, NULL);
		
		if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png_ptr);
		if (color_type != PNG_COLOR_TYPE_RGBA) {
			/* GRP:  expand palette to RGB, and grayscale or RGB to GA or RGBA */
			if (color_type == PNG_COLOR_TYPE_PALETTE)
				png_set_expand(png_ptr);
			png_set_add_alpha(png_ptr, 0xffff, PNG_FILLER_AFTER);
		}
		if (bit_depth == 16)
			png_set_strip_16(png_ptr);
		
		png_read_update_info(png_ptr, info);
		
		ASSERT(info->color_type == PNG_COLOR_TYPE_RGBA);
		
		size_t bytes_per_row = info->rowbytes;
		ASSERT(bytes_per_row / result._w == 4);	// data needs to be 32-bit RGBA at this point!
		byte raw_data[info->height * bytes_per_row];
		png_bytep rows[info->height];
		for (size_t i = 0; i < info->height; ++i) {
			rows[i] = raw_data + i * bytes_per_row;
		}
		png_read_image(png_ptr, rows);
		fclose(fp);
		
		ASSERT(result._w * result._h * sizeof(Pixel) == info->height * bytes_per_row);
		
		result._data = new Pixel[result._w * result._h];
		memcpy(result._data, raw_data, result._w * result._h * sizeof(Pixel));
		//png_destroy_read_struct(&png_ptr, &info, &info + 1);
	}
}