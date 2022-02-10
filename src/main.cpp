
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <string>
#include <vector>

#include <gk/core/context/context.hpp>
#include <gk/core/graphics/graphics.hpp>
#include <gk/core/graphics/quad.hpp>
#include <gk/core/graphics/texture.hpp>

#include <gk/3d/renderer/frame_renderer.hpp>

#include <windows.h>

const size_t KB = 1024;
const size_t MB = 1024 * KB;
const size_t GB = 1024 * MB;

const unsigned int WIDTH  = 512;
const unsigned int HEIGHT = 512;

const size_t g_TextureSize = 4096;
const size_t g_PixelSize = 4;
size_t g_NumTextures = 0;

size_t g_SizeOfAllocations = 0;

Context* g_Context = nullptr;
const RenderingContext* g_Renderer = nullptr;

Texture** g_TextureArray = nullptr;

FrameRenderer* g_FrameRenderer = nullptr;

unsigned int g_FrameIndex = 0;

bool InitResources()
{
	bool status = true;

	g_Context = Context::CreateInstance("Allocator", WIDTH, HEIGHT);
	if(g_Context == nullptr)
	{
		status = false;
		printf("error: could not create context\n");
	}

	if (status)
	{
		if (RenderingContext::CreateInstance())
		{
			g_Renderer = RenderingContext::GetInstance();
		}
		else
		{
			status = false;
			printf("error: could not create rendering context\n");
		}
	}

	if (status)
	{
		if (Quad::CreateInstance() == nullptr)
		{
			status = false;
			printf("error: could create quad object\n");
		}
	}

	if (status)
	{
		g_FrameRenderer = FrameRenderer::CreateInstance();
		if (g_FrameRenderer == nullptr)
		{
			status = false;
			printf("error: could not create frame renderer\n");
		}
	}

	if (status)
	{
		g_NumTextures = (g_SizeOfAllocations * GB) / (g_TextureSize * g_TextureSize * g_PixelSize);
		printf("number of textures = %llu\n", g_NumTextures);

		g_TextureArray = new Texture* [g_NumTextures];

		std::vector<uint8_t> vTextureBuffer(g_TextureSize * g_TextureSize * g_PixelSize);

		std::srand(std::time(nullptr));

		for (unsigned int i = 0; i < g_NumTextures; i++)
		{
			float r = std::rand() % 256;
			float g = std::rand() % 256;
			float b = std::rand() % 256;
			for (size_t j = 0; j < vTextureBuffer.size(); j += g_PixelSize)
			{
				vTextureBuffer[j + 0] = r;
				vTextureBuffer[j + 1] = g;
				vTextureBuffer[j + 2] = b;
				vTextureBuffer[j + 3] = 1;
			}

			g_TextureArray[i] = new Texture(GFX_RGBA, g_TextureSize, g_TextureSize, GFX_TYPE_UNSIGNED_BYTE, vTextureBuffer.data(), GFX_LINEAR, GFX_CLAMP_TO_EDGE);
		}
	}

	return status;
}

void FreeResources()
{
	if (g_TextureArray != nullptr)
	{
		for (unsigned int i = 0; i < g_NumTextures; i++)
		{
			if (g_TextureArray[i] != nullptr)
			{
				delete g_TextureArray[i];
			}
		}

		delete[] g_TextureArray;
		g_TextureArray = nullptr;
	}

	g_FrameRenderer = nullptr;

	Quad::DeleteInstance();
	FrameRenderer::DeleteInstance();

	g_Context = nullptr;
	g_Renderer = nullptr;

	RenderingContext::DeleteInstance();
	Context::DeleteInstance();
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

		unsigned int error = g_Renderer->ErrorCheck();
		if (error != 0)
		{
			printf("error: %u\n", error);
		}

		g_Renderer->ClearFramebuffer(GFX_COLOR_BUFFER_BIT);
		
		g_FrameRenderer->Bind();
		g_FrameRenderer->Render(g_TextureArray[g_FrameIndex]->GetHandle());
		g_FrameIndex = (g_FrameIndex + 1) % g_NumTextures;

		g_Context->Update();

		Sleep(1000);
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

	if (status == 0)
	{
		g_SizeOfAllocations = std::stoi(argv[1]);
		if (g_SizeOfAllocations == 0)
		{
			status = -1;
			printf("error: invalid size\n");
		}
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

	return status;
}
