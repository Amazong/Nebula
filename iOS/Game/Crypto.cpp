#include "headers/Crypto.h"

// ------------------------- CRYPTO KEY -------------------------

CryptoKey::CryptoKey()
{
	set_key("");
}

CryptoKey::CryptoKey(const CryptoKey & k)
{
	*this = CryptoKey(k.get_key());
}

CryptoKey::CryptoKey(std::string new_key)
{
	set_key(new_key);
}

bool CryptoKey::set_key(std::string new_key, mode mod, char repeater)
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
			key[i] = repeater;
		}
	}

	key[32] = '\0';

	update_repetitions();

	return true;
}

void CryptoKey::update_repetitions()
{
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
	return key[i % 32];
}

int CryptoKey::get_repetitions() const
{
	return reps;
}

CryptoKey CryptoKey::get_decrypt_key() const
{
	CryptoKey k(*this);
	k.update_repetitions();

	for (int i = 0; i < reps - 1; i++) {
		k++;
	}

	return k;
}

std::string CryptoKey::get_key() const
{
	std::string key;

	for (int i = 0; i < 32; i++) {
		key += this->key[i];
	}

	return key;
}

CryptoKey * CryptoKey::operator=(CryptoKey k)
{
	set_key(k.get_key());

	return this;
}

CryptoKey * CryptoKey::operator++(int)
{
	return increment();
}

CryptoKey * CryptoKey::operator--(int)
{
	return decrement();
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
	std::ifstream fin(name);

	if (fin.fail()) {
		filename = "";
		return false;
	}

	fin.close();

	filename = name;

	return true;
}

void CryptoFile::set_status(statuses stat)
{
	status = stat;
}

int CryptoFile::encrypt(std::string target_name, const CryptoKey & key, bool overwrite)
{
	// return values:
	// 0 - all OK
	// -1 - couldn't open target without overwriting
	// -2 - couldn't open source
	// -3 - file can't be encrypted, because it is not plaintext

	if (status != statuses::plaintext) return -3;

	int length, j, i;
	length = j = i = 0;

	std::ifstream fin(target_name);

	if (!fin.fail() && !overwrite) { // file exists and we do not want to overwrite
		do {
			i++;
			if (i == 10) break;
			fin.close();
			fin.open(target_name + "_" + std::to_string(i));
		} while (!fin.fail());

		if (i == 10) return -1;

		fin.close();

		return (encrypt(target_name + "_" + std::to_string(i), key));
	}

	std::ifstream source(filename, std::fstream::binary);
	if (source.fail()) return -2;

	char * chunk = new char[32000];

	// get message content to RAM
	for (length = 0; !source.eof() && length < 32000; length++) {
		chunk[length] = source.get();
	}
	length--; // to compensate for last increment

	CryptoKey k(key);
	int n_runs = k.get_repetitions();

	// encrypt chunk
	for (i = 0; i < n_runs; i++, k++) {
		for (j = 0; j < length; j++) {
			chunk[j] ^= k.get_char(j);
		}
	}

	std::ofstream target(target_name, std::fstream::trunc | std::fstream::binary);

	// output encrypted content
	for (i = 0; i < length; i++) {
		target << chunk[i];
	}

	target.close();

	while (!source.eof()) { // repeat until file is done
		target.open(target_name, std::fstream::app); // now we append to whatever exists there
		k = key;

		// get message content to RAM
		for (length = 0; !source.eof() && length < 32000; length++) {
			chunk[i] = source.get();
		}
		length--; // to compensate for last increment

				  // encrypt chunk
		for (i = 0; i < n_runs; i++, k++) {
			for (j = 0; j < length; j++) {
				chunk[j] ^= k.get_char(j);
			}
		}

		// output encrypted content
		for (i = 0; i < length; i++) {
			target << chunk[i];
		}
	}

	if (target.is_open()) target.close();
	source.close();

	delete[] chunk;

	std::remove(filename.c_str());

	return 0;
}

int CryptoFile::decrypt(std::string target_name, const CryptoKey & key, bool overwrite)
{
	// return values:
	// 0 - all OK
	// -1 - couldn't open target without overwriting
	// -2 - couldn't open source
	// -3 - file can't be encrypted, because it is not plaintext

	if (status != statuses::encrypted) return -3;

	int length, j, i;
	length = j = i = 0;

	std::ifstream fin(target_name);

	if (!fin.fail() && !overwrite) { // file exists and we do not want to overwrite
		do {
			i++;
			if (i == 10) break;
			fin.close();
			fin.open(target_name + "_" + std::to_string(i));
		} while (!fin.fail());

		if (i == 10) return -1;

		fin.close();

		return (encrypt(target_name + "_" + std::to_string(i), key));
	}

	std::ifstream source(filename, std::fstream::binary);
	if (source.fail()) return -2;

	char * chunk = new char[32000];

	// get message content to RAM
	for (length = 0; !source.eof() && length < 32000; length++) {
		chunk[length] = source.get();
	}
	length--; // to compensate for last increment

	CryptoKey k(key.get_decrypt_key());
	int n_runs = key.get_repetitions();

	// decrypt chunk
	for (i = 0; i < n_runs; i++, k--) {
		for (j = 0; j < length; j++) {
			chunk[j] ^= k.get_char(j);
		}
	}

	std::ofstream target(target_name, std::fstream::trunc | std::fstream::binary);

	// output encrypted content
	for (i = 0; i < length; i++) {
		target << (char)chunk[i];
	}

	target.close();

	while (!source.eof()) { // repeat until file is done
		target.open(target_name, std::fstream::app); // now we append to whatever exists there
		k = key;

		// get message content to RAM
		for (length = 0; !source.eof() && length < 32000; length++) {
			chunk[i] = source.get();
		}
		length--; // to compensate for last increment

		// decrypt chunk
		for (i = 0; i < n_runs; i++, k++) {
			for (j = 0; j < length; j++) {
				chunk[j] ^= k.get_char(j);
			}
		}

		// output encrypted content
		for (i = 0; i < length; i++) {
			target << chunk[i];
		}
	}

	if (target.is_open()) target.close();
	source.close();

	delete[] chunk;

	std::remove(filename.c_str());

	return 0;
}

CryptoFile::~CryptoFile()
{
}