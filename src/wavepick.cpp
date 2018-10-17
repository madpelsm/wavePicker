#include <src/wavepick.h>

WavePick::WavePick() {}

WavePick::WavePick(double H, double T, double d) : m_H(H), m_T(T), m_d(d) {}

WavePick::~WavePick() {}

std::string WavePick::get_theory_name(int R, int G, int B) {
    if (R == 0 && G == 255 && B == 58) {
        return "StokesIII";
    } else if (R == 255 && G == 255 && B == 0) {
        return "StokesIV";
    } else if (R == 255 && G == 148 && B == 0) {
        return "streamFunction";
    } else if (R == 189 && G == 255 && B == 255) {
        return "cnoidal";
    } else if (R == 0 && G == 25 && B == 255) {
        return "StokesII";
    } else if (R == 255 && G == 0 && B == 41) {
        return "StokesII";
    }
    return "Theory not found";
}

void WavePick::draw_marker(std::vector<unsigned char> &image, int xCoord,
                           int yCoord, int w, int radius) {
    for (auto i = 0; i < radius * 2; i++) {
        for (auto k = 0; k < radius * 2; k++) {
            image[(yCoord - radius + k) * 4 * w + xCoord * 4 + i * 4 + 0] = 255;
            image[(yCoord - radius + k) * 4 * w + xCoord * 4 + i * 4 + 1] = 0;
            image[(yCoord - radius + k) * 4 * w + xCoord * 4 + i * 4 + 2] = 0;
            image[(yCoord - radius + k) * 4 * w + xCoord * 4 + i * 4 + 3] = 255;
        }
    }
}

int *WavePick::get_wave_color(std::vector<unsigned char> &image, int xCoord,
                              int yCoord, int w, int radius) {
    static int RGB[] = {0, 0, 0};
    for (auto i = 0; i < radius * 2; i++) {
        for (auto k = 0; k < radius * 2; k++) {
            RGB[0] +=
                image[(yCoord - radius + k) * 4 * w + xCoord * 4 + i * 4 + 0];
            RGB[1] +=
                image[(yCoord - radius + k) * 4 * w + xCoord * 4 + i * 4 + 1];
            RGB[2] +=
                image[(yCoord - radius + k) * 4 * w + xCoord * 4 + i * 4 + 2];
        }
    }
    for (size_t t = 0; t < 3; t++) {
        RGB[t] /= (4 * radius * radius);
    }
    return RGB;
}
void WavePick::set_H(double H) { m_H = H; }

void WavePick::set_T(double T) { m_T = T; }

void WavePick::set_d(double d) { m_d = d; }

int *WavePick::get_coords(int w, int h) {
    x_value = m_d / (g * m_T * m_T);
    y_value = m_H / (g * m_T * m_T);
    double xMin = 5.34 * pow(10, -4.0);
    double xMax = 0.2;
    double xMaxPix = w;

    double yMin = 0.00005;
    double yMinPix = h;
    double yMax = 0.05;
    double yMaxPix = 0;

    if (x_value > xMax) {
        x_value = xMax;
    } else if (x_value < xMin) {
        x_value = xMin;
    }

    if (y_value > yMax) {
        y_value = yMax;
    } else if (y_value < yMin) {
        y_value = yMin;
    }

    double auxX = h / 3 * log10(xMax / x_value);
    double auxY = h / 3 * log10(y_value / yMin);

    int xCoord = xMaxPix - auxX;
    int yCoord = yMinPix - auxY;
    static int coords[] = {xCoord, yCoord};
    return coords;
}
