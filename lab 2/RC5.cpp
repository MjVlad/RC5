#include "RS5.h"
//
//uint32_t cut_long(uint64_t in, bool flag) { //flag = false - left; flag = true - right // need to test
//	uint32_t out;
//	if (!flag)
//		out = in >> 32;
//	else out = (in << 32) >> 32;
//	return out;
//}
//
//uint64_t concatenation_two_int(uint32_t l, uint32_t r) { //need to test
//	uint64_t res = (l << 32) + r;
//	return res;
//}
//
//RS5::RS5(short word_lenght, short raunds, std::string key){
//	if (!(word_lenght == 16 || word_lenght == 32)) {
//		throw std::exception("wrong word size");
//	}
//	word_len = word_lenght;
//	raund_amount = raunds;
//	this->key = key;
//	T = 2 * (raund_amount + 1);
//	w4 = word_len / 4;
//	w8 = word_len / 8; //=4:32
//	//modul = pow(2, word_lenght);
//	modul = UINT32_MAX;
//	modul++;
//	key_len = key.size();
//	key_space.resize(T);
//	key_align();
//	key_extend();
//	shuffle();
//}
//
//void RS5::set_xor_key(bool flag) {
//	//std::cout << "hi" << std::endl;
//	//xor_key = new char[8];
//	if (flag) {
//		xor_key = 0x0101010101010101;
//		//for (int i = 0; i < 8; i++)
//		//	xor_key[i] = 0x01; // 00000001 = 0x01
//	}
//	else {
//		xor_key = 0xAAAAAAAAAAAAAAAA;
//		//for (int i = 0; i < 8; i++)
//		//	xor_key[i] = 0xAA;//10101010 = 0xAA
//	}
//}
//
//uint32_t RS5::cyclic_shift(uint32_t num, uint32_t step, char dir){
//	if (!(dir == 'r' || dir == 'l'))
//		throw std::exception("Wrong shift direction");
//	step %= 32;
//	if (step == 0)
//		return num;
//	if (dir == 'r')
//		return (num >> step) | (num << (32 - step));
//	else
//		return (num >> (32 - step) | (num << step));
//}
//
//void RS5::key_align(){
//	if (key_len == 0) {
//		key_al.resize(1);
//		key_al.at(0) = 0;
//	}
//	else if (key_len % w8 != 0) {
//		int tmp = w8 - key_len % w8;
//		for (int i = 0; i < tmp; i++)
//			key.push_back(0);
//		key_len += tmp;
//	}
//	key_al.resize(key_len / w8);
//	for (int i = 0; i < key_len; i++) {
//		key_al.at(i / w8) = (key_al.at(i / w8) << 8) + key[i];
//	}
//}
//
//void RS5::key_extend() {
//	key_space.at(0) = P;
//	for (size_t i = 1; i < T; i++) {
//		key_space.at(i) = key_space.at(i - 1) + Q;
//	}
//}
//
//void RS5::shuffle(){
//	int i = 0;
//	int j = 0;
//	uint32_t A = 0;
//	uint32_t B = 0;
//	for (size_t k = 0; k < 3 * std::max(key_al.size(), (size_t)T); k++) {
//		A = key_space[i] = cyclic_shift((key_space.at(i) + A + B), 3, 'l');
//		B = key_al[j] = cyclic_shift((key_al.at(j) + A + B), (A + B), 'l');
//		i = (i + 1) % T;
//		j = (j + 1) % key_al.size();
//	}
//}
//
//uint64_t RS5::encrypting_block(uint64_t data){
//	uint32_t A = cut_long(data, 0);
//	uint32_t B = cut_long(data, 1);
//	A += key_space.at(0);
//	B += key_space.at(1);
//	for (int i = 1; i < raund_amount; i++) {
//		A = cyclic_shift((A ^ B), B, 'l') + key_space.at(2 * i);
//		B = cyclic_shift((B ^ A), A, 'l') + key_space.at(2 * i + 1);
//	}
//	return concatenation_two_int(A, B);
//}
//
//uint64_t RS5::decrypting_block(uint64_t data) {
//	uint32_t A = cut_long(data, 0);
//	uint32_t B = cut_long(data, 1);
//	for (int i = raund_amount - 1; i > 0; i--) {
//		B = cyclic_shift(B - key_space.at(2 * i + 1), A, 'r') ^ A;
//		A = cyclic_shift(A - key_space.at(2 * i), B, 'r') ^ B;
//	}
//	B -= key_space.at(1);
//	A -= key_space.at(0);
//	return concatenation_two_int(A, B);
//}
//
//void RS5::encrypting_file_all(std::string input_file_name, std::string output_file_name, bool CBC, uint64_t vec_init, short xoring){
//	if (xoring) {
//		if (xoring - 1 == 0) {
//			set_xor_key(false);
//		}
//		else
//			set_xor_key(true);
//	}
//
//	std::ifstream in(input_file_name, std::ios::binary | std::ios::in);
//	std::ofstream out(output_file_name, std::ios::binary | std::ios::out);
//	/*FILE* in;
//	FILE* out;
//	in = fopen(input_file_name.data(), "rb");
//	out = fopen(output_file_name.data(), "wb");*/
//	if (!in.is_open() || !out.is_open()) {
//		std::exception ex("File not found");
//	}
//
//	std::transform(input_file_name.begin(), input_file_name.end(), input_file_name.begin(),
//		[](unsigned char c) { return std::tolower(c); });
//	if (input_file_name.find(".bmp") != std::string::npos) {
//		char* tmp = new char[54];
//		/*fread(tmp, 1, 54, in);
//		fwrite(tmp, 1, 54, out);*/
//		in.read(tmp, 54);
//		out.write(tmp, 54);
//	}
//
//	//char* vec = bytes_from_long(vec_init);
//	//char* buf = new char[8];
//	//size_t size = 0;
//	//while ((size = fread(buf, 1, 8, in)) == 8) {
//	//	if (feof(in) || ferror(in)) break;
//	//	if (CBC) {
//	//		buf = this->encrypting_block_XOR(buf, vec);
//	//		copy_bytes(vec, buf);
//	//	}
//	//	else
//	//		buf = this->encrypting_block_XOR(buf);
//	//	//buf = this->encrypting_block_CBC(buf, vec);
//	//	//copy_bytes(vec, buf);
//	//	fwrite(buf, 1, 8, out);
//	//}
//	//for (int i = size; i < 8; i++)
//	//	buf[i] = -128;
//	//if (CBC)
//	//	buf = this->encrypting_block_XOR(buf, vec);
//	//else
//	//	buf = this->encrypting_block_XOR(buf);
//	//fwrite(buf, 1, 8, out);
//	//fclose(in);
//	//fclose(out);
//	short size;
//	uint64_t buf = 0;
//	while (!in.eof()) {
//		in.read((char*)&buf, sizeof(buf));
//		//size = in.gcount();
//		if ((size = in.gcount()) != 8)
//			break;
//		if (CBC) {
//			buf = this->encrypting_block_XOR(buf, vec_init);
//			vec_init = buf;
//		}
//		else
//			buf = this->encrypting_block_XOR(buf);
//		out.write(reinterpret_cast<char*>(&buf), sizeof(buf));
//	}
//	if (size != 8) {
//		buf <= (8 - size) * 8;
//	}
//	if (CBC)
//		buf = this->encrypting_block_XOR(buf, vec_init);
//	else
//		buf = this->encrypting_block_XOR(buf);
//	out.write(reinterpret_cast<char*>(&buf), sizeof(buf));
//	in.close();
//	out.close();
//}
//
//void RS5::decrypting_file_all(std::string input_file_name, std::string output_file_name, bool CBC, uint64_t vec_init, short xoring){
//	if (xoring) {
//		if (xoring - 1 == 0)
//			set_xor_key(false);
//		else
//			set_xor_key(true);
//	}
//	std::ifstream in(input_file_name, std::ios::binary | std::ios::in);
//	std::ofstream out(output_file_name, std::ios::binary | std::ios::out);
//	/*FILE* in;
//	FILE* out;
//	in = fopen(input_file_name.data(), "rb");
//	out = fopen(output_file_name.data(), "wb");*/
//	if (!in.is_open() || !out.is_open()) {
//		std::exception ex("File not found");
//	}
//	std::transform(input_file_name.begin(), input_file_name.end(), input_file_name.begin(),
//		[](unsigned char c) { return std::tolower(c); });
//	if (input_file_name.find(".bmp") != std::string::npos) {
//		char* tmp = new char[54];
//		in.read(tmp, 54);
//		out.write(tmp, 54);
//	}
//	//char* vec = bytes_from_long(vec_init);
//	//char* buf = new char[8];
//	//char* second_buf = new char[8];
//	//size_t size = fread(second_buf, 1, 8, in);
//	//if (size == 0)
//	//	return;
//	//char* tmp;
//	//while (true) {
//	//	if (CBC) {
//	//		buf = this->decrypting_block_XOR(second_buf, vec);
//	//		copy_bytes(vec, second_buf);
//	//	}
//	//	else
//	//		buf = this->decrypting_block_XOR(second_buf);
//	//	//buf = this->decrypting_block_CBC(second_buf, vec);
//	//	//copy_bytes(vec, second_buf);
//	//	size = fread(second_buf, 1, 8, in);
//	//	if (size == 0) {
//	//		int i = 7;
//	//		for (i; i >= 0 && buf[i] == -128; i--);
//	//		fwrite(buf, 1, i + 1, out);
//	//		break;
//	//	}
//	//	fwrite(buf, 1, 8, out);
//	//}
//	//fclose(in);
//	//fclose(out);
//	uint64_t buf = 0;
//	uint64_t second_buf;
//	in.read(reinterpret_cast<char*>(&second_buf), 8);
//	short size;
//	if ((size = in.gcount()) == 0)
//		return;
//	while (true) {
//		if (CBC) {
//			buf = this->decrypting_block_XOR(second_buf, vec_init);
//			vec_init = second_buf;
//		}
//		else
//			buf = this->decrypting_block_XOR(second_buf);
//		in.read(reinterpret_cast<char*>(&second_buf), 8);
//		if ((size = in.gcount()) == 0) {
//			int i = 8;
//			for (i; i > 0 && buf % 255 == 0; i--)
//				buf >>= 8;
//			out.write(reinterpret_cast<char*>(&buf), i); //возможно обрезание младших битов, лечится buf <<= (8 - i) * 8
//			break;
//		}
//		out.write(reinterpret_cast<char*>(&buf), 8);
//	}
//	in.close();
//	out.close();
//}
