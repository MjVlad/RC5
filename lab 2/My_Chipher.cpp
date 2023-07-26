#define _CRT_SECURE_NO_WARNINGS 

#include "My_Chipher.h"
#include <iostream>
#include <algorithm>

uint16_t short_from_bytes(const char* bytes) {
	uint16_t res = bytes[2];
	for (int i = 1; i >= 0; i--) {
		res = (res << 8) + (unsigned char)bytes[i];
	}
	return res;
}

char* bytes_from_short(uint16_t num1) {
	char* res = new char[2];
	for (int i = 0; i < 2; i++) {
		res[i] = (char)(num1 % 256);
		num1 >>= 8;
	}
	return res;
}

std::bitset<16> operator+(const std::bitset<16>& l, const std::bitset<16>& r) {
	std::bitset<16> out(l.to_ulong() + r.to_ulong());
	return out;
}

std::bitset<16> operator-(const std::bitset<16>& l, const std::bitset<16>& r) {
	std::bitset<16> out(l.to_ulong() - r.to_ulong());
	return out;
}

My_Chipher::My_Chipher(uint16_t key, uint8_t raunds){
	this->key = key;
	rounds_count = raunds;
	C ^= key;
	key_space.resize(rounds_count);
	space.resize(rounds_count);
	gen_space();
	gen_key_space();
}

std::bitset<16> My_Chipher::cyclic_shift(std::bitset<16> num, uint32_t step, char dir) {
	if (!(dir == 'r' || dir == 'l'))
		throw std::exception("Wrong shift direction");
	step %= 16;
	if (step == 0)
		return num;
	if (dir == 'r')
		return std::bitset<16>((num >> step) | (num << (16 - step)));
	else
		return std::bitset<16>((num >> (16 - step) | (num << step)));
}

void My_Chipher::gen_space(){
	space.at(0) = P;
	for (int i = 1; i < rounds_count; i++) {
		if (i % 2 == 0) {
			space.at(i) = space.at(i - 1) | C;
		}
		else {
			space.at(i) = space.at(i - 1) & (P ^ C);
		}
	}
}

void My_Chipher::gen_key_space(){
	std::bitset<16> A(0);
	std::bitset<16> B(0);
	for (int i = 0; i < rounds_count; i++) {
		A = key_space.at(i) = space.at(i) | A ^ B;
		B = space.at(i) = key_space.at(i) & A;
	}
}

uint16_t My_Chipher::encrypting_block_CBC(uint16_t block, uint16_t CBC_key){
	return encrypting_block(block ^ CBC_key);
}

uint16_t My_Chipher::decrypting_block_CBC(uint16_t block, uint16_t CBC_key){
	return decrypting_block(block) ^ CBC_key;
}

uint16_t My_Chipher::encrypting_block(uint16_t block){
	std::bitset<16> A(block);
	for (int i = 0; i < rounds_count; i++) {
		if (i % 2 == 0)
			A = cyclic_shift((A ^ C), (P).to_ulong(), 'l') + key_space.at(i);
		else 
			A = cyclic_shift((A ^ C), (C).to_ulong(), 'l') + key_space.at(i) - P;
		std::cout << A << std::endl;
	}
	uint16_t res = A.to_ulong();
	return res;
}

uint16_t My_Chipher::decrypting_block(uint16_t block){
	std::bitset<16> A(block);
	for (int i = rounds_count - 1; i >= 0; i--) {
		if (i % 2 == 0)
			A = cyclic_shift(A - key_space.at(i), (P).to_ulong(), 'r') ^ C;
		else
			A = cyclic_shift(A - key_space.at(i) + P, (C).to_ulong(), 'r') ^ C;
		std::cout << A << std::endl;
	}
	uint16_t res = A.to_ulong();
	return res;
}

void My_Chipher::encrypting_file_all(std::string input_file_name, std::string output_file_name, bool CBC, uint16_t vec_init){
	FILE* in;
	FILE* out;
	in = fopen(input_file_name.data(), "rb");
	out = fopen(output_file_name.data(), "wb");
	if (!in || !out) {
		std::exception ex("File not found");
	}

	std::transform(input_file_name.begin(), input_file_name.end(), input_file_name.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (input_file_name.find(".bmp") != std::string::npos) {
		char* tmp = new char[54];
		fread(tmp, 1, 54, in);
		fwrite(tmp, 1, 54, out);
	}

	/*void copy_bytes(char* first, const char* second) {
		for (int i = 0; i < 8; i++) {
			first[i] = second[i];
		}
	}*/

	uint16_t vec = vec_init;
	char* buf = new char[2];
	size_t size = 0;
	while ((size = fread(buf, 1, 2, in)) == 2) {
		if (feof(in) || ferror(in)) break;
		if (CBC) {
			vec = this->encrypting_block_CBC(short_from_bytes(buf), vec);
			buf = bytes_from_short(vec);
		}
		else
			buf = bytes_from_short(this->encrypting_block_CBC(short_from_bytes(buf)));
		//buf = this->encrypting_block_CBC(buf, vec);
		//copy_bytes(vec, buf);
		fwrite(buf, 1, 2, out);
	}
	for (int i = size; i < 8; i++)
		buf[i] = -128;
	if (CBC)
		buf = bytes_from_short(this->encrypting_block_CBC(short_from_bytes(buf), vec));
	else
		buf = bytes_from_short(this->encrypting_block_CBC(short_from_bytes(buf)));
	fwrite(buf, 1, 2, out);
	fclose(in);
	fclose(out);
}

void My_Chipher::decrypting_file_all(std::string input_file_name, std::string output_file_name, bool CBC, uint16_t vec_init){
	FILE* in;
	FILE* out;
	in = fopen(input_file_name.data(), "rb");
	out = fopen(output_file_name.data(), "wb");
	if (!in || !out) {
		std::exception ex("File not found");
	}

	std::transform(input_file_name.begin(), input_file_name.end(), input_file_name.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (input_file_name.find(".bmp") != std::string::npos) {
		char* tmp = new char[54];
		fread(tmp, 1, 54, in);
		fwrite(tmp, 1, 54, out);
	}

	uint16_t vec = vec_init;
	char* buf = new char[2];
	char* second_buf = new char[2];
	size_t size = fread(second_buf, 1, 2, in);
	if (size == 0)
		return;
	char* tmp;
	while (true) {
		if (CBC) {
			buf = bytes_from_short(this->decrypting_block_CBC(short_from_bytes(second_buf), vec));
			vec = short_from_bytes(second_buf);
		}
		else
			buf = bytes_from_short(this->decrypting_block_CBC(short_from_bytes(second_buf)));
		size = fread(second_buf, 1, 2, in);
		if (size == 0) {
			int i = 1;
			for (i; i >= 0 && buf[i] == -128; i--);

			fwrite(buf, 1, i + 1, out);
			break;
		}
		fwrite(buf, 1, 2, out);
	}
	fclose(in);
	fclose(out);
}
