#include "Core/Application.h"

#include "AppLayer.h"
#include "Core/EditorLayer.h"

//ADD - Main Game Loop handling and more.
int main()
{
    Core::ApplicationSpecification appSpec;
    appSpec.Name = "Architecture";
    appSpec.WindowSpec.Width = 1920;
    appSpec.WindowSpec.Height = 1080;

    Core::Application application(appSpec);
    application.PushLayer<AppLayer>();          //Game Layer
    application.PushLayer<Core::EditorLayer>();        //Debug/Engine Layer
    //application.PushLayer<HUDLayer>();        //Heads up Display/Menu Layer
    application.Run();
}
