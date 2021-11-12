#pragma once
#include <memory>
#include <variant>
#include <vector>
#include "CircleCollider.h"
#include "RectCollider.h"
#include "LineCollider.h"

using CollVec = std::vector<std::variant<std::shared_ptr<CircleCollider>, std::shared_ptr<LineCollider>, std::shared_ptr<RectCollider>>>;

