#include <src/ofwavedictgen.h>
#include <fstream>

Ofwavedictgen::Ofwavedictgen() {}

Ofwavedictgen::~Ofwavedictgen() {}

void Ofwavedictgen::gen_setupfile() {
    m_setupfile = "";
    m_setupfile += m_setupfilefront;

    for (auto x : m_parameters) {
        m_setupfile += x.first + " " + x.second + ";\n";
    }
    m_setupfile += m_setupfileback;
    //    printf("setupfile generated: \n *********\n\n%s\n",
    //    m_setupfile.c_str());
}

void Ofwavedictgen::write_file(std::string filename) {
    std::ofstream myfile;
    myfile.open(filename);
    myfile << m_setupfile;
    myfile.close();
}

void Ofwavedictgen::set_parameter(std::string key, std::string val) {
    printf("setting %s\n", key.c_str());
    m_parameters[key] = val;
}
