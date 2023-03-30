#pragma once
#include <iostream>
#include <fstream>
#include "Fragment.h"
using namespace std;
using ushort = unsigned short int;

struct MyFormat {
	MyFormat(ushort width, ushort height) {
		this->width = width;
		this->height = height;
		data = new uint8_t[width * height]{ 0 };
	}

	void read(const string path) {
		ifstream in{ path, ios_base::binary };
		in.read((char*)data, width * height);
		in.close();
	}
	void write(const string path) const {
		ofstream out{ path, ios_base::binary };
		out.write((char*)data, width * height);
		out.close();
	}

	ushort size() { return width * height; }
	uint8_t* getData() const { return data; }
	void datArrFragment(float* arr, ushort x, ushort y, ushort w, ushort h) {
		ushort size = w * h;
		for (ushort i = 0; i < size; ++i) arr[i] = (float)data[x + (y + (i / w)) * width + (i % w)] / 255;
	}

	void setPixel(ushort x, ushort y, uint8_t satur) { 
		data[y * width + x] = satur;
	}
	void drawFragment(Fragment& fragment, uint8_t satur = 0) {
		auto [w, h] = fragment.getSize();
		auto [posX, posY] = fragment.getPosithion();
		w = (posX + w > width ? width - posX : w);
		h = (posY + h > height ? width - posY : h);
		for (ushort y = posY; y < (posY + h); ++y) {
			for (ushort x = posX; x < (posX + w); ++x) {
				if (fragment(x, y)) setPixel(x, y, satur);
			}
		}
	}
	void drawFragment(ushort x, ushort y, ushort w, ushort h, uint8_t* arr) {
		ushort size = w * h;
		for (ushort i = 0; i < size; ++i) data[x + (y + (i / w)) * width + (i % w)] = arr[i];
	}

	/*-----test-----*/
	uint8_t getPixel(ushort x, ushort y) const {
		return data[y * width + x];
	}
	uint8_t getPixel(ushort pos) const { return data[pos]; }
	void drawFragmentINVERS(ushort x, ushort y, ushort w, ushort h, uint8_t satur = 255) {
		ushort size = w * h;
		for (ushort i = 0; i < size; ++i) data[x + (y + (i / w)) * width + (i % w)] = satur;
	}

private:
	ushort width, height;
	uint8_t* data;
};