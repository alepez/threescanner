#include "lib/projector/Projector.h"
#include "lib/common/Config.h"

#include <string>

using namespace threescanner;

int main(int argc, char* argv[]) {
	std::string confFilepath = argc > 1 ? argv[1] : "threescanner.json";
	std::string confChildname = argc > 2 ? argv[2] : "projector";
	Config cfg(confFilepath);
	auto projector = Projector::create(cfg.getChild(confChildname));
	Projector::destroy(projector);
	return 0;
}
