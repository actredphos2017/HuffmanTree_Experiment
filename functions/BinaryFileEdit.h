#include <fstream>

void Binary_Output(std::string file_name, std::string binary_text){
    std::ofstream oF(file_name, std::ios_base::binary);
    int bad_count = 8 - binary_text.size() % 8;
    std::string bad_code = "";
    while(-- bad_count)
        bad_code += '0';
    bad_code += '1';
    binary_text = bad_code + binary_text;
    uint8_t single[8];
    unsigned int n = 1;
    for(auto it = binary_text.begin(); it != binary_text.end(); it ++, n ++){
        single[n-1] = (uint8_t)(*it - '0' == 1);
        if(n == 8){
            uint8_t temp = 0x00;
            for(int i = 0; i < 8; i ++)
                temp |= single[i] << (7 - i);
            oF.write((char*)&temp, sizeof(uint8_t));
            n = 0;
        }
    }
    oF.close();
    return;
}

std::string Binary_Input(std::string file_name){
    std::ifstream iF(file_name);
    iF.seekg(0, std::ios::end);
    size_t length = iF.tellg();
    iF.seekg(0, std::ios::beg);
    uint8_t* data = new uint8_t[length];
    iF.read((char*)data, length * sizeof(uint8_t));
    iF.close();
    std::string binary_text = "";
    for(int i = 0; i < length; i ++)
        for(int j = 7; j >= 0; j --)
            binary_text += ('0' + ((data[i] >> j) % 2));
    for(int i = 0; i < 8; i ++)
        if(binary_text[i] == '1')
            return binary_text.substr(++ i, std::string::npos);
    return binary_text;
}
