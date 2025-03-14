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

// March 11, 2025
// changes:
// update SFML to v3.0

#include <SFML/Graphics.hpp>
import std;

constexpr double xMin{ -2.0 };
constexpr double xMax{ 0.47 };
constexpr double yMin{ -1.12 };
constexpr double yMax{ 1.12 };
constexpr unsigned maxIteration{ 1000 };
constexpr unsigned width{ 800 };
constexpr unsigned height{ 800 };
constexpr double zoomUnit{ 1.5 };

void make_pixels(sf::VertexArray& va);
void update_colors(sf::VertexArray& va, const double scale, const double dx, const double dy);
void update_colors_ranges(
  sf::VertexArray& va,
  const size_t begin,
  const size_t end,
  const double xCoeff,
  const double xOffset,
  const double yCoeff,
  const double yOffset
);

int main()
{
  try
  {
    double scale{ 1.0 };
    double deltaX{};
    double deltaY{};
    bool update{};
    sf::VertexArray pixels{ sf::PrimitiveType::Points };
    make_pixels(pixels);

    sf::RenderWindow window(
      sf::VideoMode({ width, height }),
      "Mandelbrot Set",
      sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    while (window.isOpen())
    {
      while (const std::optional event{ window.pollEvent() })
      {
        if (event->is<sf::Event::Closed>())
        {
          window.close();
        }
        else if (const auto * mwScrolled{ event->getIf<sf::Event::MouseWheelScrolled>() }) // zoom in/out centered at mouse cursor position
        {
          if (mwScrolled->delta > 0) // zoom in
          {
            deltaX += ((xMax - xMin) * mwScrolled->position.x / width + xMin) / scale;
            deltaY += ((yMax - yMin) * (height - mwScrolled->position.y) / height + yMin) / scale;
            scale *= zoomUnit;
            deltaX -= ((xMax - xMin) * mwScrolled->position.x / width + xMin) / scale;
            deltaY -= ((yMax - yMin) * (height - mwScrolled->position.y) / height + yMin) / scale;
            update = true;
          }
          else // zoom out
          {
            if (scale == 1.0)
            {
              continue;
            }
            deltaX += ((xMax - xMin) * mwScrolled->position.x / width + xMin) / scale;
            deltaY += ((yMax - yMin) * (height - mwScrolled->position.y) / height + yMin) / scale;
            scale /= zoomUnit;
            deltaX -= ((xMax - xMin) * mwScrolled->position.x / width + xMin) / scale;
            deltaY -= ((yMax - yMin) * (height - mwScrolled->position.y) / height + yMin) / scale;
            update = true;
            if (scale < 1.0)
            {
              scale = 1.0;
            }
          }
        }
        else if (const auto * mbPressed{ event->getIf<sf::Event::MouseButtonPressed>() })
        {
          if (mbPressed->button == sf::Mouse::Button::Left)
          {
            auto pos{ sf::Mouse::getPosition(window) };
            deltaX -= (xMax - xMin) * (width / 2.0 - pos.x) / width / scale;
            deltaY -= (yMax - yMin) * (pos.y - height / 2.0) / height / scale;
            update = true;
          }
        }
        else if (const auto * keyPressed{ event->getIf<sf::Event::KeyPressed>() })
        {
          if (keyPressed->scancode == sf::Keyboard::Scancode::I) // zoom in
          {
            deltaX += (xMin + xMax) / 2.0 / scale;
            scale *= zoomUnit;
            deltaX -= (xMin + xMax) / 2.0 / scale;
            update = true;
          }
          else if (keyPressed->scancode == sf::Keyboard::Scancode::O) // zoom out
          {
            if (scale == 1.0)
            {
              continue;
            }
            deltaX += (xMin + xMax) / 2.0 / scale;
            scale /= zoomUnit;
            deltaX -= (xMin + xMax) / 2.0 / scale;
            update = true;
            if (scale < 1.0)
            {
              scale = 1.0;
            }
          }
          else if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
          {
            deltaX += 0.5 / scale;
            update = true;
          }
          else if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
          {
            deltaX -= 0.5 / scale;
            update = true;
          }
          else if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
          {
            deltaY -= 0.5 / scale;
            update = true;
          }
          else if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
          {
            deltaY += 0.5 / scale;
            update = true;
          }
          else if (keyPressed->scancode == sf::Keyboard::Scancode::R) // reset
          {
            scale = 1.0;
            deltaX = 0.0;
            deltaY = 0.0;
            update = true;
          }
        }
      }

      if (update)
      {
        update_colors(pixels, scale, deltaX, deltaY);
        update = false;
      }

      window.clear();

      window.draw(pixels);

      window.display();
    }
  }
  catch (const std::exception& e)
  {
    std::println(std::cerr, "{}", e.what());
  }
  catch (...)
  {
    std::println(std::cerr, "{}", "other error");
  }
}

void make_pixels(sf::VertexArray& va)
{
  for (unsigned px{}; px < width; ++px)
  {
    for (unsigned py{}; py < height; ++py)
    {
      va.append(sf::Vertex{ sf::Vector2f{static_cast<float>(px), static_cast<float>(py)} });
    }
  }
  update_colors(va, 1.0, 0.0, 0.0);
}

void update_colors(sf::VertexArray& va, const double scale, const double dx, const double dy)
{
  try
  {
    size_t size{ va.getVertexCount() };
    size_t numJthreads{ 64 };
    std::vector<std::jthread> workers(numJthreads);

    // calculate values beforehand
    auto xCoeff{ (xMax - xMin) / width / scale };
    auto xOffset{ xMin / scale + dx };
    auto yCoeff{ -(yMax - yMin) / height / scale };
    auto yOffset{ yMax / scale + dy };

    for (auto [i, x] : std::views::enumerate(workers))
    {
      x = std::jthread{
          update_colors_ranges,
          std::ref(va),
          size * i / numJthreads,
          size * (i + 1) / numJthreads,
          xCoeff,
          xOffset,
          yCoeff,
          yOffset
      };
    }

    // alternative
    /*workers.reserve(numJthreads);
    for (size_t i : std::views::iota(0u, numJthreads))
    {
        workers.emplace_back(update_colors_ranges,
            std::ref(va), size * i / numJthreads, size * (i + 1) / numJthreads, xCoeff, xOffset, yCoeff, yOffset);
    }*/
  }
  catch (const std::exception& e)
  {
    std::println(std::cerr, "{}", e.what());
  }
}

void update_colors_ranges(
  sf::VertexArray& va,
  const size_t begin,
  const size_t end,
  const double xCoeff,
  const double xOffset,
  const double yCoeff,
  const double yOffset
)
{
  // x0{ ((xMax - xMin) * va[i].position.x / width + xMin) / scale + dx };
  // y0{ ((yMax - yMin) * (height - va[i].position.y) / height + yMin) / scale + dy };
  // to save computations rewrite above two lines with coefficients and offsets

  for (size_t i{ begin }; i < end; ++i)
  {
    double x0{ xCoeff * va[i].position.x + xOffset };
    double y0{ yCoeff * va[i].position.y + yOffset };

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

    va[i].color.r = iteration / 256 * 5 + 127;
    va[i].color.g = iteration % 256;
    va[i].color.b = 127;
  }
}
