
#line 4 "index.md"

	
#line 14 "index.md"

	#include <cstdlib>
	#include <iostream>

#line 38 "index.md"

	#include <fstream>

#line 5 "index.md"
;
	
#line 21 "index.md"

	void fail(const std::string &msg) {
		std::cerr << msg << "\n";
		std::exit(EXIT_FAILURE);
	}

#line 71 "index.md"

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

#line 105 "index.md"

	std::string to_str(int value, unsigned pad = 0) {
		std::string result { std::to_string(value) };
		while (result.size() < pad) {
			result = "0" + result;
		}
		return result;
	}

#line 143 "index.md"

	std::string to_str_not_null(
		int value,
		const std::string &placeholder
	) {
		return value ?  to_str(value,
			placeholder.size()
		) : placeholder;
	}

#line 203 "index.md"

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

#line 218 "index.md"

	void out_pos(unsigned char pos) {
		static const std::string row { "?abcdefgh???????" };
		static const std::string col { "?12345678???????" };
		std::cout << row[pos >> 4] << col[pos & 0xf];
	}

#line 228 "index.md"

	void move(unsigned char &pos, int dx, int dy) {
		int x = (pos >> 4) + dx;
		while (x > 8) { x -= 8; }
		int y = (pos & 0xf) + dy;
		while (y > 8) { y -= 8; }
		pos = (x << 4) | y;
	}

#line 240 "index.md"

	void move_king(unsigned char &pos, int dx, int dy) {
		if (! pos) { fail("no king to move"); }
		std::cout << "K";
		out_pos(pos);
		move(pos, dx, dy);
		out_pos(pos);
		std::cout << " ";
	}

#line 253 "index.md"

	void move_queen(unsigned char &pos, int dx, int dy) {
		if (! pos) { fail("no queen to move"); }
		std::cout << "Q";
		out_pos(pos);
		move(pos, dx, dy);
		out_pos(pos);
		std::cout << " ";
	}

#line 266 "index.md"

	void move_rook(unsigned char &pos, int dx, int dy) {
		if (! pos) { fail("no rook to move"); }
		std::cout << "R";
		out_pos(pos);
		move(pos, dx, dy);
		out_pos(pos);
		std::cout << " ";
	}

#line 279 "index.md"

	void move_bishop(unsigned char &pos, int dx, int dy) {
		if (! pos) { fail("no bishop to move"); }
		std::cout << "B";
		out_pos(pos);
		move(pos, dx, dy);
		out_pos(pos);
		std::cout << " ";
	}

#line 292 "index.md"

	void move_knight(unsigned char &pos, int dx, int dy) {
		if (! pos) { fail("no knight to move"); }
		std::cout << "N";
		out_pos(pos);
		move(pos, dx, dy);
		out_pos(pos);
		std::cout << " ";
	}

#line 305 "index.md"

	void move_pawn(unsigned char &pos, int dx, int dy, bool is_white) {
		if (! pos) { fail("no pawn to move"); }
		if (! is_white) { dy = 8 - dy; dx = (8 - dx) % 8; }
		out_pos(pos);
		move(pos, dx, dy);
		out_pos(pos);
		std::cout << " ";
	}

#line 318 "index.md"

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

#line 339 "index.md"

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

