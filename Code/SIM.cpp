/*
<Author> : Thamara
<Date> : 06.06.2021
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <bitset>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;

fstream newfile;

std::map<uint32_t,int> mydictionary;
std::map<int,uint32_t> inst_dic;

int no_of_lines = 0;
int len_inst_dic = 16;
string output = "";

vector<uint32_t> arr_lines;
vector<uint32_t> arr_unique_element;
vector<string> arr_string;
vector<int> element_frequencies;

/***
 * 
 * 
 * ************************Compression*****************************
 * 
 * 
 */

void readfile(string file_name){
   newfile.open(file_name,ios::in); //open a file to perform read operation using file object
   if (newfile.is_open()){   //checking whether the file is open
        string string_line;
        while(getline(newfile, string_line)){ //read data from file object and put it into string.
        
            uint32_t line_val = 0x0000;
            
            for(int i = 0; i < 32; i++){
                if (string_line[i] == '1') {
                    line_val += 0x0001;    
                }
                if (i != 31) line_val = line_val << 1;
            }
            arr_lines.push_back(line_val);
            no_of_lines = no_of_lines + 1;
        }
        newfile.close(); //close the file object.
        arr_lines.shrink_to_fit();
        arr_string.shrink_to_fit();
    }
}

void write_file(string file_name){
    newfile.open(file_name,ios::out);  // open a file to perform write operation using file object
    if(newfile.is_open()){//checking whether the file is open       
        int count = 0;
        for(int i = 0; i< output.size(); i++){
            if (count < 32){
                newfile << output[i];
                count ++;
            }else{
                newfile << "\n";
                count =0;
                i--;
            }
        }    
        newfile << "\nxxxx\n";
        for (auto &dic_element: inst_dic){
            newfile << std::bitset<32>(dic_element.second).to_string() <<  "\n";
        }    
    newfile.close();    //close the file object 
    }
}

void create_dictionary(){
    
    for(int i = 0; i < arr_lines.size() ; i++){

        std::map<uint32_t, int>::iterator no = mydictionary.find(arr_lines[i]);
        // key already present on the map
        if (no != mydictionary.end()) {
            mydictionary[arr_lines[i]]++;
        } 
        // key not found
        else {
            arr_unique_element.push_back(arr_lines[i]);
            mydictionary.insert(std::make_pair(arr_lines[i], 1));
        }
    }
    arr_unique_element.shrink_to_fit();

    //change instrucion dictionary length if unique elements are low than 16
    if (arr_unique_element.size() < 16) len_inst_dic = arr_unique_element.size();
 
    //create the unique element list
    for(int j = 0; j < arr_unique_element.size(); j++){
        element_frequencies.push_back(mydictionary[arr_unique_element[j]]);
    }
    element_frequencies.shrink_to_fit();

    //create instruction dictionary
    int inst_dic_ele_count = 0;
    for(int no_of_occurances = no_of_lines; no_of_occurances > 0; no_of_occurances--){
        for(int no_of_unique_occurances = 0; no_of_unique_occurances < arr_unique_element.size(); no_of_unique_occurances++){
            if (no_of_occurances == element_frequencies[no_of_unique_occurances]){
                if (inst_dic_ele_count < len_inst_dic){
                    inst_dic.insert(std::make_pair((uint8_t)inst_dic_ele_count, arr_unique_element[no_of_unique_occurances]));
                    inst_dic_ele_count++;
                }
            }
        }
    }
}

