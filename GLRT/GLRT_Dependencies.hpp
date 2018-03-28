#pragma once

//standard library
#include <iostream>
#include <memory>
#include <vector>
#include <string>

//glbinding
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>
#include <glbinding/gl/gl.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/extension.h>

//globjects
#include <globjects/globjects.h>
#include <globjects/logging.h>
#include <globjects/glmlogging.h>
#include <globjects/glbindinglogging.h>
#include <globjects/objectlogging.h>
#include <globjects/globjects-version.h>

//glm
#include <glm/glm.hpp>

//glfw
#define GLFW_INCLUDE_NONE //do not load own OpenGL library
#include <GLFW/glfw3.h>

//glrt
#include "Utils/Helpers.hpp"
