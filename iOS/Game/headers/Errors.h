#pragma once

enum ErrNo
{
	corrupt_profile = 10,
	file_access = 42,
	profile_not_found = 404,
};

namespace error {
	int trace_error(int code);

	void corrupt_profile();
	void file_access();
	void profile_not_found();
}
