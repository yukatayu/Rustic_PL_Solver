#include <iostream>
#include "Rustic_SAT_Solver/data.hpp"
#include "Rustic_SAT_Solver/solver.hpp"
#include "parser.hpp"

int main(){
	// std::set<Proposition>, ClauseSet
	auto [prop, CNF] = parse("!((a&(b))|!(c|d&e&!f))");
	// solve
	auto [isSAT, conds] = solve(CNF);
	// print
	std::cout << (isSAT ? "This is SAT" : "This is not SAT") << std::endl;
	for(auto&& cond : conds){
		std::cout << "----------" << std::endl;
		for(auto&& kv : cond)
			std::cout << kv.first.name << " = " << kv.second << std::endl;
	}
}
