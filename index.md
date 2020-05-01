# Convert ChessBase files to PGN

```
@Def(file: cb2pgn.cpp)
	@put(includes);
	@put(globals);
	int main(int argc, char *argv[]) {
		@put(main);
	}
@End(file: cb2pgn.cpp)
```

```
@def(includes)
	#include <cstdlib>
	#include <iostream>
@end(includes)
```

```
@def(globals)
	void fail(const std::string &msg) {
		std::cerr << msg << "\n";
		std::exit(EXIT_FAILURE);
	}
@end(globals)
```

```
@def(main)
	if (argc != 2) {
		fail("call with cb base name");
	}
@end(main)
```

```
@add(includes)
	#include <fstream>
@end(includes)
```

```
@add(main) {
	@put(unit-tests);
	}
	std::string path { argv[1] };
	std::ifstream main_file {
		(path + ".cbh").c_str(),
		std::ifstream::binary
	};
	std::ifstream game_file {
		(path + ".cbg").c_str(),
		std::ifstream::binary
	};
	std::ifstream player_file {
		(path + ".cbp").c_str(),
		std::ifstream::binary
	};
	std::ifstream tournament_file {
		(path + ".cbt").c_str(),
		std::ifstream::binary
	};
	char main_entry[46];
@end(main)
```

```
@add(main)
	if (! main_file.read(
		main_entry, sizeof(main_entry)
	)) {
		fail("can't read header of "
			"cbh file"
		);
	}
@end(main)
```

```
@add(globals)
	int get_int(
		const char *c, int len = 4
	) {
		int result { 0 };
		for (int i { 0 }; i < len; ++i) {
			result = (result << 8) |
				(c[i] & 0xff);
		}
		return result;
	}
@end(globals)
```

```
@add(main)
	int count { get_int(main_entry + 6) };
	--count;
	std::cout << "number of games: " <<
		count << "\n";
@end(main)
```

```
@add(main)
	for (; main_file.read(
		main_entry, sizeof(main_entry)
	); --count) {
		@put(process game);
	}
@end(main)
```

```
@add(globals)
	std::string to_str(int value, unsigned pad = 0) {
		std::string result { std::to_string(value) };
		while (result.size() < pad) {
			result = "0" + result;
		}
		return result;
	}
@end(globals)
```

```
@add(main)
	if (count > 0) {
		fail("missing" + to_str(count) + " games");
	} else {
		fail(to_str(-count) + " surplus games");
	}
@end(main)
```

```
@def(process game) {
	std::cout << "[Event \"";
	int nr { get_int(main_entry + 15, 3) };
	if (! tournament_file.seekg(32 + 99 * nr, std::ifstream::beg)) {
		fail("can't move to tournament " + to_str(nr));
	}
	char entry[99];
	if (! tournament_file.read(entry, sizeof(entry))) {
		fail("can't read tournament " + to_str(nr));
	}
	for (char *c { entry + 9 }; *c && c < entry + 49; ++c) {
		std::cout << *c;
	}
	if (! entry[9]) { std::cout << '?'; }
	std::cout << "\"]\n";
	std::cout << "[Site \"";
	for (char *c { entry + 49 }; *c && c < entry + 79; ++c) {
		std::cout << *c;
	}
	if (! entry[49]) { std::cout << '?'; }
	if (entry[83] & 0xff) {
		std::cout << ' ';
		switch (entry[83]) {
			case 1: std::cout << "AFG"; break;
			case 2: std::cout << "ALB"; break;
			case 3: std::cout << "ALG"; break;
			case 4: std::cout << "ANG"; break;
			case 5: std::cout << "AND"; break;
			case 6: std::cout << "ANT"; break;
			case 7: std::cout << "ARG"; break;
			case 8: std::cout << "ARM"; break;
			case 9: std::cout << "AUS"; break;
			case 10: std::cout << "AUT"; break;
			case 11: std::cout << "AZE"; break;
			case 12: std::cout << "BAH"; break;
			case 13: std::cout << "BHN"; break;
			case 14: std::cout << "BAN"; break;
			case 15: std::cout << "BAR"; break;
			case 16: std::cout << "BLR"; break;
			case 17: std::cout << "BEL"; break;
			case 18: std::cout << "BLZ"; break;
			case 19: std::cout << "BER"; break;
			case 20: std::cout << "BOL"; break;
			case 21: std::cout << "BIH"; break;
			case 22: std::cout << "BOT"; break;
			case 23: std::cout << "BRA"; break;
			case 24: std::cout << "IVB"; break;
			case 25: std::cout << "BRU"; break;
			case 26: std::cout << "BUL"; break;
			case 27: std::cout << "BUR"; break;
			case 28: std::cout << "CAN"; break;
			case 29: std::cout << "CHI"; break;
			case 30: std::cout << "CHN"; break;
			case 31: std::cout << "COL"; break;
			case 32: std::cout << "CRI"; break;
			case 33: std::cout << "CRO"; break;
			case 34: std::cout << "CUB"; break;
			case 35: std::cout << "CYP"; break;
			case 36: std::cout << "CZE"; break;
			case 37: std::cout << "DEN"; break;
			case 38: std::cout << "DJI"; break;
			case 39: std::cout << "DOM"; break;
			case 40: std::cout << "ECU"; break;
			case 41: std::cout << "EGY"; break;
			case 42: std::cout << "ENG"; break;
			case 43: std::cout << "ESP"; break;
			case 44: std::cout << "EST"; break;
			case 45: std::cout << "ETH"; break;
			case 46: std::cout << "FAI"; break;
			case 47: std::cout << "FIJ"; break;
			case 48: std::cout << "FIN"; break;
			case 49: std::cout << "FRA"; break;
			case 50: std::cout << "FRM"; break;
			case 51: std::cout << "GAM"; break;
			case 52: std::cout << "GEO"; break;
			case 53: std::cout << "GER"; break;
			case 54: std::cout << "GHA"; break;
			case 55: std::cout << "GRE"; break;
			case 56: std::cout << "GUA"; break;
			case 57: std::cout << "GCI"; break;
			case 58: std::cout << "GUY"; break;
			case 59: std::cout << "HAI"; break;
			case 60: std::cout << "HON"; break;
			case 61: std::cout << "HKG"; break;
			case 62: std::cout << "HUN"; break;
			case 63: std::cout << "ISL"; break;
			case 64: std::cout << "IND"; break;
			case 65: std::cout << "INA"; break;
			case 66: std::cout << "IRI"; break;
			case 67: std::cout << "IRQ"; break;
			case 68: std::cout << "ISR"; break;
			case 69: std::cout << "IRL"; break;
			case 70: std::cout << "ITA"; break;
			case 71: std::cout << "IVO"; break;
			case 72: std::cout << "JAM"; break;
			case 73: std::cout << "JPN"; break;
			case 74: std::cout << "JCI"; break;
			case 75: std::cout << "JOR"; break;
			case 76: std::cout << "KAZ"; break;
			case 77: std::cout << "KEN"; break;
			case 78: std::cout << "KOR"; break;
			case 79: std::cout << "KGZ"; break;
			case 80: std::cout << "KUW"; break;
			case 81: std::cout << "LAT"; break;
			case 82: std::cout << "LBN"; break;
			case 83: std::cout << "LBY"; break;
			case 84: std::cout << "LIE"; break;
			case 85: std::cout << "LTU"; break;
			case 86: std::cout << "LUX"; break;
			case 87: std::cout << "MAC"; break;
			case 88: std::cout << "MAD"; break;
			case 89: std::cout << "MAS"; break;
			case 90: std::cout << "MLI"; break;
			case 91: std::cout << "MLT"; break;
			case 92: std::cout << "MAU"; break;
			case 93: std::cout << "MRI"; break;
			case 94: std::cout << "MEX"; break;
			case 95: std::cout << "MDA"; break;
			case 96: std::cout << "MNC"; break;
			case 97: std::cout << "MGL"; break;
			case 98: std::cout << "MAR"; break;
			case 99: std::cout << "MOZ"; break;
			case 100: std::cout << "MYA"; break;
			case 101: std::cout << "NAM"; break;
			case 102: std::cout << "NEP"; break;
			case 103: std::cout << "NED"; break;
			case 104: std::cout << "AHO"; break;
			case 105: std::cout << "NZL"; break;
			case 106: std::cout << "NCA"; break;
			case 107: std::cout << "NGR"; break;
			case 108: std::cout << "NOR"; break;
			case 109: std::cout << "PAK"; break;
			case 110: std::cout << "PLE"; break;
			case 111: std::cout << "PAN"; break;
			case 112: std::cout << "PNG"; break;
			case 113: std::cout << "PAR"; break;
			case 114: std::cout << "PER"; break;
			case 115: std::cout << "PHI"; break;
			case 116: std::cout << "POL"; break;
			case 117: std::cout << "POR"; break;
			case 118: std::cout << "PUR"; break;
			case 119: std::cout << "QAT"; break;
			case 120: std::cout << "ROM"; break;
			case 121: std::cout << "RUS"; break;
			case 122: std::cout << "ESA"; break;
			case 123: std::cout << "SMR"; break;
			case 124: std::cout << "SCO"; break;
			case 125: std::cout << "SEN"; break;
			case 126: std::cout << "SEY"; break;
			case 127: std::cout << "SIN"; break;
			case 128: std::cout << "SVK"; break;
			case 129: std::cout << "SLO"; break;
			case 130: std::cout << "RSA"; break;
			case 131: std::cout << "SRI"; break;
			case 132: std::cout << "SUD"; break;
			case 133: std::cout << "SUR"; break;
			case 134: std::cout << "SWE"; break;
			case 135: std::cout << "SUI"; break;
			case 136: std::cout << "SYR"; break;
			case 137: std::cout << "TJK"; break;
			case 138: std::cout << "TAN"; break;
			case 139: std::cout << "THA"; break;
			case 140: std::cout << "TRI"; break;
			case 141: std::cout << "TUN"; break;
			case 142: std::cout << "TUR"; break;
			case 143: std::cout << "TKM"; break;
			case 144: std::cout << "UGA"; break;
			case 145: std::cout << "UKR"; break;
			case 146: std::cout << "UAE"; break;
			case 147: std::cout << "USA"; break;
			case 148: std::cout << "URU"; break;
			case 149: std::cout << "URS"; break;
			case 150: std::cout << "UZB"; break;
			case 151: std::cout << "VEN"; break;
			case 152: std::cout << "VIE"; break;
			case 153: std::cout << "ISV"; break;
			case 154: std::cout << "WLS"; break;
			case 155: std::cout << "YEM"; break;
			case 156: std::cout << "YUG"; break;
			case 157: std::cout << "ZAM"; break;
			case 158: std::cout << "ZIM"; break;
			case 159: std::cout << "ZRE"; break;
			case 160: std::cout << "DDR"; break;
			case 161: std::cout << "CSR"; break;
			case 162: std::cout << "CAM"; break;
			case 163: std::cout << "CHD"; break;
			case 164: std::cout << "KAP"; break;
			case 165: std::cout << "KBA"; break;
			case 166: std::cout << "COM"; break;
			case 167: std::cout << "CON"; break;
			case 168: std::cout << "NKO"; break;
			case 169: std::cout << "LAO"; break;
			case 170: std::cout << "LES"; break;
			case 171: std::cout << "MWI"; break;
			case 172: std::cout << "MDV"; break;
			case 173: std::cout << "MSH"; break;
			case 174: std::cout << "OMN"; break;
			case 175: std::cout << "NAU"; break;
			case 176: std::cout << "MIC"; break;
			case 177: std::cout << "NIG"; break;
			case 178: std::cout << "SAU"; break;
			case 179: std::cout << "TOG"; break;
			case 180: std::cout << "TON"; break;
			case 181: std::cout << "VAN"; break;
			case 182: std::cout << "VAT"; break;
			case 183: std::cout << "TUV"; break;
			case 184: std::cout << "SWA"; break;
			case 185: std::cout << "SIE"; break;
			case 186: std::cout << "SLU"; break;
			case 187: std::cout << "PAP"; break;
			case 188: std::cout << "SVI"; break;
			case 189: std::cout << "SAM"; break;
			case 190: std::cout << "SKI"; break;
			case 191: std::cout << "SAL"; break;
			case 192: std::cout << "GE2"; break;
			case 193: std::cout << "ZAR"; break;
			case 194: std::cout << "RWA"; break;
			case 195: std::cout << "LBR"; break;
			case 196: std::cout << "NET"; break;
			case 197: std::cout << "TWN"; break;
			case 198: std::cout << "ASU"; break;
			case 199: std::cout << "AGG"; break;
			case 200: std::cout << "BNN"; break;
			case 201: std::cout << "BTN"; break;
			case 202: std::cout << "BRI"; break;
			case 203: std::cout << "CMB"; break;
			case 204: std::cout << "CAY"; break;
			case 205: std::cout << "CAR"; break;
			case 206: std::cout << "CIB"; break;
			case 207: std::cout << "CIA"; break;
			case 208: std::cout << "COA"; break;
			case 209: std::cout << "CIN"; break;
			case 210: std::cout << "ELG"; break;
			case 211: std::cout << "ERI"; break;
			case 212: std::cout << "FGB"; break;
			case 213: std::cout << "FRG"; break;
			case 214: std::cout << "FRP"; break;
			case 215: std::cout << "GAB"; break;
			case 216: std::cout << "GGB"; break;
			case 217: std::cout << "GRA"; break;
			case 218: std::cout << "GRL"; break;
			case 219: std::cout << "FGA"; break;
			case 220: std::cout << "GMA"; break;
			case 221: std::cout << "GUI"; break;
			case 222: std::cout << "GUB"; break;
			case 223: std::cout << "IOM"; break;
			case 224: std::cout << "JMY"; break;
			case 225: std::cout << "MFR"; break;
			case 226: std::cout << "MYF"; break;
			case 227: std::cout << "MSG"; break;
			case 228: std::cout << "NCF"; break;
			case 229: std::cout << "NNN"; break;
			case 230: std::cout << "NNA"; break;
			case 231: std::cout << "NMI"; break;
			case 232: std::cout << "OTM"; break;
			case 233: std::cout << "PAL"; break;
			case 234: std::cout << "PIG"; break;
			case 235: std::cout << "RUF"; break;
			case 236: std::cout << "SAO"; break;
			case 237: std::cout << "SOM"; break;
			case 238: std::cout << "SVN"; break;
			case 239: std::cout << "HGB"; break;
			case 240: std::cout << "PGB"; break;
			case 241: std::cout << "TKI"; break;
			case 242: std::cout << "TCI"; break;
			case 243: std::cout << "WFR"; break;
			case 244: std::cout << "NIR"; break;
			case 245: std::cout << "ISS"; break;
			case 246: std::cout << "GBR"; break;
			case 247: std::cout << "SAA"; break;
			case 248: std::cout << "MNT"; break;
			case 249: std::cout << "SER"; break;
			case 250: std::cout << "CAT"; break;
			case 251: std::cout << "BAS"; break;
			case 252: std::cout << "KOS"; break;
			default: std::cout << "???";
		}
	}
	std::cout << "\"]\n";
} @end(process game)
```

