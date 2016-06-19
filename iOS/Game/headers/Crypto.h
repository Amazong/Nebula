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
public:
	CryptoKey();
	CryptoKey(std::string new_key);

	bool set_key(std::string new_key, mode mod = repeating);
	char get_char(int i) const;
	std::string get_key();

	~CryptoKey();
};

class CryptoFile
{
private:
	std::string filename;
	statuses status;

public:
	CryptoFile(std::string name, statuses stat);

	bool set_file(std::string name);
	void set_status(statuses stat);

	bool encrypt(std::string target_name, const CryptoKey &key);
	bool decrypt(std::string target_name, const CryptoKey &key);

	~CryptoFile();
};