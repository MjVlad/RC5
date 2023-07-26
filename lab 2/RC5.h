#pragma once

#define _CRT_SECURE_NO_WARNINGS 
#include <string>
#include <math.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string.h>
#include <bitset>
//#include <bitset>



class RS5{
private:
	short word_len;
	short raund_amount;
	std::string key;
	uint16_t T;
	short w4;
	short w8;
	uint64_t modul;
	int key_len;

	uint32_t P = 0xB7E15163;
	uint32_t Q = 0x9E3779B9;

	std::vector<uint32_t> key_al;
	std::vector<uint32_t> key_space;

	char* xor_key = nullptr;

public:
	RS5(short word_lenght, short raunds, std::string key) {
		if (!(word_lenght == 16 || word_lenght == 32)) {
			throw std::exception("wrong word size");
		}
		word_len = word_lenght;
		raund_amount = raunds;
		this->key = key;
		T = 2 * (raund_amount + 1);
		w4 = word_len / 4;
		w8 = word_len / 8; //=4:32
		//modul = pow(2, word_lenght);
		modul = UINT32_MAX;
		modul++;
		key_len = key.size();
		key_space.resize(T);
		key_align();
		key_extend();
		shuffle();
	}

	void set_xor_key(bool flag) {
		//std::cout << "hi" << std::endl;
		xor_key = new char[8];
		if (flag) {
			for (int i = 0; i < 8; i++)
				xor_key[i] = 0x01; // 00000001 = 0x01
		}
		else {
			for (int i = 0; i < 8; i++)
				xor_key[i] = 0xAA;//10101010 = 0xAA
			//0101010101 = 0x44
		}
	}

	uint32_t cyclic_shift(uint32_t num, uint32_t step, char dir) {
		if (!(dir == 'r' || dir == 'l'))
			throw std::exception("Wrong shift direction");
		step %= 32;
		if (step == 0)
			return num;
		if (dir == 'r')
			return (num >> step) | (num << (32 - step));
		else
			return (num >> (32 - step) | (num << step));
	}

	uint32_t int_from_bytes(const char* bytes) {
		uint32_t res = bytes[4];
		for (int i = 3; i >= 0; i--) {
			res = (res << 8) + (unsigned char)bytes[i];
		}
		return res;
	}

	char* XOR(char* bytes, const char* key) {
		char* tmp = new char[8];
		for (int i = 0; i < 8; i++) {
			tmp[i] = (char)((uint8_t)bytes[i] ^ (uint8_t)key[i]);
		}
		return tmp;
	}

	uint64_t long_from_bytes(const char* bytes) {
		uint64_t res = bytes[8];
		for (int i = 7; i >= 0; i--) {
			res = (res << 8) + (unsigned char)bytes[i];
		}
		return res;
	}

	char* bytes_from_long(uint64_t num1) {
		char* res = new char[8];
		for (int i = 0; i < 8; i++) {
			res[i] = (char)(num1 % 256);
			num1 >>= 8;
		}
		return res;
	}

	void copy_bytes(char* first, const char* second) {
		for (int i = 0; i < 8; i++) {
			first[i] = second[i];
		}
	}

	char* copy_bytes(char* second) {
		char* n = new char[8];
		for (int i = 0; i < 8; i++) {
			n[i] = second[i];
		}
		return n;
	}

	char* bytes_from_two_int(uint32_t num1, uint32_t num2) {
		char* res = new char[w4];
		for (int i = 0; i < w8; i++) {
			res[i] = (char)(num1 % 256);
			num1 >>= 8;
		}
		for (int i = 0; i < w8; i++) {
			res[i+4] = (char)(num2 % 256);
			num2 >>= 8;
		}
		return res;
	}

//private:
	void key_align() {
		if (key_len == 0) {
			key_al.resize(1);
			key_al.at(0) = 0;
			return;
		}
		else if (key_len % w8 != 0) {
			int tmp = w8 - key_len % w8;
			for (int i = 0; i < tmp; i++)
				key.push_back(0);
			key_len += tmp;
		}
		key_al.resize(key_len / w8);
		for (int i = 0; i < key_len; i++) {
			key_al.at(i / w8) = (key_al.at(i / w8) << 8) + key[i];
		}
	}

	void key_extend() {
		key_space.at(0) = P;
		for (size_t i = 1; i < T; i++) {
			key_space.at(i) = key_space.at(i - 1) + Q;
		}
	}

	void shuffle() {
		int i = 0;
		int j = 0;
		uint32_t A = 0;
		uint32_t B = 0;
		for (size_t k = 0; k < 3 * std::max(key_al.size(), (size_t)T); k++) {
			A = key_space[i] = cyclic_shift((key_space.at(i) + A + B), 3, 'l');
			B = key_al[j] = cyclic_shift((key_al.at(j) + A + B), (A + B), 'l');
			i = (i + 1) % T;
			j = (j + 1) % key_al.size();
		}
	}