```
@add(process game) {
	int date { get_int(
		main_entry + 24, 3
	) };
	int day { date & 0x1f };
	int month { (date & 0x1e0) >> 5 };
	int year { date >> 9 };
	@put(print date);
} @end(process game)
```

```
@add(globals)
	std::string to_str_not_null(
		int value,
		const std::string &placeholder
	) {
		return value ?  to_str(value,
			placeholder.size()
		) : placeholder;
	}
@end(globals)
```

```
@def(print date)
	std::cout << "[Date \"" << 
		to_str_not_null(year, "????") <<
		"." <<
		to_str_not_null(month, "??") <<
		"." <<
		to_str_not_null(day, "??") <<
		"\"]\n";
@end(print date)
```

```
@add(process game) {
	int round { main_entry[29] };
	std::cout << "[Round \"";
	std::cout << (round ? to_str(round) : "?");
	int sub_round { main_entry[30] };
	if (sub_round) {
		std::cout << "." << sub_round;
	}
	std::cout << "\"]\n";
} @end(process game)
```

```
@add(globals)
	void print_player(std::ifstream &in, int nr) {
		if (! in.seekg(32 + 67 * nr, std::ifstream::beg)) {
			fail("can't move to player " + to_str(nr));
		}
		char entry[67];
		if (! in.read(entry, sizeof(entry))) {
			fail("can't read player " + to_str(nr));
		}
		for (char *c { entry + 9 }; *c && c < entry + 39; ++c) {
			std::cout << *c;
		}
		if (entry[9] && entry[39]) {
			std::cout << ", ";
		}
		for (char *c { entry + 39 }; *c && c < entry + 59; ++c) {
			std::cout << *c;
		}
	}
@end(globals)
```

```
@add(process game) {
	std::cout << "[White \"";
	print_player(player_file, get_int(main_entry + 9, 3));
	std::cout << "\"]\n";
	std::cout << "[Black \"";
	print_player(player_file, get_int(main_entry + 12, 3));
	std::cout << "\"]\n";
} @end(process game)
```

```
@add(process game)
	std::string result;
	switch (main_entry[27]) {
		case 0:
			result = "0-1"; break;
		case 1:
			result = "1/2-1/2"; break;
		case 2:
			result = "1-0"; break;
		default:
			result = "*"; break;
	}
	std::cout << "[Result \"" <<
		result << "\"]\n";
@end(process game)
```

```
@add(process game)
	@put(process moves);
	std::cout << ' ' << result << "\n\n";
@end(process game)
```

```
@add(globals)
	int get_int(
		std::istream &in, int len = 4
	) {
		int result { 0 };
		for (int i { 0 }; i < len; ++i) {
			result = (result << 8) |
				(in.get() & 0xff);
		}
		return result;
	}
@end(globals)
```

