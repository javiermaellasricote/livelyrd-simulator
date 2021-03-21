#include "../thirdPartyLibraries/catch2/catch.hpp"
#include "../src/DataInfrastructure/AbstractDataElement.h"

SCENARIO("Show name of DataElement", "")
{
    GIVEN("An element named 'Total Profits' owned by 'me' ")
    {
        livelyRD::AbstractDataElement totalProfits("Oil Fields", "me");

        WHEN("The name is public variable")
        {
            totalProfits.setNamePublic("me");
            THEN("The name should be retrieved")
            {
                REQUIRE(totalProfits.getName("") == "Oil Fields");
            }
        }

        WHEN("The name is private variable")
        {
            totalProfits.setNamePrivate("me");

            AND_WHEN("It is called by owner")
            {
                THEN("The name should be retrieved")
                {
                    REQUIRE(totalProfits.getName("me") == "Oil Fields");
                }
            }

            AND_WHEN("It is called by other player")
            {
                THEN("The name should NOT be retrieved")
                {
                    REQUIRE(totalProfits.getName("") == "");
                }
            }
        }
    }
}