#pragma once
#include <string>
#include <string_view>

namespace spide2d {

struct hotkey {
    std::string key_name_;
    bool        shift_ {false};
    bool        alt_ {false};
    bool        strg_ {false};

    bool operator==(const hotkey &h) const {
        return key_name_ == h.key_name_ && shift_ == h.shift_ && alt_ == h.alt_ && strg_ == h.strg_;
    }
};

}  // namespace spide2d