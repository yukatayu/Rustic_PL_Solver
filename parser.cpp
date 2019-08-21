#include "parser.hpp"

// 出現した命題変数の集合, CNF, 中身を表す変数名, 文字の読み込み位置
using result_t = std::tuple<std::set<Proposition>, std::vector<Clause>, std::string, int>;

// ユーティリティ
std::string getTmpName(){
	static int i = 0;
	return "_" + std::to_string(i++);
}

bool isAlphaNum(char c){
	return ('a' <= c && c <= 'z')
		|| ('A' <= c && c <= 'Z')
		|| ('0' <= c && c <= '9')
		|| c == '_'
	;
}

using P = Proposition;
using L = Literal;

// パーサ実装
result_t atomic(const std::string& s, int p = 0);
result_t literal(const std::string& s, int p = 0);
result_t conjunction(const std::string& s, int p = 0);
result_t disjunction(const std::string& s, int p = 0);
result_t expression(const std::string& s, int p = 0);

result_t atomic(const std::string& s, int p){
	std::string name;
	while(isAlphaNum(s[p]))
		name.push_back(s[p++]);
	return result_t{{P{name}}, {}, name, p};
}

result_t literal(const std::string& s, int p){
	if(s[p] == '!' || s[p] == '-'){
		auto&& [prop, CNF, alias, newPos] = literal(s, p+1);
		auto newName = getTmpName();
		CNF.push_back({{ L{alias,false}, L{newName,false} }});
		CNF.push_back({{ L{alias,true}, L{newName,true}}});
		return result_t{prop, CNF, newName, newPos};
	} else if(s[p] == '(') {
		result_t r = expression(s, p+1);
		std::get<3>(r)++;
		return r;
	} else {
		return atomic(s, p);
	}
}

result_t conjunction(const std::string& s, int p){
	auto[prop0, CNF0, alias0, newPos0] = literal(s, p);
	while(s[newPos0] == '&'){
		auto&& [prop1, CNF1, alias1, newPos1] = literal(s, newPos0 + 1);
		for(auto&& p : prop1) prop0.insert(p);
		for(auto&& cnf : CNF1) CNF0.push_back(cnf);
		auto newAlias = getTmpName();
		// alias0 && alias1 <-> newAlias
		CNF0.push_back({{L{alias0,true}, L{alias1,true}, L{newAlias,false}}});
		CNF0.push_back({{L{newAlias,true}, L{alias0,false}}});
		CNF0.push_back({{L{newAlias,true}, L{alias1,false}}});
		alias0 = newAlias;
		newPos0 = newPos1;
	}
	return result_t{prop0, CNF0, alias0, newPos0};
}

result_t disjunction(const std::string& s, int p){
	auto&& [prop0, CNF0, alias0, newPos0] = conjunction(s, p);
	while(s[newPos0] == '|'){
		auto[prop1, CNF1, alias1, newPos1] = conjunction(s, newPos0 + 1);
		for(auto&& p : prop1) prop0.insert(p);
		for(auto&& cnf : CNF1) CNF0.push_back(cnf);
		auto newAlias = getTmpName();
		// alias0 || alias1 <-> newAlias
		CNF0.push_back({{L{alias0,false}, L{alias1,false}, L{newAlias,true}}});
		CNF0.push_back({{L{newAlias,false}, L{alias0,true}}});
		CNF0.push_back({{L{newAlias,false}, L{alias1,true}}});
		alias0 = newAlias;
		newPos0 = newPos1;
	}
	return result_t{prop0, CNF0, alias0, newPos0};
}

result_t expression(const std::string& s, int p){
	auto&& [prop0, CNF0, alias0, newPos0] = disjunction(s, p);
	while(s[newPos0] == '='){
		auto[prop1, CNF1, alias1, newPos1] = disjunction(s, newPos0 + 1);
		for(auto&& p : prop1) prop0.insert(p);
		for(auto&& cnf : CNF1) CNF0.push_back(cnf);
		auto newAlias = getTmpName();
		// (!alias0 || alias1) <-> newAlias
		// TODO: 実装
		//CNF0.push_back({{L{alias0,false}, L{alias1,false}, L{newAlias,true}}});
		//CNF0.push_back({{L{newAlias,false}, L{alias0,true}}});
		//CNF0.push_back({{L{newAlias,false}, L{alias1,true}}});
		alias0 = newAlias;
		newPos0 = newPos1;
	}
	return result_t{prop0, CNF0, alias0, newPos0};
}

// エントリーポイント
cnf_t parse(const std::string& s){
	auto [prop, CNF, alias, newPos] = expression(s);
	if(newPos != static_cast<int>(s.size()))
		throw std::runtime_error("unknown token remains");
	CNF.push_back({{L{alias,false}}});
	return cnf_t{prop, CNF};
}

