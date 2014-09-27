#include <projector/ProjectorFactory.h>
#include <projector/Projector.h>
#include <common/Config.h>

#include <string>

using namespace threescanner;

int main(int argc, char* argv[]) {
	std::string type = argc > 1 ? argv[1] : "threephase";
	std::string confFilepath = argc > 2 ? argv[2] : "threescanner.json";
	std::string confChildname = argc > 3 ? argv[3] : "projector";
	Config cfg = Config(confFilepath).getChild(confChildname);
	ProjectorPtr projector = ProjectorFactory::create(type, cfg);
	auto fut = projector->start();
	fut.wait();
	return 0;
}
