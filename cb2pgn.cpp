
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

#line 73 "index.md"

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

#line 107 "index.md"

	std::string to_str(int value, unsigned pad = 0) {
		std::string result { std::to_string(value) };
		while (result.size() < pad) {
			result = "0" + result;
		}
		return result;
	}

#line 145 "index.md"

	std::string to_str_not_null(
		int value,
		const std::string &placeholder
	) {
		return value ?  to_str(value,
			placeholder.size()
		) : placeholder;
	}

#line 205 "index.md"

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

#line 220 "index.md"

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
		bool any_attacks(Board &brd, const Piece &p) {
			if (can_move_to(brd, p)) {
				return true;
			}
			return next() ? next()->any_attacks(brd, p) : false;
		}
	};

	struct Side {
		std::unique_ptr<Piece> king;
		std::unique_ptr<Piece> queens;
		std::unique_ptr<Piece> rooks;
		std::unique_ptr<Piece> bishops;
		std::unique_ptr<Piece> knights;
		std::unique_ptr<Piece> pawns;
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
		bool any_attacks(Board &brd, const Piece &p) {
			if (queens && queens->any_attacks(brd, p)) {
				return true;
			}
			if (rooks && rooks->any_attacks(brd, p)) {
				return true;
			}
			if (bishops && bishops->any_attacks(brd, p)) {
				return true;
			}
			if (knights && knights->any_attacks(brd, p)) {
				return true;
			}
			return pawns->any_attacks(brd, p);
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
		void move(Piece *first, Piece *from, const Position &to);
		void switch_players() { 
			Side *tmp { other_ };
			other_ = cur_;
			cur_ = tmp;
		}
		bool white() const { return &white_ == cur_; }
		bool mate() const { return false; }
		bool check() { 
			return cur_->any_attacks(*this, *other_->king.get());
		}
		void kingside_rochade();
		void queenside_rochade();
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

	void Board::move(Piece *first, Piece *from, const Position &to) {
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
		std::cout << from->name();// << '(' << (std::string) *from << ')';

		if (from->pawn()) {
			if (captures) {
				std::cout << from->file_str();
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
					std::cout << from->file_str();
				}
				if (same_f) {
					std::cout << from->rank_str();
				}
			}
		}
		if (captures) { std::cout << "x"; }
		if (to_piece) {
			remove(to_piece);
			to_piece = nullptr;
		}
		from->file(to.file());
		from->rank(to.rank());
		std::cout << (std::string) to;
		if (check()) {
			if (mate()) {
				std::cout << '#';
			} else {
				std::cout << '+';
			}
		}
		std::cout << ' ';
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
			//std::cout << "{R" << (std::string) *this << " -> " << (std::string) to << ": ";
			if (to.file() == file() || to.rank() == rank()) {
				//std::cout << Piece::can_move_to(brd, to) << " (+)} ";
				return Piece::can_move_to(brd, to);
			}
			//std::cout << " false} ";
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
				return file() == to.file() && ! to_piece;
			} else if (rank() + 1 == to) {
				if (file() == to.file()) {
					return ! to_piece;
				};
				Position sq { (to - *this).square() };
				if (sq.file() == 1) {
					if (to_piece) { return ! to_piece->white(); }
					Piece *p { brd.get(to + Position{0, 1}) };
					return p && ! p->white() && p->pawn();
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
				return file() == to.file() && ! to_piece;
			} else if (rank() - 1 == to) {
				if (file() == to.file()) {
					return ! to_piece;
				};
				Position sq { (to - *this).square() };
				if (sq.file() == 1) {
					if (to_piece) { return to_piece->white(); }
					Piece *p { brd.get(to + Position{0, -1}) };
					return p && p->white() && p->pawn();
				}
			}
			return false;
		}
		std::string name() const override {
			return "";
		}
	};

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

#line 736 "index.md"

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

#line 775 "index.md"

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

#line 803 "index.md"

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

#line 839 "index.md"

	void test_add(const Position &p, int f, int r, const std::string &exp) {
		Position q { p + Position { f, r } };
		auto got { (std::string) q };
		if (got != exp) {
			fail("adding returns " + got + " instead of " + exp);
		}
	}

