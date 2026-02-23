#include <string>
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
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>
#include <type_traits>
#include <utility>

struct Hobbit {
	std::string name;
	int hp, off, def;

	friend bool operator == (const Hobbit&, const Hobbit&) = default;
};

std::ostream& operator << (std::ostream& out, const Hobbit& h) {
	return out
		<< "Hobbit{\"" << h.name << "\", "
		<< ".hp=" << h.hp << ", "
		<< ".off=" << h.off << ", "
		<< ".def=" << h.def << "}";
}

template < typename T >
std::ostream& operator << (std::ostream& out, const std::optional<T>& x) {
	if (!x) return out << "EMPTY_OPTIONAL";
	return out << "Optional{" << *x << "}";
}

#endif

struct Stats {
	int hp_diff = 0;
	int off_diff = 0;
	int def_diff = 0;

	friend Stats& operator += ( Stats& a, const Stats & b ) {
		a . hp_diff += b .  hp_diff;
		a . off_diff += b . off_diff;
		a . def_diff += b . def_diff;
		return a;
	}
	friend Hobbit& operator += ( Hobbit& a, const Stats & b ) {
		a . hp += b .  hp_diff;
		a . off += b . off_diff;
		a . def += b . def_diff;
		return a;
	}

	bool empty () const { return hp_diff == 0 && off_diff == 0 && def_diff == 0; }
	void reset () { hp_diff = 0; off_diff = 0; def_diff = 0; }
};

struct HobbitArmy {
	private:
	struct Node {
		Node ( const Hobbit& oth_value ) : value ( oth_value ),
										   min ( this ),
										   max ( this ),
										   hpMin ( oth_value . hp ) {}
		void calculate () {
			int rightCount = right ? std::abs( right -> hight ) + 1 : 0;
			int leftCount = left ? std::abs(left -> hight) + 1 : 0;
			hight = rightCount - leftCount;
		}

		bool check ( const int& oth ) const { return value . hp + oth > 0; }

		void minMax () {
			min = this;
			max = this;

			if ( left )  min = left -> min;
			if ( right ) max = right -> max;

			apply();
			hpMin = value . hp ;
			if ( left ) hpMin = std::min ( hpMin, left -> hpMin );
			if ( right ) hpMin = std::min ( hpMin, right -> hpMin );
		}
		
		void minHP () {
			hpMin = value . hp ;
			if ( left ) hpMin = std::min ( hpMin, left -> hpMin );
			if ( right ) hpMin = std::min ( hpMin, right -> hpMin );
		}

		void chengeStats ( const Stats& oth ) {
			stats += oth;
			hpMin += oth . hp_diff;
		}

		void addState ( const Stats & oth ) {
			stats += oth;
			hpMin += oth . hp_diff;
		}

		void apply() {
			if ( stats . empty() ) return;
			value += stats;
			if (left) left->chengeStats ( stats ); 
			if (right) right->chengeStats ( stats );
			stats.reset();
		}

		Hobbit value;
		Stats stats;

		Node * min = this;
		Node * max = this;
		
		int hpMin = 0;

		Node * left = nullptr;
		Node * right = nullptr;
		int hight = 0;
	};
	public:

	static constexpr bool CHECK_NEGATIVE_HP = true;

	~HobbitArmy () { deleteAvl(root); }

	void deleteAvl ( Node * curr ) {
		if ( !curr ) return;
		deleteAvl( curr -> left );
		deleteAvl( curr -> right );
		delete curr;
	}

	bool add(const Hobbit& hobbit) {
		if ( hobbit . hp <= 0 ) return false;
		if ( stats( hobbit . name ) . has_value() ) return false;
		if ( !root ) root = new Node ( hobbit );
		root = insert ( root, hobbit );
		return true;
	}

	std::optional<Hobbit> erase(const std::string& hobbit_name) {
		if ( !root ) return std::nullopt;
		auto tmp = stats ( hobbit_name );
		if ( !tmp.has_value() ) return std::nullopt;
		root = erase ( root, hobbit_name );
		return tmp;
	}


	std::optional<Hobbit> stats(const std::string& hobbit_name) const {
		if ( !root ) return std::nullopt;
		Node * elem = stats ( root, hobbit_name );
		if ( elem ) return elem -> value;
		return std::nullopt;
	}

	bool enchant( const std::string& first,
				const std::string& last,
				const int& hp_diff,
				const int& off_diff,
				const int& def_diff ) {
		if ( first > last ) return true;
		Stats stats = { hp_diff, off_diff, def_diff };

		if ( hp_diff < 0 ) {
			if ( !propagetionCheck( first, last, root, stats ) ) return false;
		}
		propagetion ( first, last, root, stats);
		return true;
	}


