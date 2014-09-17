#include "lib/engine/threephase/ThreephaseEngine.h"
#include "lib/common/Config.h"

#include <string>

using namespace threescanner;

int main(int argc, char* argv[]) {
	std::string type = argc > 1 ? argv[1] : "threephase";
	std::string confFilepath = argc > 2 ? argv[2] : "threescanner.json";
	std::string confChildname = argc > 3 ? argv[3] : "engine";
	Config cfg = Config(confFilepath).getChild(confChildname);
	if (type == "threephase") {
		ThreephaseEngine prj(cfg);
		// TODO return prj.run();
	}
	return 0;
}
