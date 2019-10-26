#include <doctest.h>
#include <spide2d/geometry.hpp>

SCENARIO("Rectangle is working") {
    GIVEN("A default constructed rectangle") {
        spide2d::rect<double> r;
        WHEN("Checking the values") {
            THEN("All should be zero") {
                r.translate(1, 2);
                // CHECK(r.);
            }
        }
    }
}
