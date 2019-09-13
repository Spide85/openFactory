#pragma once
#include <string>
#include <string_view>

namespace spide2d {

/**
 * @brief Represents a key.
 *
 * Address a key from a string. This string can be output to instruct the user to press a key or to read a config file
 * with keys.
 */
struct key {
    /**
     * Returns a human readable string representation of this key as utf-8.
     * \return For example "Backspace" or "d". These names may vary depending on underlying backend.
     * \todo We need a solution to convert or uniform these string on other backends.
     */
    [[nodiscard]] std::string human_readable() const;

    /**
     * @brief Creates a key corresponding to \p key_name.
     *
     * This function is the inverse of \ref human_readable().
     */
    [[nodiscard]] static key create_from_human_readable(std::string_view key_name);

private:
    key() = default;
};

}  // namespace spide2d