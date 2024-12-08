#include "VisualizerScreen.h"


#if defined(_MSC_VER)
#    pragma comment(linker, "/ENTRY:mainCRTStartup")
#endif

int main()
{
	VisualizerScreen visualizer;
	visualizer.run();
	
	return 0;
}