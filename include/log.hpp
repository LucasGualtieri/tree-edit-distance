#ifndef LOG_HPP
#define LOG_HPP

#include <cstddef>

struct Log {

    double duration_secs = 0.0;
    int edit_distance = 0;
    size_t T1 = 0, T2 = 0;

    int count = 0; // Contador interno de entradas acumuladas

    Log& operator+=(const Log& other) {

		// Atualiza o contador antes de calcular nova média
		if (count != 0) {
			duration_secs = (duration_secs * count + other.duration_secs) / (count + 1);
		}

		else duration_secs = other.duration_secs;

		count++;

		edit_distance = other.edit_distance;
		T1 = other.T1;
		T2 = other.T2;

		return *this;
	}

	bool operator==(const Log& other) const {
        return duration_secs == other.duration_secs && edit_distance == other.edit_distance;
    }

	bool operator>(const Log& other) const {
        return duration_secs == other.duration_secs && edit_distance == other.edit_distance;
    }

	bool operator<(const Log& other) const {
        return duration_secs == other.duration_secs && edit_distance == other.edit_distance;
    }
};

#endif
