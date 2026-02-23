#include "Core/Application.h"

#include "AppLayer.h"
#include "Core/EditorLayer.h"

//ADD - Main Game Loop handling and more.
int main()
{
    Core::ApplicationSpecification appSpec;
    appSpec.Name = "Interloper Game Engine";
    appSpec.WindowSpec.Width = 1600;
    appSpec.WindowSpec.Height = 900;

    Core::Application application(appSpec);
    application.PushLayer<AppLayer>();          //Game Layer
    application.PushLayer<Core::EditorLayer>();        //Debug/Engine Layer
    //application.PushLayer<HUDLayer>();        //Heads up Display/Menu Layer
    application.Run();
}
