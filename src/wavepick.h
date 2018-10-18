#pragma once
#include <string>
class WavePick {
   public:
    double m_H, m_T, m_d;
    double x_value, y_value;
    double g = 9.81;

    WavePick(double H, double T, double d);
    ~WavePick();

    std::string get_theory_name(int R, int G, int B);

    void draw_marker(std::vector<unsigned char> &image, int xCoord, int yCoord,
                     int w, int radius = 2, int offset_x = 0, int offset_y = 0);

    int *get_wave_color(std::vector<unsigned char> &image, int xCoord,
                        int yCoord, int w, int radius = 2);

    int *get_coords(int w, int h);

    void set_H(double H);

    void set_T(double T);

    void set_d(double d);

};
