#include <iostream>

// clear && g++ colorMessages.cc && ./a.out

struct Color {
    int r, g, b;
};

Color interpolateColor(const Color& colorA, const Color& colorB, double t) {

    return {
        static_cast<int>(colorA.r + t * (colorB.r - colorA.r)),
        static_cast<int>(colorA.g + t * (colorB.g - colorA.g)),
        static_cast<int>(colorA.b + t * (colorB.b - colorA.b))
    };
}

void printColoredMessage(const Color& colorA, const Color& colorB, double inferiorLimit, double superiorLimit, double value, const std::string& message) {

    if (value < inferiorLimit) value = inferiorLimit;
    if (value > superiorLimit) value = superiorLimit;
    
    double t = (value - inferiorLimit) / (superiorLimit - inferiorLimit);
    Color color = interpolateColor(colorA, colorB, t);
    
    std::cout << "\033[38;2;" << color.r << ";" << color.g << ";" << color.b << "m" << message << "\033[0m" << std::endl;
}

// int main() {
//
//     Color red = {255, 0, 0};
//     Color green = {0, 255, 0};
//
// 	for (int i = 0; i <= 100; i++) {
// 		printColoredMessage(red, green, 0.0, 100.0, i, "This is a test message!");
// 	}
//
//     return 0;
// }
