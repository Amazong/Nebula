#include "headers/Crypto.h"


// ------------------------- CRYPTO KEY -------------------------
CryptoKey::CryptoKey()
{

}

CryptoKey::CryptoKey(std::string new_key)
{
	set_key(new_key);
}

bool CryptoKey::set_key(std::string new_key, mode mod)
{
	if (new_key.empty()) {
		strcpy_s(key, 33, "00000000000000000000000000000000");
		return false;
	}

	int length = new_key.length();
		
	if (mod == repeating) {
		for (int i = 0; i < 32; i++) {
			key[i] = new_key.at(i % length);
		}
	}
	else {
		int i;
		for (i = 0; i < length && i < 32; i++) {
			key[i] = new_key.at(i);
		}
		for (; i < 32; i++) {
			key[i] = 0;
		}
	}
	
	key[32] = '\0';
	return true;
}

char CryptoKey::get_char(int i) const
{
	return key[i];
}

std::string CryptoKey::get_key()
{
	std::string key;

	for (int i = 0; i < 32; i++) {
		key += this->key[i];
	}

	return key;
}

CryptoKey::~CryptoKey()
{
	delete[] key;
}




// ------------------------- CRYPTO FILE ------------------------- 

CryptoFile::CryptoFile(std::string name, statuses stat)
{
	set_file(name);
	set_status(stat);
}

bool CryptoFile::set_file(std::string name)
{
	filename = name;
	return true;
}

void CryptoFile::set_status(statuses stat)
{
	status = stat;
}

bool CryptoFile::encrypt(std::string target_name, const CryptoKey &key)
{
	if (status != 1) {
		std::cerr << "File is already encrypted!";
		return false;
	}

	std::ifstream in;
	std::ofstream out;

	in.open(filename, std::ofstream::binary);
	if (!in.is_open()) return false;

	out.open(target_name, std::ofstream::binary | std::ofstream::trunc);

	int i = 0;
	while (!in.eof()) {
		out << (char)(in.get() ^ key.get_char(i % 32));
		i++;
		in.peek();
	}

	out << std::endl; // all files that have been properly output will end with an empty line

	in.close();
	out.close();

	std::remove(filename.c_str());

	return true;
}

bool CryptoFile::decrypt(std::string target_name, const CryptoKey &key)
{
	if (status != 0) {
		std::cerr << "File is already decrypted!";
		return false;
	}

	std::ifstream in;
	std::ofstream out;

	in.open(filename, std::ofstream::binary);
	if (!in.is_open()) return false;

	out.open(target_name, std::ofstream::binary | std::ofstream::trunc);

	int i = 0;
	
	in.seekg(0, in.end);
	int end = (int)(in.tellg()) - 1;
	in.seekg(0, in.beg);
	
	while (i < end) {
		out << (char)(in.get() ^ key.get_char(i % 32));
		i++;
	}

	in.close();
	out.close();
	
	return true;
}


CryptoFile::~CryptoFile()
{
}

