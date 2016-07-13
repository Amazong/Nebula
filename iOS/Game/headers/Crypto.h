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
	int reps = 0; // number of times that the encryption algorithm is run; depends on key.

	CryptoKey * increment();
	CryptoKey * decrement();

	CryptoKey * operator ++ (int);
	CryptoKey * operator -- (int);

public:
	CryptoKey();
	CryptoKey(const CryptoKey &k);
	CryptoKey(std::string new_key, mode mod = repeating, char repeater = '0');
	~CryptoKey();

	bool set_key(std::string new_key, mode mod = repeating, char repeater = '0');
	void update_repetitions();

	char get_char(int i) const;
	int get_repetitions() const;

	std::string get_key() const;

	CryptoKey get_decrypt_key() const;

	CryptoKey * operator = (CryptoKey k);

	friend class CryptoFile;
};

class CryptoFile
{
private:
	// Variables
	std::string filename;
	statuses status;

public:
	CryptoFile(std::string name, statuses stat);

	bool set_file(std::string name);
	void set_status(statuses stat);

	int encrypt(std::string target_name, const CryptoKey &key, bool overwrite = 0);
	int decrypt(std::string target_name, const CryptoKey &key, bool overwrite = 0);

	~CryptoFile();
};