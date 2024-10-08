#ifndef MOVEGEN_H_6A20B59292CA
#define MOVEGEN_H_6A20B59292CA

#include <vector>
#include <iostream>
#include "position.h"
#include "movement.h"
#include "bitboard.h"
#include "types.h"
#include "juicer.h"


enum GenType: int
{
	LEGAL = 0,
	CAPTURES = 1,
	CHECKS = 2,
};

template<GenType Gt>
forceinline Move* enumerate(const Position& restrict pos, Move* moves) constfunc;

template<GenType Gt>
struct MoveList
{
public:
	// restricting Position& eliminates ~1500 missed gvn (global value numbering) optimizations for +1'100'000 NPS
	explicit MoveList(const Position& restrict pos): last(enumerate<Gt>(pos, moves)) {}

	const Move* begin() const { return moves; }
	const Move* end() const { return last; }
	size_t size() const { return last - moves; }

private:
	Move moves[218];
	Move* last;
}; // struct MoveList


struct GenData
{
	uint64_t ep_target = 0;

	uint64_t checkmask = Bitboard::BOARD;
	uint64_t kingban = 0;

	uint64_t bishop_pins = 0;
	uint64_t rook_pins = 0;

	inline void register_slider_check(Square ksq, Square ssq);

	template<Color Us, bool EnPassant>
	inline void register_bishop_pin(const Position& restrict pos, Square ksq, Square bsq);
	
	template<Color Us>
	inline void register_rook_pin(const Position& restrict pos, Square ksq, Square rsq);

	template<Color Us>
	inline void register_ep_pin(const Position& restrict pos, Square ksq);

	template<GenType Gt, Color Us, bool EnPassant>
	forceinline uint64_t king_attacks(const Position& restrict pos) constfunc;

	template<bool IsCheck>
	inline uint64_t const_checkmask() const constfunc;

	template<Color Us, GenType Gt>
	inline uint64_t moveable_sqs(const Position& restrict pos, uint64_t cm) const constfunc;

}; // struct GenData


