#pragma once

// =============================================================
//  Enum tanımlamaları
//  Meslek grupları, uzmanlık alanları ve diller burada tutulur.
// =============================================================

//  Meslek Grupları
enum class Profession
{
    ELECTRONIC_ENGINEERING = 1
};

// Elektronik Mühendisliği Alt Dalları 
enum class ElectronicSpec
{
    EMBEDDED_SOFTWARE = 1,
    DIGITAL_ELECTRONICS = 2,
    HARDWARE_DESIGN = 3,
    POWER_ELECTRONICS = 4
};

// Programlama Dilleri / Araçlar 
enum class Language
{
    CPP = 1,
    C = 2,
    JAVA = 3,
    HTML = 4,
    REACTJS = 5,
    PYTHON = 6,
    CSHARP = 7,
    MATLAB = 8,
    KICAD = 9,
    ALTIUM = 10,
    SIMULINK = 11,
    LTSPICE = 12,
    XILINX = 13,
    ALTERA = 14,
    VERILOG = 15,
    VHDL = 16
};

// Zaman Birimi (deneyim girişi için) 
enum class TimeUnit
{
    MONTH = 1,
    YEAR = 2
};
