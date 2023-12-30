#include "pch/pch.h"

#include "Renderer.h"
#include "utils/log.h"

namespace bk::gpu::opengl {
	Renderer::Renderer() {
		renderQue = std::queue<RendererElement*>();
	}

	Renderer::~Renderer() {
	}

	void Renderer::clear() {
		GLCheckErr(glClear(GL_COLOR_BUFFER_BIT));
	}

	void Renderer::enableBlending() {
		GLCheckErr(glEnable(GL_BLEND));
		GLCheckErr(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}

	void Renderer::push(const VAO& v, const IBO& i, const Shader& sh) {
		renderQue.push(new RendererElement{v, i, sh});
	}

	void Renderer::Render(RendererElement elem) {
		Render(elem.v, elem.i, elem.sh);
	}

	void Renderer::Render(const VAO& va, const IBO& i, const Shader& sh) {
		sh.bind();
		va.bind();
		i.bind();

		GLCheckErr(glDrawElements(GL_TRIANGLES, i.getElementCount(), GL_UNSIGNED_INT, nullptr));
		//GLCheckErr(glDrawArrays(GL_TRIANGLES, 0, i.getElementCount()));

		//sh.unbind();
		va.unbind();
		i.unbind();
	}

	void Renderer::Flush() {
		while (!renderQue.empty()) {
			Render(*renderQue.front());
			renderQue.pop();
		}
	}
}