```
@add(globals)
	#include <memory>

	class Position {
		int file_ { 0 };
		int rank_ { 0 };
		static int norm(int v) {
			return v > 0 ? 1 : v < 0 ? -1 : 0;
		}
	public:
		Position() {}
		virtual ~Position() {}
		Position(int file, int rank):
			file_ { file },
			rank_ { rank }
		{}
		operator bool() const {
			return file_ >= 1 && file_ <= 8 &&
				rank_ >= 1 && rank_ <= 8;
		}
		int file() const { return file_; }
		void file(int file) { file_ = file; }
		int rank() const { return rank_; }
		void rank(int rank) { rank_ = rank; }
		Position &operator+=(const Position &o) {
			file_ += o.file_;
			rank_ += o.rank_;
			return *this;
		}
		Position &operator-=(const Position &o) {
			file_ -= o.file_;
			rank_ -= o.rank_;
			return *this;
		}
		std::string file_str() const {
			char buffer[2] = {
				*this ? (char) ('a' + file_ - 1) : '?',
				'\0'
			};
			return buffer;
		}
		std::string rank_str() const {
			char buffer[2] = {
				*this ? (char) ('1' + rank_ - 1) : '?',
				'\0'
			};
			return buffer;
		}

		operator std::string() const {
			return file_str() + rank_str();
		}

		Position norm() const {
			return Position { norm(file_), norm(rank_) };
		}

		Position square() const {
			return Position { file_ * file_, rank_ * rank_ };
		}
	};
	Position operator+(Position a, const Position &b) {
		return a += b;
	}
	bool operator==(const Position &a, const Position &b) {
		return a.file() == b.file() && a.rank() == b.rank();
	}
	bool operator!=(const Position &a, const Position &b) {
		return ! (a == b);
	}
	Position operator-(Position a, const Position &b) {
		return a -= b;
	}


	#include <memory>

	class Board;

	class Piece: public Position {
		std::unique_ptr<Piece> next_;
		bool white_;
	public:
		Piece(int file = 0, int rank = 0, bool white = true): Position { file, rank }, white_ { white } {}
		Piece(std::unique_ptr<Piece> &&next, int file = 0, int rank = 0, bool white = true): Position { file, rank }, next_ { std::move(next) }, white_ { white } {}

		virtual bool can_move_to(Board &brd, const Position &to);
		virtual std::string name() const = 0;
		bool pawn() const { return name().empty(); }
		bool white() const { return white_; }
		Piece *next() const { return next_.get(); }
		Piece *release() { return next_.release(); }
		void reset(Piece *n) { next_.reset(n); }
		Piece *find(const Position &pos) {
			if (pos == *this) { return this; };
			return next() ? next()->find(pos) : nullptr;
		}
		bool any_attacks(Board &brd, const Position &p, bool verbose = false) {
			if (*this && can_move_to(brd, p)) {
				if (verbose) {
					std::cout << "{attack from " << name() << (std::string) *this << " -> " << (std::string) p << "}";
				}
				return true;
			}
			return next() ? next()->any_attacks(brd, p, verbose) : false;
		}
		bool can_avoid_check_on(Board &brd, const Position &to, Piece *nw);
		bool can_avoid_check(Board &brd);
	};

	struct Side {
		std::unique_ptr<Piece> king;
		std::unique_ptr<Piece> queens;
		std::unique_ptr<Piece> rooks;
		std::unique_ptr<Piece> bishops;
		std::unique_ptr<Piece> knights;
		std::unique_ptr<Piece> pawns;
		void clear() {
			king.reset();
			queens.reset();
			rooks.reset();
			bishops.reset();
			knights.reset();
			pawns.reset();
		}
		Piece *find(const Position &pos) {
			Piece *r = king.get() ? king.get()->find(pos) : nullptr;
			if (r) { return r; }
			r = queens.get() ? queens.get()->find(pos) : nullptr;
			if (r) { return r; }
			r = rooks.get() ? rooks.get()->find(pos) : nullptr;
			if (r) { return r; }
			r = bishops.get() ? bishops.get()->find(pos) : nullptr;
			if (r) { return r; }
			r = knights.get() ? knights.get()->find(pos) : nullptr;
			if (r) { return r; }
			r = pawns.get() ? pawns.get()->find(pos) : nullptr;
			return r;
		}
		bool any_attacks(Board &brd, const Position &p, bool verbose = false) {
			if (king && king->any_attacks(brd, p, verbose)) {
				return true;
			}
			if (queens && queens->any_attacks(brd, p, verbose)) {
				return true;
			}
			if (rooks && rooks->any_attacks(brd, p, verbose)) {
				return true;
			}
			if (bishops && bishops->any_attacks(brd, p, verbose)) {
				return true;
			}
			if (knights && knights->any_attacks(brd, p, verbose)) {
				return true;
			}
			return pawns && pawns->any_attacks(brd, p, verbose);
		}
		bool remove_inner(Piece *p, Piece *prev) {
			while (prev) {
				Piece *n { prev->next() };
				if (p == n) {
					prev->reset(p->release());
					return true;
				}
				prev = n;
			}
			return false;
		}
		bool remove(Piece *p) {
			if (p == king.get()) {
				king.reset(p->release());
				return true;
			}
			if (remove_inner(p, king.get())) {
				return true;
			}
			if (p == queens.get()) {
				queens.reset(p->release());
				return true;
			}
			if (remove_inner(p, queens.get())) {
				return true;
			}
			if (p == rooks.get()) {
				rooks.reset(p->release());
				return true;
			}
			if (remove_inner(p, rooks.get())) {
				return true;
			}
			if (p == bishops.get()) {
				bishops.reset(p->release());
				return true;
			}
			if (remove_inner(p, bishops.get())) {
				return true;
			}
			if (p == knights.get()) {
				knights.reset(p->release());
				return true;
			}
			if (remove_inner(p, knights.get())) {
				return true;
			}
			return false;
		}
		bool can_avoid_check(Board &brd) {
			if (king) {
				if (king->can_avoid_check(brd)) { return true; }
			}
			if (queens) {
				if (queens->can_avoid_check(brd)) { return true; }
			}
			if (rooks) {
				if (rooks->can_avoid_check(brd)) { return true; }
			}
			if (bishops) {
				if (bishops->can_avoid_check(brd)) { return true; }
			}
			if (knights) {
				if (knights->can_avoid_check(brd)) { return true; }
			}
			if (pawns) {
				if (pawns->can_avoid_check(brd)) { return true; }
			}
			return false;
		}
		Piece *add_piece(Piece *np, std::unique_ptr<Piece> &lst) {
			if (! lst) {
				lst.reset(np);
			} else {
				Piece *p { lst.get() };
				for (; p->next(); p = p->next()) {}
				p->reset(np);
			}
			return np;
		}
	};

	class Board {
		Side white_;
		Side black_;
		Side *cur_ { &white_ };
		Side *other_ { &black_ };
		Piece *find(int nr, Piece *pcs) const;
		Board(const Board &) = delete;
		Board &operator=(const Board &) = delete;
	public:
		Board();
		Piece *king(int nr = 1, bool other = false) const;
		Piece *queen(int nr = 1, bool other = false) const;
		Piece *bishop(int nr = 1, bool other = false) const;
		Piece *knight(int nr = 1, bool other = false) const;
		Piece *rook(int nr = 1, bool other = false) const;
		Piece *pawn(int nr = 1, bool other = false) const;
		Piece *get(const Position &pos) { 
			Piece *r { white_.find(pos) };
			if (r) { return r; }
			return black_.find(pos);
		}
		void move(std::ostream &out, Piece *first, Piece *from, const Position &to);
		void switch_players() { 
			Side *tmp { other_ };
			other_ = cur_;
			cur_ = tmp;
		}
		bool white() const { return &white_ == cur_; }
		bool mate() { 
			switch_players();
			bool result { ! cur_->can_avoid_check(*this) };
			switch_players();
			return result;
		}
		bool check(bool other = true, bool verbose = false) { 
			Side *s { other ? other_ : cur_ };
			Side *o { other ? cur_ : other_ };
			if (s->king) {
				return o->any_attacks(*this, *s->king.get(), verbose);
			}
			return false;
		}
		void kingside_rochade(std::ostream &out);
		void queenside_rochade(std::ostream &out);
		void remove(Piece *piece) {
			if (piece->pawn()) {
				piece->file(0);
				piece->rank(0);
			} else {
				std::string name { piece->name() + (std::string) *piece };
				if (white_.remove(piece)) { return; };
				if (black_.remove(piece)) { return; }
				fail("can't remove piece " + name);
			}
		}
		Piece *add_king(const Position &pos, bool other = false);
		Piece *add_queen(const Position &pos, bool other = false);
		Piece *add_rook(const Position &pos, bool other = false);
		Piece *add_bishop(const Position &pos, bool other = false);
		Piece *add_knight(const Position &pos, bool other = false);
		Piece *add_pawn(const Position &pos, bool other = false);
		Piece *first(const Piece &p) {
			if (p.name() == "") {
				return cur_->pawns.get();
			} else if (p.name() == "K") {
				return cur_->king.get();
			} else if (p.name() == "Q") {
				return cur_->queens.get();
			} else if (p.name() == "R") {
				return cur_->rooks.get();
			} else if (p.name() == "B") {
				return cur_->bishops.get();
			} else if (p.name() == "N") {
				return cur_->knights.get();
			} else {
				fail("uknown name " + p.name());
			}
			return nullptr;
		}
		void clear() {
			white_.clear();
			black_.clear();
			cur_ = &white_;
			other_ = &black_;
		}
	} brd;

	Piece *Board::find(int nr, Piece *pcs) const {
		for (; nr > 1 && pcs; --nr) {
			pcs = pcs->next();
		}
		return pcs;
	}
	Piece *Board::king(int nr, bool other) const {
		return find(nr, other ? other_->king.get() : cur_->king.get());
	}
	Piece *Board::queen(int nr, bool other) const {
		return find(nr, other ? other_->queens.get() : cur_->queens.get());
	}
	Piece *Board::bishop(int nr, bool other) const {
		return find(nr, other ? other_->bishops.get() : cur_->bishops.get());
	}
	Piece *Board::knight(int nr, bool other) const {
		return find(nr, other ? other_->knights.get() : cur_->knights.get());
	}
	Piece *Board::rook(int nr, bool other) const {
		return find(nr, other ? other_->rooks.get() : cur_->rooks.get());
	}
	Piece *Board::pawn(int nr, bool other) const {
		return find(nr, other ? other_->pawns.get() : cur_->pawns.get());
	}

	bool Piece::can_avoid_check_on(Board &brd, const Position &to, Piece *nw) {
		if (pawn() && ! nw && (to.rank() == 1 || to.rank() == 8)) {
			if (can_avoid_check_on(brd, to, brd.add_queen(to))) {
				return true;
			}
			if (can_avoid_check_on(brd, to, brd.add_knight(to))) {
				return true;
			}
			if (can_avoid_check_on(brd, to, brd.add_rook(to))) {
				return true;
			}
			if (can_avoid_check_on(brd, to, brd.add_bishop(to))) {
				return true;
			}
		} else {
			Position from { file(), rank() };
			Piece *old { brd.get(to) };
			if (old) { old->file(-old->file()); }
			if (nw) {
				file(-file());
			} else {
				file(to.file());
				rank(to.rank());
			}

			bool can_avoid { ! brd.check(false) };
			if (name() == "Kxx") {
				std::cout << "{" << (std::string) *this << ": "
					<< (can_avoid ? "1" : "0") << (nw ? "n" : "") << "}";
			}

			file(from.file());
			rank(from.rank());
			if (old) { old->file(-old->file()); }
			if (nw) { brd.remove(nw); };

			return can_avoid;
		}
		return false;
	}
	bool Piece::can_avoid_check(Board &brd) {
		Position p;
		if (*this) {
			for (int r { 1 }; r <= 8; ++r) {
				p.rank(r);
				for (int f { 1 }; f <= 8; ++f) {
					p.file(f);
					if (! can_move_to(brd, p)) {
						continue;
					}
					if (can_avoid_check_on(brd, p, nullptr)) {
						return true;
					}
				}
			}
		}
		return next() ? next()->can_avoid_check(brd) : false;
	}
	void Board::move(std::ostream &out, Piece *first, Piece *from, const Position &to) {
		if (! from) {
			fail("no piece to move from");
		}
		Piece *to_piece { get(to) };
		bool captures { to_piece };
		if (from->pawn() && from->file() != to.file()) {
			if (! captures) {
				if (white()) {
					to_piece = get(to + Position { 0, -1 });
				} else {
					to_piece = get(to + Position { 0, 1 });
				}
			}
			captures = true;
		}
		out << from->name();// << '(' << (std::string) *from << ')';

		if (from->pawn()) {
			if (captures) {
				out << from->file_str();
			}
		} else {
			bool same_f { false };
			bool same_r { false };
			bool multiple { false };
			for (auto cur { first }; cur; cur = cur->next()) {
				if (cur == from) { continue; }
				if (! cur->can_move_to(*this, to)) { continue; }
				if (cur->file() == from->file()) { same_f = true; }
				if (cur->rank() == from->rank()) { same_r = true; }
				multiple = true;
			}
			if (multiple) {
				if (! same_f || (same_f && same_r)) {
					out << from->file_str();
				}
				if (same_f) {
					out << from->rank_str();
				}
			}
		}
		if (captures) { out << "x"; }
		if (to_piece) {
			remove(to_piece);
			to_piece = nullptr;
		}
		from->file(to.file());
		from->rank(to.rank());
		out << (std::string) to;
		if (check()) {
			if (mate()) {
				out << '#';
			} else {
				out << '+';
			}
		}
	}

	bool Piece::can_move_to(Board &brd, const Position &to) {
		Piece *to_piece { brd.get(to) };
		if (to_piece && to_piece->white() == white_) { return false; }
		
		Position d { (to - *this).norm() };
		Position cur { *this };
		for (;;) {
			cur = cur + d;
			if (! cur) { return false; }
			if (cur == to) { return true; }
			if (brd.get(cur)) { return false; }
		}
	}

	class Rook_Piece: public virtual Piece {
	public:
		Rook_Piece(int file = 0, int rank = 0, bool white = true): Piece { file, rank, white } {}
		Rook_Piece(std::unique_ptr<Piece> &&next, int file = 0, int rank = 0, bool white = true): Piece { std::move(next), file, rank, white } {}

		bool can_move_to(Board &brd, const Position &to) override {
			if (to.file() == file() || to.rank() == rank()) {
				return Piece::can_move_to(brd, to);
			}
			return false;
		}
		std::string name() const override {
			return "R";
		}
	};

	class Bishop_Piece: public virtual Piece {
	public:
		Bishop_Piece(int file = 0, int rank = 0, bool white = true): Piece { file, rank, white } {}
		Bishop_Piece(std::unique_ptr<Piece> &&next, int file = 0, int rank = 0, bool white = true): Piece { std::move(next), file, rank, white } {}

		bool can_move_to(Board &brd, const Position &to) override {
			Position d { to - *this };
			if (d.file() == d.rank() || d.file() == -d.rank()) {
				return Piece::can_move_to(brd, to);
			}
			return false;
		}
		std::string name() const override {
			return "B";
		}
	};

	class Queen_Piece: public Rook_Piece, public Bishop_Piece {
	public:
		Queen_Piece(int file = 0, int rank = 0, bool white = true): Piece { file, rank, white } {}
		Queen_Piece(std::unique_ptr<Piece> &&next, int file = 0, int rank = 0, bool white = true): Piece { std::move(next), file, rank, white } {}
		bool can_move_to(Board &brd, const Position &to) override {
			return Rook_Piece::can_move_to(brd, to) ||
				Bishop_Piece::can_move_to(brd, to);
		}
		std::string name() const override {
			return "Q";
		}
	};

	class King_Piece: public Piece {
	public:
		King_Piece(int file = 0, int rank = 0, bool white = true): Piece { file, rank, white } {}
		King_Piece(std::unique_ptr<Piece> &&next, int file = 0, int rank = 0, bool white = true): Piece { std::move(next), file, rank, white } {}
		bool can_move_to(Board &brd, const Position &to) override {
			Position sq { (to - *this).square() };
			if (sq.file() <= 1 && sq.rank() <= 1) {
				return Piece::can_move_to(brd, to);
			}
			return false;
		}
		std::string name() const override {
			return "K";
		}
	};

	class Knight_Piece: public Piece {
	public:
		Knight_Piece(int file = 0, int rank = 0, bool white = true): Piece { file, rank, white } {}
		Knight_Piece(std::unique_ptr<Piece> &&next, int file = 0, int rank = 0, bool white = true): Piece { std::move(next), file, rank, white } {}
		bool can_move_to(Board &brd, const Position &to) override {
			Position sq { (to - *this).square() };
			if (sq.file() + sq.rank() == 5 && (sq.file() == 1 || sq.rank() == 1)) {
				Piece *to_piece { brd.get(to) };
				if (to_piece && to_piece->white() == white()) { return false; }
				return true;
				
			}
			return false;
		};
		std::string name() const override {
			return "N";
		}
	};

	class White_Pawn_Piece: public Piece {
	public:
		White_Pawn_Piece(int file = 0, int rank = 0): Piece { file, rank, true } {}
		White_Pawn_Piece(std::unique_ptr<Piece> &&next, int file = 0, int rank = 0): Piece { std::move(next), file, rank, true } {}
		bool can_move_to(Board &brd, const Position &to) override {
			Piece *to_piece { brd.get(to) };
			if (rank() == 2 && to.rank() == 4) {
				return file() == to.file() && ! to_piece && ! brd.get(*this + Position { 0, 1});
			} else if (rank() + 1 == to.rank()) {
				if (file() == to.file()) {
					return ! to_piece;
				};
				Position sq { (to - *this).square() };
				if (sq.file() == 1) {
					if (to_piece) { return ! to_piece->white(); }
					if (rank() == 5) {
						Piece *p { brd.get(to + Position{0, -1}) };
						return p && ! p->white() && p->pawn();
					}
				}
			}
			return false;
		}
		std::string name() const override {
			return "";
		}
	};

	class Black_Pawn_Piece: public Piece {
	public:
		Black_Pawn_Piece(int file = 0, int rank = 0): Piece { file, rank, false } {}
		Black_Pawn_Piece(std::unique_ptr<Piece> &&next, int file = 0, int rank = 0): Piece { std::move(next), file, rank, false } {}
		bool can_move_to(Board &brd, const Position &to) override {
			Piece *to_piece { brd.get(to) };
			if (rank() == 7 && to.rank() == 5) {
				return file() == to.file() && ! to_piece && ! brd.get(*this + Position { 0, -1 });
			} else if (rank() - 1 == to) {
				if (file() == to.file()) {
					return ! to_piece;
				};
				Position sq { (to - *this).square() };
				if (sq.file() == 1) {
					if (to_piece) { return to_piece->white(); }
					if (rank() == 4) {
						Piece *p { brd.get(to + Position{0, 1}) };
						return p && p->white() && p->pawn();
					}
				}
			}
			return false;
		}
		std::string name() const override {
			return "";
		}
	};

	Piece *Board::add_king(const Position &pos, bool other) {
		if (other) {
		return other_->add_piece(new King_Piece(pos.file(), pos.rank(), ! white()), other_->king);
		} else {
		return cur_->add_piece(new King_Piece(pos.file(), pos.rank(), white()), cur_->king);
		}
	}
	Piece *Board::add_queen(const Position &pos, bool other) {
		if (other) {
			return other_->add_piece(new Queen_Piece(pos.file(), pos.rank(), ! white()), other_->queens);
		} else {
			return cur_->add_piece(new Queen_Piece(pos.file(), pos.rank(), white()), cur_->queens);
		}
	}
	Piece *Board::add_rook(const Position &pos, bool other) {
		if (other) {
			return other_->add_piece(new Rook_Piece(pos.file(), pos.rank(), ! white()), other_->rooks);
		} else {
			return cur_->add_piece(new Rook_Piece(pos.file(), pos.rank(), white()), cur_->rooks);
		}
	}
	Piece *Board::add_bishop(const Position &pos, bool other) {
		if (other) {
			return other_->add_piece(new Bishop_Piece(pos.file(), pos.rank(), ! white()), other_->bishops);
		} else {
			return cur_->add_piece(new Bishop_Piece(pos.file(), pos.rank(), white()), cur_->bishops);
		}
	}
	Piece *Board::add_knight(const Position &pos, bool other) {
		if (other) {
			return other_->add_piece(new Knight_Piece(pos.file(), pos.rank(), ! white()), other_->knights);
		} else {
			return cur_->add_piece(new Knight_Piece(pos.file(), pos.rank(), white()), cur_->knights);
		}
	}
	Piece *Board::add_pawn(const Position &pos, bool other) {
		if (other) {
			return other_->add_piece(white() ? (Piece *) new Black_Pawn_Piece(pos.file(), pos.rank()): (Piece *) new White_Pawn_Piece(pos.file(), pos.rank()), other_->pawns);
		} else {
			return cur_->add_piece(white() ? (Piece *) new White_Pawn_Piece(pos.file(), pos.rank()): (Piece *) new Black_Pawn_Piece(pos.file(), pos.rank()), cur_->pawns);
		}
	}

	Board::Board() {
		white_.king = std::make_unique<King_Piece>(5, 1, true);
		black_.king = std::make_unique<King_Piece>(5, 8, false);
		white_.queens = std::make_unique<Queen_Piece>(4, 1, true);
		black_.queens = std::make_unique<Queen_Piece>(4, 8, false);
		white_.rooks = std::make_unique<Rook_Piece>(
			std::make_unique<Rook_Piece>(8, 1, true), 1, 1, true
		);
		black_.rooks = std::make_unique<Rook_Piece>(
			std::make_unique<Rook_Piece>(8, 8, false), 1, 8, false
		);
		white_.bishops = std::make_unique<Bishop_Piece>(
			std::make_unique<Bishop_Piece>(6, 1, true), 3, 1, true
		);
		black_.bishops = std::make_unique<Bishop_Piece>(
			std::make_unique<Bishop_Piece>(6, 8, false), 3, 8, false
		);
		white_.knights = std::make_unique<Knight_Piece>(
			std::make_unique<Knight_Piece>(7, 1, true), 2, 1, true
		);
		black_.knights = std::make_unique<Knight_Piece>(
			std::make_unique<Knight_Piece>(7, 8, false), 2, 8, false
		);
		for (int i { 8 }; i > 0; --i) {
			white_.pawns = std::make_unique<White_Pawn_Piece>(
				std::move(white_.pawns), i, 2
			);
			black_.pawns = std::make_unique<Black_Pawn_Piece>(
				std::move(black_.pawns), i, 7
			);
		}
	}
@end(globals)
```