bool one_bit_mismatch(uint32_t entry){

    std::map<uint32_t,uint16_t> mismatch_location_dic = {{0b10000000000000000000000000000000, 0x0},
                                                        {0b01000000000000000000000000000000, 0x1},
                                                        {0b00100000000000000000000000000000, 0x2},
                                                        {0b00010000000000000000000000000000, 0x3},
                                                        {0b00001000000000000000000000000000, 0x4},
                                                        {0b00000100000000000000000000000000, 0x5},
                                                        {0b00000010000000000000000000000000, 0x6},
                                                        {0b00000001000000000000000000000000, 0x7},
                                                        {0b00000000100000000000000000000000, 0x8},
                                                        {0b00000000010000000000000000000000, 0x9},
                                                        {0b00000000001000000000000000000000, 0xa},
                                                        {0b00000000000100000000000000000000, 0xb},
                                                        {0b00000000000010000000000000000000, 0xc},
                                                        {0b00000000000001000000000000000000, 0xd},
                                                        {0b00000000000000100000000000000000, 0xe},
                                                        {0b00000000000000010000000000000000, 0xf},
                                                        {0b00000000000000001000000000000000, 0x10},
                                                        {0b00000000000000000100000000000000, 0x11},
                                                        {0b00000000000000000010000000000000, 0x12},
                                                        {0b00000000000000000001000000000000, 0x13},
                                                        {0b00000000000000000000100000000000, 0x14},
                                                        {0b00000000000000000000010000000000, 0x15},
                                                        {0b00000000000000000000001000000000, 0x16},
                                                        {0b00000000000000000000000100000000, 0x17},
                                                        {0b00000000000000000000000010000000, 0x18},
                                                        {0b00000000000000000000000001000000, 0x19},
                                                        {0b00000000000000000000000000100000, 0x1a},
                                                        {0b00000000000000000000000000010000, 0x1b},
                                                        {0b00000000000000000000000000001000, 0x1c},
                                                        {0b00000000000000000000000000000100, 0x1d},
                                                        {0b00000000000000000000000000000010, 0x1e},                                                        
                                                        {0b00000000000000000000000000000001, 0x1f}};
    string out = "011";
    
    for (auto &inst_dic_entry: inst_dic) {
        uint32_t result = entry ^ inst_dic_entry.second;
        std::map<uint32_t, uint16_t>::iterator flag = mismatch_location_dic.find(result);
        if (flag != mismatch_location_dic.end()){
            
            // 011 + mismatch location(5) + dic_index(4)
            output = output + out + std::bitset<5>(mismatch_location_dic[result]).to_string() + std::bitset<4>(inst_dic_entry.first).to_string();
            return true; 
        }    
    }
    // cannot compress with 1 bit mis match
    return false;

}

bool two_cons_bit_mismatch(uint32_t entry){

    std::map<uint32_t,uint16_t> two_bit_mismatch_location_dic = {{0b11000000000000000000000000000000, 0x0},
                                                        {0b01100000000000000000000000000000, 0x1},
                                                        {0b00110000000000000000000000000000, 0x2},
                                                        {0b00011000000000000000000000000000, 0x3},
                                                        {0b00001100000000000000000000000000, 0x4},
                                                        {0b00000110000000000000000000000000, 0x5},
                                                        {0b00000011000000000000000000000000, 0x6},
                                                        {0b00000001100000000000000000000000, 0x7},
                                                        {0b00000000110000000000000000000000, 0x8},
                                                        {0b00000000011000000000000000000000, 0x9},
                                                        {0b00000000001100000000000000000000, 0xa},
                                                        {0b00000000000110000000000000000000, 0xb},
                                                        {0b00000000000011000000000000000000, 0xc},
                                                        {0b00000000000001100000000000000000, 0xd},
                                                        {0b00000000000000110000000000000000, 0xe},
                                                        {0b00000000000000011000000000000000, 0xf},
                                                        {0b00000000000000001100000000000000, 0x10},
                                                        {0b00000000000000000110000000000000, 0x11},
                                                        {0b00000000000000000011000000000000, 0x12},
                                                        {0b00000000000000000001100000000000, 0x13},
                                                        {0b00000000000000000000110000000000, 0x14},
                                                        {0b00000000000000000000011000000000, 0x15},
                                                        {0b00000000000000000000001100000000, 0x16},
                                                        {0b00000000000000000000000110000000, 0x17},
                                                        {0b00000000000000000000000011000000, 0x18},
                                                        {0b00000000000000000000000001100000, 0x19},
                                                        {0b00000000000000000000000000110000, 0x1a},
                                                        {0b00000000000000000000000000011000, 0x1b},
                                                        {0b00000000000000000000000000001100, 0x1c},
                                                        {0b00000000000000000000000000000110, 0x1d},
                                                        {0b00000000000000000000000000000011, 0x1e} };

    string out = "100";                                                    
    
    for (auto &inst_dic_entry: inst_dic) {
        uint32_t result = entry ^ inst_dic_entry.second;
        std::map<uint32_t, uint16_t>::iterator flag = two_bit_mismatch_location_dic.find(result);
        // key already present on the map
        if (flag != two_bit_mismatch_location_dic.end()){
           
           // 100 + mismatch location(5) + dic_index(4)
            output = output + out + std::bitset<5>(two_bit_mismatch_location_dic[result]).to_string() + std::bitset<4>(inst_dic_entry.first).to_string();
            return true; 
        }     
    }
    // cannot compress with 1 bit mis match
    return false;

}