// forceinlining enumerate_pawn_moves eliminates ~800 missed gvn (global value numbering) optimizations from -Rpass
// similarly to enumerate(), the compiler deems this funciton too costly to inline, however I do not
// constfunc and both restrictions have no measurable benefits on clang 15 -O3, but are kept incase other compilers would see a gain
template<GenType Gt, Color Us, bool HasEP>
forceinline constfunc Move* enumerate_pawn_moves(const Position& restrict pos, const GenData& restrict gendata, uint64_t checkmask, Move* restrict moves)
{
	constexpr Color Them = ~Us;

	if constexpr (Gt == LEGAL)
	{
		const uint64_t vertical_pawns = pos.bitboard<Us, PAWN>() & ~gendata.bishop_pins;
		const uint64_t diagonal_pawns = pos.bitboard<Us, PAWN>() & ~gendata.rook_pins;

		uint64_t w_atk_pawns = diagonal_pawns & pawn_atk_east_bb<Them>(pos.bitboard<Them>() & checkmask);
		uint64_t e_atk_pawns = diagonal_pawns & pawn_atk_west_bb<Them>(pos.bitboard<Them>() & checkmask);
		uint64_t step_pawns = vertical_pawns & pawn_step_bb<Them>(~pos.pieces);
		uint64_t push_pawns = step_pawns & Bitboard::rank_2<Us>() & pawn_push_bb<Them>(~pos.pieces & checkmask);
		step_pawns &= pawn_step_bb<Them>(checkmask);

		w_atk_pawns &= pawn_atk_east_bb<Them>(gendata.bishop_pins) | ~gendata.bishop_pins;
		e_atk_pawns &= pawn_atk_west_bb<Them>(gendata.bishop_pins) | ~gendata.bishop_pins;
		step_pawns &= pawn_step_bb<Them>(gendata.rook_pins) | ~gendata.rook_pins;
		push_pawns &= pawn_push_bb<Them>(gendata.rook_pins) | ~gendata.rook_pins;

		if constexpr (HasEP)
		{
			if (gendata.ep_target != 0)
			{
				uint64_t ep_atk_west = diagonal_pawns & shift<E>(checkmask & gendata.ep_target);
				uint64_t ep_atk_east = diagonal_pawns & shift<W>(checkmask & gendata.ep_target);

				#if (DEBUG)
					assert(Bitboard::rank_5<Us>() & gendata.ep_target);
					assert(Bitboard::rank_6<Us>() & (square_of(gendata.ep_target) + pawn_step<Us>()));
				#endif

				if (ep_atk_west | ep_atk_east)
				{
					ep_atk_west &= pawn_atk_east_bb<Them>(gendata.bishop_pins) | ~gendata.bishop_pins;
					ep_atk_east &= pawn_atk_west_bb<Them>(gendata.bishop_pins) | ~gendata.bishop_pins;

					if (ep_atk_west)
						*moves++ = Move(EN_PASSANT, square_of(ep_atk_west), square_of(gendata.ep_target) + pawn_step<Us>(), PAWN);
					if (ep_atk_east)
						*moves++ = Move(EN_PASSANT, square_of(ep_atk_east), square_of(gendata.ep_target) + pawn_step<Us>(), PAWN);
				}
			}
		}

		if ((w_atk_pawns | e_atk_pawns | step_pawns) & Bitboard::rank_7<Us>())
		{
			for (uint64_t w_promote_pawns = w_atk_pawns & Bitboard::rank_7<Us>(); w_promote_pawns; )
			{
				const Square from = pop_square(w_promote_pawns);
				const Square to = from + pawn_atk_west<Us>();
				*moves++ = Move(PROMOTION, from, to, QUEEN); 
				*moves++ = Move(PROMOTION, from, to, KNIGHT); 
				*moves++ = Move(PROMOTION, from, to, ROOK); 
				*moves++ = Move(PROMOTION, from, to, BISHOP); 
			}

			for (uint64_t e_promote_pawns = e_atk_pawns & Bitboard::rank_7<Us>(); e_promote_pawns; )
			{
				const Square from = pop_square(e_promote_pawns);
				const Square to = from + pawn_atk_east<Us>();
				*moves++ = Move(PROMOTION, from, to, QUEEN); 
				*moves++ = Move(PROMOTION, from, to, KNIGHT);
				*moves++ = Move(PROMOTION, from, to, ROOK); 
				*moves++ = Move(PROMOTION, from, to, BISHOP); 
			}

			for (uint64_t s_promote_pawns = step_pawns & Bitboard::rank_7<Us>(); s_promote_pawns; )
			{
				const Square from = pop_square(s_promote_pawns);
				const Square to = from + pawn_step<Us>();
				*moves++ = Move(PROMOTION, from, to, QUEEN); 
				*moves++ = Move(PROMOTION, from, to, KNIGHT); 
				*moves++ = Move(PROMOTION, from, to, ROOK); 
				*moves++ = Move(PROMOTION, from, to, BISHOP); 
			}

			w_atk_pawns &= ~Bitboard::rank_7<Us>();
			e_atk_pawns &= ~Bitboard::rank_7<Us>();
			step_pawns &= ~Bitboard::rank_7<Us>();
		}

		while (w_atk_pawns)
		{
			const Square s = pop_square(w_atk_pawns);
			*moves++ = Move(NORMAL, s, s + pawn_atk_west<Us>(), PAWN);
		}

		while (e_atk_pawns)
		{
			const Square s = pop_square(e_atk_pawns);
			*moves++ = Move(NORMAL, s, s + pawn_atk_east<Us>(), PAWN);
		}

		while (step_pawns)
		{
			const Square s = pop_square(step_pawns);
			*moves++ = Move(NORMAL, s, s + pawn_step<Us>(), PAWN);
		}

		while (push_pawns)
		{
			const Square s = pop_square(push_pawns);
			*moves++ = Move(NORMAL, s, s + pawn_push<Us>(), PAWN);
		}
	}
	else if constexpr (Gt == CAPTURES)
	{
		const uint64_t diagonal_pawns = pos.bitboard<Us, PAWN>() & ~gendata.rook_pins;

		uint64_t w_atk_pawns = diagonal_pawns & pawn_atk_east_bb<Them>(pos.bitboard<Them>() & checkmask);
		uint64_t e_atk_pawns = diagonal_pawns & pawn_atk_west_bb<Them>(pos.bitboard<Them>() & checkmask);
		w_atk_pawns &= pawn_atk_east_bb<Them>(gendata.bishop_pins) | ~gendata.bishop_pins;
		e_atk_pawns &= pawn_atk_west_bb<Them>(gendata.bishop_pins) | ~gendata.bishop_pins;

		if constexpr (HasEP)
		{
			if (gendata.ep_target != 0)
			{
				uint64_t ep_atk_west = diagonal_pawns & shift<E>(checkmask & gendata.ep_target);
				uint64_t ep_atk_east = diagonal_pawns & shift<W>(checkmask & gendata.ep_target);

				#if (DEBUG)
					assert(Bitboard::rank_5<Us>() & gendata.ep_target);
					assert(Bitboard::rank_6<Us>() & (square_of(gendata.ep_target) + pawn_step<Us>()));
				#endif

				if (ep_atk_west | ep_atk_east)
				{
					ep_atk_west &= pawn_atk_east_bb<Them>(gendata.bishop_pins) | ~gendata.bishop_pins;
					ep_atk_east &= pawn_atk_west_bb<Them>(gendata.bishop_pins) | ~gendata.bishop_pins;

					if (ep_atk_west)
						*moves++ = Move(EN_PASSANT, square_of(ep_atk_west), square_of(gendata.ep_target) + pawn_step<Us>(), PAWN);
					if (ep_atk_east)
						*moves++ = Move(EN_PASSANT, square_of(ep_atk_east), square_of(gendata.ep_target) + pawn_step<Us>(), PAWN);
				}
			}
		}

		if ((w_atk_pawns | e_atk_pawns) & Bitboard::rank_7<Us>())
		{
			for (uint64_t w_promote_pawns = w_atk_pawns & Bitboard::rank_7<Us>(); w_promote_pawns; )
			{
				const Square from = pop_square(w_promote_pawns);
				const Square to = from + pawn_atk_west<Us>();
				*moves++ = Move(PROMOTION, from, to, QUEEN); 
				*moves++ = Move(PROMOTION, from, to, KNIGHT); 
				*moves++ = Move(PROMOTION, from, to, ROOK); 
				*moves++ = Move(PROMOTION, from, to, BISHOP); 
			}

			for (uint64_t e_promote_pawns = e_atk_pawns & Bitboard::rank_7<Us>(); e_promote_pawns; )
			{
				const Square from = pop_square(e_promote_pawns);
				const Square to = from + pawn_atk_east<Us>();
				*moves++ = Move(PROMOTION, from, to, QUEEN); 
				*moves++ = Move(PROMOTION, from, to, KNIGHT);
				*moves++ = Move(PROMOTION, from, to, ROOK); 
				*moves++ = Move(PROMOTION, from, to, BISHOP); 
			}

			w_atk_pawns &= ~Bitboard::rank_7<Us>();
			e_atk_pawns &= ~Bitboard::rank_7<Us>();
		}

		while (w_atk_pawns)
		{
			const Square s = pop_square(w_atk_pawns);
			*moves++ = Move(NORMAL, s, s + pawn_atk_west<Us>(), PAWN);
		}

		while (e_atk_pawns)
		{
			const Square s = pop_square(e_atk_pawns);
			*moves++ = Move(NORMAL, s, s + pawn_atk_east<Us>(), PAWN);
		}
	}

	return moves;
}

