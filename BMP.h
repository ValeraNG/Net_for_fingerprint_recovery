#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "MyFormat.h"
using namespace std;

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{ 0x4D42 };		// “ип файла растрового изображени€ должен быть BMP (2 байта)
    uint32_t file_size{ 0 };			// –азмер файла растрового изображени€ в байтах (4 байта)
    uint16_t reserved1{ 0 };			// «арезервированное слово растрового файла, должно быть 0 (2 байта)
    uint16_t reserved2{ 0 };			// «арезервированное слово растрового файла, должно быть 0 (2 байта)
    uint32_t offset_data{ 0 };		    // Ќачальна€ позици€ данных растрового изображени€ относительно растрового изображени€ (4 байта) 
};

struct BMPInfoHeader {
    uint32_t size{ 0 };					//  оличество байтов, занимаемых этой структурой (4 байта)
    int32_t width{ 0 };                 // Ўирина растрового изображени€ в пиксел€х (4 байта)
    int32_t height{ 0 };                // ¬ысота растрового изображени€ в пиксел€х (4 байта)
    uint16_t planes{ 1 };               // ”ровень целевого устройства должен быть 1 (2 байта)
    uint16_t bit_count{ 0 };            //  оличество битов, необходимых дл€ каждого пиксел€, должно быть 8 (256 цветов), 32 (улучшенный истинный цвет) (2 байта)
    uint32_t compression{ 0 };          // “ип сжати€ растрового изображени€ должен быть 0 (без сжати€)
    uint32_t size_image{ 0 };           // –азмер растрового изображени€ в байтах (4 байта)
    int32_t x_pixels_per_meter{ 0 };    // –азрешение растрового изображени€ по горизонтали, количество пикселей на метр (4 байта)
    int32_t y_pixels_per_meter{ 0 };    // ¬ертикальное разрешение растрового изображени€, пикселей на метр (4 байта)
    uint32_t colors_used{ 0 };          //  оличество цветов в таблице цветов, фактически используемых растровым изображением (4 байта)
    uint32_t colors_important{ 0 };     //  оличество важных цветов в процессе отображени€ растрового изображени€ (4 байта)
};

struct BMPColorHeader {
    uint8_t rgbBlue;                    // яркость синего (диапазон значений 0-255)
    uint8_t rgbGreen;                   // яркость зеленого (диапазон значений 0-255)
    uint8_t rgbRed;                     // яркость красного (диапазон значений 0-255)
    uint8_t rgbReserved;                // зарезервировано, должно быть 0
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