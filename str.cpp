/*
 *  str.cpp
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 27/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "str.h"

namespace wings {
	String::String(const String& other) {
		_len = other._len;
		_data = new char[_len+1];
		memcpy(_data, other._data, _len);
		_data[_len] = '\0';
	}
	
	String::String(const char* cstr) {
		_len = strlen(cstr);
		_data = new char[_len+1];
		memcpy(_data, cstr, _len);
		_data[_len] = '\0';
	}
	
	String& String::operator=(const char* cstr) {
		delete[] _data;
		_len = strlen(cstr);
		_data = new char[_len+1];
		memcpy(_data, cstr, _len);
		_data[_len] = '\0';
		return *this;
	}
	
	String& String::operator=(const String& other) {
		delete[] _data;
		_len = other._len;
		_data = new char[_len+1];
		memcpy(_data, other._data, _len);
		_data[_len] = '\0';
		return *this;
	}
	
	String& String::operator+=(const char* cstr) {
		size_t append_len = strlen(cstr);
		char* new_data = new char[_len + append_len + 1];
		memcpy(new_data, _data, _len);
		memcpy(new_data + _len, cstr, append_len);
		_len += append_len;
		new_data[_len] = '\0';
		delete[] _data;
		_data = new_data;
		return *this;
	}
	
	String& String::operator+=(const String& other) {
		char* new_data = new char[_len + other._len + 1];
		memcpy(new_data, _data, _len);
		memcpy(new_data + _len, other._data, other._len);
		_len += other._len;
		new_data[_len] = '\0';
		delete[] _data;
		_data = new_data;
		return *this;
	}
	
	String String::operator+(const char* str) const {
		String copy = *this;
		copy += str;
		return copy;
	}
	
	String String::operator+(const String& other) const {
		String copy = *this;
		copy += other;
		return copy;
	}
	
	String operator+(const char* cstr, const String& append) {
		String str;
		size_t len = strlen(cstr);
		str._len = len + append._len;
		str._data = new char[str._len];
		memcpy(str._data, cstr, len);
		memcpy(str._data + len, append._data, append._len);
		str._data[str._len] = '\0';
		return str;
	}
}