// forceinlining enumerate removes a great deal of missed gvn (global value numbering) and licm remarks resulting in +200'000 NPS
// clang 15's inlining heuristic simply deemed it too costly to inline, however I am willing to pay an extra
// few dozen milliseconds of compile time. executable size does not change
// restricting Position& and GenData& have no measurable benefits with clang 15 -O3...
// however they are kept incase another compiler cannot make the same assumptions
// __attribute__((const)) i.e. constfunc also has no measurable gain but is kept for the same reason
template<GenType Gt, Boardstate State, bool IsCheck>
forceinline constfunc Move* enumerate(const Position& restrict pos, const GenData& restrict gendata, uint64_t king_atk, Move* restrict moves)
{
	constexpr Color Us = State.turn;

	const uint64_t checkmask = gendata.const_checkmask<IsCheck>();
	const uint64_t moveable_sqs = gendata.moveable_sqs<Us, Gt>(pos, checkmask);

	const Square ksq = pos.king_sq<Us>();

	// copying this var to local boosts NPS by ~4,000,000
	// compiler cannot eliminate load when we access pos.pieces
	const uint64_t pieces = pos.pieces;

	{
		while (king_atk)
			*moves++ = Move(NORMAL, ksq, pop_square(king_atk), KING);

		if constexpr (!IsCheck && State.can_castle_queenside())
			if (State.can_castle_queenside(gendata.kingban, pieces, pos.bitboard<Us, ROOK>()))
				*moves++ = Move(CASTLING, ksq, ksq + 2 * Direction::W, KING);

		if constexpr (!IsCheck && State.can_castle_kingside())
			if (State.can_castle_kingside(gendata.kingban, pieces, pos.bitboard<Us, ROOK>()))
				*moves++ = Move(CASTLING, ksq, ksq + 2 * Direction::E, KING);
	}

	moves = enumerate_pawn_moves<Gt, Us, State.has_ep_pawn>(pos, gendata, checkmask, moves);

	{
		uint64_t knights = pos.bitboard<Us, KNIGHT>() & ~(gendata.rook_pins | gendata.bishop_pins);
		while (knights)
		{
			const Square from = pop_square(knights);
			for (uint64_t to = PIECE_ATTACKS[KNIGHT][from] & moveable_sqs; to; )
				*moves++ = Move(NORMAL, from, pop_square(to), KNIGHT);
		}
	}

	uint64_t queens = pos.bitboard<Us, QUEEN>();

	{
		uint64_t bishops = pos.bitboard<Us, BISHOP>() & ~gendata.rook_pins;
		uint64_t pinned_bishops = (bishops | queens) & gendata.bishop_pins;
		uint64_t unpinned_bishops = bishops & ~gendata.bishop_pins;

		while (pinned_bishops)
		{
			const Square from = pop_square(pinned_bishops);
			uint64_t to = BISHOP_MAGICS[from][pieces] & moveable_sqs & gendata.bishop_pins;
			const PieceType slider = (queens & from ? QUEEN : BISHOP);

			while (to)
				*moves++ = Move(NORMAL, from, pop_square(to), slider);
		}

		while (unpinned_bishops)
		{
			const Square from = pop_square(unpinned_bishops);
			uint64_t to = BISHOP_MAGICS[from][pieces] & moveable_sqs;
			
			while (to)
				*moves++ = Move(NORMAL, from, pop_square(to), BISHOP);
		}
	}

	{
		uint64_t rooks = pos.bitboard<Us, ROOK>() & ~gendata.bishop_pins;
		uint64_t pinned_rooks = (rooks | queens) & gendata.rook_pins;
		uint64_t unpinned_rooks = rooks & ~gendata.rook_pins;

		while (pinned_rooks)
		{
			const Square from = pop_square(pinned_rooks);
			uint64_t to = ROOK_MAGICS[from][pieces] & moveable_sqs & gendata.rook_pins;
			const PieceType slider = (queens & from ? QUEEN : ROOK);

			while (to)
				*moves++ = Move(NORMAL, from, pop_square(to), slider);
		}

		while (unpinned_rooks)
		{
			const Square from = pop_square(unpinned_rooks);
			uint64_t to = ROOK_MAGICS[from][pieces] & moveable_sqs;

			while (to)
				*moves++ = Move(NORMAL, from, pop_square(to), ROOK);
		}
	}

	{
		for (queens &= ~(gendata.rook_pins | gendata.bishop_pins); queens; )
		{
			const Square from = pop_square(queens);

			for (uint64_t to = attacks_bb<QUEEN>(from, pieces) & moveable_sqs; to; )
				*moves++ = Move(NORMAL, from, pop_square(to), QUEEN);
		}
	}

	return moves;
}

