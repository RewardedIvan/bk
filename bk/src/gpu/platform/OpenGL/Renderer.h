#pragma once

#include "buf/VAO.h"
#include "buf/IBO.h"
#include "Shader.h"

namespace bk::gpu::opengl {
	struct RendererElement {
		VAO v;
		IBO i;
		Shader sh;
	};

	struct Renderer {
		Renderer();
		~Renderer();

		void clear();
		void enableBlending();

		void push(const VAO& v, const IBO& i, const Shader& sh);
		void Render(RendererElement elem);
		void Render(const VAO& v, const IBO& i, const Shader& sh);
		void Flush();

	private:
		std::queue<RendererElement*> renderQue;
	};
}
