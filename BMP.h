#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "MyFormat.h"
using namespace std;

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{ 0x4D42 };		// ��� ����� ���������� ����������� ������ ���� BMP (2 �����)
    uint32_t file_size{ 0 };			// ������ ����� ���������� ����������� � ������ (4 �����)
    uint16_t reserved1{ 0 };			// ����������������� ����� ���������� �����, ������ ���� 0 (2 �����)
    uint16_t reserved2{ 0 };			// ����������������� ����� ���������� �����, ������ ���� 0 (2 �����)
    uint32_t offset_data{ 0 };		    // ��������� ������� ������ ���������� ����������� ������������ ���������� ����������� (4 �����) 
};

struct BMPInfoHeader {
    uint32_t size{ 0 };					// ���������� ������, ���������� ���� ���������� (4 �����)
    int32_t width{ 0 };                 // ������ ���������� ����������� � �������� (4 �����)
    int32_t height{ 0 };                // ������ ���������� ����������� � �������� (4 �����)
    uint16_t planes{ 1 };               // ������� �������� ���������� ������ ���� 1 (2 �����)
    uint16_t bit_count{ 0 };            // ���������� �����, ����������� ��� ������� �������, ������ ���� 8 (256 ������), 32 (���������� �������� ����) (2 �����)
    uint32_t compression{ 0 };          // ��� ������ ���������� ����������� ������ ���� 0 (��� ������)
    uint32_t size_image{ 0 };           // ������ ���������� ����������� � ������ (4 �����)
    int32_t x_pixels_per_meter{ 0 };    // ���������� ���������� ����������� �� �����������, ���������� �������� �� ���� (4 �����)
    int32_t y_pixels_per_meter{ 0 };    // ������������ ���������� ���������� �����������, �������� �� ���� (4 �����)
    uint32_t colors_used{ 0 };          // ���������� ������ � ������� ������, ���������� ������������ ��������� ������������ (4 �����)
    uint32_t colors_important{ 0 };     // ���������� ������ ������ � �������� ����������� ���������� ����������� (4 �����)
};

struct BMPColorHeader {
    uint8_t rgbBlue;                    // ������� ������ (�������� �������� 0-255)
    uint8_t rgbGreen;                   // ������� �������� (�������� �������� 0-255)
    uint8_t rgbRed;                     // ������� �������� (�������� �������� 0-255)
    uint8_t rgbReserved;                // ���������������, ������ ���� 0
};
#pragma pack(pop)

class BMP8 {
public:
    BMP8(int32_t width, int32_t height) {
        file_header.file_size = sizeof(file_header) + sizeof(bmp_info_header) + sizeof(bmp_color_header) + height * width + height * ((3 * width) % 4);
        file_header.offset_data = sizeof(file_header) + sizeof(bmp_info_header) + sizeof(bmp_color_header);

        bmp_info_header.size = sizeof(bmp_info_header);
        bmp_info_header.width = width;
        bmp_info_header.height = height;
        bmp_info_header.bit_count = 8;
        bmp_info_header.size_image = height * width;

        data.resize(width * height + height * ((3 * width) % 4));

        uint8_t temp = 0;
        for (auto& i : bmp_color_header) i = { temp, temp, temp, temp }, temp++;
    }

    void readMyFormatFloat(const float* arr) {
        int32_t width = bmp_info_header.width;
        for (size_t i = 0, pos = 0; i < data.size();) {
            vector v((width + (3 * width) % 4), uint8_t(0));
            for (size_t temp = 0; temp < width; ++temp) v[temp] = uint8_t(arr[pos++] * 255);
            for (auto& temp : v) data[i++] = temp;
        }
    }
    void readMyFormatUint8_t(const uint8_t* arr) {
        int32_t width = bmp_info_header.width;
        for (size_t i = 0, pos = 0; i < data.size();) {
            vector v((width + (3 * width) % 4), uint8_t(0));
            for (size_t temp = 0; temp < width; ++temp) v[temp] = arr[pos++];
            for (auto& temp : v) data[i++] = temp;
        }
    }
    void loadMyFormatFloat(const string path) {
        ifstream in{ path, ios_base::binary };
        if (!in) {
            cout << "No file: " << path;
            return;
        }
        int32_t width = bmp_info_header.width;
        for (size_t i = 0; i < data.size();) {
            vector v((width + (3 * width) % 4), float(0));
            in.read((char*)v.data(), width * sizeof(float));
            for (auto& temp : v) data[i++] = uint8_t(temp * 255);
        }
        in.close();
    }
    void loadMyFormatUint8_t(const string path) {
        ifstream in{ path, ios_base::binary };
        if (!in) {
            cout << "No file: " << path;
            return;
        }
        int32_t width = bmp_info_header.width;
        for (size_t i = 0; i < data.size();) {
            vector v((width + (3 * width) % 4), uint8_t(0));
            in.read((char*)v.data(), width);
            for (auto& temp : v) data[i++] = temp;
        }
        in.close();
    }

    void write(const string path) {
        ofstream out{ path, ios_base::binary };
        out.write((const char*)&file_header, sizeof(file_header));
        out.write((const char*)&bmp_info_header, sizeof(bmp_info_header));
        out.write((const char*)&bmp_color_header, sizeof(bmp_color_header));
        out.write((const char*)data.data(), data.size());
        out.close();
    }

    inline static void opean_file(const string path) { system(path.c_str()); }

private:
    BMPFileHeader file_header;
    BMPInfoHeader bmp_info_header;
    BMPColorHeader bmp_color_header[256];
    vector<uint8_t> data;
};