// restriction Position& results in -9,600 missed gvn optimizations but +48 slp-vectorizer optimizations
template<GenType Gt, Boardstate State>
inline Move* enumerate(const Position& restrict pos, Move* moves)
{
	constexpr Color Us = State.turn;

	GenData gendata;
	gendata.ep_target = square_to_bb(pos.ep_target);

	uint64_t king_atk = gendata.king_attacks<Gt, Us, State.has_ep_pawn>(pos);

	if (gendata.checkmask == Bitboard::BOARD) // not in check
		moves = enumerate<Gt, State, false>(pos, gendata, king_atk, moves);
	else if (gendata.checkmask != 0) // single check
		moves = enumerate<Gt, State, true>(pos, gendata, king_atk, moves);
	else // double check, only enumerate king moves
	{
		const Square ksq = pos.king_sq<Us>();
		while (king_atk)
			*moves++ = Move(NORMAL, ksq, pop_square(king_atk), KING);
	}

	return moves;
}


inline void GenData::register_slider_check(Square ksq, Square ssq)
{
	if (checkmask == Bitboard::BOARD) // no checks found yet
		checkmask = BETWEEN_BB[ksq][ssq];
	else // already one check, hence there is a double check
		checkmask = 0;

	kingban |= CHECK_RAY_BB[ksq][ssq];
}

