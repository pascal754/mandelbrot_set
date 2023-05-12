# Mandelbrot Set

## Wikipedia

https://en.wikipedia.org/wiki/Mandelbrot_set

```
for each pixel (Px, Py) on the screen do
    x0 := scaled x coordinate of pixel (scaled to lie in the Mandelbrot X scale (-2.00, 0.47))
    y0 := scaled y coordinate of pixel (scaled to lie in the Mandelbrot Y scale (-1.12, 1.12))
    x := 0.0
    y := 0.0
    iteration := 0
    max_iteration := 1000
    while (x*x + y*y ≤ 2*2 AND iteration < max_iteration) do
        xtemp := x*x - y*y + x0
        y := 2*x*y + y0
        x := xtemp
        iteration := iteration + 1
    
    color := palette[iteration]
    plot(Px, Py, color)
```

## Development Environment

- Microsoft Visual Studio Community 2022 (64-bit) - Preview
Version 17.6.0 Preview 6.0

- SFML 2.5.1 via vcpkg

## Usage

- R: Reset view
- I: Zoom in
- O: Zoom out
- Arrow kyes: move the object
- Mouse left button click: set the mouse cursor point to the center of the window

## Screenshot

![screenshot](Screenshot.png)
