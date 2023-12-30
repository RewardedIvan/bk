#pragma once

namespace gl = bk::gpu::opengl;
struct OpenGLTest : bk::Overlay {
	gl::Renderer r;
	//bk::archive::Archive* shaders;
	bk::gpu::opengl::GLFWOpenGLContext& win;

	OpenGLTest(bk::gpu::opengl::GLFWOpenGLContext& win);

	void render() override;
};