template<Color Us, bool EnPassant>
inline void GenData::register_bishop_pin(const Position& restrict pos, Square ksq, Square bsq)
{
	const uint64_t pinmask = BETWEEN_BB[ksq][bsq];

	if constexpr (EnPassant)
		if (pinmask & ep_target)
			ep_target = 0;

	if (pinmask & pos.bitboard<Us>()) // there lies a friendly piece in the enemy bishop's path
		bishop_pins |= pinmask;
}

template<Color Us>
inline void GenData::register_rook_pin(const Position& restrict pos, Square ksq, Square rsq)
{
	const uint64_t pinmask = BETWEEN_BB[ksq][rsq];

	if (pinmask & pos.bitboard<Us>()) // there lies a friendly piece in the enemy rook's path
		rook_pins |= pinmask;
}

template<Color Us>
inline void GenData::register_ep_pin(const Position& restrict pos, Square ksq)
{
	constexpr Color Them = ~Us;

	const uint64_t pawns = pos.bitboard<Us, PAWN>();
	const uint64_t enemy_rq = pos.bitboards<Them, ROOK, QUEEN>();

	if ((Bitboard::rank_5<Us>() & ksq) && (Bitboard::rank_5<Us>() & enemy_rq) && (Bitboard::rank_5<Us>() & pawns))
	{
		uint64_t ep_atk_west = pawns & shift<E>(ep_target);
		uint64_t ep_atk_east = pawns & shift<W>(ep_target);

		if ((ep_atk_west && (ROOK_MAGICS[ksq][pos.pieces & ~(ep_atk_west | ep_target)] & Bitboard::rank_5<Us>()) & enemy_rq)
		 || (ep_atk_east && (ROOK_MAGICS[ksq][pos.pieces & ~(ep_atk_east | ep_target)] & Bitboard::rank_5<Us>()) & enemy_rq))
			ep_target = 0;
	}
}

