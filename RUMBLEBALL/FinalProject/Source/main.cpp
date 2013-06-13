#include <windows.h>
#include "CommonHeader.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	glfwInit();

	OpenGLWindow* window = new OpenGLWindow("Final Project",1400,800);
	window->Initialize();


	glfwDisable( GLFW_AUTO_POLL_EVENTS );
    glfwEnable( GLFW_STICKY_KEYS );
    glfwEnable( GLFW_STICKY_MOUSE_BUTTONS );



	glewInit();

	//gameManager->OneTimeInit();
	window->mGameManager->OneTimeInit();

	int running = true;
	while(running)
	{

		glfwPollEvents();

		window->Update();

		window->Draw();
		
		//glfwSwapBuffers();
		
		// Check if ESC key was pressed or window was closed
        running = glfwGetWindowParam( GLFW_OPENED );
		

	}

	glfwTerminate();

	delete(window);

	return 0;
}