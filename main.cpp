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
	if(isSAT){
		std::cout << "satisfiable." << std::endl;
		for(auto&& kv : *conds.begin())
			if(prop.count(kv.first))
				std::cout << kv.first.name << " = " << kv.second << std::endl;
	}else{
		std::cout << "unsatisfiable." << std::endl;
	}
}
