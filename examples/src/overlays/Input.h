#pragma once

struct InputTest : bk::Overlay {
	bk::gpu::Context& win;

	InputTest(bk::gpu::Context& w);
	
	void render() override;
};