uint16_t four_cons_bit_mismatch(uint32_t entry){

    std::map<uint32_t,uint16_t> four_bit_mismatch_location_dic = {{0b11110000000000000000000000000000, 0x0},
                                                        {0b01111000000000000000000000000000, 0x1},
                                                        {0b00111100000000000000000000000000, 0x2},
                                                        {0b00011110000000000000000000000000, 0x3},
                                                        {0b00001111000000000000000000000000, 0x4},
                                                        {0b00000111100000000000000000000000, 0x5},
                                                        {0b00000011110000000000000000000000, 0x6},
                                                        {0b00000001111000000000000000000000, 0x7},
                                                        {0b00000000111100000000000000000000, 0x8},
                                                        {0b00000000011110000000000000000000, 0x9},
                                                        {0b00000000001111000000000000000000, 0xa},
                                                        {0b00000000000111100000000000000000, 0xb},
                                                        {0b00000000000011110000000000000000, 0xc},
                                                        {0b00000000000001111000000000000000, 0xd},
                                                        {0b00000000000000111100000000000000, 0xe},
                                                        {0b00000000000000011110000000000000, 0xf},
                                                        {0b00000000000000001111000000000000, 0x10},
                                                        {0b00000000000000000111100000000000, 0x11},
                                                        {0b00000000000000000011110000000000, 0x12},
                                                        {0b00000000000000000001111000000000, 0x13},
                                                        {0b00000000000000000000111100000000, 0x14},
                                                        {0b00000000000000000000011110000000, 0x15},
                                                        {0b00000000000000000000001111000000, 0x16},
                                                        {0b00000000000000000000000111100000, 0x17},
                                                        {0b00000000000000000000000011110000, 0x18},
                                                        {0b00000000000000000000000001111000, 0x19},
                                                        {0b00000000000000000000000000111100, 0x1a},
                                                        {0b00000000000000000000000000011110, 0x1b},
                                                        {0b00000000000000000000000000001111, 0x1c} };

    string out = "101";

    for (auto &inst_dic_entry: inst_dic) {
        uint32_t result = entry ^ inst_dic_entry.second;
        std::map<uint32_t, uint16_t>::iterator flag = four_bit_mismatch_location_dic.find(result);
        // key already present on the map
        if (flag != four_bit_mismatch_location_dic.end()){
           
           // 100 + mismatch location(5) + dic_index(4)
            output = output + out + std::bitset<5>(four_bit_mismatch_location_dic[result]).to_string() + std::bitset<4>(inst_dic_entry.first).to_string();
            return true;  
        }     
    }
    // cannot compress with 1 bit mis match
    return false;

}

bool two_anywh_bit_mismatch(uint32_t entry){

    string out = "110";

    for (auto &inst_dic_entry: inst_dic) {
        
        uint32_t result = entry ^ inst_dic_entry.second;
        uint32_t N = result;
        
        int ones_count = 0;
        int location_2 = 0;
        int location_1 = 0;
        int index_counter = 0;
        bool is_two_bit_mis_match_found = false;
        
        while (N > 0) {
            // If current bit is 1
            if (N & 1) {
                ones_count++;
                if (ones_count == 1) location_2 = 31 - index_counter;
                if (ones_count == 2) {
                    location_1 = 31 - index_counter;
                    is_two_bit_mis_match_found = true;
                } 
                if (ones_count > 2) {
                    location_1 = 0;
                    location_2 = 0;
                    is_two_bit_mis_match_found = false;
                    break;
                }
                
            }
            index_counter++;
            N = N >> 1;
        }
        if (is_two_bit_mis_match_found){

            output = output + out + std::bitset<5>(location_1).to_string() + std::bitset<5>(location_2).to_string() + std::bitset<4>(inst_dic_entry.first).to_string();
            //cout << out + std::bitset<5>(location_1).to_string() + std::bitset<5>(location_2).to_string() + std::bitset<4>(inst_dic_entry.first).to_string() << endl;
            return true;
        }
    }
    // cannot compress with 1 bit mis match
    return false;

}

