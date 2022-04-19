#include"App.h"


int main(int argc, char* argv[])
{
    {
        App app;
        app.initialize();
        app.run();
        app.destroy();
    }
	return 0;
}
