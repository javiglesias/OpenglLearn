#include "App.h"
#include "2DApp.h"
// TODO: Hebra que cargue modelos  
// y avise cuando acabe para renderizarlos en el siguiente frame
int main(int args, char** argv)
{
	if (args >= 2)
	{
		App2D* app_2d = new App2D();
		if (strcpy(argv[1], "2D"))
		{
			return app_2d->Run(args, argv);
		}
	}
	else
	{
		App* app = new App();
		return app->Run(args, argv);
	}
	return -1;
}
