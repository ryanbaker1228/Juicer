#include "engine.h"

#if (DEBUG || VERBOSE)
	#include <iostream>
#endif

#include "movegen.h"


Engine::Engine()
{
	states = std::unique_ptr<std::deque<Gamestate>>(new std::deque<Gamestate>(1));
	pos.seed(STARTPOS_FEN, states->back());
}

void Engine::set_position(const std::string& fen)
{
	states = std::unique_ptr<std::deque<Gamestate>>(new std::deque<Gamestate>(1));
	pos.seed(fen, states->back());
}


void Engine::make_move(Move move)
{
	states->emplace_back();
	pos.make_move(move, states->back());
}

void Engine::undo_move(Move move)
{
	pos.undo_move(move);
	states->pop_back();
}


uint64_t Engine::perft(int depth, bool isroot, bool suppress_output)
{
	uint64_t count = 0;
	uint64_t nodes = 0;
	const bool leaf = depth == 2;

	for (const ValuedMove& m: MoveList<LEGAL>(this->pos))
	{
		if (depth <= 1)
		{
			count = 1;
			++nodes;
		}
		else
		{
			this->make_move(m);
			count = leaf ? MoveList<LEGAL>(pos).size() : perft(depth - 1, false, suppress_output);
			nodes += count;
			this->undo_move(m);
		}
		#if VERBOSE == true
			if (isroot && !suppress_output)
				std::cout << UCI::move_to_string(m) << ": " << count << std::endl;
		#endif
	}
	return nodes;
}
