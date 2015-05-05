#include "core/Engine.hpp"
#include "Visualize.hpp"


#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char* argv[])
{
	engine::Engine::log.setLogLevel(engine::LOG_LEVEL_VERBOSE);
	engine::ModeHandler *visualize = new lm::Visualize();
	engine::Engine& eng = engine::Engine::create(visualize);

	eng.start();

	delete &eng;

	return 0;
}
