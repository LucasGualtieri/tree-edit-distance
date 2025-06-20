#ifndef LOG_HPP
#define LOG_HPP

#include <cstddef>
struct Log {

    double duration_secs = 0;
    int edit_distance = 0;
	size_t T1, T2;

    Log& operator+=(const Log& other) {
        duration_secs += other.duration_secs;
        edit_distance += other.edit_distance;
		T1 = other.T1;
		T2 = other.T2;
        return *this;
    }

	void avg(const int& numberOfRepetions) {
		duration_secs /= numberOfRepetions;
		edit_distance /= numberOfRepetions;
	}

    bool operator==(const Log& other) {
		return this->duration_secs == other.duration_secs && this->edit_distance == other.edit_distance;
    }
};

#endif
