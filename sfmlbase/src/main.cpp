#include "constants.h"

#ifndef WIN32
#include <unistd.h>
#endif

#include "ParamManager.h"
#include "helpers/WinConsoleUtils.h"
#include "Game.h"
#include "ZombieScene.h"
#include "Logger.h"
#include "helpers/BackTrace.h"

int main(int argc, char** argv) {

	Params::InitFromCmdLine(argc,argv);

#ifdef _MSC_VER
	//OpenConsole();
#endif

#ifndef WIN32
	//Make sure the working dir is the executable dir
	std::string executablePath = argv[0];
	for (int i = executablePath.size()-1; executablePath.size() > 0 && executablePath[i] != '/'; --i) {
		executablePath.erase(executablePath.begin()+i);
	}
	chdir(executablePath.c_str());
#endif

	try {
		Game* game = new Game();
        game->init();
        Scene* scene = new ZombieScene();
		game->run(scene);
		delete game;
		return EXIT_SUCCESS;
	} catch(const std::string& s) {
		Log::e(s);
		BackTrace::print();
		return EXIT_FAILURE;
	} catch(const char* s) {
		Log::e(s);
		BackTrace::print();
		return EXIT_FAILURE;
	}

}
