#include <src/wavepick.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
void encodeWithState(const char* filename, std::vector<unsigned char>& image,
                     unsigned width, unsigned height) {
    std::vector<unsigned char> png;
    lodepng::State state;  // optionally customize this one

    unsigned error = lodepng::encode(png, image, width, height, state);
    if (!error) lodepng::save_file(png, filename);

    // if there's an error, display it
    if (error)
        std::cout << "encoder error " << error << ": "
                  << lodepng_error_text(error) << std::endl;
}

void setParameter(std::string input,
                  std::map<std::string, double>* parameters) {
    std::string param = input.substr(0, input.find("="));
    double argument = std::atof(input.substr(input.find("=") + 1).c_str());
    std::vector<std::string> validCommands = {"H", "T", "d"};
    if (std::find(validCommands.begin(), validCommands.end(), param) !=
        validCommands.end()) {
        parameters->insert(std::pair<std::string, double>(param, argument));
        printf("set %s to %f\n", param.c_str(), argument);
    }
}

int main(int argc, char* argv[]) {
    double d = 62.8, H = 6.59, T = 6, g = 9.81;
    int radius = 4;
    std::map<std::string, double> parameters;
    for (size_t i = 0; i < argc; i++) {
        setParameter(argv[i], &parameters);
    }
    for (auto x : parameters) {
        printf("%s:%f\n", x.first.c_str(), x.second);
    }
    // d = parameters["d"], H = parameters["H"], T = parameters["T"];
    WavePick wavepicker1(H, T, d);

    std::string filename = "mehaute.png";

    std::vector<unsigned char> buffer;
    std::vector<unsigned char> image;

    lodepng::load_file(buffer, filename);

    lodepng::State state;
    unsigned w, h;

    unsigned error = lodepng::decode(image, w, h, state, buffer);
    if (error) {
        std::cout << "decode error " << error << ": "
                  << lodepng_error_text(error) << std::endl;
    }
    printf("image: width: %d, height %d\n", w, h);
    int xCoord = wavepicker1.get_coords(w, h)[0];
    int yCoord = wavepicker1.get_coords(w, h)[1];

    int* p = wavepicker1.get_coords(w, h);
    printf("coordinates x: %d, y: %d\n", p[0], yCoord);
    printf("for d/gT^2: %f, and H/gT^2 %f\n", wavepicker1.x_value,
           wavepicker1.y_value);
    // obtain color
    int* RGB = wavepicker1.get_wave_color(image, xCoord, yCoord, w, 2);

    printf("Average color for wave is (%d,%d,%d)\n", RGB[0], RGB[1], RGB[2]);
    printf("The appropriate wave theory is %s\n",
           wavepicker1.get_theory_name(RGB[0], RGB[1], RGB[2]).c_str());
    // set color
    wavepicker1.draw_marker(image, xCoord, yCoord, w, 2);

    encodeWithState("file.png", image, w, h);

    return 0;
}