// forceinlining GenData::king_attacks seems to eliminate 84 -Rpass-missed=licm (loop invariant code motion) missed optimizations
// These occur inside the scope of "if (king_moves != 0)" reading from global arrays
// __attribute__((const)) and __restrict__ seem to have no measurable effect with clang 15 -O3... 
// however I keep them incase another compiler cannot make the same logical assumptions as clang
template<GenType Gt, Color Us, bool EnPassant>
forceinline constfunc uint64_t GenData::king_attacks(const Position& restrict pos)
{
	constexpr Color Them = ~Us;

	const Square ksq = pos.king_sq<Us>();
	const uint64_t king_bb = pos.bitboard<Us, KING>();

	// copying this var to local boosts NPS by ~4,000,000
	// clang cannot eliminate load when we access pos.pieces
	const uint64_t pieces = pos.pieces;

	// pawn checks
	{
		const uint64_t w_pawn_atk = pawn_atk_west_bb<Them>(pos.bitboard<Them, PAWN>());
		const uint64_t e_pawn_atk = pawn_atk_east_bb<Them>(pos.bitboard<Them, PAWN>());
		
		if (w_pawn_atk & king_bb)
			checkmask = pawn_atk_east_bb<Us>(king_bb);
		else if (e_pawn_atk & king_bb)
			checkmask = pawn_atk_west_bb<Us>(king_bb);
	}

	// knight checks
	{
		const uint64_t knight_atk = PIECE_ATTACKS[KNIGHT][ksq] & pos.bitboard<Them, KNIGHT>();
		if (knight_atk)
			checkmask = knight_atk;
	}

	// bishop checks and pins
	{
		if (PIECE_ATTACKS[BISHOP][ksq] & pos.bitboards<Them, BISHOP, QUEEN>())
		{
			uint64_t bishop_atk = BISHOP_MAGICS[ksq][pieces] & pos.bitboards<Them, BISHOP, QUEEN>();
			while (bishop_atk)
				register_slider_check(ksq, pop_square(bishop_atk));

			uint64_t bishop_pin = BISHOP_XRAY_MAGICS[ksq][pieces] & pos.bitboards<Them, BISHOP, QUEEN>();
			while (bishop_pin)
				register_bishop_pin<Us, EnPassant>(pos, ksq, pop_square(bishop_pin));
		}
	}

	// rook checks and pins
	{
		if (PIECE_ATTACKS[ROOK][ksq] & pos.bitboards<Them, ROOK, QUEEN>())
		{
			uint64_t rook_atk = ROOK_MAGICS[ksq][pieces] & pos.bitboards<Them, ROOK, QUEEN>();
			while (rook_atk)
				register_slider_check(ksq, pop_square(rook_atk));

			uint64_t rook_pin = ROOK_XRAY_MAGICS[ksq][pieces] & pos.bitboards<Them, ROOK, QUEEN>();
			while (rook_pin)
				register_rook_pin<Us>(pos, ksq, pop_square(rook_pin));
		}
	}

	if constexpr (EnPassant)
		register_ep_pin<Us>(pos, ksq);

	uint64_t king_moves = PIECE_ATTACKS[KING][ksq] & ~pos.bitboard<Us>() & ~kingban;
	if (king_moves != 0)
	{
		for (uint64_t enemy_knights = pos.bitboard<Them, KNIGHT>(); enemy_knights; )
		kingban |= PIECE_ATTACKS[KNIGHT][pop_square(enemy_knights)];

		kingban |= pawn_atk_bb<Them>(pos.bitboard<Them, PAWN>());

		for (uint64_t enemy_bishops = pos.bitboards<Them, BISHOP, QUEEN>(); enemy_bishops; )
			kingban |= BISHOP_MAGICS[pop_square(enemy_bishops)][pieces];
			
		for (uint64_t enemy_rooks = pos.bitboards<Them, ROOK, QUEEN>(); enemy_rooks; )
			kingban |= ROOK_MAGICS[pop_square(enemy_rooks)][pieces];

		kingban |= PIECE_ATTACKS[KING][pos.king_sq<Them>()];

		if constexpr (Gt == CAPTURES)
			kingban |= ~pos.bitboard<Them>();

		king_moves &= ~kingban;
	}

	return king_moves;
}

template<bool IsCheck>
inline constfunc uint64_t GenData::const_checkmask() const
{
	if constexpr (IsCheck)
		return checkmask;
	else
		return Bitboard::BOARD;
}

template<Color Us, GenType Gt>
inline constfunc uint64_t GenData::moveable_sqs(const Position& restrict pos, uint64_t cm) const
{
	if constexpr (Gt == LEGAL)
		return ~pos.bitboard<Us>() & cm;
	if constexpr (Gt == CAPTURES)
		return pos.bitboard<~Us>() & cm;
	else
		return 0;
}


