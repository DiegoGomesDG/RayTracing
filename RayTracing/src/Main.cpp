#include "Application.h"
#include "layers/AppLayer.h"

int main(int argc, char *argv[]) {

    Core::ApplicationSpecification appSpec;
    appSpec.name                    = "RayTracer";
    appSpec.settings.width          = 1280;
    appSpec.settings.height         = 720;
    appSpec.settings.is_resizable   = false;

    Core::Application app(appSpec);

    app.push_layer<AppLayer>();
    app.run();

    return 0;
}
