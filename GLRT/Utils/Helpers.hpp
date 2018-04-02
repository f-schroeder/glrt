#pragma once

#include <globjects/Object.h>

using Buffer_ptr = std::shared_ptr<globjects::Buffer>;
using Texture_ptr = std::shared_ptr<globjects::Texture>;
using Shader_ptr = std::shared_ptr<globjects::Shader>;
using Program_ptr = std::shared_ptr<globjects::Program>;
using VertexArray_ptr = std::shared_ptr<globjects::VertexArray>;

static void DelFunc(globjects::Object * o)
{
	o->detach();
	o->destroy();
}

// Calls the provided work function and returns the number of milliseconds 
// that it takes to call that function.
template <class Function>
double time_call(Function&& f)
{
	double begin = glfwGetTime();
	f();
	return glfwGetTime() - begin;
}
