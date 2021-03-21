#include "../thirdPartyLibraries/catch2/catch.hpp"
#include "../src/DataInfrastructure/Resource.h"

SCENARIO("Show name of Resource", "")
{
    GIVEN("A Resource named 'Oil Fields' owned by 'me' ")
    {
        livelyRD::Resource oilFields("Oil Fields", "me");

        WHEN("The name is public variable")
        {
            oilFields.setNamePublic("me");
            THEN("The name should be retrieved")
            {
                REQUIRE(oilFields.getName("") == "Oil Fields");
            }
        }

        WHEN("The name is private variable")
        {
            oilFields.setNamePrivate("me");

            AND_WHEN("It is called by owner")
            {
                THEN("The name should be retrieved")
                {
                    REQUIRE(oilFields.getName("me") == "Oil Fields");
                }
            }

            AND_WHEN("It is called by other player")
            {
                THEN("The name should NOT be retrieved")
                {
                    REQUIRE(oilFields.getName("") == "");
                }
            }
        }
    }
}


SCENARIO("Show quantity of Resource", "")
{
    GIVEN("A Resource named 'Oil Fields' owned by 'me', with quantity 3.4 ")
    {
        livelyRD::Resource oilFields("Oil Fields", "me", 3.4);

        WHEN("The quantity is public variable")
        {
            oilFields.setQuantityPublic("me");
            THEN("The quantity should be retrieved")
            {
                REQUIRE(oilFields.getQuantity("") == 3.4);
            }
        }

        WHEN("The quantity is private variable")
        {
            oilFields.setQuantityPrivate("me");

            AND_WHEN("It is called by owner")
            {
                THEN("The quantity should be retrieved")
                {
                    REQUIRE(oilFields.getQuantity("me") == 3.4);
                }
            }

            AND_WHEN("It is called by other player")
            {
                THEN("The quantity should NOT be retrieved")
                {
                    REQUIRE(oilFields.getQuantity("") == -1.0);
                }
            }
        }
    }
}

SCENARIO("Change quantity of resource", "")
{
    GIVEN("A Resource named 'Oil Fields' owned by 'me', with quantity 3.4 ")
    {
        livelyRD::Resource oilFields("Oil Fields", "me", 3.4);

        WHEN("Quantity is added by owner")
        {
            oilFields.add(5.2, "me");
            THEN("The quanity should be added")
            {
                REQUIRE(oilFields.getQuantity("me") == 8.6);
            }
        }

        WHEN("Quantity is added by other")
        {
            oilFields.add(5.2, "");

            THEN("The quanity should NOT be changed")
            {
                REQUIRE(oilFields.getQuantity("me") == 3.4);
            }
        }

        WHEN("Quantity is subtracted by owner")
        {
            oilFields.subtract(2.0, "me");

            THEN("The quanity should be subtracted")
            {
                REQUIRE(oilFields.getQuantity("me") == 1.4);
            }
        }

        WHEN("Quantity subtracted by other")
        {
            oilFields.subtract(2.0, "");

            THEN("The quanity should NOT be subtracted")
            {
                REQUIRE(oilFields.getQuantity("me") == 3.4);
            }
        }
    }
}

SCENARIO("Show cost to obtain resource", "")
{
    GIVEN("A Resource named 'Potatoes' owned by 'me', with quantity 200, and cost to obtain 0.34")
    {
        livelyRD::Resource potatoes("Potatoes", "me", 200, 0.34);

        WHEN("The cost is public variable")
        {
            potatoes.setCostToObtainPublic("me");
            THEN("The cost should be retrieved")
            {
                REQUIRE(potatoes.getCostToObtain("") == 0.34);
            }
        }

        WHEN("The cost is private variable")
        {
            potatoes.setCostToObtainPrivate("me");

            AND_WHEN("It is called by owner")
            {
                THEN("The cost should be retrieved")
                {
                    REQUIRE(potatoes.getCostToObtain("me") == 0.34);
                }
            }

            AND_WHEN("It is called by other player")
            {
                THEN("The cost should NOT be retrieved")
                {
                    REQUIRE(potatoes.getCostToObtain("") == -1.0);
                }
            }
        }

        WHEN("The cost is changed by owner")
        {
            potatoes.changeCostToObtain(0.2, "me");
            THEN("The cost should be changed")
            {
                REQUIRE(potatoes.getCostToObtain("me") == 0.2);
            }
        }

        WHEN("The cost is changed by other")
        {
            potatoes.changeCostToObtain(0.2, "");
            THEN("The cost should NOT be changed")
            {
                REQUIRE(potatoes.getCostToObtain("me") == 0.34);
            }
        }
    }
}

SCENARIO("Show reservation value to sell")
{
    GIVEN("A Resource named 'Potatoes' owned by 'me', quantity 200, and cost 0.34, RV to sell 0.5")
    {
        livelyRD::Resource potatoes
        (
            "Potatoes",
            "me",
            200,
            0.34,
            0.5
        );

        WHEN("The reservation value is called by owner")
        {
            THEN("The reservation value should be retrieved")
            {
                REQUIRE(potatoes.getReservationValueToSell("me") == 0.5);
            }
        }

        WHEN("The reservation value is called by other")
        {
            THEN("The reservation value should NOT be retrieved")
            {
                REQUIRE(potatoes.getReservationValueToSell("") == -1.0);
            }
        }

        WHEN("Reservation value is changed by owner")
        {
            potatoes.changeReservationValueToSell(0.4, "me");

            THEN("Reservation value should be changed")
            {
                REQUIRE(potatoes.getReservationValueToSell("me") == 0.4);
            }
        }

        WHEN("Reservation value is changed by other")
        {
            potatoes.changeReservationValueToSell(0.4, "");

            THEN("Reservation value should NOT be changed")
            {
                REQUIRE(potatoes.getReservationValueToSell("me") == 0.5);
            }
        }
    }
}

SCENARIO("Show reservation value to buy")
{
    GIVEN("A Resource named 'Potatoes' owned by 'me', quantity 200, and cost 0.34, RV to sell 0.5, RV to buy 0.4")
    {
        livelyRD::Resource potatoes
                (
                        "Potatoes",
                        "me",
                        200,
                        0.34,
                        0.5,
                        0.4
                );

        WHEN("The reservation value is called by owner")
        {
            THEN("The reservation value should be retrieved")
            {
                REQUIRE(potatoes.getReservationValueToBuy("me") == 0.4);
            }
        }

        WHEN("The reservation value is called by other")
        {
            THEN("The reservation value should NOT be retrieved")
            {
                REQUIRE(potatoes.getReservationValueToBuy("") == -1.0);
            }
        }

        WHEN("Reservation value is changed by owner")
        {
            potatoes.changeReservationValueToBuy(0.2, "me");

            THEN("Reservation value should be changed")
            {
                REQUIRE(potatoes.getReservationValueToBuy("me") == 0.2);
            }
        }

        WHEN("Reservation value is changed by other")
        {
            potatoes.changeReservationValueToBuy(0.2, "");

            THEN("Reservation value should NOT be changed")
            {
                REQUIRE(potatoes.getReservationValueToBuy("me") == 0.4);
            }
        }
    }
}