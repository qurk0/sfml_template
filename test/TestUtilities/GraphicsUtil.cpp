#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Transform.hpp>

#include <GraphicsUtil.hpp>
#include <ostream>

namespace sf
{
std::ostream& operator<<(std::ostream& os, const BlendMode& blendMode)
{
    return os << "( " << blendMode.colorSrcFactor << ", " << blendMode.colorDstFactor << ", " << blendMode.colorEquation
              << ", " << blendMode.alphaSrcFactor << ", " << blendMode.alphaDstFactor << ", " << blendMode.alphaEquation
              << " )";
}

std::ostream& operator<<(std::ostream& os, const Color& color)
{
    return os << "0x" << std::hex << color.toInteger() << std::dec << " (r=" << static_cast<int>(color.r)
              << ", g=" << static_cast<int>(color.g) << ", b=" << static_cast<int>(color.b)
              << ", a=" << static_cast<int>(color.a) << ")";
}

std::ostream& operator<<(std::ostream& os, const Transform& transform)
{
    const auto& matrix = transform.getMatrix();
    os << matrix[0] << ", " << matrix[4] << ", " << matrix[12] << ", ";
    os << matrix[1] << ", " << matrix[5] << ", " << matrix[13] << ", ";
    os << matrix[3] << ", " << matrix[7] << ", " << matrix[15];
    return os;
}
} // namespace sf

bool operator==(const sf::Transform& lhs, const Approx<sf::Transform>& rhs)
{
    return lhs.getMatrix()[0] == Approx(rhs.value.getMatrix()[0]) &&
           lhs.getMatrix()[4] == Approx(rhs.value.getMatrix()[4]) &&
           lhs.getMatrix()[12] == Approx(rhs.value.getMatrix()[12]) &&
           lhs.getMatrix()[1] == Approx(rhs.value.getMatrix()[1]) &&
           lhs.getMatrix()[5] == Approx(rhs.value.getMatrix()[5]) &&
           lhs.getMatrix()[13] == Approx(rhs.value.getMatrix()[13]) &&
           lhs.getMatrix()[3] == Approx(rhs.value.getMatrix()[3]) &&
           lhs.getMatrix()[7] == Approx(rhs.value.getMatrix()[7]) &&
           lhs.getMatrix()[15] == Approx(rhs.value.getMatrix()[15]);
}

bool compareImages(const std::string& expected, const std::string& actual)
{
    sf::Image image1;
    if (!image1.loadFromFile(expected))
    {
        return false;
    }

    sf::Image image2;
    if (!image2.loadFromFile(actual))
    {
        return false;
    }

    if (image1.getSize() != image2.getSize())
    {
        return false;
    }

    auto size = image1.getSize();

    double totalDiff = 0;
    for (unsigned int y = 0; y < size.y; ++y)
    {
        for (unsigned int x = 0; x < size.x; ++x)
        {
            unsigned int index = ((y * x) + x) * 4;
            totalDiff += std::abs(int(image1.getPixelsPtr()[index + 0]) - int(image2.getPixelsPtr()[index + 0])) / 255.0;
            totalDiff += std::abs(int(image1.getPixelsPtr()[index + 1]) - int(image2.getPixelsPtr()[index + 1])) / 255.0;
            totalDiff += std::abs(int(image1.getPixelsPtr()[index + 2]) - int(image2.getPixelsPtr()[index + 2])) / 255.0;
        }
    }

    const double diffPercentage = (totalDiff * 100) / (size.x * size.y * 3);
    return diffPercentage <= 1.;
}
