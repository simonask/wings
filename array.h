/*
 *  array.h
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 24/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "basic.h"
#include "placement_new.h"

namespace wings {

	template <typename T>
	class Array {
	private:
		T* _data;
		size_t _size;
		size_t _alloc_size;
		
		
		void resize(size_t new_size) {
			if (new_size > _alloc_size) {
				size_t new_alloc_size = new_size; // TODO: allocate moar!
				T* new_data = alloc_range(new_alloc_size);
				move_construct_range(new_data, _data, _size);
				free_range(_data);
				_data = new_data;
				_alloc_size = new_alloc_size;
			}
		}
		
		static int sort_compare_callback(const void* _a, const void* _b) {
			const T& a = *reinterpret_cast<const T*>(_a);
			const T& b = *reinterpret_cast<const T*>(_b);
			return a < b ? -1 : (a == b ? 0 : 1);
		}
	public:
		static T* alloc_range(size_t n) {
			return reinterpret_cast<T*>(new byte[n * sizeof(T)]);
		}
		static void free_range(T* data) {
			delete [] reinterpret_cast<byte*>(data);
		}
		static void construct_range(T* ptr, size_t n) {
			for (size_t i = 0; i < n; ++i) {
				new(&ptr[i]) T();
			}
		}
		static void destruct_range(T* ptr, size_t n) {
			for (size_t i = 0; i < n; ++i) {
				ptr[i].~T();
			}
		}
		static void copy_range(T* dst, const T* src, size_t n) {
			for (size_t i = 0; i < n; ++i) {
				dst[i] = src[i];
			}
		}
		static void copy_construct_range(T* dst, const T* src, size_t n) {
			for (size_t i = 0; i < n; ++i) {
				new(&dst[i]) T(src[i]);
			}
		}
		static void move_construct_range(T* dst, T* src, size_t n) {
			// same as copy_construct + destruct, but with better locality
			for (size_t i = 0; i < n; ++i) {
				new(&dst[i]) T(src[i]);
				src[i].~T();
			}
		}
		
		
		Array() : _data(NULL), _size(0), _alloc_size(0) {}
		Array(const Array<T>& other) : _data(NULL), _size(other._size), _alloc_size(_size) {
			_data = alloc_range(_size);
			copy_construct_range(_data, other._data, _size);
		}
		explicit Array(size_t sz) : _data(NULL), _size(sz) {
			_data = new T[_size];
		}
		~Array() {
			destruct_range(_data, _size);
			free_range(_data);
		}
		Array<T>& operator=(const Array<T>& other) {
			destruct_range(_data, _size);
			_size = 0;
			resize(other._size);
			_size = other._size;
			copy_construct_range(_data, other._data, other._size);
		}
		
		void preallocate(size_t sz) { resize(sz); }
		
		size_t size() const { return _size; }
		
		T& operator[](size_t i) {
			ASSERT(i < _size);
			return _data[i];
		}
		const T& operator[](size_t i) const {
			ASSERT(i < _size);
			return _data[i];
		}
		
		const T& top() const { ASSERT(_size); return _data[_size-1]; }
		T& top() { ASSERT(_size); return _data[_size-1]; }
		void pop() {
			ASSERT(_size);
			_data[--_size].~T();
		}
		void push(const T& value) {
			resize(_size + 1);
			new (&_data[_size++]) T(value);
		}
		T& push() {
			resize(_size + 1);
			new (&_data[_size++]) T();
			return top();
		}
		
		void erase(size_t i) {
			ASSERT(i < _size);
			if (i == _size - 1) {
				_data[i].~T();
				--_size;
			} else {
				copy_range(_data + i, _data + i + 1, _size-i);
				_data[_size--].~T();
			}
		}
		
		void qsort() {
			// TODO: Don't use libc's qsort, for better inlining
			::qsort(_data, _size, sizeof(T), sort_compare_callback);
		}
	};

}

