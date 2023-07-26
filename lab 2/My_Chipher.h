#pragma once
#include <string>
#include <vector>
#include <bitset>

class My_Chipher{
private:
	uint16_t key;
	uint8_t rounds_count;
	std::vector<std::bitset<16>> key_space;

	std::bitset<16> C = std::bitset<16>(0xfc14);
	std::bitset<16> P = std::bitset<16>(0xacdc);

	//uint16_t T;

	std::vector<std::bitset<16>> space;

private:
	//std::bitset<16> cyclic_shift(std::bitset<16> num, uint32_t step, char dir);

	void gen_space();
	void gen_key_space();
	uint16_t encrypting_block_CBC(uint16_t block, uint16_t CBC_key = 0);
	uint16_t decrypting_block_CBC(uint16_t block, uint16_t CBC_key = 0);

public:
	std::bitset<16> cyclic_shift(std::bitset<16> num, uint32_t step, char dir);
	My_Chipher(uint16_t key, uint8_t raunds);
	uint16_t encrypting_block(uint16_t block);
	uint16_t decrypting_block(uint16_t block);
	void encrypting_file_all(std::string input_file_name, std::string output_file_name, bool CBC = false, uint16_t vec_init = 0);
	void decrypting_file_all(std::string input_file_name, std::string output_file_name, bool CBC = false, uint16_t vec_init = 0);
};

