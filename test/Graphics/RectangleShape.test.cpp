#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <doctest/doctest.h>

#include <GraphicsUtil.hpp>
#include <SystemUtil.hpp>
#include <type_traits>

static_assert(std::is_copy_constructible_v<sf::RectangleShape>);
static_assert(std::is_copy_assignable_v<sf::RectangleShape>);
static_assert(std::is_move_constructible_v<sf::RectangleShape>);
static_assert(std::is_move_assignable_v<sf::RectangleShape>);

TEST_CASE("[Graphics] sf::RectangleShape")
{
    SUBCASE("Default constructor")
    {
        const sf::RectangleShape rectangle;
        CHECK(rectangle.getSize() == sf::Vector2f(0, 0));
        CHECK(rectangle.getPointCount() == 4);
        CHECK(rectangle.getPoint(0) == sf::Vector2f(0, 0));
        CHECK(rectangle.getPoint(1) == sf::Vector2f(0, 0));
        CHECK(rectangle.getPoint(2) == sf::Vector2f(0, 0));
        CHECK(rectangle.getPoint(3) == sf::Vector2f(0, 0));
    }

    SUBCASE("Size constructor")
    {
        const sf::RectangleShape rectangle({9, 8});
        CHECK(rectangle.getSize() == sf::Vector2f(9, 8));
        CHECK(rectangle.getPointCount() == 4);
        CHECK(rectangle.getPoint(0) == sf::Vector2f(0, 0));
        CHECK(rectangle.getPoint(1) == sf::Vector2f(9, 0));
        CHECK(rectangle.getPoint(2) == sf::Vector2f(9, 8));
        CHECK(rectangle.getPoint(3) == sf::Vector2f(0, 8));
    }

    SUBCASE("Set size")
    {
        sf::RectangleShape rectangle({7, 6});
        rectangle.setSize({5, 4});
        CHECK(rectangle.getSize() == sf::Vector2f(5, 4));
    }

    SUBCASE("Render rectangle shape")
    {
        sf::RectangleShape rectangle({50.f, 20.f});
        rectangle.setFillColor(sf::Color::Red);
        rectangle.setOutlineColor(sf::Color::Green);
        rectangle.setOutlineThickness(2.f);
        rectangle.setPosition({10.f, 10.f});

        sf::RenderTexture renderTexture;
        REQUIRE(renderTexture.create({100, 100}));

        renderTexture.clear();
        renderTexture.draw(rectangle);
        renderTexture.display();

        auto image = renderTexture.getTexture().copyToImage();
        REQUIRE(image.saveToFile("testOutput.png"));

        REQUIRE(compareImages("expected/rectangleShape.png", "testOutput.png"));
    }
}
