#include "lib/projector/threephase/ThreephaseProjector.h"
#include "lib/common/Config.h"

#include <string>

using namespace threescanner;

int main(int argc, char* argv[]) {
	std::string type = argc > 1 ? argv[1] : "threescanner.json";
	std::string confFilepath = argc > 2 ? argv[2] : "threescanner.json";
	std::string confChildname = argc > 3 ? argv[3] : "projector";
	Config cfg = Config(confFilepath).getChild(confChildname);
	if (type == "threephase") {
		ThreephaseProjector prj(cfg);
		// TODO return prj.run();
	}
	return 0;
}
