#include <iostream>
#include <string>
#include <string_view>
#include "engine.h"
#include "evaluation.h"
#include "juicer.h"


namespace CLI
{
	static constexpr std::string_view juicer_title = "Juicer V1 by Ryan Baker";
	static constexpr std::string_view seperator = "========================================================================================================================";

	
} // namespace CLI


int main(int argc, char* argv[])
{
	std::cout << sizeof(key_t) << std::endl;
	Engine juicer;

	std::string command = std::string();
	std::string arg;

	std::cout << std::endl << CLI::seperator << std::endl;
	std::cout << CLI::juicer_title << std::endl << std::endl;

	const bool clargs = argc > 1;
	for (int i = 1; i < argc; ++i)
		command += std::string(argv[i]) + " ";

	while (true)
	{
		if (command == "")
			std::getline(std::cin, command);
		std::istringstream ss(command);
		ss >> arg;

		if (arg == "show")
		{
			std::cout << "Fen: " << FEN::string(juicer.position()) << std::endl;
			std::cout << "Key: " << juicer.position().key << std::endl;
		}
		else if (arg == "move")
			while (ss >> arg)
				juicer.make_move(UCI::string_to_move(juicer.position(), arg));
		else if (arg == "perft")
		{
			if (!(ss >> arg)) arg = "4";
			uint64_t perft = juicer.perft<true>(std::stoi(arg));
			std::cout << std::endl << "Nodes searched: " << perft << std::endl << std::endl;
		}
		else if (arg == "quit")
			break;

		command = "";

		if (clargs)
			return 0;
	}

	return 0;
}