```
@def(unit-tests) 
	Board brd;
@end(unit-tests)
```

```
@add(globals)
	void test_1_fig(
		Piece *(Board::*fn)(int, bool) const,
		int nr,
		bool other,
		const std::string &exp
	) {
		Piece *pc { (brd.*fn)(nr, other) };
		if (! pc) {
			fail("no figure at index " + std::to_string(nr));
		}
		auto pcs { (std::string) *pc };
		if (pcs != exp) {
			fail("got " + pcs + " but expecting " + exp);
		}
	}
	void test_1_fig(
		Piece *(Board::*fn)(int, bool) const,
		bool other,
		const std::string &exp
	) {
		test_1_fig(fn, 1, other, exp);
		if ((brd.*fn)(2, other)) {
			fail("more than one piece");
		}
	}
@end(globals)
```

```
@add(unit-tests)
	test_1_fig(&Board::king, false, "e1");
	test_1_fig(&Board::king, true, "e8");
	test_1_fig(&Board::queen, false, "d1");
	test_1_fig(&Board::queen, true, "d8");
@end(unit-tests)
```

```
@add(globals)
	void test_2_fig(
		Piece *(Board::*fn)(int, bool) const,
		bool other,
		const std::string &exp_1,
		const std::string &exp_2
	) {
		test_1_fig(fn, 1, other, exp_1);
		test_1_fig(fn, 2, other, exp_2);
		if ((brd.*fn)(3, other)) {
			fail("more than two pieces");
		}
	}
@end(globals);
```

