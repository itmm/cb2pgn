
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

	bool white_is_playing { true };
	bool board[0x89] = { 0 };
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


#line 248 "index.md"

	unsigned char add(unsigned char a, unsigned char b) {
		int x { (a >> 4) + (b >> 4) };
		while (x > 8) { x -= 8; }
		int y { (a & 0xf) + (b & 0xf) };
		while (y > 8) { y -= 8; }
		return (x << 4) | y;
	}

	bool move(unsigned char &pos, unsigned char d) {
		board[pos] = false;
		pos = add(pos, d);
		bool capture { board[pos] };
		board[pos] = true;
		return capture;
	}
	bool move(unsigned char &pos, int dx, int dy) {
		return move(pos, (dx << 4) | dy);
	}

#line 271 "index.md"

	void move_king(unsigned char &pos, int dx, int dy) {
		if (! pos) { fail("no king to move"); }
		std::cout << "K";
		if (move(pos, dx, dy)) { std::cout << "x"; }
		out_pos(pos);
		std::cout << " ";
		board[pos] = true;
	}

#line 284 "index.md"

	bool valid(unsigned char pos) {
		int x { pos >> 4 };
		int y { pos & 0xf };
		return x >= 1 && x <= 8 && y >=1  && y <= 8;
	}

	unsigned char other_king() {
		return white_is_playing ? b_king : w_king;
	}

	bool in_check_row(unsigned char pos, int dx, int dy) {
		for (;;) {
			pos = pos + dx * 0x10 + dy * 0x01;
			if (! valid(pos)) { return false; }
			if (! board[pos]) { continue; }
			return other_king() == pos;
		}
	}

#line 307 "index.md"

	bool rook_checks(unsigned char pos) {
		return in_check_row(pos, +1, 0) ||
			in_check_row(pos, -1, 0) ||
			in_check_row(pos, 0, +1) ||
			in_check_row(pos, 0, -1);
	}

	bool bishop_checks(unsigned char pos) {
		return in_check_row(pos, +1, +1) ||
			in_check_row(pos, +1, -1) ||
			in_check_row(pos, -1, +1) ||
			in_check_row(pos, -1, -1);
	}

	bool queen_checks(unsigned char pos) {
		return rook_checks(pos) ||
			bishop_checks(pos);
	}

	bool is_rook_move(unsigned char from, unsigned char to, unsigned char orig) {
		if ((from >> 4) == (to >> 4)) {
			if ((from & 0xf) < (to & 0xf)) {
				for (unsigned char x = from + 0x01; x < to; x += 0x01) {
					if (x == orig) { return false; }
					if (! board[x]) { return false; }
				}
			} else {
				for (unsigned char x = to + 0x01; x < from; x += 0x01) {
					if (x == orig) { return false; }
					if (! board[x]) { return false; }
				}
			}
			return true;
		} else if ((from & 0xf) == (to & 0xf)) {
			if ((from >> 4) < (to >> 4)) {
				for (unsigned char x = from + 0x10; x < to; x += 0x10) {
					if (x == orig) { return false; }
					if (! board[x]) { return false; }
				}
			} else {
				for (unsigned char x = to + 0x10; x < from; x += 0x10) {
					if (x == orig) { return false; }
					if (! board[x]) { return false; }
				}
			}
			return true;
		} else {
			return false;
		}
	}

	bool is_bishop_move(unsigned char from, unsigned char to, unsigned char orig) {
		int dx { (to >> 4) - (from >> 4) };
		int dy { (to & 0xf) - (from & 0xf) };
		if (dx != dy && dx != -dy) { return false; }
		dx = dx > 0 ? 1 : -1;
		dy = dy > 0 ? 1 : -1;
		int d = dx * 0x10 + dy * 0x01;
		for (from += d; from != to; from += d) {
			if (from == orig) { return false; }
			if (! board[from]) { return false; }
		}
		return true;
	}

	bool is_queen_move(unsigned char from, unsigned char to, unsigned char orig) {
		return is_rook_move(from, to, orig) || is_bishop_move(from, to, orig);
	}
	void move_queen(unsigned char &pos, int dx, int dy) {
		if (! pos) { fail("no queen to move"); }
		std::cout << "Q";
		board[pos] = false;
		unsigned char orig { pos };
		bool capt { move(pos, dx, dy) };
		bool same_x { false };
		bool same_y { false };
		for (unsigned char *queens { white_is_playing ? w_queens : b_queens }; *queens; ++queens) {
			if (*queens == pos) { continue; }
			if (! is_queen_move(*queens, pos, orig)) { continue; }
			if ((*queens >> 4) == (orig >> 4)) {
				same_x = true;
			}
			if ((*queens & 0xf) == (orig & 0xf)) {
				same_y = true;
			}
		}
		if (same_y) {
			if (same_x) {
				out_pos(orig);
			} else {
				std::cout << (char) ('a' + (orig >> 4) - 1);
			}
		} else if (same_x) {
			std::cout << (char) ('0' + (orig & 0xf));
		}
		if (capt) { std::cout << "x"; }
		out_pos(pos);
		if (queen_checks(pos)) { std::cout << "+"; }
		std::cout << " ";
		board[pos] = true;
	}

