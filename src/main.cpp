#include "App.h"
// TODO: Hebra que cargue modelos  
// y avise cuando acabe para renderizarlos en el siguiente frame
int main(int args, char** argv)
{
	App* app = new App();
	return app->Run(args, argv);
}
