#include "parser.hpp"

using std::vector, 
      std::string,
      std::cerr,
      std::string;

vector<chunk> load_chunks(std::ifstream &file) {
    vector<chunk> chunks;
    string raw_data = std::string(std::istreambuf_iterator<char>{file}, {});
    // verify png signature
    if (raw_data.substr(0, 8) != "\211PNG\r\n\032\n"){
        cerr << "Error: invalid png signature\n";
        exit(1);
    }

    uint32_t position = 8;
    while (position < raw_data.size() - 1) {
        chunk c;
        // form four byte unsigned int from 4 bytes of data
        uint32_t length = (uint8_t)raw_data[position] << 24 |
                          (uint8_t)raw_data[position + 1] << 16 |
                          (uint8_t)raw_data[position + 2] << 8 |
                          (uint8_t)raw_data[position + 3];
        std::cout << "Length: " << length << "\n";
        std::cout << "current pos: " << position << "\n";
        position += 4;

        // get chunk type
        c.chunk_type = (CHTYPE)(raw_data[position] + raw_data[position + 1] + raw_data[position + 2] + raw_data[position + 3]);
        std::cout << "Chunk type: " << c.chunk_type << "\n";
        position += 4;

        // get chunk data
        if (length > 0){
            c.chunk_data = new uint8_t[length];
            for (uint32_t i = 0; i < length; ++i){
                c.chunk_data[i] = raw_data[position + i];
                ++position;
            }
        } else {
            c.chunk_data = nullptr;
        }
        std::cout << "position after: " << position << "\n";

        // TODO: load and check chunk crc, currently unnessesary
        position += 4;

        chunks.push_back(c);
    }
    return chunks;
}

// not using exceptions because those suck
image parse_png(std::string name) {
    std::ifstream file(name, std::ios::binary);
    if (!file.is_open()) {
        cerr << "Error: could not open file " << name << "\n";
        exit(1);
    }
    image img;
    img.chunks = load_chunks(file);
    for (chunk c: img.chunks){
        switch (c.chunk_type){
            case CHTYPE::IHDR:
                std::cout << (((uint32_t)c.chunk_data[0] << 24) | ((uint32_t)c.chunk_data[1] << 16) | ((uint32_t)c.chunk_data[2] << 8) | ((uint32_t)c.chunk_data[3])) << "\n";
                img.width = c.chunk_data[0] << 24 |
                            c.chunk_data[1] << 16 |
                            c.chunk_data[2] << 8  |
                            c.chunk_data[3];
                img.height = c.chunk_data[4] << 24 |
                             c.chunk_data[5] << 16 |
                             c.chunk_data[6] << 8  |
                             c.chunk_data[7];
                img.bit_depth = c.chunk_data[8];
                img.color_type = c.chunk_data[9];
                img.compression_method = c.chunk_data[10];
                img.filter_method = c.chunk_data[11];
                img.interlace_method = c.chunk_data[12];
                break;
            case CHTYPE::PLTE:
                //TODO
                break;
            case CHTYPE::IDAT:
                //TODO
                break;
            case CHTYPE::IEND:
                if (c.chunk_data != nullptr){
                    cerr << "Error: IEND chunk has data\n";
                    exit(1);
                }
                break;
        }
    }
    return img;
}
