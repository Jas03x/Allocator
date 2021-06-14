
#include <cstdio>

#include <gk/core/context/context.hpp>
#include <gk/core/graphics/graphics.hpp>

#define WIDTH 1024
#define HEIGHT 768

Context* g_Context = nullptr;

bool InitResources()
{
	bool status = true;

	g_Context = Context::CreateInstance("Automata", WIDTH, HEIGHT);
	if(g_Context == nullptr)
	{
		status = false;
		printf("error: could not create context\n");
	}

	if (status)
	{
		if (!\RenderingContext::CreateInstance())
		{
			status = false;
			printf("error: could not create rendering context\n");
		}
	}

	return status;
}

void FreeResources()
{
	RenderingContext::DeleteInstance();
	Context::DeleteInstance();
	
	g_Context = nullptr;
}

void Run()
{
	bool running = true;
	Context::Event e = {};

	while (running)
	{
		while (g_Context->PollEvent(e))
		{
			switch (e.type)
			{
				case Context::EVENT_QUIT:
				{
					running = false;
					break;
				}
				default:
				{
					break;
				}
			}
		}

		g_Context->Update();
	}
}

int main(int argc, char* argv[])
{
	int status = 0;

	if (argc != 2)
	{
		status = -1;
		printf("usage: allocator.exe <size_in_gb>\n");
	}

	if(status == 0)
	{
		if (!InitResources())
		{
			status = -1;
			printf("error: initialization failure\n");
		}
	}

	if(status == 0)
	{
		Run();
	}

	FreeResources();

	return 0;
}
