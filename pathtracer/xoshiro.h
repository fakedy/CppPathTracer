#pragma once

class Xoshiro {

public:

	Xoshiro() {
		seed_xoshiro256p();
	}

	void seed_xoshiro256p();
	double random_double(double lower, double upper);

private:
	uint64_t s[4];
	inline uint64_t rotl(const uint64_t x, int k);
	uint64_t next(void);

	void jump(void);

	void long_jump(void);


};