	char* decrypting_block_XOR(char* data, const char* key = nullptr) {
		if (xor_key == nullptr && key == nullptr)
			return decrypting_block(data);

		if (key == nullptr)
			return XOR(decrypting_block(data), xor_key);
	
		if (xor_key == nullptr)
			return decrypting_block_CBC(data, key);

		return XOR(decrypting_block_CBC(data, key), xor_key);
	}

	void print(char* c) {
		uint64_t n = long_from_bytes(c);
		for (int i = 0; i < 64; i++) {
			std::cout << (n % 2);
			n >>= 1;
		}
		std::cout << std::endl;
	}

	char* encrypting_block_XOR(char* data, const char* key = nullptr) {
		
		if (xor_key == nullptr && key == nullptr) {
			std::cout << "block: ";
			print(data);
			//std::cout << "xor block: " << key << std::endl;
			char* tmp = encrypting_block(data);
			std::cout << "out block";
			print(tmp);
			return tmp;
		}

		if (key == nullptr) {
			std::cout << "block: ";
			print(data);
			std::cout << "xor_block: ";
			print(xor_key);
			std::cout << "block xor xor_block: ";
			print(XOR(data, xor_key));
			char* tmp = encrypting_block(XOR(data, xor_key));
			std::cout << "out block";
			print(tmp);
			return tmp;
		}

		if (xor_key == nullptr) {
			std::cout << "block: ";
			print(data);
			//std::cout << "xor_block: ";
			//print(xor_key);
			//std::cout << "block xor xor_block: ";
			//print(XOR(data, xor_key));
			char* tmp = encrypting_block_CBC(data, key);
			std::cout << "out block";
			print(tmp);
			return tmp;
		}
		
		std::cout << "block: ";
		print(data);
		std::cout << "xor_block: ";
		print(xor_key);
		std::cout << "block xor xor_block: ";
		print(XOR(data, xor_key));
		char* tmp = encrypting_block_CBC(XOR(data, xor_key), key);
		std::cout << "out block";
		print(tmp);
		return tmp;
	}

	char* decrypting_block_CBC(char* data, const char* key) {
		return XOR(decrypting_block(data), key);
	}

	char* encrypting_block_CBC(char* data, const char* key) {
		return encrypting_block(XOR(data, key));
	}

	/*char* encrypting_block(char* data) {
		uint32_t A = this->int_from_bytes(data);
		uint32_t B = this->int_from_bytes(data + 4);
		A += key_space.at(0);
		B += key_space.at(1);
		for (int i = 1; i < raund_amount; i++) {
			A = cyclic_shift((A ^ B), B, 'l') + key_space.at(2 * i);
			B = cyclic_shift((B ^ A), A, 'l') + key_space.at(2 * i + 1);
		}
		return bytes_from_two_int(A, B);
	}*/

	uint64_t long_from_two_int(int A, int B) {
		uint64_t res = (A << 32) + B;
		return res;
	}

	char* encrypting_block(char* data) {
		uint32_t A = this->int_from_bytes(data);
		uint32_t B = this->int_from_bytes(data + 4);
		uint64_t tmp = long_from_two_int(A, B);
		std::bitset<64> prev(tmp);
		A += key_space.at(0);
		B += key_space.at(1);
		tmp = long_from_two_int(A, B);
		std::bitset<64> cur(tmp);
		std::vector<int> res;
		res.resize(64);
		prev ^= cur;
		for (int i = 0; i < 64; i++) {
			if (prev[i]) res[i]++;
		}
		for (int i = 1; i < raund_amount; i++) {
			prev = cur;
			A = cyclic_shift((A ^ B), B, 'l') + key_space.at(2 * i);
			B = cyclic_shift((B ^ A), A, 'l') + key_space.at(2 * i + 1);
			cur = std::bitset<64>(long_from_two_int(A, B));
			prev ^= cur;
			for (int i = 0; i < 64; i++) {
				if (prev[i]) res[i]++;
			}
		}

		std::ofstream out;
		out.open("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/it is result.txt", std::ios_base::app);
		for (int i = 0; i < 64; i++) {
			out << res[i] << " ";
		}
		out << std::endl;

		return bytes_from_two_int(A, B);
	}

	char* decrypting_block(char* data) {
		uint32_t A = this->int_from_bytes(data);
		uint32_t B = this->int_from_bytes(data + 4);
		for (int i = raund_amount - 1; i > 0; i--) {
			B = cyclic_shift(B - key_space.at(2 * i + 1), A, 'r') ^ A;
			A = cyclic_shift(A - key_space.at(2 * i), B, 'r') ^ B;
		}
		B -= key_space.at(1);
		A -= key_space.at(0);
		return bytes_from_two_int(A, B);
	}

//public:
	void encrypting_file(std::string input_file_name, std::string output_file_name) {
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

		char* buf = new char[8];
		size_t size = 0;
		while ((size = fread(buf, 1, 8, in)) == 8) {
			if (feof(in) || ferror(in)) break;
			buf = this->encrypting_block(buf);
			fwrite(buf, 1, 8, out);
		}
		for (int i = size; i < 8; i++)
			buf[i] = -128;
		buf = this->encrypting_block(buf);
		fwrite(buf, 1, 8, out);
		fclose(in);
		fclose(out);
	}

