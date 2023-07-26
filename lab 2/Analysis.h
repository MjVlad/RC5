#pragma once

#define _CRT_SECURE_NO_WARNINGS 
#include <string>
//#include <math.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
//#include <string.h>
#include <bitset>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "My_Chipher.h"


class Analys {
//private:
public:
	short fcount_zero(char ch) {
		uint8_t tmp = ch; //need to test
		short res = 0;
		for (int i = 0; i < 8; i++) {
			if (tmp % 2 == 0) {
				res++;
			}
			tmp >>= 1;
		}
		return res;
	}

	uint64_t long_from_bytes(const char* bytes) {
		uint64_t res = bytes[8];
		for (int i = 7; i >= 0; i--) {
			res = (res << 8) + (unsigned char)bytes[i];
		}
		return res;
	}

//public: 
	void first_analys(std::string file_name) {
		FILE* in;
		in = fopen(file_name.data(), "rb");
		if (!in) {
			std::exception ex("File not found");
		}

		//std::transform(file_name.begin(), file_name.end(), file_name.begin(),
		//	[](unsigned char c) { return std::tolower(c); });
		//if (file_name.find(".bmp") != std::string::npos) {
		//	char* tmp = new char[54];
		//	fread(tmp, 1, 54, in);
		//}

		int64_t count_zero = 0;
		int64_t count_one = 0;

		char* buf = new char[1];
		while (fread(buf, 1, 8, in)) {
			short tmp = 0;
			for (int i = 0; i < 8; i++)
				tmp += fcount_zero(buf[i]);
			count_zero += tmp;
			count_one += 64 - tmp;
		}

		fclose(in);

		double x = (pow((count_zero - count_one), 2) / (count_one + count_zero));
		std::cout << "count zero: " << count_zero << std::endl << "count one: " << count_one << std::endl << "X: " << x << std::endl;
		//std::cout << "frq test: " << x << std::endl;
	}

	void second_test(std::string file_name) {

		FILE* in;
		in = fopen(file_name.data(), "rb");
		if (!in) {
			std::exception ex("File not found");
		}

		//std::transform(file_name.begin(), file_name.end(), file_name.begin(),
		//	[](unsigned char c) { return std::tolower(c); });
		//if (file_name.find(".bmp") != std::string::npos) {
		//	char* tmp = new char[54];
		//	fread(tmp, 1, 54, in);
		//}

		int size_file = 64; //времянка
		int k;
		std::vector<double> e; e.resize(64);
		double tmp;
		for (k = 0; (tmp = ((size_file - k + 3) / pow(2, k + 2)) >= 5); k++) {
			e.at(k) = tmp;
		}
		k--;

		//std::cout << "k: " << k << std::endl;

		std::vector<uint32_t> space; space.resize(size_file); //space.resize(k);
		std::vector<uint32_t> block; block.resize(size_file); //block.resize(k);

		char* buf = new char[8];
		//while (fread(buf, 1, 8, in)) {
		//	
		//}
		fread(buf, 1, 8, in);

		fclose(in);

		uint64_t buff = long_from_bytes(buf);
		//int cur_len = 0;
		//short cur_el = buff % 2;
		for (int i = 0; i < 64; i++) {
			int cur_len = 0;
			short cur_el = buff % 2;
			while (buff % 2 == cur_el) {
				cur_len++;
				i++;
				buff >>= 1;
				if (i == 64) break;
			}
			if (cur_el == 0)
				space.at(cur_len)++;
			else
				block.at(cur_len)++;
		}

		//for (int i = 0; i < size_file; i++) {
		//	std::cout << "[" << i << "]\t" << space.at(i) << "\t" << block.at(i) << "\t" << e.at(i) << std::endl;
		//}

		double sum1 = 0;
		double sum2 = 0;
		for (int i = 1; i <= k; i++) {
			sum1 += pow(space.at(i) - e.at(i), 2) / e.at(i);
			sum2 += pow(block.at(i) - e.at(i), 2) / e.at(i);
		}

		std::cout << "series test: " << sum1 + sum2 << std::endl;
	}

	void third_test(std::string file_name, std::string out_fname) {
		FILE* in;
		in = fopen(file_name.data(), "rb");
		if (!in) {
			std::exception ex("File not found");
		}

		char* buf = new char[8];
		fread(buf, 1, 8, in);
		fclose(in);
		uint64_t buff = long_from_bytes(buf);
		std::bitset<64> bits(buff);

		//long long d = 1;//SIZE::HALF_BLOCK;
		//double* A = new double[32];
		std::vector<double> A; A.resize(32);
		std::ofstream out;
		out.open(out_fname);
		out.clear();

		for (int d = 1; d < 32; d++) {
			for (int i = 0; i < 64 - d - 1; ++i) {
				A[d] += bits[i] ^ bits[i + d];
			}
			A[d] = (2 * (A[d] - double((64 - d) / 2))) / (sqrt(64 - d));
			out << A[d] << " ";
		}


		/*FILE* out;
		out = fopen(out_fname.data(), "w");
		if (!out) {
			std::exception ex("File not found");
		}*/

		//double x = (2 * (A - (64 - d) / 2)) / (sqrt(64 - d));
		std::cout << "test3: done";// << x;// << std::endl;
	}

	void brute_force1(uint16_t control_key, uint16_t size_key) {
		int size = pow(2, size_key);
		for (uint16_t i = 0; i < size; i++) {
			if (i == control_key) {
				std::cout << "Sucsessfull!!" << std::endl;
				return;
			}
		}
	}

	//int get_file_size(std::string filename) // path to file
	//{
	//	FILE* p_file = NULL;
	//	p_file = fopen(filename.c_str(), "rb");
	//	fseek(p_file, 0, SEEK_END);
	//	int size = ftell(p_file);
	//	fclose(p_file);
	//	return size;
	//}
	

	/*bool equals_file(std::string left_file, std::string right_file) {
		int size_left = get_file_size(left_file);
		int size_right = get_file_size(right_file);

		std::ifstream in(left_file, std::ios::binary | std::ios::in);
		std::ofstream out(right_file, std::ios::binary | std::ios::out);


	}

	void brute_forse2(uint16_t control_key, uint16_t size_key, std::string encrypting_file, std::string tmp, std::string original_file) {
		int size = pow(2, size_key);
		for (uint16_t i = 0; i < size; i++) {

			if (i == control_key) {
				std::cout << "Sucsessfull!!" << std::endl;
				return;
			}
		}
	}*/
};