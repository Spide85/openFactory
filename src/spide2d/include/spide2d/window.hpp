#pragma once
#include "key.hpp"

#include <boost/signals2.hpp>
#include <nlohmann/json_fwd.hpp>

namespace spide2d {

using json = nlohmann::json;

struct window {
    template<typename slot>
    boost::signals2::connection hotkey_pressed(std::string_view action, slot &&callback_slot);

    template<typename slot>
    boost::signals2::connection hotkey_released(std::string_view action, slot &&callback_slot);

    window create(const json &settings);

protected:
    window();
};

}  // namespace spide2d