#line 413 "index.md"

	void move_rook(unsigned char &pos, int dx, int dy) {
		if (! pos) { fail("no rook to move"); }
		std::cout << "R";
		board[pos] = false;
		unsigned char orig { pos };
		bool capt { move(pos, dx, dy) };
		bool same_x { false };
		bool same_y { false };
		for (unsigned char *rooks { white_is_playing ? w_rooks : b_rooks }; *rooks; ++rooks) {
			if (*rooks == pos) { continue; }
			if (! is_rook_move(*rooks, pos, orig)) { continue; }
			if ((*rooks >> 4) == (orig >> 4)) {
				same_x = true;
			}
			if ((*rooks & 0xf) == (orig & 0xf)) {
				same_y = true;
			}
		}
		if (same_y) {
			if (same_x) {
				out_pos(orig);
			} else {
				std::cout << (char) ('a' + (orig >> 4) - 1);
			}
		} else if (same_x) {
			std::cout << (char) ('0' + (orig & 0xf));
		}
		if (capt) { std::cout << "x"; }
		out_pos(pos);
		if (rook_checks(pos)) { std::cout << "+"; }
		std::cout << " ";
		board[pos] = true;
	}

#line 451 "index.md"

	void move_bishop(unsigned char &pos, int dx, int dy) {
		if (! pos) { fail("no bishop to move"); }
		std::cout << "B";
		auto orig { pos };
		bool capt { move(pos, dx, dy) };
		bool same_x { false };
		bool same_y { false };
		for (unsigned char *bishops { white_is_playing ? w_bishops : b_bishops }; *bishops; ++bishops) {
			if (*bishops == pos) { continue; }
			if (! is_bishop_move(*bishops, pos, orig)) { continue; }
			if ((*bishops >> 4) == (orig >> 4)) {
				same_x = true;
			}
			if ((*bishops & 0xf) == (orig & 0xf)) {
				same_y = true;
			}
		}
		if (same_y) {
			if (same_x) {
				out_pos(orig);
			} else {
				std::cout << (char) ('a' + (orig >> 4) - 1);
			}
		} else if (same_x) {
			std::cout << (char) ('0' + (orig & 0xf));
		}
		if (capt) { std::cout << "x"; }
		out_pos(pos);
		if (bishop_checks(pos)) { std::cout << "+"; }
		std::cout << " ";
		board[pos] = true;
	}

#line 488 "index.md"

	bool check_chess(unsigned char pos, unsigned char kp) {
		return valid(pos) && pos == kp;
	}
	bool knight_checks(unsigned char pos) {
		auto kp { other_king() };
		return check_chess(pos + 0x10 + 0x02, kp) ||
			check_chess(pos + 0x10 - 0x02, kp) ||
			check_chess(pos - 0x10 + 0x02, kp) ||
			check_chess(pos - 0x10 - 0x02, kp) ||
			check_chess(pos + 0x20 + 0x01, kp) ||
			check_chess(pos + 0x20 - 0x01, kp) ||
			check_chess(pos - 0x20 + 0x01, kp) ||
			check_chess(pos - 0x20 - 0x01, kp);
	}

	void move_knight(unsigned char &pos, int dx, int dy) {
		if (! pos) { fail("no knight to move"); }
		std::cout << "N";
		board[pos] = false;
		out_pos(pos);
		if (move(pos, dx, dy)) { std::cout << "x"; }
		out_pos(pos);
		if (knight_checks(pos)) { std::cout << "+"; }
		std::cout << " ";
		board[pos] = true;
	}

#line 519 "index.md"

	void move_pawn(unsigned char &pos, int dx, int dy) {
		if (! pos) { fail("no pawn to move"); }
		if (! white_is_playing) { dy = 8 - dy; dx = (8 - dx) % 8; }
		board[pos] = false;
		if (dx) { std::cout << (char) ('a' + (pos >> 4) - 1) << "x"; }
		move(pos, dx, dy);
		out_pos(pos);
		if (white_is_playing) {
			if (b_king == pos + 0x10 + 1 || b_king == pos - 0x10 + 1) {
				std::cout << '+';
			}
		} else {
			if (w_king == pos + 0x10 - 1 || w_king == pos - 0x10 - 1) {
				std::cout << '+';
			}
		}
		std::cout << " ";
		board[pos] = true;
		// TODO: e.p.
	}

#line 544 "index.md"

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
		board[king] = board[*rook] = false;
		*rook = king + 0x10;
		board[*rook] = true;
		king = *rook + 0x10;
		board[king] = true;
		std::cout << "O-O ";
	}

#line 568 "index.md"

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
		board[king] = board[*rook] = false;
		*rook = king - 0x10;
		board[*rook] = true;
		king = *rook - 0x10;
		board[king] = true;
		std::cout << "O-O-O ";
	}

