#include "RS5.h"
#include "My_Chipher.h"
#include "Analysis.h"
#include <time.h>
#include <bitset>

//std::bitset<16> operator+(const std::bitset<16>& l, const std::bitset<16>& r) {
//	std::bitset<16> out(l.to_ulong() + r.to_ulong());
//	printf("%d\n", out.to_ulong());
//	return out;
//}
//
//std::bitset<16> operator-(const std::bitset<16>& l, const std::bitset<16>& r) {
//	std::bitset<16> out(l.to_ulong() - r.to_ulong());
//	return out;
//}

void print(uint64_t n) {
	for (int i = 0; i < 64; i++) {
		std::cout << (n % 2);
		n >>= 1;
	}
	std::cout << std::endl;
}

int main() {
	//RS5 rs5(32, 12, "From fairest creatures we desire increase,\nThat thereby beauty's rose might never die,\nBut as the riper should by time decease,\nHis tender heir might bear his memory :\nBut thou, contracted to thine own bright eyes,\nFeed'st thy light's flame with self - substantial fuel,\nMaking a famine where abundance lies,\nThyself thy foe, to thy sweet self too cruel.\nThou that art now the world's fresh ornament\nAnd only herald to the gaudy spring,\nWithin thine own bud buriest thy content,\nAnd, tender churl, mak'st waste in niggarding:\nPity the world, or else this glutton be,\nTo eat the world's due, by the grave and thee.");
	//uint64_t vec_init = 0xffff; //UINT64_MAX - UINT32_MAX * 3; //UINT64_MAX - UINT32_MAX * 1.5
	//uint32_t beg = clock();
	//rs5.encrypting_file_all("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/cat4k.bmp", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted cat.bmp", true, vec_init);
	//std::cout << clock() - beg << std::endl;
	////system("PAUSE");
	//beg = clock();
	//rs5.decrypting_file_all("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted cat.bmp", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/decrypted cat.bmp", true, vec_init);
	//std::cout << clock() - beg << std::endl;
	////system("PAUSE");

	uint16_t key = 0x1234;
	Analys anal;
	My_Chipher chip(key, 18);
	//uint16_t vec_init = 0xffff; //UINT64_MAX - UINT32_MAX * 3; //UINT64_MAX - UINT32_MAX * 1.5
	//uint32_t beg = clock();
	//chip.encrypting_file_all("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/cat1.bmp", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted cat.bmp", false, vec_init);
	//std::cout << clock() - beg << std::endl;
	////system("PAUSE");
	//beg = clock();
	//chip.decrypting_file_all("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted cat.bmp", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/decrypted cat.bmp", false, vec_init);
	//std::cout << clock() - beg << std::endl;
	//system("PAUSE");

	std::cout << "Encrypted cat: ";
	anal.first_analys("C:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted cat.bmp");
	std::cout << "Original cat: ";
	anal.first_analys("C:/Учёба/Крипта/lab 2 c++/Crypta lab 2/cat.bmp");
	
	
	uint16_t block = 0x5678;
	printf("%x\n", block);
	uint16_t en = chip.encrypting_block(block);
	uint32_t beg = clock();
	anal.brute_force1(block, 16);
	std::cout << clock() - beg << std::endl;
	beg = clock();
	anal.brute_force1(key, 16);
	std::cout << clock() - beg << std::endl;
	printf("%x\n", en);
	uint16_t de = chip.decrypting_block(en);
	printf("%x\n", de);


	//RS5 rs5(32, 12, "This is pain");
	//uint64_t vec_init = UINT64_MAX - UINT32_MAX * 3; //UINT64_MAX - UINT32_MAX * 1.5
	//std::cout << "nature block:\n";
	//rs5.encrypting_file_all("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/in.txt", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/nat.txt", true, vec_init);
	//std::cout << "XOR2 block:\n";
	//rs5.encrypting_file_all("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/in.txt", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/XOR2.txt", true, vec_init, 1);
	//std::cout << "XOR8 block:\n";
	//rs5.encrypting_file_all("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/in.txt", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/XOR8.txt", true, vec_init, 2);

	//std::cout << std::endl << "nature" << std::endl;
	//Analys analys;
	//analys.first_analys("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/nat.txt");
	//std::cout << "XOR2" << std::endl;
	//analys.first_analys("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/XOR2.txt");
	//std::cout << "XOR8" << std::endl;
	//analys.first_analys("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/XOR8.txt");
	//std::cout << std::endl;
	//analys.second_test("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/nat.txt");
	//std::cout << "XOR2" << std::endl;
	//analys.second_test("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/XOR2.txt");
	//std::cout << "XOR8" << std::endl;
	//analys.second_test("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/XOR8.txt");
	//std::cout << std::endl;
	//analys.third_test("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/nat.txt", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/nat_out.txt");
	//std::cout << "XOR2" << std::endl;;
	//analys.third_test("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/XOR2.txt", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/XOR2_out.txt");
	//std::cout << "XOR8" << std::endl;
	//analys.third_test("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/XOR8.txt", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/XOR8_out.txt");

	//uint32_t beg = clock();
	//rs5.encrypting_file_CBC("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/Rammstein-Links 2 3 4.wav", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted Rammstein-Links 2 3 4.wav", vec_init);
	//std::cout << clock() - beg << std::endl;
	////system("PAUSE");
	//beg = clock();
	//rs5.decrypting_file_CBC("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted Rammstein-Links 2 3 4.wav", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/decrypted Rammstein-Links 2 3 4.wav", vec_init);
	//std::cout << clock() - beg << std::endl;
	//system("PAUSE");

	/*uint32_t beg = clock();
	rs5.encrypting_file_CBC("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/input.txt", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted.txt", vec_init);
	std::cout << clock() - beg << std::endl;
	beg = clock();
	rs5.decrypting_file_CBC("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted.txt", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/decrypted.txt", vec_init);
	std::cout << clock() - beg << std::endl;*/

	//uint32_t beg = clock();
	//rs5.encrypting_file_CBC("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/this is cat.bmp", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted this is cat.bmp", vec_init);
	//std::cout << clock() - beg << std::endl;
	////system("PAUSE");
	//beg = clock();
	//rs5.decrypting_file_CBC("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted this is cat.bmp", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/decrypted this is cat.bmp", vec_init);
	//std::cout << clock() - beg << std::endl;
	//system("PAUSE");

	/*uint32_t beg = clock();
	rs5.encrypting_file_CBC("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/cat8k.bmp", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted this is cat.bmp", vec_init);
	std::cout << clock() - beg << std::endl;
	beg = clock();
	rs5.decrypting_file_CBC("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted this is cat.bmp", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/decrypted this is cat.bmp", vec_init);
	std::cout << clock() - beg << std::endl;*/

	//uint32_t beg = clock();
	//rs5.encrypting_file("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/Black.bmp", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted this is cat.bmp");
	//std::cout << clock() - beg << std::endl;
	//system("PAUSE");
	//beg = clock();
	//rs5.decrypting_file("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted this is cat.bmp", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/decrypted this is cat.bmp");
	//std::cout << clock() - beg << std::endl;
	//rs5.encrypting_file("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/input.txt", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted.txt");
	//rs5.decrypting_file("D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/encrypted.txt", "D:/Учёба/Крипта/lab 2 c++/Crypta lab 2/decrypted.txt");
	//std::string st(rs5.decrypting_block(rs5.encrypting_block((char*)"aaaaaaaa")));
	//std::cout << st;
	//std::cout << (uint32_t)(0 - 10);
	//rs5.decrypting_block(rs5.encrypting_block((char*)"hellogay"));

	//char* tmp = rs5.bytes_from_two_int(520, 256);
	//for (int i = 0; i < 8; i++) {
	//	std::cout << (int)tmp[i] << std::endl;
	//}
	//std::cout << rs5.int_from_bytes(tmp) << std::endl << rs5.int_from_bytes(tmp + 4) << std::endl;
}