#line 860 "index.md"

	bool move_piece(Piece *(Board::*fn)(int, bool) const, int nr, int f, int r) {
		Piece *from { (brd.*fn)(nr, false) };
		if (! from) { fail("no piece to move " + std::to_string(nr)); }
		Position to { *from + Position { f, r } };
		if (to.file() > 8) { to -= Position { 8, 0 }; }
		if (to.rank() > 8) { to -= Position { 0, 8 }; }
		if (! to) {
			return false;
		}
		brd.move((brd.*fn)(1, false), from, to);
		return true;
	}

#line 877 "index.md"

	void move_king(int f, int r) {
		if (! move_piece(&Board::king, 1, f, r)) {
			fail("invalid king move");
		}
	}

#line 887 "index.md"

	void move_queen(int nr, int f, int r) {
		if (! move_piece(&Board::queen, nr, f, r)) {
			fail("invalid queen move " + std::to_string(nr));
		}
	}

#line 897 "index.md"

	void move_rook(int nr, int f, int r) {
		if (! move_piece(&Board::rook, nr, f, r)) {
			fail("invalid rook move " + std::to_string(nr));
		}
	}

#line 907 "index.md"

	void move_bishop(int nr, int f, int r) {
		if (! move_piece(&Board::bishop, nr, f, r)) {
			fail("invalid bishop move " + std::to_string(nr));
		}
	}

#line 917 "index.md"

	void move_knight(int nr, int f, int r) {
		if (! move_piece(&Board::knight, nr, f, r)) {
			fail("invalid knight move " + std::to_string(nr));
		}
	}

#line 927 "index.md"

	void move_pawn(int nr, int f, int r) {
		if (! brd.white()) {
			r = 8 - r;
			f = f ? 8 - f : 0;
		}
		if (! move_piece(&Board::pawn, nr, f, r)) {
			fail("invalid pawn move " + std::to_string(nr) + ", " + std::to_string(f) + ", " + std::to_string(r));
		}
	}

#line 941 "index.md"

	void Board::kingside_rochade() {
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
		std::cout << "O-O ";
		/*
		std::cout << "{ " << (std::string) *kp << " ";
		for (Piece *cur { cur_->rooks.get() }; cur; cur = cur->next()) {
			std::cout << (std::string) *cur << ' ';
		}
		std::cout << "} ";
		*/
	}

