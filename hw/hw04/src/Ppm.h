/* 
 * Caleb Lucas-Foley
 * Copyright (C) 2018 
 * All rights reserved
 * 
 * Ppm.h: A crappy ASCII ppm reader that assumes there's a comment on the second
 * line.
 */

#include <iostream>
#include <fstream>
#include <string>

#include "SceneData.h"


class Ppm {
public:
	Ppm() {
                pixels = new SceneColor[0];
		width = height = 0;
                unmapped = true;
        }
        Ppm(string filename) {
                this->filename = filename;
                string magic_number;
                string comment;
                unsigned r, g, b;
                std::ifstream file(filename);
                if (file.is_open()) {
                        getline(file, magic_number);
                        getline(file, comment);
                        file >> width;
                        file >> height;
                        pixels = new SceneColor[width * height];
                        int denom;
                        file >> denom;
                        for (unsigned long i = 0; i < width * height; ++i) {
                                file >> r;
                                file >> g;
                                file >> b;
                                pixels[i].r = r / (float) denom;
                                pixels[i].g = g / (float) denom;
                                pixels[i].b = b / (float) denom;
                                pixels[i].a = 1;
                        }
                        unmapped = false;
                } else {
                        std::cerr << "Unable to open file: " << filename << endl;
                }
        }

        Ppm(const Ppm &other) {
                if (other.unmapped) {
                        unmapped = true;
                        return;
                }
                delete[] pixels;
                filename = other.filename;
                width = other.width;
                height = other.height;
                pixels = new SceneColor[width * height];
                for (unsigned long i = 0; i < width * height; ++i) {
                        pixels[i].r = other.pixels[i].r;
                        pixels[i].g = other.pixels[i].g;
                        pixels[i].b = other.pixels[i].b;
                        pixels[i].a = 1;
                }
                unmapped = false;
        }

        Ppm &operator=(const Ppm &other) {
		if (other.unmapped) {
                        unmapped = true;
                        return *this;
                }
                delete[] pixels;
                filename = other.filename;
                width = other.width;
                height = other.height;
                pixels = new SceneColor[width * height];
                for (unsigned long i = 0; i < width * height; ++i) {
                        pixels[i].r = other.pixels[i].r;
                        pixels[i].g = other.pixels[i].g;
                        pixels[i].b = other.pixels[i].b;
                        pixels[i].a = 1;
                }
                unmapped = false;
                return *this;
        }
        
        ~Ppm() {
                delete[] pixels;
        }
        
        SceneColor &at(int x, int y) {
                return pixels[x + (width * y)];
        }

        bool operator<(const Ppm& rhs) {
                return this->filename < rhs.filename;
        }
        string filename;
        unsigned long width, height;
        bool unmapped;
private:
        SceneColor *pixels;
};
