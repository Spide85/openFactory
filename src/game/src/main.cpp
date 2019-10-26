#include "game.hpp"

#include <iostream>
#include <spdlog/spdlog.h>
#include <spide2d/window.hpp>

#define DOCTEST_CONFIG_IMPLEMENT
#include <cstdlib>
#include <doctest.h>
#include <exception>

void run_tests(int argc, char *argv[]) {
    doctest::Context context(argc, argv);
    int              res = context.run();
    if (context.shouldExit())
        std::exit(res);
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::trace);
    spdlog::trace("C++ Standard used: {}", __cplusplus);

    run_tests(argc, argv);

    of::game g(spide2d::window::create(800, 600));
}
