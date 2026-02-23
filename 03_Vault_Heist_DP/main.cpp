#ifndef __PROGTEST__
#include <cassert>
#include <iomanip>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <functional>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>
#include <type_traits>
#include <compare>

struct Vault {
  unsigned value;
  unsigned pebbles;
  std::vector<std::pair<unsigned, unsigned>> missing_connections;
};

struct UnlockingSequence {
  unsigned vault_id;
  std::vector<bool> moved_pebbles;
};

#endif

struct ValidVault {
    unsigned id;
    unsigned reward;
    unsigned time_cost;
    std::vector<bool> bits;
};

std::vector<UnlockingSequence> plan_heist(
  const std::vector<Vault>& vaults,
  unsigned transition_time,
  unsigned max_time
) {
    std::vector<ValidVault> eligible;

    for (unsigned v_idx = 0; v_idx < vaults.size(); ++v_idx) {
        if (vaults[v_idx].pebbles % 2 != 0) continue;
        int n_pebs = vaults[v_idx].pebbles;
        unsigned req = n_pebs / 2;

        std::vector<std::vector<int>> adj_list(n_pebs);
        for (auto& conn : vaults[v_idx].missing_connections) {
            adj_list[conn.first].push_back(conn.second);
            adj_list[conn.second].push_back(conn.first);
        }

        std::vector<std::vector<int>> clusters;
        std::vector<bool> checked(n_pebs, false);
        for (int p_id = 0; p_id < n_pebs; ++p_id) {
            if (!checked[p_id]) {
                std::vector<int> cluster;
                std::vector<int> queue = {p_id};
                checked[p_id] = true;
                unsigned q_ptr = 0;
                while(q_ptr < queue.size()){
                    int src = queue[q_ptr++];
                    cluster.push_back(src);
                    for(int dst : adj_list[src]){
                        if(!checked[dst]){
                            checked[dst] = true;
                            queue.push_back(dst);
                        }
                    }
                }
                clusters.push_back(cluster);
            }
        }

        std::vector<int> ss_dp(req + 1, -1);
        ss_dp[0] = -2; 
        for (int c_idx = 0; c_idx < (int)clusters.size(); ++c_idx) {
            int c_size = clusters[c_idx].size();
            for (int s_val = req; s_val >= c_size; --s_val) {
                if (ss_dp[s_val] == -1 && ss_dp[s_val - c_size] != -1) {
                    ss_dp[s_val] = c_idx;
                }
            }
        }

        if (ss_dp[req] != -1) {
            std::vector<bool> config(n_pebs, false);
            int s_back = req;
            while (s_back > 0) {
                int back_idx = ss_dp[s_back];
                for (int node : clusters[back_idx]) config[node] = true;
                s_back -= clusters[back_idx].size();
            }
            eligible.push_back({v_idx, (unsigned)vaults[v_idx].value, req, config});
        }
    }

    int n_eligible = eligible.size();
    int full_limit = max_time + transition_time;

    std::vector<std::vector<unsigned>> ks_table(n_eligible + 1, std::vector<unsigned>(full_limit + 1, 0));

    for (int row = 1; row <= n_eligible; ++row) {
        int vault_weight = eligible[row-1].time_cost + transition_time;
        for (int col = 0; col <= full_limit; ++col) {
            ks_table[row][col] = ks_table[row-1][col];
            if (col >= vault_weight) {
                ks_table[row][col] = std::max(ks_table[row][col], 
                                              ks_table[row-1][col - vault_weight] + eligible[row-1].reward);
            }
        }
    }

    std::vector<UnlockingSequence> output;
    int rem_cap = full_limit;
    if (ks_table[n_eligible][full_limit] > 0) {
        for (int row = n_eligible; row > 0; --row) {
            int vault_weight = eligible[row-1].time_cost + transition_time;
            if (rem_cap >= vault_weight && ks_table[row][rem_cap] != ks_table[row-1][rem_cap]) {
                output.push_back({eligible[row-1].id, eligible[row-1].bits});
                rem_cap -= vault_weight;
            }
        }
    }

    return output;
}

#ifndef __PROGTEST__

struct TestFailed : std::runtime_error {
  using std::runtime_error::runtime_error;
};

#define CHECK(cond, msg) do { \
    if (!(cond)) throw TestFailed(msg); \
  } while (0)

void check_unlocking_sequence(
  unsigned pebbles,
  const std::vector<std::pair<unsigned, unsigned>>& missing_connections,
  const std::vector<bool>& moved
) {
  CHECK(moved.size() == pebbles, "Solution has wrong size.\n");

  size_t moved_cnt = 0;
  for (bool p : moved) moved_cnt += p;
  CHECK(2*moved_cnt == pebbles,
    "Exactly half of the pebbles must be moved.\n");

  for (auto [ u, v ] : missing_connections) CHECK(moved[u] == moved[v],
    "Pebble not connected with all on other side.\n");
}

