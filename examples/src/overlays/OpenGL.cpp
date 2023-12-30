#include "../pch.h"
#include "OpenGL.h"

OpenGLTest::OpenGLTest(bk::gpu::opengl::GLFWOpenGLContext& win) : win(win) {
	r = gl::Renderer();
	//shaders = new bk::archive::Archive("shaders.ar");
}

void OpenGLTest::render() {
	r.clear();

	static float pos[] = {
		/* vertices */ /* texcoords */
		-0.5f, -0.5f,	0.0f, 0.0f,
		 0.5f, -0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f,	1.0f, 1.0f,
		-0.5f,  0.5f,	0.0f, 1.0f,
	};

	static uint32_t indecies[] = {
		0, 1, 2,
		2, 3, 0,
	};

	static bool init = false;
	static gl::VBO* vb = new gl::VBO(&pos, sizeof(pos));
	static gl::VAO* va = new gl::VAO();

	static gl::IBO* i = new gl::IBO(&indecies, sizeof(indecies), sizeof(uint32_t));
	static gl::Shader* sh;
	static gl::Texture* tex = new gl::Texture("tex.jpg", bk::gpu::PixelFormat::RGBA8);
	if (!init) {
		r.enableBlending();

		vb->add<float>(2);
		vb->add<float>(2);
		va->addVBO(*vb);

		//sh = new gl::Shader(shaders->findFile("base.vert")->getData().c_str(), shaders->findFile("base.frag")->getData().c_str());
		sh = new gl::Shader(bk::strutils::readFile("sh/base.vert").c_str(), bk::strutils::readFile("sh/base.frag").c_str());
		sh->finish();

		init = true;
	}

	tex->bind(0);
	ImGui::Begin("image debug");
	ImGui::Image((void*)(intptr_t)tex->id, ImVec2((float)tex->getSize().x, (float)tex->getSize().y));
	ImGui::End();

	r.Render(*va, *i, *sh);

	sh->USet2f("uresolution", win.getWindowSize());
	sh->USet1f("utime", static_cast<float>(glfwGetTime()));
	sh->USetTex("tex", *tex);

	//r.Flush();
}
