#include "user.hpp"

#include "stdio.h"

void saveUser(const userFull user) {
	auto file = fopen("./users.dat", "a+");
	if (!file) {
		perror("File opening failed");
	}
}