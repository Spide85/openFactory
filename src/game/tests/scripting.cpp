#include "../src/scripting/context.hpp"

#include <doctest.h>

SCENARIO("Scripting works") {
    GIVEN("A context") {
        of::script::context context;

        WHEN("Running test scripts") {
            THEN("All should be zero") {
                // CHECK(r.);
            }
        }
    }
}
