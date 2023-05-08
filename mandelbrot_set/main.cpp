// May 8, 2023
// by SM
// 
// https://en.wikipedia.org/wiki/Mandelbrot_set
//for each pixel(Px, Py) on the screen do
//    x0 : = scaled x coordinate of pixel(scaled to lie in the Mandelbrot X scale(-2.00, 0.47))
//    y0 : = scaled y coordinate of pixel(scaled to lie in the Mandelbrot Y scale(-1.12, 1.12))
//    x : = 0.0
//    y : = 0.0
//    iteration : = 0
//    max_iteration : = 1000
//    while (x * x + y * y ≤ 2 * 2 AND iteration < max_iteration) do
//        xtemp : = x * x - y * y + x0
//        y : = 2 * x * y + y0
//        x : = xtemp
//        iteration : = iteration + 1
//
//        color : = palette[iteration]
//        plot(Px, Py, color)

import <cmath>;
#include <SFML/Graphics.hpp>

void make_pixels(sf::VertexArray& va, const unsigned width, const unsigned height);

int main()
{
    constexpr unsigned width{ 800 };
    constexpr unsigned height{ 800 };
    sf::VertexArray pixels{sf::Points};
    make_pixels(pixels, width, height);

    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Set", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        window.draw(pixels);

        window.display();
    }

}

void make_pixels(sf::VertexArray& va, const unsigned width, const unsigned height)
{
    constexpr double xMin{ -2.0 };
    constexpr double xMax{ 0.47 };
    constexpr double yMin{ -1.12 };
    constexpr double yMax{ 1.12 };
    constexpr unsigned maxIteration{ 1000 };

    for (unsigned px{}; px < width; ++px)
    {
        for (unsigned py{}; py < height; ++py)
        {
            double x0{ (xMax - xMin) * px / width + xMin };
            double y0{ (yMax - yMin) * py / height + yMin };

            double x{};
            double y{};
            unsigned iteration{};
            while (x * x + y * y <= 4.0 && iteration < maxIteration)
            {
                double xtemp{ x * x - y * y + x0 };
                y = 2.0 * x * y + y0;
                x = xtemp;
                ++iteration;
            }
            va.append(sf::Vertex{
                sf::Vector2f{static_cast<float>(px), static_cast<float>(py)},
                sf::Color{
                    static_cast<sf::Uint8>(iteration / 256),
                    static_cast<sf::Uint8>(iteration % 256),
                    0}}
            );
        }
    }
}