	void decrypting_file(std::string input_file_name, std::string output_file_name) {
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

		char* buf = new char[8];
		char* second_buf = new char[8];
		size_t size = fread(second_buf, 1, 8, in);
		if (size == 0)
			return;
		char* tmp;
		while (true) {
			buf = this->decrypting_block(second_buf);
			size = fread(second_buf, 1, 8, in);
			if (size == 0) {
				int i = 7;
				for (i; i >= 0 && buf[i] == -128; i--);

				fwrite(buf, 1, i + 1, out);
				break;
			}
			fwrite(buf, 1, 8, out);
		}
		fclose(in);
		fclose(out);
	}

	void encrypting_file_CBC(std::string input_file_name, std::string output_file_name, uint64_t vec_init) {
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
		
		char* vec = bytes_from_long(vec_init);
		char* buf = new char[8];
		size_t size = 0;
		while ((size = fread(buf, 1, 8, in)) == 8) {
			if (feof(in) || ferror(in)) break;
			buf = this->encrypting_block_CBC(buf, vec);
			copy_bytes(vec, buf);
			fwrite(buf, 1, 8, out);
		}
		for (int i = size; i < 8; i++)
			buf[i] = -128;
		buf = this->encrypting_block_CBC(buf, vec);
		fwrite(buf, 1, 8, out);
		fclose(in);
		fclose(out);
	}

	void decrypting_file_CBC(std::string input_file_name, std::string output_file_name, uint64_t vec_init) {
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

		char* vec = bytes_from_long(vec_init);
		char* buf = new char[8];
		char* second_buf = new char[8];
		size_t size = fread(second_buf, 1, 8, in);
		if (size == 0)
			return;
		char* tmp;
		while (true) {
			buf = this->decrypting_block_CBC(second_buf, vec);
			copy_bytes(vec, second_buf);
			size = fread(second_buf, 1, 8, in);
			if (size == 0) {
				int i = 7;
				for (i; i >= 0 && buf[i] == -128; i--);

				fwrite(buf, 1, i + 1, out);
				break;
			}
			fwrite(buf, 1, 8, out);
		}
		fclose(in);
		fclose(out);
	}

	void encrypting_file_all(std::string input_file_name, std::string output_file_name, bool CBC = false, uint64_t vec_init = 0, short xoring = 0) {
		if (xoring) {
			if (xoring - 1 == 0) {
				//std::cout << "hi" << std::endl;
				set_xor_key(false);
			}
			else
				set_xor_key(true);
			//set_xor_key(bool(xoring - 1)); //need to test
		}

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

		char* vec = bytes_from_long(vec_init);
		char* buf = new char[8];
		size_t size = 0;
		while ((size = fread(buf, 1, 8, in)) == 8) {
			if (feof(in) || ferror(in)) break;
			if (CBC) {
				buf = this->encrypting_block_XOR(buf, vec);
				copy_bytes(vec, buf);
			}
			else
				buf = this->encrypting_block_XOR(buf);
			//buf = this->encrypting_block_CBC(buf, vec);
			//copy_bytes(vec, buf);
			fwrite(buf, 1, 8, out);
		}
		for (int i = size; i < 8; i++)
			buf[i] = -128;
		if (CBC)
			buf = this->encrypting_block_XOR(buf, vec);
		else
			buf = this->encrypting_block_XOR(buf);
		fwrite(buf, 1, 8, out);
		fclose(in);
		fclose(out);
	}

	void decrypting_file_all(std::string input_file_name, std::string output_file_name, bool CBC = false, uint64_t vec_init = 0, short xoring = false) {
		if (xoring) {
			if (xoring - 1 == 0)
				set_xor_key(false);
			else
				set_xor_key(true);
			//set_xor_key(bool(xoring - 1)); //need to test
		}

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

		char* vec = bytes_from_long(vec_init);
		char* buf = new char[8];
		char* second_buf = new char[8];
		size_t size = fread(second_buf, 1, 8, in);
		if (size == 0)
			return;
		char* tmp;
		while (true) {
			if (CBC) {
				buf = this->decrypting_block_XOR(second_buf, vec);
				copy_bytes(vec, second_buf);
			}
			else
				buf = this->decrypting_block_XOR(second_buf);
			//buf = this->decrypting_block_CBC(second_buf, vec);
			//copy_bytes(vec, second_buf);
			size = fread(second_buf, 1, 8, in);
			if (size == 0) {
				int i = 7;
				for (i; i >= 0 && buf[i] == -128; i--);

				fwrite(buf, 1, i + 1, out);
				break;
			}
			fwrite(buf, 1, 8, out);
		}
		fclose(in);
		fclose(out);
	}
};