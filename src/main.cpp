#include <dep/lodepng/lodepng.h>
#include <src/example.h>
#include <src/ofwavedictgen.h>
#include <src/wavepick.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>

std::string filename = "mehaute2.png";

void encodeWithState(const char* filename, std::vector<unsigned char>& _image,
                     unsigned width, unsigned height) {
    std::vector<unsigned char> png;
    lodepng::State state;  // optionally customize this one

    unsigned error = lodepng::encode(png, _image, width, height, state);
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
    std::vector<std::string> validCommands = {"H",        "T", "d",
                                              "analysis", "x", "y"};
    if (std::find(validCommands.begin(), validCommands.end(), param) !=
        validCommands.end()) {
        parameters->insert(std::pair<std::string, double>(param, argument));
    }
    if (param == "help") {
        printf(
            "typical use example:\n*****\nchoose analysis type by\n"
            "analysis=1 (findcoord)\nOR\nanalysis=2 (genwavedict)\n"
            "./WaveTheoryPicker analysis=1 x=0.01 y=0.004 d=1.4\n"
            "./WaveTheoryPicker analysis=2 H=0.3 T=6 d=1.4 graph=graphname.png\n"
            "Not that not all values are to be used\n"
            "case 1 requires x, y and d\n"
            "case 2 requires H, T, and d; it will default load mehaute2.png\n");
    }
    if (param == "graph") {
        filename = input.substr(input.find("=") + 1).c_str();
    }
}

double get_T_from_x(double x, double d) { return std::sqrt(d / (9.81 * x)); }

double get_H_from_y(double y, double T) { return 9.81 * T * T * y; }

void writeData(unsigned w, unsigned h, std::vector<unsigned char> image) {
    std::ofstream myf("example.h");
    myf.is_open();
    myf << "#include<vector>\nint w=";
    myf << w << ",h=" << h << ";\n std::vector<unsigned char> image ={";
    for (size_t i = 0; i < (w * h * 4); i++) {
        myf << std::to_string(image[i]) << ",";
    }
    myf << "};\n";
    myf.close();
}

int main(int argc, char* argv[]) {
    int radius = 4;
    std::map<std::string, double> parameters;
    for (size_t i = 0; i < argc; i++) {
        setParameter(argv[i], &parameters);
    }
    for (auto x : parameters) {
        printf("%s:%f\n", x.first.c_str(), x.second);
    }
    if (parameters["analysis"] > 1.0) {
        double d = 62.8, H = 6.59, T = 6, g = 9.81;
        d = parameters["d"] != 0 ? parameters["d"] : d,
        H = parameters["H"] != 0 ? parameters["H"] : H,
        T = parameters["T"] != 0 ? parameters["T"] : T;
        WavePick wavepicker1(H, T, d);
        // load original mehaute diagram

        std::vector<unsigned char> buffer;
        std::vector<unsigned char> image_meh;

        lodepng::load_file(buffer, filename);

        lodepng::State state;
        unsigned w_meh, h_meh;

        unsigned error =
            lodepng::decode(image_meh, w_meh, h_meh, state, buffer);
        if (error) {
            std::cout << "decode error " << error << ": "
                      << lodepng_error_text(error) << std::endl;
        }

        // coordinates for color code (mehaute) diagram
        int* coord = wavepicker1.get_coords(w, h);
        printf("for d/gT^2: %f, and H/gT^2 %f\n", wavepicker1.x_value,
               wavepicker1.y_value);
        // obtain color
        int* RGB = wavepicker1.get_wave_color(image, coord[0], coord[1], w, 2);

        printf("Average color for wave is (%d,%d,%d)\n", RGB[0], RGB[1],
               RGB[2]);
        printf("The appropriate wave theory is %s\n",
               wavepicker1.get_theory_name(RGB[0], RGB[1], RGB[2]).c_str());
        printf("For H: %fm, T: %fs,d: %fm\n", H, T, d);

        // draw marker on mehaute
        wavepicker1.draw_marker(image_meh, coord[0] * w_meh / w,
                                coord[1] * h_meh / h, w_meh, 2);

        // generate setupfile for openfoam
        Ofwavedictgen wavedictgenerator;
        wavedictgenerator.set_parameter("waveHeight",
                                        std::to_string(parameters["H"]));
        wavedictgenerator.set_parameter("wavePeriod",
                                        std::to_string(parameters["T"]));
        wavedictgenerator.set_parameter(
            "waveTheory", wavepicker1.get_theory_name(RGB[0], RGB[1], RGB[2]));
        wavedictgenerator.gen_setupfile();
        wavedictgenerator.write_file("waveDict");

        // save marked image
        encodeWithState("mehaute_marked.png", image_meh, w_meh, h_meh);
    }
    // find new H and T
    else if (parameters["analysis"] == 1.0) {
        double T_new = get_T_from_x(parameters["x"], parameters["d"]);
        double H_new = get_H_from_y(parameters["y"], T_new);
        printf("desired T: %f, H:%f\n", T_new, H_new);
    }
    return 0;
}