// clang simply deems this function too costly to inline howver with no increase to executable size and minimal 
// increase in compile time, I do not. +100,000 NPS
// once again, restrict and constfunc produce no measurable gain on clang 15 -O3... but are kept incase other 
// compilers would benefit
template<GenType Gt>
forceinline constfunc Move* enumerate(const Position& restrict pos, Move* moves)
{
	switch (pos.boardstate_pattern())
	{
		case 0:  return enumerate<Gt, 0>(pos, moves);
		case 1:  return enumerate<Gt, 1>(pos, moves);
		case 2:  return enumerate<Gt, 2>(pos, moves);
		case 3:  return enumerate<Gt, 3>(pos, moves);
		case 4:  return enumerate<Gt, 4>(pos, moves);
		case 5:  return enumerate<Gt, 5>(pos, moves);
		case 6:  return enumerate<Gt, 6>(pos, moves);
		case 7:  return enumerate<Gt, 7>(pos, moves);
		case 8:  return enumerate<Gt, 8>(pos, moves);
		case 9:  return enumerate<Gt, 9>(pos, moves);
		case 10: return enumerate<Gt, 10>(pos, moves);
		case 11: return enumerate<Gt, 11>(pos, moves);
		case 12: return enumerate<Gt, 12>(pos, moves);
		case 13: return enumerate<Gt, 13>(pos, moves);
		case 14: return enumerate<Gt, 14>(pos, moves);
		case 15: return enumerate<Gt, 15>(pos, moves);
		case 16: return enumerate<Gt, 16>(pos, moves);
		case 17: return enumerate<Gt, 17>(pos, moves);
		case 18: return enumerate<Gt, 18>(pos, moves);
		case 19: return enumerate<Gt, 19>(pos, moves);
		case 20: return enumerate<Gt, 20>(pos, moves);
		case 21: return enumerate<Gt, 21>(pos, moves);
		case 22: return enumerate<Gt, 22>(pos, moves);
		case 23: return enumerate<Gt, 23>(pos, moves);
		case 24: return enumerate<Gt, 24>(pos, moves);
		case 25: return enumerate<Gt, 25>(pos, moves);
		case 26: return enumerate<Gt, 26>(pos, moves);
		case 27: return enumerate<Gt, 27>(pos, moves);
		case 28: return enumerate<Gt, 28>(pos, moves);
		case 29: return enumerate<Gt, 29>(pos, moves);
		case 30: return enumerate<Gt, 30>(pos, moves);
		case 31: return enumerate<Gt, 31>(pos, moves);
		case 32: return enumerate<Gt, 32>(pos, moves);
		case 33: return enumerate<Gt, 33>(pos, moves);
		case 34: return enumerate<Gt, 34>(pos, moves);
		case 35: return enumerate<Gt, 35>(pos, moves);
		case 36: return enumerate<Gt, 36>(pos, moves);
		case 37: return enumerate<Gt, 37>(pos, moves);
		case 38: return enumerate<Gt, 38>(pos, moves);
		case 39: return enumerate<Gt, 39>(pos, moves);
		case 40: return enumerate<Gt, 40>(pos, moves);
		case 41: return enumerate<Gt, 41>(pos, moves);
		case 42: return enumerate<Gt, 42>(pos, moves);
		case 43: return enumerate<Gt, 43>(pos, moves);
		case 44: return enumerate<Gt, 44>(pos, moves);
		case 45: return enumerate<Gt, 45>(pos, moves);
		case 46: return enumerate<Gt, 46>(pos, moves);
		case 47: return enumerate<Gt, 47>(pos, moves);
		case 48: return enumerate<Gt, 48>(pos, moves);
		case 49: return enumerate<Gt, 49>(pos, moves);
		case 50: return enumerate<Gt, 50>(pos, moves);
		case 51: return enumerate<Gt, 51>(pos, moves);
		case 52: return enumerate<Gt, 52>(pos, moves);
		case 53: return enumerate<Gt, 53>(pos, moves);
		case 54: return enumerate<Gt, 54>(pos, moves);
		case 55: return enumerate<Gt, 55>(pos, moves);
		case 56: return enumerate<Gt, 56>(pos, moves);
		case 57: return enumerate<Gt, 57>(pos, moves);
		case 58: return enumerate<Gt, 58>(pos, moves);
		case 59: return enumerate<Gt, 59>(pos, moves);
		case 60: return enumerate<Gt, 60>(pos, moves);
		case 61: return enumerate<Gt, 61>(pos, moves);
		case 62: return enumerate<Gt, 62>(pos, moves);
		case 63: return enumerate<Gt, 63>(pos, moves);
		default:
			std::cerr << "Error, unrecognizable Boardstate pattern: " << pos.boardstate_pattern() << '.' << std::endl;
			return nullptr;
	}
}


#endif // MOVEGEN_H_6A20B59292CA