void check_solution(
  const std::vector<UnlockingSequence>& solution,
  unsigned expected_value,
  const std::vector<Vault>& vaults,
  unsigned transition_time,
  unsigned max_time
) {
  unsigned time = 0, value = 0;
  std::vector<bool> robbed(vaults.size(), false);

  for (size_t i = 0; i < solution.size(); i++) {
    const auto& [ id, moved ] = solution[i];

    CHECK(id < vaults.size(), "Id is out of range.\n");
    CHECK(!robbed[id], "Robbed same vault twice.\n");
    robbed[id] = true;
    
    const auto& vault = vaults[id];
    value += vault.value;

    if (i != 0) time += transition_time;
    time += vault.pebbles / 2;
    CHECK(time <= max_time, "Run out of time.\n");

    check_unlocking_sequence(vault.pebbles, vault.missing_connections, moved);
  }

  CHECK(value == expected_value, "Total value mismatch.\n");
}


struct Test {
  unsigned expected_value;
  unsigned max_time;
  unsigned transition_time;
  std::vector<Vault> vaults;
};

inline const std::vector<Test> TESTS = {
  Test{
    .expected_value = 3010, .max_time = 3, .transition_time = 8,
    .vaults = {
      { .value = 3010, .pebbles = 6, .missing_connections = { {3,4}, {0,1}, {4,5}, {5,3}, } },
      { .value = 3072, .pebbles = 6, .missing_connections = { {2,1}, {1,3}, {0,1}, {0,3}, {4,5}, {2,3}, } },
      { .value = 5069, .pebbles = 10, .missing_connections = { {7,2}, {3,4}, {0,1}, {8,4}, {1,2}, {8,3}, {7,0}, {5,6}, {9,5}, {9,6}, } },
      { .value = 2061, .pebbles = 4, .missing_connections = { {3,0}, {2,1}, {0,2}, {1,3}, } },
    }
  },
  Test{
    .expected_value = 6208, .max_time = 13, .transition_time = 12,
    .vaults = {
      { .value = 6011, .pebbles = 12, .missing_connections = { {1,5}, {2,4}, {5,10}, {1,10}, {0,3}, {8,3}, {8,0}, {9,8}, {2,6}, {3,9}, {0,9}, {4,6}, {11,7}, } },
      { .value = 2056, .pebbles = 4, .missing_connections = { {1,0}, {2,0}, {2,1}, } },
      { .value = 5885, .pebbles = 12, .missing_connections = { {1,6}, {3,7}, {1,0}, {2,9}, {9,8}, {2,8}, {5,7}, {11,4}, {10,1}, {5,3}, {0,10}, } },
      { .value = 5818, .pebbles = 12, .missing_connections = { {9,0}, {7,1}, {6,4}, {8,6}, {4,2}, {11,5}, {5,3}, {9,7}, {8,4}, {2,8}, {10,11}, {5,10}, {10,3}, {9,1}, } },
      { .value = 4880, .pebbles = 10, .missing_connections = { {7,3}, {4,1}, {9,2}, {6,9}, {2,6}, {5,0}, {8,4}, } },
      { .value = 5233, .pebbles = 10, .missing_connections = { {0,2}, {4,5}, {8,3}, {9,7}, {7,1}, {6,3}, {6,8}, } },
      { .value = 6208, .pebbles = 12, .missing_connections = { {1,7}, {3,4}, {10,7}, {0,3}, {8,2}, {5,1}, {9,11}, {0,6}, {6,3}, {10,1}, {0,4}, } },
      { .value = 4182, .pebbles = 8, .missing_connections = { {5,7}, {7,4}, {4,5}, {1,0}, {5,6}, {3,1}, {6,4}, {0,3}, {6,7}, } },
    }
  },
  Test{
    .expected_value = 1, .max_time = 100, .transition_time = 8,
    .vaults = {
      { .value = 1, .pebbles = 14, .missing_connections = {
        {0,1}, {0,2}, {3,4}, {4,5}, {6,7}, {8,9}, {10,11}, {12,13},
      } },
    }
  },
};

int main() {
  int ok = 0, fail = 0;

  for (auto t : TESTS) {
    try {
      auto sol = plan_heist(t.vaults, t.transition_time, t.max_time);
      check_solution(sol, t.expected_value, t.vaults, t.transition_time, t.max_time);
      ok++;
    } catch (const TestFailed&) {
      fail++;
    }
  }

  if (!fail) std::cout << "Passed all " << ok << " tests!" << std::endl;
  else std::cout << "Failed " << fail << " of " << (ok + fail) << " tests." << std::endl;
}

#endif


