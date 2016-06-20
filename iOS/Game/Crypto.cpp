#include "headers/Crypto.h"


// ------------------------- CRYPTO KEY -------------------------
CryptoKey::CryptoKey()
{
	set_key("");
	update_repetitions();
}

CryptoKey::CryptoKey(std::string new_key)
{
	set_key(new_key);
	update_repetitions();
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

	update_repetitions();

	return true;
}

void CryptoKey::update_repetitions()
{
	reps = 1000;
	return;
	//

	int reps = 1;
	for (int i = 0; i < 32; i++) {
		if (key[i] == '\0') continue;
		if (i % 2 == 0) { // alternately multiply and add
			reps *= (int)(key[i]);
		}
		else {
			reps += (int)(key[i]);
		}
		reps %= 65536; // reps will be between 0-65536 - 2^16, because 2^16 * 2^8 = 2^32 (int)
	}
	this->reps = reps + 100; // always an added bonus, to ensure at least 10000, but in random number
}

CryptoKey * CryptoKey::increment()
{
	for (int i = 0; i < 32; i++) {
		key[i] += (i % 10);
	}

	return this;
}

CryptoKey * CryptoKey::decrement()
{
	for (int i = 0; i < 32; i++) {
		key[i] -= (i % 10);
	}

	return this;
}

char CryptoKey::get_char(int i) const
{
	return key[i];
}

int CryptoKey::get_repetitions() const
{
	return reps;
}

std::string CryptoKey::get_key() const
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

bool CryptoFile::encrypt(std::string target_name, CryptoKey &key, int run)
{
	if (status != statuses::plaintext) {
		std::cerr << "File is already encrypted!";
		return false;
	}

	std::ifstream in;
	std::ofstream out;

	in.open(filename, std::ifstream::binary);
	if (!in.is_open()) return false;

	if (run == 0) {
		old_filename = filename;
		filename = target_name;
		temp_filename = "_" + filename;
	}

	out.open(temp_filename, std::ofstream::binary | std::ofstream::trunc);

	int i = 0;
	while (!in.eof()) {
		out << (char)(in.get() ^ key.get_char(i % 32));
		i++;
		in.peek();
	}

	if (run == key.get_repetitions()) {
		out << std::endl; // all files that have been properly output will end with an empty line
		
		in.close();
		out.close();
		
		std::remove(filename.c_str());
		rename(temp_filename.c_str(), filename.c_str());
		std::remove(old_filename.c_str());
		set_status(encrypted);
		return true;
	}

	in.close();
	out.close();
	
	rename(temp_filename.c_str(), filename.c_str());
	
	for (i = 0; i < 10; i++) key.increment();
	
	if (encrypt(temp_filename, key, run + 1)) return true; // optimise to make use of dynamic memory, to prevent overflow, if we have time
	return false;
}

bool CryptoFile::decrypt(std::string target_name, CryptoKey &key, int run)
{
	if (status != statuses::encrypted) {
		std::cerr << "File is already decrypted!";
		return false;
	}

	std::ifstream in;
	std::ofstream out;

	in.open(filename, std::ifstream::binary);
	if (!in.is_open()) return false;

	if (run == 0) {
		old_filename = filename;
		filename = target_name;
		temp_filename = "_" + filename;
	}

	out.open(temp_filename, std::ofstream::binary | std::ofstream::trunc);

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

	if (run == key.get_repetitions()) {
		std::remove(filename.c_str());
		rename(temp_filename.c_str(), filename.c_str());
		std::remove(old_filename.c_str());
		set_status(plaintext);
		return true;
	}

	rename(temp_filename.c_str(), filename.c_str());

	for (i = 0; i < 10; i++) key.decrement();

	if (decrypt(temp_filename, key, run + 1)) return true;
	return false;
}


CryptoFile::~CryptoFile()
{
}

