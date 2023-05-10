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

#include <SFML/Graphics.hpp>

constexpr double xMin{ -2.0 };
constexpr double xMax{ 0.47 };
constexpr double yMin{ -1.12 };
constexpr double yMax{ 1.12 };
constexpr unsigned maxIteration{ 1000 };
constexpr unsigned width{ 800 };
constexpr unsigned height{ 800 };

void make_pixels(sf::VertexArray& va);
void update_colors(sf::VertexArray& va, const double scale, const double dx, const double dy);
unsigned get_color(const double px, const double py, const double scale = 1.0, const double dx = 0.0, const double dy = 0.0);

int main()
{
    double scale{ 1.0 };
    double deltaX{};
    double deltaY{};
    sf::VertexArray pixels{sf::Points};
    make_pixels(pixels);

    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Set", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) // zoom in
            {
                scale *= 1.5;
                update_colors(pixels, scale, deltaX, deltaY);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) // zoom out
            {
                if (scale == 1.0)
                {
                    continue;
                }

                scale /= 1.5;
                update_colors(pixels, scale, deltaX, deltaY);
                if (scale < 1.0)
                {
                    scale = 1.0;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                deltaX += 0.5 / scale;
                update_colors(pixels, scale, deltaX, deltaY);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                deltaX -= 0.5 / scale;
                update_colors(pixels, scale, deltaX, deltaY);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                deltaY += 0.5 / scale;
                update_colors(pixels, scale, deltaX, deltaY);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                deltaY -= 0.5 / scale;
                update_colors(pixels, scale, deltaX, deltaY);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) // reset
            {
                scale = 1.0;
                deltaX = 0.0;
                deltaY = 0.0;
                update_colors(pixels, scale, deltaX, deltaY);
            }
        }

        window.clear();

        window.draw(pixels);

        window.display();
    }

}

void make_pixels(sf::VertexArray& va)
{
    for (unsigned px{}; px < width; ++px)
    {
        for (unsigned py{}; py < height; ++py)
        {
            unsigned color{ get_color(px, py) };

            va.append(sf::Vertex{
                sf::Vector2f{static_cast<float>(px), static_cast<float>(py)},
                sf::Color{
                    static_cast<sf::Uint8>(color / 256),
                    static_cast<sf::Uint8>(color % 256),
                    0}}
            );
        }
    }
}

void update_colors(sf::VertexArray& va, const double scale, const double dx, const double dy)
{
    for (size_t i{}; i < va.getVertexCount(); ++i)
    {
        unsigned color{ get_color(va[i].position.x, va[i].position.y, scale, dx, dy) };

        va[i].color.r = static_cast<sf::Uint8>(color / 256);
        va[i].color.g = static_cast<sf::Uint8>(color % 256);
    }
}

unsigned get_color(const double px, const double py, const double scale, const double dx, const double dy)
{
    double x0{ ((xMax - xMin) * px / width + xMin) / scale + dx };
    double y0{ ((yMax - yMin) * py / height + yMin) / scale + dy };

    double x{};
    double y{};
    unsigned iteration{};
    while (x * x + y * y <= 4.0 && iteration < maxIteration)
    {
        double xTemp{ x * x - y * y + x0 };
        y = 2.0 * x * y + y0;
        x = xTemp;
        ++iteration;
    }

    return iteration;
}