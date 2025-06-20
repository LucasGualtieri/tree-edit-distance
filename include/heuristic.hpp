#include <vector>

#include "log.hpp"
#include "../external/cpp-datastructures/include/utils/timer.hpp"
#include "../external/cpp-datastructures/include/graph/graph.hpp"

inline int levenshteinDistance(const std::string& str1, const std::string& str2) {

	int n = str1.size(), m = str2.size();
	std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1));

	for (int i = 0; i < n + 1; i++) dp[i][0] = i;

	for (int j = 0; j < m + 1; j++) dp[0][j] = j;

	for (int i = 1; i < n + 1; i++) {
		for (int j = 1; j < m + 1; j++) {

			int cost = (str1[i-1] == str2[j-1]) ? 0 : 1;

			dp[i][j] = std::min({
				dp[i-1][j] + 1,
				dp[i][j-1] + 1,
				dp[i-1][j-1] + cost
			});
		}
	}

	return dp[n][m];
}

inline void linearizeTreeDFS(const Graph& T, int u, std::string& s) {

	s += "(" + std::to_string(u);

	for (int v : T.neighbors(u)) {
		linearizeTreeDFS(T, v, s);
	}

	s += ")";
}

inline std::string linearizeTree(const Graph& T) {

	std::string s;
	linearizeTreeDFS(T, 0, s);
	return s;
} 

inline Log Heuristic(const Graph& T1, const Graph& T2) {

	Timer timer;
	timer.start();

	std::string str1 = linearizeTree(T1);
	std::string str2 = linearizeTree(T2);
	int distance = levenshteinDistance(str1, str2);

	timer.stop();

	return {
		.duration_secs = timer.result(),
		.edit_distance = distance,
		.T1 = T1.n,
		.T2 = T2.n,
	};
}