#line 973 "index.md"

	void Board::queenside_rochade() {
		Piece *kp { king() };
		int kf { kp->file() };
		int mf { 0 };
		Piece *r { nullptr };
		for (Piece *cur { cur_->rooks.get() }; cur; cur = cur->next()) {
			int pf { cur->file() };
			if (pf < kf && pf > mf) {
				mf = pf;
				r = cur;
			}
		}
		if (! r) {
			fail("no queenside rook found");
		}
		Position nr { *kp + Position { -1, 0 } };
		Position nk { *kp + Position { -2, 0 } };
		r->file(kf - 1);
		kp->file(kf - 2);
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
 {
	
#line 730 "index.md"
 
	Board brd;

#line 766 "index.md"

	test_1_fig(&Board::king, false, "e1");
	test_1_fig(&Board::king, true, "e8");
	test_1_fig(&Board::queen, false, "d1");
	test_1_fig(&Board::queen, true, "d8");

#line 792 "index.md"

	test_2_fig(&Board::rook, false, "a1", "h1");
	test_2_fig(&Board::rook, true, "a8", "h8");
	test_2_fig(&Board::knight, false, "b1", "g1");
	test_2_fig(&Board::knight, true, "b8", "g8");
	test_2_fig(&Board::bishop, false, "c1", "f1");
	test_2_fig(&Board::bishop, true, "c8", "f8");

#line 832 "index.md"

	test_8_fig(&Board::pawn, false, "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2");
	test_8_fig(&Board::pawn, true, "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7");

#line 851 "index.md"
 {
	const auto &wk { *brd.king() };
	test_add(wk, 0, 0, "e1");
	test_add(wk, -4, 0, "a1");
	test_add(wk, 3, 0, "h1");
} 
#line 45 "index.md"
;
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
	char main_entry[46];

#line 61 "index.md"

	if (! main_file.read(
		main_entry, sizeof(main_entry)
	)) {
		fail("can't read header of "
			"cbh file"
		);
	}

#line 88 "index.md"

	int count { get_int(main_entry + 6) };
	--count;
	std::cout << "number of games: " <<
		count << "\n";

#line 97 "index.md"

	for (; main_file.read(
		main_entry, sizeof(main_entry)
	); --count) {
		
#line 131 "index.md"
 {
	int date { get_int(
		main_entry + 24, 3
	) };
	int day { date & 0x1f };
	int month { (date & 0x1e0) >> 5 };
	int year { date >> 9 };
	if (year || month || day) {
		
#line 158 "index.md"

	std::cout << "[Date \"" << 
		to_str_not_null(year, "????") <<
		"." <<
		to_str_not_null(month, "??") <<
		"." <<
		to_str_not_null(day, "??") <<
		"\"]\n";

#line 139 "index.md"
;
	}
} 
#line 170 "index.md"
 {
	int round { main_entry[29] };
	if (round) {
		std::cout << "[Round \"" <<
			to_str(round) << "\"]\n";
	}
} 
#line 180 "index.md"

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

#line 198 "index.md"

	
#line 999 "index.md"
 {
	brd.~Board();
	new (&brd) Board();
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

	std::cout << "\n";
	for (int count { 0 }, nr { 0 }; len; ++count, ++nr, --len) {
		int ch { (game_file.get() - count) & 0xff };
		if (ch == 0x0c) {
			break;
		}
		if (! (nr & 1)) {
			std::cout << (1 + (nr >> 1)) << ". ";
		}
		//std::cout << '[' << count << ", " << len << ", " << nr << "] ";
		//std::cout << '{' << std::hex << ch << std::dec << "} ";

		switch (ch) {
			case 0x02:
				move_bishop(1, 1, 1);
				break;
			case 0x05:
				move_rook(2, 2, 0);
				break;
			case 0x06:
				move_bishop(1, 1, 7);
				break;
			case 0x07:
				move_knight(2, 7, 6);
				break;
			case 0x08:
				move_bishop(2, 3, 3);
				break;
			case 0x09:
				move_pawn(6, 0, 1);
				break;
			case 0x0b:
				move_pawn(4, 0, 2);
				break;
			case 0x0e:
				move_knight(2, 1, 2);
				break;
			case 0x12:
				move_pawn(8, 0, 1);
				break;
			case 0x13:
				move_pawn(8, 1, 1);
				break;
			case 0x14:
				move_rook(2, 0, 1);
				break;
			case 0x15:
				move_pawn(5, 1, 1);
				break;
			case 0x16:
				move_bishop(2, 7, 1);
				break;
			case 0x17:
				move_pawn(2, 0, 2);
				break;
			case 0x18:
				move_queen(1, 7, 1);
				break;
			case 0x19:
				move_pawn(8, 7, 1);
				break;
			case 0x21:
				move_queen(1, 4, 0);
				break;
			case 0x24:
				move_queen(1, 6, 6);
				break;
			case 0x26:
				move_rook(1, 3, 0);
				break;
			case 0x28:
				move_queen(1, 3, 5);
				break;
			case 0x2c:
				move_bishop(1, 5, 3);
				break;
			case 0x2d:
				move_pawn(1, 0, 1);
				break;
			case 0x2e:
				move_rook(1, 1, 0);
				break;
			case 0x2f:
				move_queen(1, 5, 3);
				break;
			case 0x30:
				move_rook(1, 5, 0);
				break;
			case 0x32:
				move_rook(2, 6, 0);
				break;
			case 0x33:
				move_pawn(8, 0, 2);
				break;
			case 0x34:
				move_knight(2, 2, 7);
				break;
			case 0x35:
				move_bishop(2, 1, 7);
				break;
			case 0x36:
				move_pawn(5, 7, 1);
				break;
			case 0x37:
				move_bishop(1, 7, 1);
				break;
			case 0x39:
				move_king(1, 1);
				break;
			case 0x3a:
				move_pawn(7, 7, 1);
				break;
			case 0x3d:
				move_knight(1, 1, 2);
				break;
			case 0x3f:
				move_bishop(2, 2, 2);
				break;
			case 0x41:
				move_bishop(1, 4, 4);
				break;
			case 0x43:
				move_rook(1, 0, 3);
				break;
			case 0x47:
				move_king(-1, 1);
				break;
			case 0x48:
				move_queen(1, 2, 6);
				break;
			case 0x49:
				move_king(0, 1);
				break;
			case 0x4a:
				move_knight(1, 2, 7);
				break;
			case 0x4d:
				move_queen(1, 1, 1);
				break;
			case 0x4e:
				move_rook(1, 0, 1);
				break;
			case 0x52:
				move_rook(2, 7, 0);
				break;
			case 0x53:
				move_queen(1, 0, 4);
				break;
			case 0x55:
				move_bishop(1, 3, 5);
				break;
			case 0x57:
				move_queen(1, 7, 0);
				break;
			case 0x58:
				move_knight(1, 2, 1);
				break;
			case 0x5a:
				move_queen(1, 6, 2);
				break;
			case 0x5d:
				move_king(1, -1);
				break;
			case 0x5e:
				move_bishop(2, 6, 6);
				break;
			case 0x5f:
				move_knight(2, 6, 1);
				break;
			case 0x61:
				move_rook(1, 6, 0);
				break;
			case 0x62:
				move_queen(1, 4, 4);
				break;
			case 0x63:
				move_rook(1, 0, 5);
				break;
			case 0x64:
				move_pawn(2, 0, 1);
				break;
			case 0x68:
				move_rook(2, 0, 3);
				break;
			case 0x6b:
				move_queen(1, 0, 6);
				break;
			case 0x6d:
				move_bishop(2, 3, 5);
				break;
			case 0x6e:
				move_queen(1, 4, 4);
				break;
			case 0x6f:
				move_rook(1, 7, 0);
				break;
			case 0x70:
				move_pawn(2, 1, 1);
				break;
			case 0x71:
				move_bishop(2, 4, 4);
				break;
			case 0x73:
				move_bishop(2, 5, 5);
				break;
			case 0x75:
				move_knight(2, 6, 7);
				break;
			case 0x76:
				brd.kingside_rochade();
				break;
			case 0x77:
				move_rook(2, 0, 6);
				break;
			case 0x78:
				move_bishop(2, 7, 7);
				break;
			case 0x79:
				move_queen(1, 1, 0);
				break;
			case 0x7b:
				move_pawn(3, 0, 1);
				break;
			case 0x7c:
				move_bishop(1, 6, 6);
				break;
			case 0x7d:
				move_pawn(6, 1, 1);
				break;
			case 0x7f:
				move_queen(1, 0, 5);
				break;
			case 0x84:
				move_pawn(5, 0, 1);
				break;
			case 0x85:
				move_pawn(3, 7, 1);
				break;
			case 0x88:
				move_rook(1, 4, 0);
				break;
			case 0x89:
				move_knight(2, 1, 6);
				break;
			case 0x8b:
				move_rook(2, 3, 0);
				break;
			case 0x8d:
				move_queen(1, 0, 7);
				break;
			case 0x8e:
				move_pawn(1, 1, 1);
				break;
			case 0x90:
				move_pawn(4, 1, 1);
				break;
			case 0x93:
				move_bishop(2, 4, 4);
				break;
			case 0x96:
				move_queen(1, 7, 7);
				break;
			case 0x97:
				move_bishop(1, 2, 2);
				break;
			case 0x98:
				move_rook(2, 5, 0);
				break;
			case 0x99:
				move_queen(1, 5, 0);
				break;
			case 0x9c:
				move_rook(1, 0, 6);
				break;
			case 0x9e:
				move_pawn(6, 0, 2);
				break;
			case 0xa1:
				move_rook(2, 4, 0);
				break;
			case 0xa2:
				move_bishop(2, 5, 3);
				break;
			case 0xa4:
				move_pawn(2, 7, 1);
				break;
			case 0xa5:
				move_queen(1, 0, 1);
				break;
			case 0xa6:
				move_rook(2, 1, 0);
				break;
			case 0xa7:
				move_queen(1, 1, 7);
				break;
			case 0xa9:
				move_rook(2, 0, 2);
				break;
			case 0xae:
				move_bishop(1, 6, 2);
				break;
			case 0xb1:
				move_king(-1, -1);
				break;
			case 0xb2:
				move_king(-1, 0);
				break;
			case 0xb4:
				move_queen(1, 2, 2);
				break;
			case 0xb5:
				brd.queenside_rochade();
				break;
			case 0xb7:
				move_bishop(1, 2, 6);
				break;
			case 0xb8:
				move_queen(1, 0, 2);
				break;
			case 0xd9:
				move_bishop(1, 4, 4);
				break;
			case 0xba:
				move_knight(1, 6, 7);
				break;
			case 0xbb:
				move_pawn(7, 0, 1);
				break;
			case 0xbc:
				move_pawn(7, 1, 1);
				break;
			case 0xbd:
				move_queen(1, 5, 5);
				break;
			case 0xbe:
				move_queen(1, 2, 0);
				break;
			case 0xbf:
				move_queen(1, 3, 3);
				break;
			case 0xc1:
				move_pawn(1, 0, 2);
				break;
			case 0xc2:
				move_king(0, -1);
				break;
			case 0xc3:
				move_bishop(1, 5, 5);
				break;
			case 0xc4:
				move_knight(2, 2, 1);
				break;
			case 0xc5:
				move_pawn(4, 0, 1);
				break;
			case 0xc6:
				move_rook(1, 2, 0);
				break;
			case 0xcb:
				move_queen(1, 0, 3);
				break;
			case 0xd2:
				move_queen(1, 0, 6);
				break;
			case 0xd4:
				move_knight(1, 7, 6);
				break;
			case 0xd7:
				move_rook(1, 0, 4);
				break;
			case 0xd8:
				move_king(1, 0);
				break;
			case 0xde:
				move_pawn(6, 7, 1);
				break;
			case 0xda:
				move_pawn(3, 0, 2);
				break;
			case 0xdd:
				move_knight(1, 1, 6);
				break;
			case 0xdf:
				move_pawn(7, 0, 2);
				break;
			case 0xe0:
				move_pawn(3, 1, 1);
				break;
			case 0xe1:
				move_bishop(1, 3, 3);
				break;
			case 0xe2:
				move_rook(2, 0, 7);
				break;
			case 0xe4:
				move_bishop(1, 7, 7);
				break;
			case 0xe6:
				move_rook(1, 0, 7);
				break;
			case 0xe9:
				move_knight(1, 6, 1);
				break;
			case 0xeb:
				move_queen(1, 3, 0);
				break;
			case 0xee:
				move_rook(2, 0, 4);
				break;
			case 0xf2:
				move_bishop(2, 2, 6);
				break;
			case 0xf3:
				move_bishop(2, 6, 2);
				break;
			case 0xf5:
				move_pawn(1, 7, 1);
				break;
			case 0xf6:
				move_bishop(2, 1, 1);
				break;
			case 0xf8:
				move_rook(1, 0, 2);
				break;
			case 0xf9:
				move_pawn(4, 7, 1);
				break;
			case 0xfa:
				move_knight(1, 7, 2);
				break;
			case 0xfb:
				move_rook(2, 0, 5);
				break;
			case 0xfe:
				move_knight(2, 7, 2);
				break;
			case 0xff:
				move_pawn(5, 0, 2);
				break;
			default:
				std::string code;
				const char digits[] = "0123456789abcdef";
				code += digits[ch >> 4];
				code += digits[ch & 0xf];
				fail("unknown code 0x" + code);
		}
		brd.switch_players();
	}
} 
#line 199 "index.md"
;
	std::cout << result << "\n\n";

#line 101 "index.md"
;
	}

#line 119 "index.md"

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
