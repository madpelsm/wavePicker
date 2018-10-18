#pragma once
#include <map>
#include <string>

class Ofwavedictgen {
   public:
    std::string m_setupfilefront =
        "FoamFile\n{\nversion 2.0;\nformat ascii;\nclass "
        "dictionary;\nlocation \"constant\";\nobject waveDict;\n};\n\n";
    std::string m_setupfileback =
        "nPaddles 1;\nwaveDir 0.0;\nwavePhase 0.0;\nwaveType "
        "regular;\ngenAbs 1;\nabsDir 0.0;";
    std::string m_setupfile;

    std::map<std::string, std::string> m_parameters;

    Ofwavedictgen();
    ~Ofwavedictgen();

    void gen_setupfile();

    void write_file(std::string filename);

    void set_parameter(std::string key, std::string val);
};
