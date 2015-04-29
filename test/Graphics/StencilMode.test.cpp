#include <SFML/Graphics/StencilMode.hpp>

// Other 1st party headers
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::StencilMode", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::StencilMode>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::StencilMode>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::StencilMode>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::StencilMode>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            const sf::StencilMode stencilMode;
            CHECK(stencilMode.stencilComparison == sf::StencilComparison::Always);
            CHECK(stencilMode.stencilUpdateOperation == sf::StencilUpdateOperation::Keep);
            CHECK(stencilMode.stencilReference == 0);
            CHECK(stencilMode.stencilMask == ~0u);
            CHECK(stencilMode.stencilOnly == false);
        }

        SECTION("Verbose constructor")
        {
            const sf::StencilMode stencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true);
            CHECK(stencilMode.stencilComparison == sf::StencilComparison::Equal);
            CHECK(stencilMode.stencilUpdateOperation == sf::StencilUpdateOperation::Replace);
            CHECK(stencilMode.stencilReference == 1);
            CHECK(stencilMode.stencilMask == 0u);
            CHECK(stencilMode.stencilOnly == true);
        }
    }

    SECTION("Operators")
    {
        SECTION("operator==")
        {
            CHECK(sf::StencilMode() == sf::StencilMode());
            CHECK(sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true) ==
                  sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true));

            CHECK_FALSE(sf::StencilMode() ==
                        sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true));
            CHECK_FALSE(sf::StencilMode(sf::StencilComparison::Greater, sf::StencilUpdateOperation::Invert, 0, ~0u, false) ==
                        sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true));
        }

        SECTION("operator!=")
        {
            CHECK_FALSE(sf::StencilMode() != sf::StencilMode());
            CHECK_FALSE(sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true) !=
                        sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true));

            CHECK(sf::StencilMode() !=
                  sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true));
            CHECK(sf::StencilMode(sf::StencilComparison::Greater, sf::StencilUpdateOperation::Invert, 0, ~0u, false) !=
                  sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true));
        }
    }

    SECTION("Draw")
    {
        sf::RenderTexture renderTexture;
        REQUIRE(renderTexture.create({100, 100}, sf::ContextSettings(0, 8)) == true);
        renderTexture.clear(sf::Color::Red, 127);

        sf::RectangleShape shape1({100, 100});
        shape1.setFillColor(sf::Color::Green);
        sf::RectangleShape shape2({100, 100});
        shape2.setFillColor(sf::Color::Blue);

        SECTION("Stencil-Only")
        {
            renderTexture.draw(shape1,
                               sf::StencilMode(sf::StencilComparison::Always, sf::StencilUpdateOperation::Keep, 1, 0xFF, true));
            renderTexture.display();
            CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Red);
        }

        SECTION("Comparisons")
        {
            SECTION("Always")
            {
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Always, sf::StencilUpdateOperation::Keep, 1, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Green);
            }

            SECTION("Equal")
            {
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Keep, 126, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Red);
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Keep, 127, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Green);
            }

            SECTION("Greater")
            {
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Greater, sf::StencilUpdateOperation::Keep, 126, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Red);
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Greater, sf::StencilUpdateOperation::Keep, 127, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Red);
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Greater, sf::StencilUpdateOperation::Keep, 128, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Green);
            }

            SECTION("GreaterEqual")
            {
                renderTexture.draw(shape1,
                                   sf::StencilMode(sf::StencilComparison::GreaterEqual,
                                                   sf::StencilUpdateOperation::Keep,
                                                   126,
                                                   0xFF,
                                                   false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Red);
                renderTexture.draw(shape1,
                                   sf::StencilMode(sf::StencilComparison::GreaterEqual,
                                                   sf::StencilUpdateOperation::Keep,
                                                   127,
                                                   0xFF,
                                                   false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Green);
                renderTexture.draw(shape2,
                                   sf::StencilMode(sf::StencilComparison::GreaterEqual,
                                                   sf::StencilUpdateOperation::Keep,
                                                   128,
                                                   0xFF,
                                                   false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Blue);
            }

            SECTION("Less")
            {
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Less, sf::StencilUpdateOperation::Keep, 128, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Red);
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Less, sf::StencilUpdateOperation::Keep, 127, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Red);
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Less, sf::StencilUpdateOperation::Keep, 126, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Green);
            }

            SECTION("LessEqual")
            {
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::LessEqual, sf::StencilUpdateOperation::Keep, 128, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Red);
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::LessEqual, sf::StencilUpdateOperation::Keep, 127, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Green);
                renderTexture
                    .draw(shape2,
                          sf::StencilMode(sf::StencilComparison::LessEqual, sf::StencilUpdateOperation::Keep, 126, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Blue);
            }

            SECTION("Never")
            {
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Never, sf::StencilUpdateOperation::Keep, 127, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Red);
            }

            SECTION("NotEqual")
            {
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::NotEqual, sf::StencilUpdateOperation::Keep, 127, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Red);
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::NotEqual, sf::StencilUpdateOperation::Keep, 128, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Green);
            }
        }

        SECTION("Updating")
        {
            SECTION("Decrement")
            {
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Always, sf::StencilUpdateOperation::Decrement, 127, 0xFF, true));
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Decrement, 126, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Green);
            }

            SECTION("Increment")
            {
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Always, sf::StencilUpdateOperation::Increment, 127, 0xFF, true));
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Increment, 128, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Green);
            }

            SECTION("Invert")
            {
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Always, sf::StencilUpdateOperation::Invert, 127, 0xFF, true));
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Invert, 0x80, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Green);
            }

            SECTION("Keep")
            {
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Always, sf::StencilUpdateOperation::Keep, 127, 0xFF, true));
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Keep, 127, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Green);
            }

            SECTION("Replace")
            {
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Always, sf::StencilUpdateOperation::Replace, 255, 0xFF, true));
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 255, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Green);
            }

            SECTION("Zero")
            {
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Always, sf::StencilUpdateOperation::Zero, 127, 0xFF, true));
                renderTexture
                    .draw(shape1,
                          sf::StencilMode(sf::StencilComparison::Equal, sf::StencilUpdateOperation::Zero, 0, 0xFF, false));
                renderTexture.display();
                CHECK(renderTexture.getTexture().copyToImage().getPixel({50, 50}) == sf::Color::Green);
            }
        }
    }
}
