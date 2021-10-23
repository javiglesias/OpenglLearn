#include "App.h"
#include "2DApp.h"
// TODO: Hebra que cargue modelos  
// y avise cuando acabe para renderizarlos en el siguiente frame
int main(int args, char** argv)
{
	if (args >= 2)
	{
		App2D* app_2d{};
		if (strcpy(argv[1], "2D"))
		{
			return app_2d->Run(args, argv);
		}
	}
	else
	{
		App* app{};
		return app->Run(args, argv);
	}
	return -1;
}
