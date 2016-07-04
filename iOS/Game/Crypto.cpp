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
	reps = 999;
	return;
	//

	int reps = 1;
	for (int i = 0; i < 32; i++) {

		if (key[i] == '0') continue;
		reps *= (int)(key[i]); // alternately multiply and add
		i++;
		reps += (int)(key[i]);

		reps %= 65536; // reps will be between 0-65536
		
		if (reps == 0) reps = 1; // modulo op can return 0
								 // need to safeguard against that
	}
	this->reps = reps + 1000; // always an added bonus, to ensure at least 1000, but in random number
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

bool CryptoFile::encrypt(std::string target_name, CryptoKey & key)
{
	if (status != statuses::plaintext) {
		std::cerr << "File is already encrypted!";
		return false;
	}

	// number of reps will always be >1000

	encrypt_private(("_" + filename), key); // first 500

	for (int i = 1; i < key.get_repetitions() / 500; i++) {
		encrypt_private(target_name, key);
	}
	
	if (key.get_repetitions() % 500 != 0) {
		encrypt_private(target_name, key, 0, key.get_repetitions() % 500);
	}

	set_status(encrypted);

	return true;
}

bool CryptoFile::encrypt_private(std::string target_name, CryptoKey &key, int run, int limit)
{
	std::ifstream * in = new std::ifstream;
	std::ofstream * out = new std::ofstream;

	in->open(filename, std::ifstream::binary);
	if (!in->is_open()) return false;

	if (run == 0) {
		old_filename = filename;
		filename = target_name;
		temp_filename = "_" + filename;
	}

	out->open(temp_filename, std::ofstream::binary | std::ofstream::trunc);

	int *i = (int*)calloc(1,sizeof(int));
	while (!in->eof()) {
		(*out) << (char)(in->get() ^ key.get_char(*i % 32));
		(*i)++;
		in->peek();
	}

	if (run == limit) {
		(*out) << std::endl; // all files that have been properly output will end with an empty line
		
		in->close();
		out->close();
		
		std::remove(filename.c_str());
		rename(temp_filename.c_str(), filename.c_str()); // overwrite previous version with final

		if (old_filename != filename) { // if they are the same, the rename operation has already solved it
			std::remove(old_filename.c_str());
		}

		return true;
	}

	in->close();
	out->close();
	
	delete in; // more stable memory management, minimum overhead
	delete out;

	rename(temp_filename.c_str(), filename.c_str());
	
	for (*i = 0; *i < 10; (*i)++) key.increment();

	delete i;
	
	if (encrypt_private(temp_filename, key, run + 1, limit)) return true; // optimise to make use of dynamic memory, to prevent overflow, if we have time
	return false;
}

bool CryptoFile::decrypt(std::string target_name, CryptoKey &key, int run)
{
	if (status != statuses::encrypted) {
		std::cerr << "File is already decrypted!";
		return false;
	}

	std::ifstream *in = new std::ifstream(filename);

	in->seekg(0, std::ios_base::end);
	std::ios_base::streampos end_pos = in->tellg();
	end_pos -= 2;
	delete in;

	decrypt_private(("_" + filename), key, end_pos); // first 500

	for (int i = 1; i < key.get_repetitions() / 500; i++) {
		decrypt_private(target_name, key, end_pos);
	}

	if (key.get_repetitions() % 500 != 0) {
		decrypt_private(target_name, key, end_pos, 0, key.get_repetitions() % 500);
	}

	set_status(plaintext);

	return true;
}

bool CryptoFile::decrypt_private(std::string target_name, CryptoKey &key, std::ios_base::streampos &n_chars, int run, int limit)
{
	std::ifstream * in = new std::ifstream;
	std::ofstream * out = new std::ofstream;

	in->open(filename, std::ifstream::binary);
	if (!in->is_open()) return false;

	if (run == 0) {
		old_filename = filename;
		filename = target_name;
		temp_filename = "_" + filename;
	}

	out->open(temp_filename, std::ofstream::binary | std::ofstream::trunc);

	int *i = (int*)calloc(1, sizeof(int)); // allocated once

	while (*i < n_chars) { // iterator relative to streampos
		(*out) << (char)(in->get() ^ key.get_char((*i) % 32));

		(*i)++;
	}
	
	in->close();
	out->close();

	delete in;
	delete out;

	if (run == limit) {
		std::remove(filename.c_str());
		rename(temp_filename.c_str(), filename.c_str());

		if (old_filename != filename) { // if they are the same, the rename operation has already solved it
			std::remove(old_filename.c_str());
		}

		return true;
	}

	rename(temp_filename.c_str(), filename.c_str());

	for (*i = 0; *i < 10; (*i)++) key.decrement(); // reuse variable

	delete i; // extend recursion lifetime

	if (decrypt_private(temp_filename, key, n_chars, run + 1, limit)) return true;
	return false;
}

CryptoFile::~CryptoFile()
{
}

