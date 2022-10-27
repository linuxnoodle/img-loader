#include "common.hpp"
#include "parser.hpp"
#include <algorithm>
#include <cstdlib>
#include <pthread.h>

using std::cout;

int main(int argc, char** argv) {
    if (argc < 2){
        cout << "Error: no input file specified\n";
    } else {
        cout << "Parsing file " << argv[1] << "\n";
        image img = parse_png(argv[1]);
        cout << "Image size: " << (uint32_t)img.width << "x" << (uint32_t)img.height << "\n";
        cout << "Bit depth: " << (uint32_t)img.bit_depth << "\n";
        cout << "Color type: " << (uint32_t)img.color_type << "\n";
        cout << "Compression method: " << (uint32_t)img.compression_method << "\n";
        cout << "Filter method: " << (uint32_t)img.filter_method << "\n";
        cout << "Interlace method: " << (uint32_t)img.interlace_method << "\n";
    }
    return 0;
}
