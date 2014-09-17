#include "lib/engine/Engine.h"
#include "lib/common/Config.h"

#include <string>

using namespace threescanner;

int main(int argc, char* argv[]) {
	std::string confFilepath = argc > 1 ? argv[1] : "threescanner.json";
	std::string confChildname = argc > 2 ? argv[2] : "engine";
	Config cfg(confFilepath);
	auto engine = Engine::create(cfg.getChild(confChildname));
	Engine::destroy(engine);
	return 0;
}