#line 6 "index.md"
;
	int main(int argc, char *argv[]) {
		
#line 30 "index.md"

	if (argc != 2) {
		fail("call with cb base name");
	}

#line 44 "index.md"

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

#line 59 "index.md"

	if (! main_file.read(
		main_entry, sizeof(main_entry)
	)) {
		fail("can't read header of "
			"cbh file"
		);
	}

#line 86 "index.md"

	int count { get_int(main_entry + 6) };
	--count;
	std::cout << "number of games: " <<
		count << "\n";

#line 95 "index.md"

	for (; main_file.read(
		main_entry, sizeof(main_entry)
	); --count) {
		
#line 129 "index.md"
 {
	int date { get_int(
		main_entry + 24, 3
	) };
	int day { date & 0x1f };
	int month { (date & 0x1e0) >> 5 };
	int year { date >> 9 };
	if (year || month || day) {
		
#line 156 "index.md"

	std::cout << "[Date \"" << 
		to_str_not_null(year, "????") <<
		"." <<
		to_str_not_null(month, "??") <<
		"." <<
		to_str_not_null(day, "??") <<
		"\"]\n";

#line 137 "index.md"
;
	}
} 
#line 168 "index.md"
 {
	int round { main_entry[29] };
	if (round) {
		std::cout << "[Round \"" <<
			to_str(round) << "\"]\n";
	}
} 
#line 178 "index.md"

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

#line 196 "index.md"

	
#line 360 "index.md"
 {
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
	unsigned char w_bishops[] = { 0x31, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char b_bishops[] = { 0x38, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char w_knights[] = { 0x21, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char b_knights[] = { 0x28, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

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
		auto &bishops { white_is_playing ? w_bishops : b_bishops };
		auto &knights { white_is_playing ? w_knights : b_knights };

		int ch = (game_file.get() - count) & 0xff;
		// std::cout << '{' << std::hex << (int) ch << std::dec << "} ";
		switch (ch) {
			case 0x02:
				move_bishop(bishops[0], 1, 1);
				break;
			case 0x05:
				move_rook(rooks[1], 2, 0);
				break;
			case 0x06:
				move_bishop(bishops[0], 1, 7);
				break;
			case 0x07:
				move_knight(knights[1], 7, 6);
				break;
			case 0x08:
				move_bishop(bishops[1], 3, 3);
				break;
			case 0x09:
				move_pawn(pawns[5], 0, 1, white_is_playing);
				break;
			case 0x0b:
				move_pawn(pawns[3], 0, 2, white_is_playing);
				break;
			case 0x0e:
				move_knight(knights[1], 1, 2);
				break;
			case 0x12:
				move_pawn(pawns[7], 0, 1, white_is_playing);
				break;
			case 0x13:
				move_pawn(pawns[7], 1, 1, white_is_playing);
				break;
			case 0x14:
				move_rook(rooks[1], 0, 1);
				break;
			case 0x15:
				move_pawn(pawns[4], 1, 1, white_is_playing);
			case 0x16:
				move_bishop(bishops[1], 7, 1);
				break;
			case 0x17:
				move_pawn(pawns[1], 0, 2, white_is_playing);
				break;
			case 0x18:
				move_queen(queens[0], 7, 1);
				break;
			case 0x19:
				move_pawn(pawns[7], 7, 1, white_is_playing);
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
			case 0x2c:
				move_bishop(bishops[0], 5, 3);
				break;
			case 0x2d:
				move_pawn(pawns[0], 0, 1, white_is_playing);
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
			case 0x33:
				move_pawn(pawns[7], 0, 2, white_is_playing);
				break;
			case 0x34:
				move_knight(knights[1], 2, 7);
				break;
			case 0x35:
				move_bishop(bishops[1], 1, 7);
				break;
			case 0x36:
				move_pawn(pawns[4], 7, 1, white_is_playing);
				break;
			case 0x37:
				move_bishop(bishops[0], 7, 1);
				break;
			case 0x39:
				move_king(king, 1, 1);
				break;
			case 0x3a:
				move_pawn(pawns[6], 7, 1, white_is_playing);
				break;
			case 0x3d:
				move_knight(knights[0], 1, 2);
				break;
			case 0x3f:
				move_bishop(bishops[1], 2, 2);
				break;
			case 0x41:
				move_bishop(bishops[0], 4, 4);
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
			case 0x4a:
				move_knight(knights[0], 2, 7);
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
			case 0x55:
				move_bishop(bishops[0], 3, 5);
				break;
			case 0x57:
				move_queen(queens[0], 7, 0);
				break;
			case 0x58:
				move_knight(knights[0], 2, 1);
				break;
			case 0x5a:
				move_queen(queens[0], 6, 2);
				break;
			case 0x5d:
				move_king(king, 1, 7);
				break;
			case 0x5e:
				move_bishop(bishops[1], 6, 6);
				break;
			case 0x5f:
				move_knight(knights[1], 6, 1);
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
			case 0x64:
				move_pawn(pawns[1], 0, 1, white_is_playing);
				break;
			case 0x68:
				move_rook(rooks[1], 0, 3);
				break;
			case 0x6b:
				move_queen(queens[0], 0, 6);
				break;
			case 0x6d:
				move_bishop(bishops[1], 3, 5);
				break;
			case 0x6e:
				move_queen(queens[0], 4, 4);
				break;
			case 0x6f:
				move_rook(rooks[0], 7, 0);
				break;
			case 0x70:
				move_pawn(pawns[1], 1, 1, white_is_playing);
				break;
			case 0x71:
				move_bishop(bishops[1], 4, 4);
				break;
			case 0x73:
				move_bishop(bishops[1], 5, 5);
				break;
			case 0x75:
				move_knight(knights[1], 6, 7);
				break;
			case 0x76:
				kingside_rochade(king, rooks);
				break;
			case 0x77:
				move_rook(rooks[1], 0, 6);
				break;
			case 0x78:
				move_bishop(bishops[1], 7, 7);
				break;
			case 0x79:
				move_queen(queens[0], 1, 0);
				break;
			case 0x7b:
				move_pawn(pawns[2], 0, 1, white_is_playing);
				break;
			case 0x7c:
				move_bishop(bishops[0], 6, 6);
				break;
			case 0x7d:
				move_pawn(pawns[5], 1, 1, white_is_playing);
				break;
			case 0x7f:
				move_queen(queens[0], 0, 5);
				break;
			case 0x84:
				move_pawn(pawns[4], 0, 1, white_is_playing);
				break;
			case 0x85:
				move_pawn(pawns[2], 7, 1, white_is_playing);
				break;
			case 0x88:
				move_rook(rooks[0], 4, 0);
				break;
			case 0x89:
				move_knight(knights[1], 1, 6);
				break;
			case 0x8b:
				move_rook(rooks[1], 3, 0);
				break;
			case 0x8d:
				move_queen(queens[0], 0, 7);
				break;
			case 0x8e:
				move_pawn(pawns[0], 1, 1, white_is_playing);
				break;
			case 0x90:
				move_pawn(pawns[3], 1, 1, white_is_playing);
				break;
			case 0x93:
				move_bishop(bishops[1], 4, 4);
				break;
			case 0x96:
				move_queen(queens[0], 7, 7);
				break;
			case 0x97:
				move_bishop(bishops[0], 2, 2);
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
			case 0x9e:
				move_pawn(pawns[5], 0, 2, white_is_playing);
				break;
			case 0xa1:
				move_rook(rooks[1], 4, 0);
				break;
			case 0xa2:
				move_bishop(bishops[1], 5, 3);
				break;
			case 0xa4:
				move_pawn(pawns[1], 7, 1, white_is_playing);
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
			case 0xae:
				move_bishop(bishops[0], 6, 2);
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
			case 0xb7:
				move_bishop(bishops[0], 2, 6);
				break;
			case 0xb8:
				move_queen(queens[0], 0, 2);
				break;
			case 0xd9:
				move_bishop(bishops[0], 4, 4);
				break;
			case 0xba:
				move_knight(knights[0], 6, 7);
				break;
			case 0xbb:
				move_pawn(pawns[6], 0, 1, white_is_playing);
				break;
			case 0xbc:
				move_pawn(pawns[6], 1, 1, white_is_playing);
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
			case 0xc1:
				move_pawn(pawns[0], 0, 2, white_is_playing);
				break;
			case 0xc2:
				move_king(king, 0, 7);
				break;
			case 0xc3:
				move_bishop(bishops[0], 5, 5);
				break;
			case 0xc4:
				move_knight(knights[1], 2, 1);
				break;
			case 0xc5:
				move_pawn(pawns[3], 0, 1, white_is_playing);
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
			case 0xd4:
				move_knight(knights[0], 7, 6);
				break;
			case 0xd7:
				move_rook(rooks[0], 0, 4);
				break;
			case 0xd8:
				move_king(king, 1, 0);
				break;
			case 0xde:
				move_pawn(pawns[5], 7, 1, white_is_playing);
				break;
			case 0xda:
				move_pawn(pawns[2], 0, 2, white_is_playing);
				break;
			case 0xdd:
				move_knight(knights[0], 1, 6);
				break;
			case 0xdf:
				move_pawn(pawns[6], 0, 2, white_is_playing);
				break;
			case 0xe0:
				move_pawn(pawns[2], 1, 1, white_is_playing);
				break;
			case 0xe1:
				move_bishop(bishops[0], 3, 3);
				break;
			case 0xe2:
				move_rook(rooks[1], 0, 7);
				break;
			case 0xe4:
				move_bishop(bishops[0], 7, 7);
				break;
			case 0xe6:
				move_rook(rooks[0], 0, 7);
				break;
			case 0xe9:
				move_knight(knights[0], 6, 1);
				break;
			case 0xeb:
				move_queen(queens[0], 3, 0);
				break;
			case 0xee:
				move_rook(rooks[1], 0, 4);
				break;
			case 0xf2:
				move_bishop(bishops[1], 2, 6);
				break;
			case 0xf3:
				move_bishop(bishops[1], 6, 2);
				break;
			case 0xf5:
				move_pawn(pawns[0], 7, 1, white_is_playing);
				break;
			case 0xf6:
				move_bishop(bishops[1], 1, 1);
				break;
			case 0xf8:
				move_rook(rooks[0], 0, 2);
				break;
			case 0xf9:
				move_pawn(pawns[3], 7, 1, white_is_playing);
				break;
			case 0xfa:
				move_knight(knights[0], 7, 2);
				break;
			case 0xfb:
				move_rook(rooks[1], 0, 5);
				break;
			case 0xfe:
				move_knight(knights[1], 7, 2);
				break;
			case 0xff:
				move_pawn(pawns[4], 0, 2, white_is_playing);
				break;
			default:
				std::cout << "xx ";
		}
	}
} 
#line 197 "index.md"
;
	std::cout << result << "\n\n";

#line 99 "index.md"
;
	}

#line 117 "index.md"

	if (count == 0) {
		std::cout << "DONE\n";
	} else if (count > 0) {
		fail("missing" + to_str(count) + " games");
	} else {
		fail(to_str(-count) + " surplus games");
	}

#line 8 "index.md"
;
	}
