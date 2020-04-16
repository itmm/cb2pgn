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
		std::cout << "K";
		move (pos, dx, dy);
		out_pos(pos);
		std::cout << " ";
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
	unsigned char w_pawns[] = { 0x12, 0x22, 0x31, 0x41, 0x51, 0x61, 0x71, 0x81, 0x0 };
	unsigned char b_pawns[] = { 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77, 0x78, 0x00 };
	unsigned char w_king { 0x51 };
	unsigned char b_king { 0x58 };
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
		int ch = game_file.get() & 0xff;
		switch (ch) {
			case 0x39:
				move_king(king, 1, 1);
				break;
			case 0x49:
				move_king(king, 0, 1);
				break;
			case 0xd8:
				move_king(king, 1, 0);
				break;
			case 0x5d:
				move_king(king, 1, 7);
				break;
			case 0xb1:
				move_king(king, 7, 7);
				break;
			case 0xb2:
				move_king(king, 7, 0);
				break;
			case 0xc2:
				move_king(king, 0, 7);
				break;

			default:
				std::cout << "xx ";
		}
	}
} @end(process moves)
```