#line 592 "index.md"

	void add_board(unsigned char *pos) {
		for (; *pos; ++pos) {
			board[*pos] = true;
		}
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

	
#line 602 "index.md"
 {
	if (! game_file.seekg(get_int(main_entry + 1), std::ifstream::beg)) {
		fail("can't move to game at " + to_str(get_int(main_entry + 1)));
	}
	int len { get_int(game_file) };
	if (len & 0x80000000) {
		fail("unknown game data of length " + to_str(len));
	}
	if (len & 0x40000000) {
		len &= 0x3fffffff;
		std::cout << "[SetUp \"1\"]\n";
		std::cout << "[FEN \"xx\"]\n";
		for (int i { 28 }; i; --i) { game_file.get(); }
	}

	board[w_king] = board[b_king] = true;
	add_board(w_pawns);
	add_board(b_pawns);
	add_board(w_rooks);
	add_board(b_rooks);
	add_board(w_queens);
	add_board(b_queens);
	add_board(w_bishops);
	add_board(b_bishops);
	add_board(w_knights);
	add_board(b_knights);

	std::cout << "\n";
	for (int count { 0 }; len; ++count, --len, white_is_playing = ! white_is_playing) {
		int ch = (game_file.get() - count) & 0xff;
		// std::cout << '{' << std::hex << (int) ch << std::dec << "} ";
		if (ch == 0x0c) {
			--count; continue;
		}
		if (! (count & 1)) {
			std::cout << (1 + (count >> 1)) << ". ";
		}
		auto &king { white_is_playing ? w_king : b_king };
		auto &pawns { white_is_playing ? w_pawns : b_pawns };
		auto &rooks { white_is_playing ? w_rooks : b_rooks };
		auto &queens { white_is_playing ? w_queens : b_queens };
		auto &bishops { white_is_playing ? w_bishops : b_bishops };
		auto &knights { white_is_playing ? w_knights : b_knights };

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
				move_pawn(pawns[5], 0, 1);
				break;
			case 0x0b:
				move_pawn(pawns[3], 0, 2);
				break;
			case 0x0e:
				move_knight(knights[1], 1, 2);
				break;
			case 0x12:
				move_pawn(pawns[7], 0, 1);
				break;
			case 0x13:
				move_pawn(pawns[7], 1, 1);
				break;
			case 0x14:
				move_rook(rooks[1], 0, 1);
				break;
			case 0x15:
				move_pawn(pawns[4], 1, 1);
			case 0x16:
				move_bishop(bishops[1], 7, 1);
				break;
			case 0x17:
				move_pawn(pawns[1], 0, 2);
				break;
			case 0x18:
				move_queen(queens[0], 7, 1);
				break;
			case 0x19:
				move_pawn(pawns[7], 7, 1);
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
				move_pawn(pawns[0], 0, 1);
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
				move_pawn(pawns[7], 0, 2);
				break;
			case 0x34:
				move_knight(knights[1], 2, 7);
				break;
			case 0x35:
				move_bishop(bishops[1], 1, 7);
				break;
			case 0x36:
				move_pawn(pawns[4], 7, 1);
				break;
			case 0x37:
				move_bishop(bishops[0], 7, 1);
				break;
			case 0x39:
				move_king(king, 1, 1);
				break;
			case 0x3a:
				move_pawn(pawns[6], 7, 1);
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
				move_pawn(pawns[1], 0, 1);
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
				move_pawn(pawns[1], 1, 1);
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
				move_pawn(pawns[2], 0, 1);
				break;
			case 0x7c:
				move_bishop(bishops[0], 6, 6);
				break;
			case 0x7d:
				move_pawn(pawns[5], 1, 1);
				break;
			case 0x7f:
				move_queen(queens[0], 0, 5);
				break;
			case 0x84:
				move_pawn(pawns[4], 0, 1);
				break;
			case 0x85:
				move_pawn(pawns[2], 7, 1);
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
				move_pawn(pawns[0], 1, 1);
				break;
			case 0x90:
				move_pawn(pawns[3], 1, 1);
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
				move_pawn(pawns[5], 0, 2);
				break;
			case 0xa1:
				move_rook(rooks[1], 4, 0);
				break;
			case 0xa2:
				move_bishop(bishops[1], 5, 3);
				break;
			case 0xa4:
				move_pawn(pawns[1], 7, 1);
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
				move_pawn(pawns[6], 0, 1);
				break;
			case 0xbc:
				move_pawn(pawns[6], 1, 1);
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
				move_pawn(pawns[0], 0, 2);
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
				move_pawn(pawns[3], 0, 1);
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
				move_pawn(pawns[5], 7, 1);
				break;
			case 0xda:
				move_pawn(pawns[2], 0, 2);
				break;
			case 0xdd:
				move_knight(knights[0], 1, 6);
				break;
			case 0xdf:
				move_pawn(pawns[6], 0, 2);
				break;
			case 0xe0:
				move_pawn(pawns[2], 1, 1);
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
				move_pawn(pawns[0], 7, 1);
				break;
			case 0xf6:
				move_bishop(bishops[1], 1, 1);
				break;
			case 0xf8:
				move_rook(rooks[0], 0, 2);
				break;
			case 0xf9:
				move_pawn(pawns[3], 7, 1);
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
				move_pawn(pawns[4], 0, 2);
				break;
			default:
				std::cout << "xx {0x" << std::hex << (int) ch << std::dec << "} ";
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
