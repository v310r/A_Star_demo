#pragma once

#ifdef __WINDOWS__
#include <Windows.h>
#endif

#include "glad/glad.h"

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <thread>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <assert.h>
#include <type_traits>
#include <random>

#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "ext/matrix_clip_space.hpp"
#include "gtc/random.hpp"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "imgui_impl_opengl3.h"

#include "SDL.h"

#include "Utilities/Logs.h"
#include "Core/Core.h"
#include "Utilities/Defines.h"
