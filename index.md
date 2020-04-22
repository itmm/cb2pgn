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
@add(main)
	std::string path { argv[1] };
	std::ifstream main_file {
		(path + ".cbh").c_str(),
		std::ifstream::binary
	};
	std::ifstream game_file {
		(path + ".cbg").c_str(),
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
	if (count == 0) {
		std::cout << "DONE\n";
	} else if (count > 0) {
		fail("missing" + to_str(count) + " games");
	} else {
		fail(to_str(-count) + " surplus games");
	}
@end(main)
```

```
@def(process game) {
	int date { get_int(
		main_entry + 24, 3
	) };
	int day { date & 0x1f };
	int month { (date & 0x1e0) >> 5 };
	int year { date >> 9 };
	if (year || month || day) {
		@put(print date);
	}
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
	if (round) {
		std::cout << "[Round \"" <<
			to_str(round) << "\"]\n";
	}
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
	std::cout << result << "\n\n";
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
	void out_pos(unsigned char pos) {
		static const std::string row { "?abcdefgh???????" };
		static const std::string col { "?12345678???????" };
		std::cout << row[pos >> 4] << col[pos & 0xf];
	}
@end(globals)
```

```
@add(globals)
	void move(unsigned char &pos, int dx, int dy) {
		int x = (pos & 0xf) + dx;
		while (x > 8) { x -= 8; }
		int y = (pos >> 4) + dy;
		while (y > 8) { y -= 8; }
		pos = (y << 4) | x;
	}
@end(globals)
```

```
@add(globals)
	void move_king(unsigned char &pos, int dx, int dy) {
		if (! pos) { fail("no king to move"); }
		std::cout << "K";
		move (pos, dx, dy);
		out_pos(pos);
		std::cout << " ";
	}
@end(globals)
```

```
@add(globals)
	void move_queen(unsigned char &pos, int dx, int dy) {
		if (! pos) { fail("no queen to move"); }
		std::cout << "Q";
		move (pos, dx, dy);
		out_pos(pos);
		std::cout << " ";
	}
@end(globals)
```

```
@add(globals)
	void move_rook(unsigned char &pos, int dx, int dy) {
		if (! pos) { fail("no rook to move"); }
		std::cout << "R";
		move (pos, dx, dy);
		out_pos(pos);
		std::cout << " ";
	}
@end(globals)
```

```
@add(globals)
	void kingside_rochade(unsigned char &king, unsigned char *rooks) {
		unsigned char *rook { nullptr };
		for (; *rooks; ++rooks) {
			if ((*rooks & 0xf) == (king & 0xf) && (*rooks & 0xf0) > (king & 0xf0)) {
				if (! rook || (*rook & 0xf0) > (*rooks & 0xf0)) {
					rook = rooks;
				}
			}
		}
		if (! rook) {
			fail("no kingside rook found");
		}
		*rook = king + 0x10;
		king = *rook + 0x10;
		std::cout << "O-O ";
	}
@end(globals)
```

```
@add(globals)
	void queenside_rochade(unsigned char &king, unsigned char *rooks) {
		unsigned char *rook { nullptr };
		for (; *rooks; ++rooks) {
			if ((*rooks & 0xf) == (king & 0xf) && (*rooks & 0xf0) < (king & 0xf0)) {
				if (! rook || (*rook & 0xf0) < (*rooks & 0xf0)) {
					rook = rooks;
				}
			}
		}
		if (! rook) {
			fail("no queenside rook found");
		}
		*rook = king - 0x10;
		king = *rook - 0x10;
		std::cout << "O-O-O ";
	}
@end(globals)
```

```
@def(process moves) {
	if (! game_file.seekg(get_int(main_entry + 1), std::ifstream::beg)) {
		fail("can't move to game at " + to_str(get_int(main_entry + 1)));
	}
	int len { get_int(game_file) };
	if (len & 0x80000000) {
		fail("unknown game data of length " + to_str(len));
	}
	unsigned char w_pawns[] = { 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x72, 0x82, 0x00 };
	unsigned char b_pawns[] = { 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77, 0x87, 0x00 };
	unsigned char w_king { 0x51 };
	unsigned char b_king { 0x58 };
	unsigned char w_rooks[] = { 0x11, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char b_rooks[] = { 0x18, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char w_queens[] = { 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char b_queens[] = { 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	bool white_is_playing { true };
	if (len & 0x40000000) {
		len &= 0x3fffffff;
		std::cout << "[SetUp \"1\"]\n";
		std::cout << "[FEN \"xx\"]\n";
		for (int i { 28 }; i; --i) { game_file.get(); }
	}
	std::cout << "\n";
	for (int count { 0 }; len; ++count, --len, white_is_playing = ! white_is_playing) {
		if (! (count & 1)) {
			std::cout << (1 + (count >> 1)) << ". ";
		}
		auto &king { white_is_playing ? w_king : b_king };
		auto &pawns { white_is_playing ? w_pawns : b_pawns };
		auto &rooks { white_is_playing ? w_rooks : b_rooks };
		auto &queens { white_is_playing ? w_queens : b_queens };
		int ch = (game_file.get() - count) & 0xff;
		switch (ch) {
			case 0x05:
				move_rook(rooks[1], 2, 0);
				break;
			case 0x14:
				move_rook(rooks[1], 0, 1);
				break;
			case 0x18:
				move_queen(queens[0], 7, 1);
				break;
			case 0x21:
				move_queen(queens[0], 4, 0);
				break;
			case 0x24:
				move_queen(queens[0], 6, 6);
				break;
			case 0x26:
				move_rook(rooks[0], 3, 0);
				break;
			case 0x28:
				move_queen(queens[0], 3, 5);
				break;
			case 0x2e:
				move_rook(rooks[0], 1, 0);
				break;
			case 0x2f:
				move_queen(queens[0], 5, 3);
				break;
			case 0x30:
				move_rook(rooks[0], 5, 0);
				break;
			case 0x32:
				move_rook(rooks[1], 6, 0);
				break;
			case 0x39:
				move_king(king, 1, 1);
				break;
			case 0x43:
				move_rook(rooks[0], 0, 3);
				break;
			case 0x47:
				move_king(king, 7, 1);
				break;
			case 0x48:
				move_queen(queens[0], 2, 6);
				break;
			case 0x49:
				move_king(king, 0, 1);
				break;
			case 0x4d:
				move_queen(queens[0], 1, 1);
				break;
			case 0x4e:
				move_rook(rooks[0], 0, 1);
				break;
			case 0x52:
				move_rook(rooks[1], 7, 0);
				break;
			case 0x53:
				move_queen(queens[0], 0, 4);
				break;
			case 0x57:
				move_queen(queens[0], 7, 0);
				break;
			case 0x5a:
				move_queen(queens[0], 6, 2);
				break;
			case 0x5d:
				move_king(king, 1, 7);
				break;
			case 0x61:
				move_rook(rooks[0], 6, 0);
				break;
			case 0x62:
				move_queen(queens[0], 4, 4);
				break;
			case 0x63:
				move_rook(rooks[0], 0, 5);
				break;
			case 0x68:
				move_rook(rooks[1], 0, 3);
				break;
			case 0x6b:
				move_queen(queens[0], 0, 6);
				break;
			case 0x6e:
				move_queen(queens[0], 4, 4);
				break;
			case 0x6f:
				move_rook(rooks[0], 7, 0);
				break;
			case 0x76:
				kingside_rochade(king, rooks);
				break;
			case 0x77:
				move_rook(rooks[1], 0, 6);
				break;
			case 0x79:
				move_queen(queens[0], 1, 0);
				break;
			case 0x7f:
				move_queen(queens[0], 0, 5);
				break;
			case 0x88:
				move_rook(rooks[0], 4, 0);
				break;
			case 0x8b:
				move_rook(rooks[1], 3, 0);
				break;
			case 0x8d:
				move_queen(queens[0], 0, 7);
				break;
			case 0x96:
				move_queen(queens[0], 7, 7);
				break;
			case 0x98:
				move_rook(rooks[1], 5, 0);
				break;
			case 0x99:
				move_queen(queens[0], 5, 0);
				break;
			case 0x9c:
				move_queen(rooks[0], 0, 6);
				break;
			case 0xa1:
				move_rook(rooks[1], 4, 0);
				break;
			case 0xa5:
				move_queen(queens[0], 0, 1);
				break;
			case 0xa6:
				move_rook(rooks[1], 1, 0);
				break;
			case 0xa7:
				move_queen(queens[0], 1, 7);
				break;
			case 0xa9:
				move_rook(rooks[1], 0, 2);
				break;
			case 0xb1:
				move_king(king, 7, 7);
				break;
			case 0xb2:
				move_king(king, 7, 0);
				break;
			case 0xb4:
				move_queen(queens[0], 2, 2);
				break;
			case 0xb5:
				queenside_rochade(king, rooks);
				break;
			case 0xb8:
				move_queen(queens[0], 0, 2);
				break;
			case 0xbd:
				move_queen(queens[0], 5, 5);
				break;
			case 0xbe:
				move_queen(queens[0], 2, 0);
				break;
			case 0xbf:
				move_queen(queens[0], 3, 3);
				break;
			case 0xc2:
				move_king(king, 0, 7);
				break;
			case 0xc6:
				move_rook(rooks[0], 2, 0);
				break;
			case 0xcb:
				move_queen(queens[0], 0, 3);
				break;
			case 0xd2:
				move_queen(queens[0], 0, 6);
				break;
			case 0xd7:
				move_rook(rooks[0], 0, 4);
				break;
			case 0xd8:
				move_king(king, 1, 0);
				break;
			case 0xe2:
				move_rook(rooks[1], 0, 7);
				break;
			case 0xe6:
				move_rook(rooks[0], 0, 7);
				break;
			case 0xeb:
				move_queen(queens[0], 3, 0);
				break;
			case 0xee:
				move_rook(rooks[1], 0, 4);
				break;
			case 0xf8:
				move_rook(rooks[0], 0, 2);
				break;
			case 0xfb:
				move_rook(rooks[1], 0, 5);
				break;
			default:
				std::cout << "xx ";
		}
	}
} @end(process moves)
```
