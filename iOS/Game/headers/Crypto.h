#pragma once
#include <iostream>
#include <string>
#include <fstream>

enum statuses { encrypted, plaintext };
enum mode { filling, repeating };

class CryptoKey
{
private:
	char *key = new char[33]; // 32 * 8 = 256 bits
	int reps; // number of times that the encryption algorithm is run; depends on key.

public:
	CryptoKey();
	CryptoKey(std::string new_key);

	bool set_key(std::string new_key, mode mod = repeating);
	void update_repetitions();
	
	CryptoKey * increment();
	CryptoKey * decrement();

	char get_char(int i) const;
	int get_repetitions() const;
	std::string get_key() const;

	~CryptoKey();
};

class CryptoFile
{
private:
	// Variables
	std::string filename;
	std::string temp_filename;
	std::string old_filename;
	statuses status;

	// Crypto Functions
	bool encrypt_private(std::string target_name, CryptoKey &key, int run = 0, int limit = 500);
	bool decrypt_private(std::string target_name, CryptoKey &key, std::ios_base::streampos &n_chars, int run = 0, int limit = 500);

public:
	CryptoFile(std::string name, statuses stat);

	bool set_file(std::string name);
	void set_status(statuses stat);

	bool encrypt(std::string target_name, CryptoKey &key);
	bool decrypt(std::string target_name, CryptoKey &key, int run = 0);

	~CryptoFile();
};