bool bitmask_compression(uint32_t entry){

    std::map<uint32_t,uint16_t> bitmask_dic =  {{0b10000000000000000000000000000000, 0x8},
                                                {0b10010000000000000000000000000000, 0x9},
                                                {0b10100000000000000000000000000000, 0xa},
                                                {0b10110000000000000000000000000000, 0xb},
                                                {0b11000000000000000000000000000000, 0xc},
                                                {0b11010000000000000000000000000000, 0xd},
                                                {0b11100000000000000000000000000000, 0xe},
                                                {0b11110000000000000000000000000000, 0xf} };

    string out = "010";

    for (auto &inst_dic_entry: inst_dic) {
        uint32_t result = entry ^ inst_dic_entry.second;
        for (auto &bitmask: bitmask_dic) {
            uint32_t bm = bitmask.first;
            for(uint8_t location = 0; location < 29; location++){
                uint32_t val = bm^result; 
                if (val == 0x0 ){

                    // 010 + start location(5) + bitmask(4) + dic_index(4)
                    output = output + out + std::bitset<5>(location).to_string() + std::bitset<4>(bitmask.second).to_string() + std::bitset<4>(inst_dic_entry.first).to_string();
                    return true;
                }else if (val < result){
                    break;
                }else{
                    bm = bm >> 1;
                }
            } 
        }
    }
    // cannot compress with bit mask 
    return false;

}


bool direct_mapping(uint32_t entry){
    string out = "111";
    for (auto &inst_dic_entry: inst_dic) {

        if ( (entry^inst_dic_entry.second) == 0x0){
            output = output + out + std::bitset<4>(inst_dic_entry.first).to_string();
            return true;
        }
    }
    return false;
}

//index - entry`s index, entry is already a duplicate
int RLE(uint32_t entry, uint32_t index){
    string out = "001";
    int no_of_duplicates = 1;
    while(no_of_duplicates < 8){
        if (entry == arr_lines[index + no_of_duplicates]){
            no_of_duplicates++;
        }else{
            break;
        }
    }
    output = output + out + std::bitset<3>(no_of_duplicates-1).to_string();
    return no_of_duplicates;
}

void ordinary_binary(uint32_t entry){
    output = output + "000" + std::bitset<32>(entry).to_string();
}


/***
 * 
 * 
 * ************************Deompression*****************************
 * 
 * 
 */

string compressed_txt = "";
vector<uint32_t> dic_arr;
vector<string> dic_string_arr;
void read_compressed_file(string file_name){
   newfile.open(file_name,ios::in); //open a file to perform read operation using file object
   if (newfile.is_open()){   //checking whether the file is open
        string string_line;
        bool xxxx_found =false;
        while(getline(newfile, string_line)){ //read data from file object and put it into string.
            
            if(string_line == "xxxx") {
                xxxx_found = true;
                continue;
            }
            if (!xxxx_found){ 
                compressed_txt = compressed_txt + string_line.substr(0, 32);
            }else{
                //adding the dic values to array
                dic_string_arr.push_back(string_line.substr(0, 32));
                uint32_t line_val = 0x0000;
                
                for(int i = 0; i < 32; i++){
                    if (string_line[i] == '1') {
                        line_val += 0x0001;    
                    }
                    if (i != 31) line_val = line_val << 1;
                }
                dic_arr.push_back(line_val);
            }
        }
        dic_arr.shrink_to_fit();
        newfile.close(); //close the file object.
    }
}

void write_original_file(string file_name, string text){
    newfile.open(file_name,ios::out);  // open a file to perform write operation using file object
    if(newfile.is_open()){//checking whether the file is open
        newfile << text;   //inserting text
    newfile.close();    //close the file object 
    }
}

