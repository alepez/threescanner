#include <scanner/Scanner.h>
#include <scanner/threephase/ThreephaseEngine.h>
#include <common/Config.h>

#include <string>
#include <signal.h>

using namespace threescanner;

static Scanner* scanner_g = 0;

int main(int argc, char* argv[]) {
	/* configure */
	std::string type = argc > 1 ? argv[1] : "threephase";
	std::string confFilepath = argc > 2 ? argv[2] : "threescanner.json";
	std::string confChildname = argc > 3 ? argv[3] : "scanner";
	Config cfg = Config(confFilepath).getChild(confChildname);
	EnginePtr engine = Engine::create(type, cfg.getChild("engine"));
	Scanner scanner(cfg, engine);
	scanner_g = &scanner;
	/* attach signal */
	signal(SIGINT, [](int) {
		scanner_g->stop();
	});
	signal(SIGQUIT, [](int) {
		scanner_g->stop();
	});
	signal(SIGTERM, [](int) {
		scanner_g->stop();
	});
	auto fut = scanner.start();
	fut.wait();
	return 0;
}