	void for_each( auto&& fun ) const {
		for_each_impl(root, fun); // FIXME root name
	}

	private:
	Node* stats ( Node * curr, const std::string& hobbit_name ) const {
		while ( curr ) {
			curr -> apply();
			if ( hobbit_name < curr -> value . name ) curr = curr -> left;
			else if ( hobbit_name > curr -> value . name ) curr = curr -> right;
			else return curr;
		}
		return nullptr;
	}

	Node * erase ( Node * r, const std::string& value ) {
		if ( r == nullptr ) return nullptr;
		r -> apply();
		if ( value < r -> value . name ) {
			r -> left = erase( r->left, value );
		} else if ( value > r -> value . name ) {
			r -> right = erase( r->right, value );
		} else {
			if ( r -> right == nullptr && r -> left == nullptr ) {
				delete r;
				return nullptr;
			}
			if ( r -> right == nullptr && r -> left != nullptr ) {
				Node * tmp = r -> left;
				r -> left = nullptr;
				delete r;
				return tmp;
			}
			if ( r -> right != nullptr && r -> left == nullptr ) {
				Node * tmp = r -> right;
				r -> right = nullptr;
				delete r;
				return tmp;
			}
			Node * s = findMin ( r -> right );
			r -> value = s -> value;
			r -> right = erase( r -> right, r -> value . name );
		}
		r -> calculate();
		r = balance ( r );
		r -> minMax();
		return r;
	}

	Node * insert ( Node * r, const Hobbit& value ) {
		if ( r == nullptr ) return new Node ( value );
		r -> apply();
		if ( value . name < r -> value . name ) {
			r -> left = insert( r -> left, value );
		} else if ( value . name > r -> value . name ) {
			r -> right = insert( r -> right, value );
		} else return r;
		r -> calculate();
		r = balance( r );
		r -> minMax();
		return r;
	}

	bool propagetionCheck ( const std::string& first,
						const std::string& last,
						Node * curr,
						const Stats& stats ) {
		if (!curr) return true;

		if ( curr->min->value.name > last || curr->max->value.name < first ) return true; 
		if ( curr -> min -> value . name >= first && curr -> max -> value . name <= last ) 
			return curr -> hpMin + stats . hp_diff > 0;
		curr->apply();
		if ( curr -> value . name >= first && curr -> value . name <= last ) {
			if ( !curr -> check ( stats . hp_diff ) ) return false;
		}

		if ( !propagetionCheck( first, last, curr -> left, stats) ) return false;
		if ( !propagetionCheck( first, last, curr -> right, stats) ) return false;
		return true;
	}

	void propagetion ( const std::string& first,
					const std::string& last,
					Node * curr,
					const Stats& stats ) {
		if ( !curr ) return; 
		if ( curr -> min -> value . name > last || curr -> max -> value . name < first ) return;
		if ( curr -> min -> value . name >= first && curr -> max -> value . name <= last ) {
			curr -> chengeStats ( stats );
			return;
		} 
		curr -> apply();
		if ( curr -> value . name >= first && curr -> value . name <= last ) curr -> value += stats;
		propagetion( first, last, curr -> left, stats );
		propagetion( first, last, curr -> right, stats );
		
		curr -> minHP();
	}

	Node* findMin ( Node * curr ) {
		curr -> apply();
		while ( curr -> left ) {
			curr = curr -> left;
			curr -> apply();
		}
		return curr;
	}

	Node* balance ( Node * r ) {
		if ( !r ) return nullptr;
		if ( r -> hight < -1 ) {
			if ( r -> left -> hight <= 0 ) 
				r = rotateRight( r );
			else {
				r -> left = rotateLeft( r -> left );
				r = rotateRight( r );
			}
		}
		if ( r -> hight > 1 ) {
			if ( r -> right -> hight >= 0 ) 
				r = rotateLeft( r );
			else {
				r -> right = rotateRight( r -> right );
				r = rotateLeft( r );
			}
		}
		return r;
	}
	Node* rotateRight ( Node * curr ) {
		if ( !curr ) return nullptr;
		curr -> apply();
		Node * tmp = curr -> left;
		tmp -> apply();
		curr -> left = tmp -> right;
		tmp -> right = curr;
		tmp -> calculate();
		curr -> calculate();
		curr -> minMax();
		tmp -> minMax();
		return tmp;
	}

	Node* rotateLeft ( Node * curr ) {
		if ( !curr ) return nullptr;
		curr -> apply();
		Node * tmp = curr -> right;
		tmp -> apply();
		curr -> right = tmp -> left;
		tmp -> left = curr;
		curr -> calculate();
		tmp -> calculate();
		curr -> minMax();
		tmp -> minMax();
		return tmp;
	}

