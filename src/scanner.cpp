#include "lib/engine/Scanner.h"
#include "lib/engine/threephase/ThreephaseEngine.h"
#include "lib/common/Config.h"

#include <string>
#include <signal.h>

using namespace threescanner;

static bool continueRunning = true;

int main(int argc, char* argv[]) {
	/* attach signal */
	signal(SIGINT, [](int) {
		continueRunning = false;
	});
	signal(SIGQUIT, [](int) {
		continueRunning = false;
	});
	signal(SIGTERM, [](int) {
		continueRunning = false;
	});
	/* configure */
	std::string type = argc > 1 ? argv[1] : "threephase";
	std::string confFilepath = argc > 2 ? argv[2] : "threescanner.json";
	std::string confChildname = argc > 3 ? argv[3] : "scanner";
	Config cfg = Config(confFilepath).getChild(confChildname);
	Engine* engine = nullptr;
	if (type == "threephase") {
		engine = new ThreephaseEngine(cfg.getChild("engine"));
	}
	Scanner scanner;
	while (continueRunning) {
		scanner.run();
	}
	delete engine;
	return 0;
}