uint32_t string_binary_to_uint_converter(string binary){
    uint32_t line_val = 0x0000;
                
    for(int i = 0; i < binary.size(); i++){
        if (binary[i] == '1') {
            line_val += 0x0001;    
        }
        if (i != (binary.size() - 1)) line_val = line_val << 1;
    }

    return line_val;
}
int main(int argc, char *argv[]){

    if(argv[1][0] == '1'){
        readfile("original.txt");
        create_dictionary();

        bool RLE_active = false;
        for(int index = 0; index < no_of_lines; index++){
            uint32_t line = arr_lines[index];
            if (index != 0 && (line == arr_lines[index-1]) && !RLE_active){
                int no_of_duplicates = RLE(line, index);
                index = index + no_of_duplicates -1; 
                RLE_active = true;
                continue;
            }else if (direct_mapping(line)){
                RLE_active = false;
                continue;
            }else if (one_bit_mismatch(line)){
                RLE_active = false;
                continue;
            }else if (two_cons_bit_mismatch(line)){
                RLE_active = false;
                continue;
            }else if (four_cons_bit_mismatch(line)){
                RLE_active = false;
                continue;
            }else if (bitmask_compression(line)){
                RLE_active = false;
                continue;
            }else if (two_anywh_bit_mismatch(line)){
                RLE_active = false;
                continue;
            }else{
                ordinary_binary(line);
                RLE_active = false;
            }
            
        }
        //'0' padding
        int len_output = output.size();
        if (len_output % 32 != 0 ){
            int len_new_output = (floor(len_output/32) + 1)*32;
            for (int i = 0; i < (len_new_output - len_output); i++){
                output = output + "0";
            }   
        }
        write_file("cout.txt");
    }

    /**************Decopression**********/
    
    if (argv[1][0] == '2'){
        read_compressed_file("compressed.txt");
        string original_code = "";
        

        int read_index = 0;
        int len_compressedtxt = compressed_txt.size();
        while (read_index <  len_compressedtxt){

            if (read_index < (len_compressedtxt - 32) || string_binary_to_uint_converter(compressed_txt.substr(read_index, len_compressedtxt-1)) != 0){
                string type = compressed_txt.substr(read_index, 3);

                read_index = read_index + 3;

                if (type == "000"){ //ordinary bin

                    string ordinary_binary = compressed_txt.substr(read_index, 32);
                    original_code = original_code + ordinary_binary + "\n";
                    read_index = read_index + 32;

                }else if (type == "001"){ // rle

                    int len_original_code = original_code.size();

                    string duplicate_code = original_code.substr(len_original_code - 33, 32);

                    uint32_t no_of_duplicates = string_binary_to_uint_converter(compressed_txt.substr(read_index, 3));

                    for (uint32_t count = 0; count < no_of_duplicates + 1; count++){
                        original_code = original_code + duplicate_code + "\n";
                    }

                    read_index = read_index + 3;

                }else if (type == "010"){ // bitmask compression
                    uint8_t mask = 0;
                    uint32_t start_location = string_binary_to_uint_converter(compressed_txt.substr(read_index, 5));
                    read_index = read_index + 5;
        
                    string bitmask = compressed_txt.substr(read_index, 4);
                    uint8_t bm = string_binary_to_uint_converter(bitmask);
                    
                    read_index = read_index + 4;
        
                    uint32_t dic_index =  string_binary_to_uint_converter(compressed_txt.substr(read_index,  4));
                    read_index = read_index + 4;

                    string dic_element = dic_string_arr[(int) dic_index];

                    uint32_t changing_bitmask = string_binary_to_uint_converter(dic_element.substr(start_location, 4));
                    
                    uint32_t result_bm = changing_bitmask ^ bm;
                    string str_result_bm = std::bitset<4>(result_bm).to_string();

                    dic_element.replace(start_location, 4 , str_result_bm );
                    original_code = original_code + dic_element + "\n";


                }else if (type == "011"){  //onebit mis match
                    uint32_t mismatch_location = string_binary_to_uint_converter(compressed_txt.substr(read_index, 5));
                    read_index = read_index + 5;

                    uint32_t dic_index =  string_binary_to_uint_converter(compressed_txt.substr(read_index, 4));
                    read_index = read_index + 4;
                    
                    string dic_element = dic_string_arr[(int) dic_index];
                    uint32_t uint_dic_element = string_binary_to_uint_converter(dic_element);

                    uint32_t mask = 0;
                    mask = mask << (mismatch_location + 1);
                    mask = mask + 0b1;
                    mask = mask << (32 - (mismatch_location + 1));

                    uint32_t result = mask ^ uint_dic_element;
                    dic_element = std::bitset<32>(result).to_string();

                    original_code = original_code + dic_element + "\n";
                }else if (type == "100"){   //2bit mis match
                    uint32_t mismatch_location = string_binary_to_uint_converter(compressed_txt.substr(read_index, 5));
                    read_index = read_index + 5;

                    uint32_t dic_index =  string_binary_to_uint_converter(compressed_txt.substr(read_index, 4));
                    read_index = read_index + 4;
                    
                    string dic_element = dic_string_arr[(int) dic_index];
                    uint32_t uint_dic_element = string_binary_to_uint_converter(dic_element);

                    uint32_t mask = 0;
                    mask = mask << (mismatch_location + 2);
                    mask = mask + 0b11;
                    mask = mask << (32 - (mismatch_location + 2));

                    uint32_t result = mask ^ uint_dic_element;
                    dic_element = std::bitset<32>(result).to_string();

                    original_code = original_code + dic_element + "\n";
                }else if (type == "101"){  //4 bit mis match
                    
                    uint32_t mismatch_location = string_binary_to_uint_converter(compressed_txt.substr(read_index, 5));
                    read_index = read_index + 5;

                    uint32_t dic_index =  string_binary_to_uint_converter(compressed_txt.substr(read_index, 4));
                    read_index = read_index + 4;
                    
                    string dic_element = dic_string_arr[(int) dic_index];
                    uint32_t uint_dic_element = string_binary_to_uint_converter(dic_element);

                    uint32_t mask = 0;
                    mask = mask << (mismatch_location + 4);
                    mask = mask + 0xf;
                    mask = mask << (32 - (mismatch_location + 4));

                    uint32_t result = mask ^ uint_dic_element;
                    dic_element = std::bitset<32>(result).to_string();

                    original_code = original_code + dic_element + "\n";
                }else if (type == "110"){ // 2 conseecutive bits mismatch

                    uint32_t mismatch_locations[2] = {};
                    mismatch_locations[0] = string_binary_to_uint_converter(compressed_txt.substr(read_index, 5));
                    read_index = read_index + 5;

                    mismatch_locations[1] = string_binary_to_uint_converter(compressed_txt.substr(read_index, 5));
                    read_index = read_index + 5;

                    uint32_t dic_index =  string_binary_to_uint_converter(compressed_txt.substr(read_index, 4));
                    read_index = read_index + 4;

                    string dic_element = dic_string_arr[(int) dic_index];

                    for (int i = 0; i < 2 ; i++){
                        
                        string part1 = dic_element.substr(0,mismatch_locations[i]+1);
                        uint32_t uint_part1 = string_binary_to_uint_converter(part1);

                        string part2 = dic_element.substr(mismatch_locations[i]+1,dic_element.size()-1);
                        uint32_t uint_part2 = string_binary_to_uint_converter(part2);
                    
                        if (uint_part1 & 1){
                            uint_part1 = uint_part1 >> 1;
                            uint_part1 = uint_part1 << 1;
                        }else{
                            uint_part1 = uint_part1 + 1;
                        }

                        uint_part1 = uint_part1 << (31 - mismatch_locations[i]);
                        uint_part1 = uint_part1 + uint_part2;

                        dic_element = std::bitset<32>(uint_part1).to_string();                
                    }
                    original_code = original_code + dic_element + "\n";

                }else if (type == "111"){  // direct mapping
                    uint32_t dic_index =  string_binary_to_uint_converter(compressed_txt.substr(read_index, 4));
                    read_index = read_index + 4;
                    
                    string dic_element = dic_string_arr[(int) dic_index];  

                    original_code = original_code + dic_element + "\n";          
                }
            }else{
                break;
            }
        }
        write_original_file("dout.txt", original_code);
    }
}