```
@add(unit-tests)
	test_2_fig(&Board::rook, false, "a1", "h1");
	test_2_fig(&Board::rook, true, "a8", "h8");
	test_2_fig(&Board::knight, false, "b1", "g1");
	test_2_fig(&Board::knight, true, "b8", "g8");
	test_2_fig(&Board::bishop, false, "c1", "f1");
	test_2_fig(&Board::bishop, true, "c8", "f8");
@end(unit-tests)
```

```
@add(globals)
	void test_8_fig(
		Piece *(Board::*fn)(int, bool) const,
		bool other,
		const std::string &exp_1,
		const std::string &exp_2,
		const std::string &exp_3,
		const std::string &exp_4,
		const std::string &exp_5,
		const std::string &exp_6,
		const std::string &exp_7,
		const std::string &exp_8
	) {
		test_1_fig(fn, 1, other, exp_1);
		test_1_fig(fn, 2, other, exp_2);
		test_1_fig(fn, 3, other, exp_3);
		test_1_fig(fn, 4, other, exp_4);
		test_1_fig(fn, 5, other, exp_5);
		test_1_fig(fn, 6, other, exp_6);
		test_1_fig(fn, 7, other, exp_7);
		test_1_fig(fn, 8, other, exp_8);
		if ((brd.*fn)(9, other)) {
			fail("more than 8 pieces");
		}
	}
@end(globals)
```

```
@add(unit-tests)
	test_8_fig(&Board::pawn, false, "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2");
	test_8_fig(&Board::pawn, true, "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7");
@end(unit-tests)
```

	
```
@add(unit-tests)
	for (int f { 1 }; f <= 8; ++f) {
		if (! brd.get(Position { f, 1 }) || ! brd.get(Position { f, 1 })->white()) {
			fail("not white on " + (std::string) Position { f, 1 });
		}
		if (! brd.get(Position { f, 2 }) || ! brd.get(Position { f, 2 })->white()) {
			fail("not white on " + (std::string) Position { f, 2 });
		}
		if (! brd.get(Position { f, 7 }) || brd.get(Position { f, 7 })->white()) {
			fail("no black on " + (std::string) Position { f, 7 });
		}
		if (! brd.get(Position { f, 8 }) || brd.get(Position { f, 8 })->white()) {
			fail("no black on " + (std::string) Position { f, 8 });
		}
		for (int r { 3 }; r <= 6; ++r) {
			if (brd.get(Position { f, r })) {
				fail("not empty on " + (std::string) Position { f, r });
			}
		}
	}
@end(unit-tests)
```

```
@add(globals)
	void test_add(const Position &p, int f, int r, const std::string &exp) {
		Position q { p + Position { f, r } };
		auto got { (std::string) q };
		if (got != exp) {
			fail("adding returns " + got + " instead of " + exp);
		}
	}
@end(globals)
```

```
@add(unit-tests) {
	const auto &wk { *brd.king() };
	test_add(wk, 0, 0, "e1");
	test_add(wk, -4, 0, "a1");
	test_add(wk, 3, 0, "h1");
} @end(unit-tests)
```

```
@add(globals)
	bool move_piece(std::ostream &out, Piece *(Board::*fn)(int, bool) const, int nr, int f, int r) {
		Piece *from { (brd.*fn)(nr, false) };
		if (! from) { fail("no piece to move " + std::to_string(nr)); }
		Position to { *from + Position { f, r } };
		if (to.file() > 8) { to -= Position { 8, 0 }; }
		if (to.rank() > 8) { to -= Position { 0, 8 }; }
		if (! to) {
			return false;
		}
		brd.move(out, (brd.*fn)(1, false), from, to);
		return true;
	}
@end(globals)
```

```
@add(globals)
	void move_king(std::ostream &out, int f, int r) {
		if (! move_piece(out, &Board::king, 1, f, r)) {
			fail("invalid king move");
		}
	}
@end(globals)
```

```
@add(globals)
	void move_queen(std::ostream &out, int nr, int f, int r) {
		if (! move_piece(out, &Board::queen, nr, f, r)) {
			fail("invalid queen move " + std::to_string(nr));
		}
	}
@end(globals)
```

```
@add(globals)
	void move_rook(std::ostream &out, int nr, int f, int r) {
		if (! move_piece(out, &Board::rook, nr, f, r)) {
			fail("invalid rook move " + std::to_string(nr));
		}
	}
@end(globals)
```

```
@add(globals)
	void move_bishop(std::ostream &out, int nr, int f, int r) {
		if (! move_piece(out, &Board::bishop, nr, f, r)) {
			fail("invalid bishop move " + std::to_string(nr));
		}
	}
@end(globals)
```

```
@add(globals)
	void move_knight(std::ostream &out, int nr, int f, int r) {
		if (! move_piece(out, &Board::knight, nr, f, r)) {
			fail("invalid knight move " + std::to_string(nr));
		}
	}
@end(globals)
```

```
@add(globals)
	void move_pawn(std::ostream &out, int nr, int f, int r) {
		if (! brd.white()) {
			r = 8 - r;
			f = f ? 8 - f : 0;
		}
		if (! move_piece(out, &Board::pawn, nr, f, r)) {
			fail("invalid pawn move " + std::to_string(nr) + ", " + std::to_string(f) + ", " + std::to_string(r));
		}
	}
@end(globals)
```

```
@add(globals)
	void Board::kingside_rochade(std::ostream &out) {
		Piece *kp { king() };
		int kf { kp->file() };
		int mf { 9 };
		Piece *r { nullptr };
		for (Piece *cur { cur_->rooks.get() }; cur; cur = cur->next()) {
			if (cur->rank() != kp->rank()) { continue; }
			int pf { cur->file() };
			if (pf > kf && pf < mf) {
				mf = pf;
				r = cur;
			}
		}
		if (! r) {
			fail("no kingside rook found");
		}
		r->file(kf + 1);
		kp->file(kf + 2);
		out << "O-O";
	}
@end(globals)
```

