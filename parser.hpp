#pragma once
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <set>
#include <stdexcept>
#include "Rustic_SAT_Solver/data.hpp"

// 出現した命題変数の集合, CNF
//using cnf_t = std::tuple<std::set<std::string>, std::vector<std::vector<std::string>>>;
using cnf_t = std::pair<std::set<Proposition>, ClauseSet>;

// エントリーポイント
cnf_t parse(const std::string& s);

