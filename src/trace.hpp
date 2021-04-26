#ifndef TRACE_HPP
#define TRACE_HPP

#include "camera.hpp"
#include "params.hpp"
#include "scene.hpp"
#include <utility>
#include <vector>

std::vector<std::vector<Color>> rayTrace(const PerspectiveCamera& camera, const Scene& scene,
                                         const RenderParams& params);

#endif