```
@add(globals)
	void Board::queenside_rochade(std::ostream &out) {
		Piece *kp { king() };
		int kf { kp->file() };
		int mf { 0 };
		Piece *r { nullptr };
		for (Piece *cur { cur_->rooks.get() }; cur; cur = cur->next()) {
			if (cur->rank() != kp->rank()) { continue; }
			int pf { cur->file() };
			if (pf < kf && pf > mf) {
				mf = pf;
				r = cur;
			}
		}
		if (! r) {
			fail("no queenside rook found");
		}
		r->file(kf - 1);
		kp->file(kf - 2);
		out << "O-O-O";
	}
@end(globals)
```

```
@add(globals)
	#include <sstream>
@end(globals)
```

```
@def(process moves) {
	brd.~Board();
	new (&brd) Board();
	if (! game_file.seekg(get_int(main_entry + 1), std::ifstream::beg)) {
		fail("can't move to game at " + to_str(get_int(main_entry + 1)));
	}
	int len { get_int(game_file) };
	if (len & 0x80000000) {
		fail("unknown game data of length " + to_str(len));
	}
	bool start_with_white { true };
	if (len & 0x40000000) {
		len &= 0x3fffffff;
		char bd[65] { 0 };
		brd.clear();
		game_file.get();
		if (game_file.get() & 0x8) {
			start_with_white = false;
		}
		game_file.get();
		/* int move_nr = */ game_file.get();
		int value { 0 };
		int bits { 0 };
		int cnt { 0 };
		for (int f { 1 }; f <= 8; ++f) {
			for (int r { 1 }; r <= 8; ++r) {
				if (bits < 1) {
					value = (value << 8) | game_file.get();
					++cnt;
					bits += 8;
				}
				if (! ((value >> (bits - 1) & 0x1))) {
					--bits;
					continue;
				}
				if (bits < 5) {
					value = (value << 8) | game_file.get();
					bits += 8;
					++cnt;
				}
				int piece { (value >> (bits - 5)) & 0x1f };
				value = value - (piece << (bits - 5));
				bits -= 5;
				int idx { (r - 1) * 8 + (f - 1) };
				Position p { f, r };
				switch (piece) {
					case 0x11:
						bd[idx] = 'K';
						brd.add_king(p, false);
						break;
					case 0x12:
						bd[idx] = 'Q';
						brd.add_queen(p, false);
						break;
					case 0x13:
						bd[idx] = 'N';
						brd.add_knight(p, false);
						break;
					case 0x14:
						bd[idx] = 'B';
						brd.add_bishop(p, false);
						break;
					case 0x15:
						bd[idx] = 'R';
						brd.add_rook(p, false);
						break;
					case 0x16:
						bd[idx] = 'P';
						brd.add_pawn(p, false);
						break;
					case 0x19:
						bd[idx] = 'k';
						brd.add_king(p, true);
						break;
					case 0x1a:
						bd[idx] = 'q';
						brd.add_queen(p, true);
						break;
					case 0x1b:
						bd[idx] = 'n';
						brd.add_knight(p, true);
						break;
					case 0x1c:
						bd[idx] = 'b';
						brd.add_bishop(p, true);
						break;
					case 0x1d:
						bd[idx] = 'r';
						brd.add_rook(p, true);
						break;
					case 0x1e:
						bd[idx] = 'p';
						brd.add_pawn(p, true);
						break;
					default:
						fail("unknown setup piece " + to_str(piece));
				}
			}
		}
		for (; cnt < 24; ++cnt) { game_file.get(); }
		std::cout << "[SetUp \"1\"]\n";
		std::cout << "[FEN \"";
		for (int r { 8 }; r >= 1; --r) {
			int spaces { 0 };
			if (r < 8) { std::cout.put('/'); }
			for (int f { 1 }; f <= 8; ++f) {
				int idx { (r - 1) * 8 + (f - 1) };
				if (bd[idx]) {
					if (spaces) { std::cout << spaces; spaces = 0; }
					std::cout.put(bd[idx]);
				} else {
					++spaces;
				}
			}
			if (spaces) { std::cout << spaces; }
		}
		std::cout << ' ' << (start_with_white ? 'w' : 'b');
		std::cout << "\"]\n";
		if (! start_with_white) {
			brd.switch_players();
		}
	}

static short MoveNumberLookup[256] = {
   0xa2, 0x95, 0x43, 0xf5, 0xc1, 0x3d, 0x4a, 0x6c, //   0 -   7
   0x53, 0x83, 0xcc, 0x7c, 0xff, 0xae, 0x68, 0xad, //   8 -  15
   0xd1, 0x92, 0x8b, 0x8d, 0x35, 0x81, 0x5e, 0x74, //  16 -  23
   0x26, 0x8e, 0xab, 0xca, 0xfd, 0x9a, 0xf3, 0xa0, //  24 -  31
   0xa5, 0x15, 0xfc, 0xb1, 0x1e, 0xed, 0x30, 0xea, //  32 -  39
   0x22, 0xeb, 0xa7, 0xcd, 0x4e, 0x6f, 0x2e, 0x24, //  40 -  47
   0x32, 0x94, 0x41, 0x8c, 0x6e, 0x58, 0x82, 0x50, //  48 -  55
   0xbb, 0x02, 0x8a, 0xd8, 0xfa, 0x60, 0xde, 0x52, //  56 -  63
   0xba, 0x46, 0xac, 0x29, 0x9d, 0xd7, 0xdf, 0x08, //  64 -  71
   0x21, 0x01, 0x66, 0xa3, 0xf1, 0x19, 0x27, 0xb5, //  72 -  79
   0x91, 0xd5, 0x42, 0x0e, 0xb4, 0x4c, 0xd9, 0x18, //  80 -  87
   0x5f, 0xbc, 0x25, 0xa6, 0x96, 0x04, 0x56, 0x6a, //  88 -  95
   0xaa, 0x33, 0x1c, 0x2b, 0x73, 0xf0, 0xdd, 0xa4, //  96 - 103
   0x37, 0xd3, 0xc5, 0x10, 0xbf, 0x5a, 0x23, 0x34, // 104 - 111
   0x75, 0x5b, 0xb8, 0x55, 0xd2, 0x6b, 0x09, 0x3a, // 112 - 119
   0x57, 0x12, 0xb3, 0x77, 0x48, 0x85, 0x9b, 0x0f, // 120 - 127
   0x9e, 0xc7, 0xc8, 0xa1, 0x7f, 0x7a, 0xc0, 0xbd, // 128 - 135
   0x31, 0x6d, 0xf6, 0x3e, 0xc3, 0x11, 0x71, 0xce, // 136 - 143
   0x7d, 0xda, 0xa8, 0x54, 0x90, 0x97, 0x1f, 0x44, // 144 - 151
   0x40, 0x16, 0xc9, 0xe3, 0x2c, 0xcb, 0x84, 0xec, // 152 - 159
   0x9f, 0x3f, 0x5c, 0xe6, 0x76, 0x0b, 0x3c, 0x20, // 160 - 167
   0xb7, 0x36, 0x00, 0xdc, 0xe7, 0xf9, 0x4f, 0xf7, // 168 - 175
   0xaf, 0x06, 0x07, 0xe0, 0x1a, 0x0a, 0xa9, 0x4b, // 176 - 183
   0x0c, 0xd6, 0x63, 0x87, 0x89, 0x1d, 0x13, 0x1b, // 184 - 191
   0xe4, 0x70, 0x05, 0x47, 0x67, 0x7b, 0x2f, 0xee, // 192 - 199
   0xe2, 0xe8, 0x98, 0x0d, 0xef, 0xcf, 0xc4, 0xf4, // 200 - 207
   0xfb, 0xb0, 0x17, 0x99, 0x64, 0xf2, 0xd4, 0x2a, // 208 - 215
   0x03, 0x4d, 0x78, 0xc6, 0xfe, 0x65, 0x86, 0x88, // 216 - 223
   0x79, 0x45, 0x3b, 0xe5, 0x49, 0x8f, 0x2d, 0xb9, // 224 - 231
   0xbe, 0x62, 0x93, 0x14, 0xe9, 0xd0, 0x38, 0x9c, // 232 - 239
   0xb2, 0xc2, 0x59, 0x5d, 0xb6, 0x72, 0x51, 0xf8, // 240 - 247
    0x28, 0x7e, 0x61, 0x39, 0xe1, 0xdb, 0x69, 0x80, // 248 - 255
 	};
	std::cout << "\n";
	int chrs { 1000 };
	for (int count { 0 }, nr { 0 }; len; ++count, ++nr, --len) {
		int ch { (game_file.get() - count) & 0xff };
		if (ch == 0x0c) {
			break;
		}
		std::ostringstream out;
		if (! (nr & 1)) {
			out << (1 + (nr >> 1)) << ". ";
		}
		std::cout << '{' << std::hex << ch << std::dec << "} ";

		switch (ch) {
			case 0x00:
				move_queen(out, 2, 6, 6);
				break;
			case 0x01:
				move_queen(out, 2, 0, 7);
				break;
			case 0x02:
				move_bishop(out, 1, 1, 1);
				break;
			case 0x04:
				move_queen(out, 3, 2, 6);
				break;
			case 0x05:
				move_rook(out, 2, 2, 0);
				break;
			case 0x06:
				move_bishop(out, 1, 1, 7);
				break;
			case 0x07:
				move_knight(out, 2, 7, 6);
				break;
			case 0x08:
				move_bishop(out, 2, 3, 3);
				break;
			case 0x09:
				move_pawn(out, 6, 0, 1);
				break;
			case 0x0a:
				move_rook(out, 3, 0, 6);
				break;
			case 0x0b:
				move_pawn(out, 4, 0, 2);
				break;
			case 0x0d:
				move_queen(out, 3, 0, 4);
				break;
			case 0x0e:
				move_knight(out, 2, 1, 2);
				break;
			case 0x0f:
				move_queen(out, 3, 0, 3);
				break;
			case 0x10:
				move_rook(out, 3, 4, 0);
				break;
			case 0x11:
				move_queen(out, 2, 0, 4);
				break;
			case 0x12:
				move_pawn(out, 8, 0, 1);
				break;
			case 0x13:
				move_pawn(out, 8, 1, 1);
				break;
			case 0x14:
				move_rook(out, 2, 0, 1);
				break;
			case 0x15:
				move_pawn(out, 5, 1, 1);
				break;
			case 0x16:
				move_bishop(out, 2, 7, 1);
				break;
			case 0x17:
				move_pawn(out, 2, 0, 2);
				break;
			case 0x18:
				move_queen(out, 1, 7, 1);
				break;
			case 0x19:
				move_pawn(out, 8, 7, 1);
				break;
			case 0x1a:
				move_queen(out, 3, 0, 1);
				break;
			case 0x1b:
				move_rook(out, 3, 0, 4);
				break;
			case 0x1d:
				move_queen(out, 2, 5, 0);
				break;
			case 0x1f:
				move_queen(out, 2, 4, 4);
				break;
			case 0x20:
				move_queen(out, 2, 2, 6);
				break;
			case 0x21:
				move_queen(out, 1, 4, 0);
				break;
			case 0x23:
				move_queen(out, 3, 0, 7);
				break;
			case 0x24:
				move_queen(out, 1, 6, 6);
				break;
			case 0x26:
				move_rook(out, 1, 3, 0);
				break;
			case 0x27:
				move_knight(out, 3, 2, 7);
				break;
			case 0x28:
				move_queen(out, 1, 3, 5);
				break;
			case 0x29: {
				int mv { MoveNumberLookup[(game_file.get() - count) & 0xff] };
				mv = (mv << 8) | MoveNumberLookup[(game_file.get() - count) & 0xff];
				int from { mv & 0x3f };
				int to { (mv >> 6) & 0x3f };
				int prom { (mv >> 12) & 0x3 };
				Piece *from_piece { brd.get(Position {(from >> 3) + 1, from % 8 + 1 }) };
				if (! from_piece) {
					fail("no from piece at: " + (std::string) Position { (from >> 3) + 1, from % 8 + 1 });
				}
				Position top { (to >> 3) + 1, to % 8 + 1};
				if (! top) {
					fail("no to position " + to_str(to >> 3) + ", " + to_str(to % 8));
				}
				if (from_piece->pawn() && (top.rank() == 1 || top.rank() == 8)) {
					if (top.file() != from_piece->file()) {
						out << from_piece->file_str() << 'x';
						Piece *old = brd.get(top);
						if (old) { brd.remove(old); }
					}
					from_piece->file(0);
					from_piece->rank(0);
					Piece *to_piece { nullptr };
					switch (prom) {
						case 0:
							to_piece = brd.add_queen(top);
							break;
						case 1:
							to_piece = brd.add_rook(top);
							break;
						case 2:
							to_piece = brd.add_bishop(top);
							break;
						case 3:
							to_piece = brd.add_knight(top);
							break;
						default:
							fail("unknown promotion piece " + to_str(prom));
					}
					out << (std::string) top << "=" << to_piece->name();
					if (brd.check()) {
						if (brd.mate()) {
							out << '#';
						} else {
							out << '+';
						}
					}
					break;
				}
				brd.move(out, brd.first(*from_piece), from_piece, top);
				break;
			}
			case 0x2a:
				move_queen(out, 2, 4, 4);
				break;
			case 0x2b:
				move_rook(out, 3, 0, 7);
				break;
			case 0x2c:
				move_bishop(out, 1, 5, 3);
				break;
			case 0x2d:
				move_pawn(out, 1, 0, 1);
				break;
			case 0x2e:
				move_rook(out, 1, 1, 0);
				break;
			case 0x2f:
				move_queen(out, 1, 5, 3);
				break;
			case 0x30:
				move_rook(out, 1, 5, 0);
				break;
			case 0x31:
				move_queen(out, 2, 0, 6);
				break;
			case 0x32:
				move_rook(out, 2, 6, 0);
				break;
			case 0x33:
				move_pawn(out, 8, 0, 2);
				break;
			case 0x34:
				move_knight(out, 2, 2, 7);
				break;
			case 0x35:
				move_bishop(out, 2, 1, 7);
				break;
			case 0x36:
				move_pawn(out, 5, 7, 1);
				break;
			case 0x37:
				move_bishop(out, 1, 7, 1);
				break;
			case 0x38:
				move_queen(out, 3, 3, 3);
				break;
			case 0x39:
				move_king(out, 1, 1);
				break;
			case 0x3a:
				move_pawn(out, 7, 7, 1);
				break;
			case 0x3b:
				move_bishop(out, 3, 4, 4);
				break;
			case 0x3d:
				move_knight(out, 1, 1, 2);
				break;
			case 0x3e:
				move_bishop(out, 3, 3, 5);
				break;
			case 0x3f:
				move_bishop(out, 2, 2, 2);
				break;
			case 0x40:
				move_queen(out, 3, 2, 2);
				break;
			case 0x41:
				move_bishop(out, 1, 4, 4);
				break;
			case 0x42:
				move_queen(out, 3, 0, 2);
				break;
			case 0x43:
				move_rook(out, 1, 0, 3);
				break;
			case 0x44:
				move_queen(out, 2, 1, 1);
				break;
			case 0x45:
				move_bishop(out, 3, 3, 3);
				break;
			case 0x46:
				move_bishop(out, 3, 4, 4);
				break;
			case 0x47:
				move_king(out, -1, 1);
				break;
			case 0x48:
				move_queen(out, 1, 2, 6);
				break;
			case 0x49:
				move_king(out, 0, 1);
				break;
			case 0x4a:
				move_knight(out, 1, 2, 7);
				break;
			case 0x4b:
				move_queen(out, 2, 7, 7);
				break;
			case 0x4d:
				move_queen(out, 1, 1, 1);
				break;
			case 0x4e:
				move_rook(out, 1, 0, 1);
				break;
			case 0x4f:
				move_queen(out, 3, 4, 0);
				break;
			case 0x50:
				move_queen(out, 2, 0, 3);
				break;
			case 0x51:
				move_bishop(out, 3, 1, 1);
				break;
			case 0x52:
				move_rook(out, 2, 7, 0);
				break;
			case 0x53:
				move_queen(out, 1, 0, 4);
				break;
			case 0x54:
				move_queen(out, 3, 3, 0);
				break;
			case 0x55:
				move_bishop(out, 1, 3, 5);
				break;
			case 0x56:
				move_bishop(out, 3, 5, 5);
				break;
			case 0x57:
				move_queen(out, 1, 7, 0);
				break;
			case 0x58:
				move_knight(out, 1, 2, 1);
				break;
			case 0x59:
				move_queen(out, 3, 4, 4);
				break;
			case 0x5a:
				move_queen(out, 1, 6, 2);
				break;
			case 0x5b:
				move_queen(out, 2, 3, 5);
				break;
			case 0x5c:
				move_queen(out, 2, 1, 0);
				break;
			case 0x5d:
				move_king(out, 1, -1);
				break;
			case 0x5e:
				move_bishop(out, 2, 6, 6);
				break;
			case 0x5f:
				move_knight(out, 2, 6, 1);
				break;
			case 0x60:
				move_queen(out, 2, 7, 1);
				break;
			case 0x61:
				move_rook(out, 1, 6, 0);
				break;
			case 0x62:
				move_queen(out, 1, 4, 4);
				break;
			case 0x63:
				move_rook(out, 1, 0, 5);
				break;
			case 0x64:
				move_pawn(out, 2, 0, 1);
				break;
			case 0x66:
				move_bishop(out, 3, 2, 6);
				break;
			case 0x67:
				move_queen(out, 2, 1, 7);
				break;
			case 0x68:
				move_rook(out, 2, 0, 3);
				break;
			case 0x69:
				move_rook(out, 3, 6, 0);
				break;
			case 0x6a:
				move_queen(out, 3, 6, 2);
				break;
			case 0x6b:
				move_queen(out, 1, 0, 6);
				break;
			case 0x6c:
				move_queen(out, 3, 7, 7);
				break;
			case 0x6d:
				move_bishop(out, 2, 3, 5);
				break;
			case 0x6e:
				move_queen(out, 1, 4, 4);
				break;
			case 0x6f:
				move_rook(out, 1, 7, 0);
				break;
			case 0x70:
				move_pawn(out, 2, 1, 1);
				break;
			case 0x71:
				move_bishop(out, 2, 4, 4);
				break;
			case 0x72:
				move_queen(out, 3, 7, 0);
				break;
			case 0x73:
				move_bishop(out, 2, 5, 5);
				break;
			case 0x74:
				move_rook(out, 3, 5, 0);
				break;
			case 0x75:
				move_knight(out, 2, 6, 7);
				break;
			case 0x76:
				brd.kingside_rochade(out);
				break;
			case 0x77:
				move_rook(out, 2, 0, 6);
				break;
			case 0x78:
				move_bishop(out, 2, 7, 7);
				break;
			case 0x79:
				move_queen(out, 1, 1, 0);
				break;
			case 0x7a:
				move_queen(out, 3, 2, 0);
				break;
			case 0x7b:
				move_pawn(out, 3, 0, 1);
				break;
			case 0x7c:
				move_bishop(out, 1, 6, 6);
				break;
			case 0x7d:
				move_pawn(out, 6, 1, 1);
				break;
			case 0x7e:
				move_queen(out, 2, 6, 0);
				break;
			case 0x7f:
				move_queen(out, 1, 0, 5);
				break;
			case 0x80:
				move_queen(out, 2, 2, 2);
				break;
			case 0x81:
				move_rook(out, 3, 0, 1);
				break;
			case 0x82:
				move_rook(out, 3, 0, 2);
				break;
			case 0x83:
				move_queen(out, 2, 5, 5);
				break;
			case 0x84:
				move_pawn(out, 5, 0, 1);
				break;
			case 0x85:
				move_pawn(out, 3, 7, 1);
				break;
			case 0x86:
				move_queen(out, 3, 1, 7);
				break;
			case 0x87:
				move_queen(out, 3, 5, 5);
				break;
			case 0x88:
				move_rook(out, 1, 4, 0);
				break;
			case 0x89:
				move_knight(out, 2, 1, 6);
				break;
			case 0x8b:
				move_rook(out, 2, 3, 0);
				break;
			case 0x8c:
				move_queen(out, 3, 4, 4);
				break;
			case 0x8d:
				move_queen(out, 1, 0, 7);
				break;
			case 0x8e:
				move_pawn(out, 1, 1, 1);
				break;
			case 0x8f:
				move_rook(out, 3, 1, 0);
				break;
			case 0x90:
				move_pawn(out, 4, 1, 1);
				break;
			case 0x91:
				move_bishop(out, 3, 6, 6);
				break;
			case 0x92:
				move_queen(out, 2, 5, 3);
				break;
			case 0x93:
				move_bishop(out, 2, 4, 4);
				break;
			case 0x94:
				move_queen(out, 2, 0, 2);
				break;
			case 0x95:
				move_queen(out, 2, 2, 0);
				break;
			case 0x96:
				move_queen(out, 1, 7, 7);
				break;
			case 0x97:
				move_bishop(out, 1, 2, 2);
				break;
			case 0x98:
				move_rook(out, 2, 5, 0);
				break;
			case 0x99:
				move_queen(out, 1, 5, 0);
				break;
			case 0x9a:
				move_rook(out, 3, 0, 3);
				break;
			case 0x9b:
				move_knight(out, 3, 2, 1);
				break;
			case 0x9c:
				move_rook(out, 1, 0, 6);
				break;
			case 0x9d:
				move_rook(out, 3, 0, 5);
				break;
			case 0x9e:
				move_pawn(out, 6, 0, 2);
				break;
			case 0x9f:
				--count; --nr;
				continue;
			case 0xa0:
				move_queen(out, 2, 3, 3);
				break;
			case 0xa1:
				move_rook(out, 2, 4, 0);
				break;
			case 0xa3:
				move_knight(out, 3, 6, 1);
				break;
			case 0xa2:
				move_bishop(out, 2, 5, 3);
				break;
			case 0xa4:
				move_pawn(out, 2, 7, 1);
				break;
			case 0xa5:
				move_queen(out, 1, 0, 1);
				break;
			case 0xa6:
				move_rook(out, 2, 1, 0);
				break;
			case 0xa7:
				move_queen(out, 1, 1, 7);
				break;
			case 0xa8:
				move_queen(out, 3, 6, 0);
				break;
			case 0xa9:
				move_rook(out, 2, 0, 2);
				break;
			case 0xaa:
				break;
			case 0xab:
				move_bishop(out, 3, 1, 7);
				break;
			case 0xac:
				move_knight(out, 3, 6, 7);
				break;
			case 0xae:
				move_bishop(out, 1, 6, 2);
				break;
			case 0xb0:
				move_queen(out, 3, 0, 5);
				break;
			case 0xb1:
				move_king(out, -1, -1);
				break;
			case 0xb2:
				move_king(out, -1, 0);
				break;
			case 0xb3:
				move_bishop(out, 3, 5, 3);
				break;
			case 0xb4:
				move_queen(out, 1, 2, 2);
				break;
			case 0xb5:
				brd.queenside_rochade(out);
				break;
			case 0xb6:
				move_queen(out, 2, 6, 2);
				break;
			case 0xb7:
				move_bishop(out, 1, 2, 6);
				break;
			case 0xb8:
				move_queen(out, 1, 0, 2);
				break;
			case 0xb9:
				move_bishop(out, 3, 2, 2);
				break;
			case 0xba:
				move_knight(out, 1, 6, 7);
				break;
			case 0xbb:
				move_pawn(out, 7, 0, 1);
				break;
			case 0xbc:
				move_pawn(out, 7, 1, 1);
				break;
			case 0xbd:
				move_queen(out, 1, 5, 5);
				break;
			case 0xbe:
				move_queen(out, 1, 2, 0);
				break;
			case 0xbf:
				move_queen(out, 1, 3, 3);
				break;
			case 0xc0:
				move_knight(out, 3, 1, 2);
				break;
			case 0xc1:
				move_pawn(out, 1, 0, 2);
				break;
			case 0xc2:
				move_king(out, 0, -1);
				break;
			case 0xc3:
				move_bishop(out, 1, 5, 5);
				break;
			case 0xc4:
				move_knight(out, 2, 2, 1);
				break;
			case 0xc5:
				move_pawn(out, 4, 0, 1);
				break;
			case 0xc6:
				move_rook(out, 1, 2, 0);
				break;
			case 0xc8:
				move_bishop(out, 3, 7, 1);
				break;
			case 0xc9:
				move_knight(out, 3, 7, 6);
				break;
			case 0xca:
				move_queen(out, 2, 3, 0);
				break;
			case 0xcb:
				move_queen(out, 1, 0, 3);
				break;
			case 0xcd:
				move_rook(out, 3, 2, 0);
				break;
			case 0xce:
				move_queen(out, 3, 5, 3);
				break;
			case 0xd1:
				move_queen(out, 3, 0, 6);
				break;
			case 0xd2:
				move_queen(out, 1, 6, 0);
				break;
			case 0xd3:
				move_queen(out, 2, 4, 0);
				break;
			case 0xd4:
				move_knight(out, 1, 7, 6);
				break;
			case 0xd6:
				move_rook(out, 3, 7, 0);
				break;
			case 0xd7:
				move_rook(out, 1, 0, 4);
				break;
			case 0xd8:
				move_king(out, 1, 0);
				break;
			case 0xd9:
				move_bishop(out, 1, 4, 4);
				break;
			case 0xdb:
				move_queen(out, 3, 7, 1);
				break;
			case 0xde:
				move_pawn(out, 6, 7, 1);
				break;
			case 0xda:
				move_pawn(out, 3, 0, 2);
				break;
			case 0xdd:
				move_knight(out, 1, 1, 6);
				break;
			case 0xdf:
				move_pawn(out, 7, 0, 2);
				break;
			case 0xe0:
				move_pawn(out, 3, 1, 1);
				break;
			case 0xe1:
				move_bishop(out, 1, 3, 3);
				break;
			case 0xe2:
				move_rook(out, 2, 0, 7);
				break;
			case 0xe3:
				move_knight(out, 3, 7, 2);
				break;
			case 0xe4:
				move_bishop(out, 1, 7, 7);
				break;
			case 0xe5:
				move_queen(out, 2, 0, 1);
				break;
			case 0xe6:
				move_rook(out, 1, 0, 7);
				break;
			case 0xe7:
				move_queen(out, 3, 1, 1);
				break;
			case 0xe8:
				move_queen(out, 3, 6, 6);
				break;
			case 0xe9:
				move_knight(out, 1, 6, 1);
				break;
			case 0xea:
				move_queen(out, 2, 0, 5);
				break;
			case 0xeb:
				move_queen(out, 1, 3, 0);
				break;
			case 0xec:
				move_knight(out, 3, 1, 6);
				break;
			case 0xed:
				move_rook(out, 3, 3, 0);
				break;
			case 0xee:
				move_rook(out, 2, 0, 4);
				break;
			case 0xef:
				move_queen(out, 2, 7, 0);
				break;
			case 0xf0:
				move_queen(out, 3, 1, 0);
				break;
			case 0xf1:
				move_queen(out, 3, 3, 5);
				break;
			case 0xf2:
				move_bishop(out, 2, 2, 6);
				break;
			case 0xf3:
				move_bishop(out, 2, 6, 2);
				break;
			case 0xf4:
				move_queen(out, 3, 5, 0);
				break;
			case 0xf5:
				move_pawn(out, 1, 7, 1);
				break;
			case 0xf6:
				move_bishop(out, 2, 1, 1);
				break;
			case 0xf8:
				move_rook(out, 1, 0, 2);
				break;
			case 0xf9:
				move_pawn(out, 4, 7, 1);
				break;
			case 0xfa:
				move_knight(out, 1, 7, 2);
				break;
			case 0xfb:
				move_rook(out, 2, 0, 5);
				break;
			case 0xfc:
				move_bishop(out, 3, 6, 2);
				break;
			case 0xfe:
				move_knight(out, 2, 7, 2);
				break;
			case 0xfd:
				move_bishop(out, 3, 7, 7);
				break;
			case 0xff:
				move_pawn(out, 5, 0, 2);
				break;
			default:
				std::string code;
				const char digits[] = "0123456789abcdef";
				code += digits[ch >> 4];
				code += digits[ch & 0xf];
				fail("unknown code 0x" + code);
		}
		if (chrs + out.str().size() > 79) {
			std::cout << "\n";
			chrs = 0;
		} else {
			chrs += 1;
			std::cout << ' ';
		}
		chrs += out.str().size();
		std::cout << out.str();
		brd.switch_players();
	}
} @end(process moves)
```
