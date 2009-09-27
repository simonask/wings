/*
 *  str.h
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 27/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _WINGS_STR_H_
#define _WINGS_STR_H_

#include <string.h>
#include <strings.h>
#include "basic.h"

namespace wings {
	class String {
		friend String operator+(const char*, const String&);
	private:
		char* _data;
		size_t _len;
	public:
		String() : _data(NULL), _len(0) {}
		String(const String& other);
		String(const char* cstr);
		~String() { delete [] _data; }
		
		size_t length() const { return _len; }
		const char* c_str() const { return _data ? _data : ""; }
		char* data() { return _data; }
		
		bool operator==(const char* cstr) const { return compare(cstr) == 0; }
		bool operator==(const String& other) const { return compare(other) == 0; }
		bool operator!=(const char* cstr) const { return compare(cstr) != 0; }
		bool operator!=(const String& other) const { return compare(other) != 0; }
		bool operator<(const char* cstr) const { return compare(cstr) < 0; }
		bool operator<(const String& other) const { return compare(other) < 0; }
		String& operator=(const char* cstr);
		String& operator=(const String& other);
		String& operator+=(const char* cstr);
		String& operator+=(const String& other);
		String operator+(const char* cstr) const;
		String operator+(const String& other) const;
		
		int compare(const char* cstr) const { return strcmp(_data ? _data : "", cstr ? cstr : ""); }
		int compare(const String& other) const { return compare(other._data); }
		int icompare(const char* cstr) const { return strcasecmp(_data ? _data : "", cstr ? cstr : ""); }
		int icompare(const String& other) const { return icompare(other._data); }
	};
	
	String operator+(const char* cstr, const String& append);
}

#endif
