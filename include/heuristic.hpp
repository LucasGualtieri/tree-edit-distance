#include <vector>
#include <algorithm>
#include <map>
#include <utility>

#include "log.hpp"
#include "../cpp-datastructures/utils/timer.hpp"
#include "../cpp-datastructures/graph/graph.hpp"

// Compute subtree sizes
inline void computeSubtreeSizes(const Graph& T, int u, std::vector<int>& sz) {
	sz[u] = 1;
	for (int v : T.neighbors(u)) {
		computeSubtreeSizes(T, v, sz);
		sz[u] += sz[v];
	}
}

// Approximate Tree Edit Distance (APTED) recursive with memo
inline int aptedRec(const Graph& T1, const Graph& T2, int u, int v, const std::vector<std::vector<int>>& ch1, const std::vector<std::vector<int>>& ch2, const std::vector<int>& sz1, const std::vector<int>& sz2, std::map<std::pair<int,int>,int>& memo) {

	auto key = std::make_pair(u, v);

	if (memo.count(key)) return memo[key];

	const auto& U = ch1[u];
	const auto& V = ch2[v];

	int m = U.size(), n = V.size();

	std::vector<std::vector<int>> d(m + 1, std::vector<int>(n + 1, 0));

	// base cases
	for (int i = 1; i <= m; ++i) d[i][0] = d[i-1][0] + sz1[U[i-1]];
	for (int j = 1; j <= n; ++j) d[0][j] = d[0][j-1] + sz2[V[j-1]];

	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= n; ++j) {

			int cost_sub = aptedRec(T1, T2, U[i-1], V[j-1], ch1, ch2, sz1, sz2, memo);

			d[i][j] = std::min({
				d[i - 1][j]   + sz1[U[i - 1]],  // delete u_i
				d[i][j - 1]   + sz2[V[j - 1]],  // insert v_j
				d[i - 1][j - 1] + cost_sub      // match
			});
		}
	}

	int labelCost = (T1.getLabel(u) == T2.getLabel(v)) ? 0 : 1;

	return memo[key] = d[m][n] + labelCost;
}

inline Log Heuristic(const Graph& T1, const Graph& T2) {
	Timer timer; timer.start();
	int n1 = static_cast<int>(T1.n), n2 = static_cast<int>(T2.n);
	// build children lists
	std::vector<std::vector<int>> ch1(n1), ch2(n2);
	for (int u = 0; u < n1; ++u) {
		for (int v : T1.neighbors(u)) ch1[u].push_back(v);
	}
	for (int u = 0; u < n2; ++u) {
		for (int v : T2.neighbors(u)) ch2[u].push_back(v);
	}
	// subtree sizes
	std::vector<int> sz1(n1), sz2(n2);
	computeSubtreeSizes(T1, 0, sz1);
	computeSubtreeSizes(T2, 0, sz2);
	// recursive APTED
	std::map<std::pair<int,int>,int> memo;
	int dist = aptedRec(T1, T2, 0, 0, ch1, ch2, sz1, sz2, memo);
	timer.stop();
	return { .duration_secs = timer.result(), .edit_distance = dist, .T1 = T1.n, .T2 = T2.n };
}
