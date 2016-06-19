#pragma once

enum ErrNo
{
	corrupt_profile = 10,
	file_access = 42
};

namespace error {
	int trace_error(int code);

	void profile_error();
	void file_access();
}
