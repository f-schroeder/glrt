#pragma once

#include <globjects/Object.h>

static void DelFunc(globjects::Object * o)
{
	o->detach();
	o->destroy();
}