	static void for_each_impl(Node *node, auto& fun) {
		if ( !node ) return;
		node -> apply();
		for_each_impl ( node -> left, fun);
		fun ( node -> value );
		for_each_impl ( node -> right, fun);
	}

	Node * root = nullptr;
};

#ifndef __PROGTEST__

////////////////// Dark magic, ignore ////////////////////////

template < typename T >
auto quote(const T& t) { return t; }

std::string quote(const std::string& s) {
	std::string ret = "\"";
	for (char c : s) if (c != '\n') ret += c; else ret += "\\n";
	return ret + "\"";
}

#define STR_(a) #a
#define STR(a) STR_(a)

#define CHECK_(a, b, a_str, b_str) do { \
	auto _a = (a); \
	decltype(a) _b = (b); \
	if (_a != _b) { \
		std::cout << "Line " << __LINE__ << ": Assertion " \
		<< a_str << " == " << b_str << " failed!" \
		<< " (lhs: " << quote(_a) << ")" \
		<< " (rhs: " << quote(_b) << ")" << std::endl; \
		fail++; \
	} else ok++; \
} while (0)

#define CHECK(a, b) CHECK_(a, b, #a, #b)


////////////////// End of dark magic ////////////////////////


void check_army(const HobbitArmy& A, const std::vector<Hobbit>& ref, int& ok, int& fail) {
	size_t i = 0;

	A.for_each([&](const Hobbit& h) {
			CHECK(i < ref.size(), true);
			CHECK(h, ref[i]);
			i++;
			});
	CHECK(i, ref.size());
}

void test1(int& ok, int& fail) {
	HobbitArmy A;
	check_army(A, {}, ok, fail);

	CHECK(A.add({"Frodo", 100, 10, 3}), true);
	CHECK(A.add({"Frodo", 200, 10, 3}), false);
	CHECK(A.erase("Frodo"), std::optional(Hobbit("Frodo", 100, 10, 3)));
	CHECK(A.add({"Frodo", 200, 10, 3}), true);

	CHECK(A.add({"Sam", 80, 10, 4}), true);
	CHECK(A.add({"Pippin", 60, 12, 2}), true);
	CHECK(A.add({"Merry", 60, 15, -3}), true);
	CHECK(A.add({"Smeagol", 0, 100, 100}), false);

	if constexpr(HobbitArmy::CHECK_NEGATIVE_HP)
		CHECK(A.add({"Smeagol", -100, 100, 100}), false);

	CHECK(A.add({"Smeagol", 200, 100, 100}), true);

	CHECK(A.enchant("Frodo", "Frodo", 10, 1, 1), true);
	CHECK(A.enchant("Sam", "Frodo", -1000, 1, 1), true); // empty range
	CHECK(A.enchant("Bilbo", "Bungo", 1000, 0, 0), true); // empty range

	if constexpr(HobbitArmy::CHECK_NEGATIVE_HP)
		CHECK(A.enchant("Frodo", "Sam", -60, 1, 1), false);

	CHECK(A.enchant("Frodo", "Sam", 1, 0, 0), true);
	CHECK(A.enchant("Frodo", "Sam", -60, 1, 1), true);

	CHECK(A.stats("Gandalf"), std::optional<Hobbit>{});
	CHECK(A.stats("Frodo"), std::optional(Hobbit("Frodo", 151, 12, 5)));
	CHECK(A.stats("Merry"), std::optional(Hobbit("Merry", 1, 16, -2)));

	check_army(A, {
			{"Frodo", 151, 12, 5},
			{"Merry", 1, 16, -2},
			{"Pippin", 1, 13, 3},
			{"Sam", 21, 11, 5},
			{"Smeagol", 200, 100, 100},
			}, ok, fail);
}

int main() {
	int ok = 0, fail = 0;
	test1(ok, fail);

	if (!fail) std::cout << "Passed all " << ok << " tests!" << std::endl;
	else std::cout << "Failed " << fail << " of " << (ok + fail) << " tests." << std::endl;

	/*
	HobbitArmy army;
	std::string test[] = { "a", "b", "c", "d", "e", "f", "g", "h" };
	for ( const auto& elem : test ) {
		army . add ( { elem, 1, 0, 0 } );
	}


	for ( int i = 0; i < 8; i++ ){
		for ( int j = i; j < 8; j ++ ) {
			army . enchant( test [ i ], test [ j ], 1, 1, 1 );
			std::cout << test [ i ] << " - " << test [ j ] << std::endl;
			for ( int a = i; a <= j; a ++ ) {
				std::cout << army . stats ( test [ a ] ) << std::endl;
			}
			std::cout << std::endl;
		}
	}
	for ( const auto& elem : test ) {
		std::cout << army . stats ( elem ) << std::endl;
	}
	*/

}

#endif


