#include "cv_calculator.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cmath>
#include <limits>
#include <cctype>

// ─────────────────────────────────────────────────────────────
//  Renk / Format sabitleri (ANSI)
// ─────────────────────────────────────────────────────────────
namespace Color {
    constexpr const char* RESET = "\033[0m";
    constexpr const char* BOLD = "\033[1m";
    constexpr const char* RED = "\033[31m";
    constexpr const char* GREEN = "\033[32m";
    constexpr const char* YELLOW = "\033[33m";
    constexpr const char* CYAN = "\033[36m";
    constexpr const char* MAGENTA = "\033[35m";
    constexpr const char* BLUE = "\033[34m";
    constexpr const char* WHITE = "\033[37m";
}

// ─────────────────────────────────────────────────────────────
//  Yardımcı fonksiyonlar (namespace içinde gizli)
// ─────────────────────────────────────────────────────────────
static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

static void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void printSeparator(char c = '=', int width = 58) {
    std::cout << Color::CYAN;
    for (int i = 0; i < width; ++i) std::cout << c;
    std::cout << Color::RESET << "\n";
}

// ─────────────────────────────────────────────────────────────
//  Kurucu
// ─────────────────────────────────────────────────────────────
CVCalculator::CVCalculator()
    : m_name(std::make_unique<std::string>()),
    m_languages(std::make_unique<std::vector<Language>>()),
    m_langExpMonths(std::make_unique<std::map<Language, double>>())
{}

// ─────────────────────────────────────────────────────────────
//  Karşılama ekranı
// ─────────────────────────────────────────────────────────────
void CVCalculator::welcome() const {
    printSeparator();
    std::cout << Color::BOLD << Color::CYAN
        << "  ██████╗██╗   ██╗    ██████╗ ██████╗ \n"
        << "  ██╔════╝██║   ██║    ██╔══██╗██╔══██╗\n"
        << "  ██║     ╚██╗ ██╔╝    ██████╔╝██████╔╝\n"
        << "  ██║      ╚████╔╝     ██╔═══╝ ██╔══██╗\n"
        << "  ╚██████╗  ╚██╔╝      ██║     ██║  ██║\n"
        << "   ╚═════╝   ╚═╝       ╚═╝     ╚═╝  ╚═╝\n"
        << Color::RESET;
    printSeparator();
    std::cout << Color::BOLD
        << "   CV Basarı ORANI HESAPLAMA PROGRAMI  \n"
        << "   Beta v1.0 – Elektronik Mühendisligi \n"
        << Color::RESET;
    printSeparator();
    std::cout << "\n"
        << Color::GREEN
        << "  Hos geldiniz! Bu program mezuniyet sonrası\n"
        << "  CV basarı oraninizi hesaplamaniza yardimci olur.\n"
        << Color::RESET << "\n";
}

// ─────────────────────────────────────────────────────────────
//  Yükleme animasyonu (thread içinde)
// ─────────────────────────────────────────────────────────────
void CVCalculator::loadingAnimation(const std::string& msg, int ms) const {
    const char* frames[] = { "⣾", "⣽", "⣻", "⢿", "⡿", "⣟", "⣯", "⣷" };
    int frameCount = 8;
    auto duration = std::chrono::milliseconds(ms);
    auto start = std::chrono::steady_clock::now();
    int  i = 0;

    while (std::chrono::steady_clock::now() - start < duration) {
        std::cout << "\r  " << Color::YELLOW << frames[i % frameCount]
            << Color::RESET << "  " << msg << "   " << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
        ++i;
    }
    std::cout << "\r  " << Color::GREEN << "✔" << Color::RESET
        << "  " << msg << " tamamlandı.\n";
}

// ─────────────────────────────────────────────────────────────
//  İsim girişi
// ─────────────────────────────────────────────────────────────
void CVCalculator::inputName() {
    std::cout << Color::BOLD << "\n  ► lutfen adinizi ve soyadinizi girin: "
        << Color::RESET;
    std::getline(std::cin, *m_name);

    while (m_name->empty()) {
        std::cout << Color::RED
            << "  ✘ İsim boş bırakılamaz. Tekrar girin: "
            << Color::RESET;
        std::getline(std::cin, *m_name);
    }
    std::cout << Color::GREEN << "\n  Merhaba, " << *m_name << "!\n"
        << Color::RESET;
}

// ─────────────────────────────────────────────────────────────
//  Not ortalaması girişi
// ─────────────────────────────────────────────────────────────
void CVCalculator::inputGPA() {
    printSeparator('-', 58);
    std::cout << Color::BOLD << "  [ NOT ORTALAMASI ]\n" << Color::RESET;
    std::cout << "  ⚠  lutfen not ortalamsini nokta (.) ile girin.\n"
        << "     Or: 3.05  |  2.78  |  3.50\n\n";

    while (true) {
        std::cout << Color::BOLD << "  ► Not ortalamanizi girin: " << Color::RESET;
        std::string input;
        std::cin >> input;
        clearInputBuffer();

        // Virgül kontrolü
        if (input.find(',') != std::string::npos) {
            std::cout << Color::RED
                << "  ✘ Virgül (,) kullandınız! Lütfen nokta (.) kullanın."
                << " Örnek: 3,05 → 3.05\n"
                << Color::RESET;
            continue;
        }

        // Sayısal dönüşüm
        try {
            std::size_t pos;
            double val = std::stod(input, &pos);

            if (pos != input.size()) {
                std::cout << Color::RED
                    << "  ✘ Geçersiz format. Sadece sayı girin.\n"
                    << Color::RESET;
                continue;
            }

            if (val < ScoreConstants::GPA_MIN || val > ScoreConstants::GPA_MAX) {
                std::cout << Color::RED
                    << "  ✘ Not ortalaması 0.00 ile 4.00 arasında olmalıdır.\n"
                    << Color::RESET;
                continue;
            }

            m_gpa = val;
            break;

        }
        catch (...) {
            std::cout << Color::RED
                << "  ✘ Geçersiz giriş. Lütfen sayısal bir değer girin.\n"
                << Color::RESET;
        }
    }

    // Kritiklik uyarısı
    std::cout << "\n";
    if (m_gpa < ScoreConstants::GPA_CRITICAL_THRESHOLD) {
        std::cout << Color::RED << Color::BOLD
            << "  ⚠  KRİTİK UYARI: Not ortalamanız " << std::fixed
            << std::setprecision(2) << m_gpa
            << " ile kritik seviyenin altındadır!\n"
            << "     CV başarı oranına katkısı: %" << ScoreConstants::GPA_SCORE_CRITICAL
            << " (Düşük rekabet gücü)\n"
            << Color::RESET;
    }
    else if (m_gpa < ScoreConstants::GPA_NORMAL_THRESHOLD) {
        std::cout << Color::YELLOW
            << "  ✓ Not ortalamanız: " << std::fixed << std::setprecision(2) << m_gpa
            << " → Normal Düzey | Katkı: %" << ScoreConstants::GPA_SCORE_NORMAL << "\n"
            << Color::RESET;
    }
    else if (m_gpa < ScoreConstants::GPA_COMPETITIVE_THRESHOLD) {
        std::cout << Color::GREEN
            << "  ✓ Not ortalamanız: " << std::fixed << std::setprecision(2) << m_gpa
            << " → Rekabet Edilebilir | Katkı: %" << ScoreConstants::GPA_SCORE_COMPETITIVE << "\n"
            << Color::RESET;
    }
    else {
        std::cout << Color::CYAN << Color::BOLD
            << "  ★ Not ortalamanız: " << std::fixed << std::setprecision(2) << m_gpa
            << " → Yüksek Rekabet | Katkı: %" << ScoreConstants::GPA_SCORE_HIGH << "\n"
            << Color::RESET;
    }
}

// ─────────────────────────────────────────────────────────────
//  Meslek seçimi
// ─────────────────────────────────────────────────────────────
void CVCalculator::inputProfession() {
    printSeparator('-', 58);
    std::cout << Color::BOLD << Color::MAGENTA
        << "  [ BETA MODELI – MESLEK SEÇİMİ ]\n"
        << Color::RESET
        << "  Bu model şu an yalnızca Elektronik Mühendisleri içindir.\n\n"
        << "    1. Elektronik Mühendisliği\n\n"
        << Color::BOLD << "  ► Seçiminiz (1): " << Color::RESET;

    int choice = 0;
    while (true) {
        std::cin >> choice;
        clearInputBuffer();
        switch (choice) {
        case 1:
            m_profession = Profession::ELECTRONIC_ENGINEERING;
            std::cout << Color::GREEN
                << "  ✔ Elektronik Mühendisliği seçildi.\n"
                << Color::RESET;
            return;
        default:
            std::cout << Color::RED
                << "  ✘ Geçersiz seçim. Lütfen 1 girin: "
                << Color::RESET;
        }
    }
}

// ─────────────────────────────────────────────────────────────
//  Uzmanlık seçimi
// ─────────────────────────────────────────────────────────────
void CVCalculator::inputSpecialization() {
    printSeparator('-', 58);
    std::cout << Color::BOLD
        << "  [ UZMANLIK ALANI ]\n\n"
        << Color::RESET
        << "    1. Gömülü Yazılım\n"
        << "    2. Sayısal Elektronik Tasarım\n"
        << "    3. Donanım Tasarım\n"
        << "    4. Güç Elektroniği\n\n"
        << Color::BOLD << "  ► Uzmanlık alanınızı seçin (1-4): " << Color::RESET;

    int choice = 0;
    while (true) {
        std::cin >> choice;
        clearInputBuffer();
        switch (choice) {
        case 1:
            m_spec = ElectronicSpec::EMBEDDED_SOFTWARE;
            std::cout << Color::GREEN << "  ✔ Gömülü Yazılım seçildi.\n" << Color::RESET;
            return;
        case 2:
            m_spec = ElectronicSpec::DIGITAL_ELECTRONICS;
            std::cout << Color::GREEN << "  ✔ Sayısal Elektronik Tasarım seçildi.\n" << Color::RESET;
            return;
        case 3:
            m_spec = ElectronicSpec::HARDWARE_DESIGN;
            std::cout << Color::GREEN << "  ✔ Donanım Tasarım seçildi.\n" << Color::RESET;
            return;
        case 4:
            m_spec = ElectronicSpec::POWER_ELECTRONICS;
            std::cout << Color::GREEN << "  ✔ Güç Elektroniği seçildi.\n" << Color::RESET;
            return;
        default:
            std::cout << Color::RED
                << "  ✘ Geçersiz seçim (1-4): "
                << Color::RESET;
        }
    }
}

// ─────────────────────────────────────────────────────────────
//  Dil menüsü
// ─────────────────────────────────────────────────────────────
void CVCalculator::printLanguageMenu() const {
    std::cout << "\n"
        << "  ┌─────────────────────────────────────────────┐\n"
        << "  │   #   Dil / Araç           Açıklama         │\n"
        << "  ├─────────────────────────────────────────────┤\n"
        << "  │   1   C++                  Nesne Yönelimli  │\n"
        << "  │   2   C                    Gömülü/Sistem    │\n"
        << "  │   3   Java                 Genel Amaçlı     │\n"
        << "  │   4   HTML                 Web Front-End    │\n"
        << "  │   5   React.js             Web Front-End    │\n"
        << "  │   6   Python               Veri/Otomasyon   │\n"
        << "  │   7   C#                   .NET Geliştirme  │\n"
        << "  │   8   MATLAB               Simülasyon       │\n"
        << "  │   9   KiCad                PCB Tasarım      │\n"
        << "  │  10   Altium Designer       PCB Tasarım      │\n"
        << "  │  11   Simulink              Model Tabanlı    │\n"
        << "  │  12   LTSpice              Devre Sim.       │\n"
        << "  │  13   Xilinx (Vivado/ISE)  FPGA Tasarım     │\n"
        << "  │  14   Altera (Quartus)      FPGA Tasarım     │\n"
        << "  │  15   Verilog              HDL Dili         │\n"
        << "  │  16   VHDL                 HDL Dili         │\n"
        << "  └─────────────────────────────────────────────┘\n";

    // Özel araçları uzmanlığa göre vurgula
    std::cout << Color::YELLOW;
    if (m_spec == ElectronicSpec::EMBEDDED_SOFTWARE)
        std::cout << "  ★ Gömülü Yazılım için özel araçlar: MATLAB(8), Simulink(11), LTSpice(12) → %1.4 bonus\n";
    else if (m_spec == ElectronicSpec::DIGITAL_ELECTRONICS)
        std::cout << "  ★ Sayısal Elektronik için özel araçlar: Xilinx(13), Altera(14) → %1.4 bonus\n";
    else if (m_spec == ElectronicSpec::HARDWARE_DESIGN)
        std::cout << "  ★ Donanım Tasarım için araçlar: KiCad(9), Altium(10) → %0.7 her biri\n";
    std::cout << Color::RESET;
}

// ─────────────────────────────────────────────────────────────
//  Özel araç kontrolcüleri
// ─────────────────────────────────────────────────────────────
bool CVCalculator::isEmbeddedSpecialTool(Language lang) const {
    return (lang == Language::MATLAB ||
        lang == Language::SIMULINK ||
        lang == Language::LTSPICE);
}

bool CVCalculator::isDigitalSpecialTool(Language lang) const {
    return (lang == Language::XILINX ||
        lang == Language::ALTERA);
}

bool CVCalculator::isHardwareTool(Language lang) const {
    return (lang == Language::KICAD ||
        lang == Language::ALTIUM);
}

// ─────────────────────────────────────────────────────────────
//  Dil girişi
// ─────────────────────────────────────────────────────────────
void CVCalculator::inputLanguages() {
    printSeparator('-', 58);
    std::cout << Color::BOLD
        << "  [ PROGRAMLAMA & UYGULAMA BİLGİSİ ]\n"
        << Color::RESET;
    printLanguageMenu();

    std::cout << "\n  Bildiğiniz dil/araçların numaralarını boşlukla\n"
        << "  ayırarak girin. Örnek: 1 2 6 8\n"
        << Color::BOLD << "\n  ► Seçimleriniz: " << Color::RESET;

    std::string line;
    std::getline(std::cin, line);

    std::istringstream iss(line);
    int num;
    while (iss >> num) {
        if (num >= 1 && num <= ScoreConstants::LANG_COUNT) {
            Language lang = static_cast<Language>(num);
            // Tekrar eklemeyi önle
            auto& langs = *m_languages;
            if (std::find(langs.begin(), langs.end(), lang) == langs.end()) {
                langs.push_back(lang);
            }
        }
    }

    if (m_languages->empty()) {
        std::cout << Color::YELLOW
            << "  ⚠ Hiçbir dil/araç seçilmedi.\n"
            << Color::RESET;
    }
    else {
        std::cout << Color::GREEN << "  ✔ " << m_languages->size()
            << " dil/araç seçildi.\n" << Color::RESET;
    }
}

// ─────────────────────────────────────────────────────────────
//  Yardımcı: Language → görünen ad
// ─────────────────────────────────────────────────────────────
static std::string langName(Language lang) {
    switch (lang) {
    case Language::CPP:      return "C++";
    case Language::C:        return "C";
    case Language::JAVA:     return "Java";
    case Language::HTML:     return "HTML";
    case Language::REACTJS:  return "React.js";
    case Language::PYTHON:   return "Python";
    case Language::CSHARP:   return "C#";
    case Language::MATLAB:   return "MATLAB";
    case Language::KICAD:    return "KiCad";
    case Language::ALTIUM:   return "Altium Designer";
    case Language::SIMULINK: return "Simulink";
    case Language::LTSPICE:  return "LTSpice";
    case Language::XILINX:   return "Xilinx (Vivado/ISE)";
    case Language::ALTERA:   return "Altera (Quartus)";
    case Language::VERILOG:  return "Verilog";
    case Language::VHDL:     return "VHDL";
    default:                 return "Bilinmeyen";
    }
}

// ─────────────────────────────────────────────────────────────
//  Deneyim süresi girişi  – her dil için ayrı ayrı
// ─────────────────────────────────────────────────────────────
void CVCalculator::inputExperience() {
    printSeparator('-', 58);
    std::cout << Color::BOLD
        << "  [ DENEYIM SÜRESİ – Her Dil / arac icin ]\n"
        << Color::RESET;

    if (m_languages->empty()) {
        std::cout << Color::YELLOW
            << "  ⚠ Seçili dil/araç yok, deneyim puanı hesaplanmayacak.\n"
            << Color::RESET;
        return;
    }

    std::cout << "  sectiginiz " << m_languages->size()
        << " dil/arac icin calısma surelerini ayrı ayrı gireceksiniz.\n\n";

    int langIdx = 0;
    for (const Language& lang : *m_languages) {
        ++langIdx;
        std::string name = langName(lang);

        std::cout << Color::CYAN << Color::BOLD
            << "  [" << langIdx << "/" << m_languages->size() << "] "
            << name << Color::RESET << "\n";

        // ── Zaman birimi ──────────────────────────────────────
        std::cout << "    Birim secin → 1. Ay   2. Yil\n"
            << Color::BOLD << "    ► Birim (1/2): " << Color::RESET;

        int unitChoice = 0;
        while (true) {
            std::cin >> unitChoice;
            clearInputBuffer();
            if (unitChoice == 1 || unitChoice == 2) break;
            std::cout << Color::RED
                << "    ✘ Lütfen 1 (Ay) veya 2 (Yıl) girin: "
                << Color::RESET;
        }
        TimeUnit unit = (unitChoice == 1) ? TimeUnit::MONTH : TimeUnit::YEAR;

        // ── Süre ──────────────────────────────────────────────
        std::cout << Color::BOLD << "    ► Süre (tam sayı): " << Color::RESET;
        int duration = 0;
        while (true) {
            std::cin >> duration;
            clearInputBuffer();
            if (duration >= 0) break;
            std::cout << Color::RED
                << "    ✘ Negatif değer girilemez: "
                << Color::RESET;
        }

        // ── Ay'a çevir & kaydet ───────────────────────────────
        double months = 0.0;
        if (unit == TimeUnit::YEAR) {
            months = static_cast<double>(duration * ScoreConstants::MONTHS_PER_YEAR);
            std::cout << Color::GREEN
                << "    ✔ " << duration << " yıl = "
                << static_cast<int>(months) << " ay | +"
                << std::fixed << std::setprecision(2)
                << (months * ScoreConstants::EXP_SCORE_PER_MONTH) << "%\n"
                << Color::RESET;
        }
        else {
            months = static_cast<double>(duration);
            if (duration < ScoreConstants::MONTHS_PER_YEAR) {
                std::cout << Color::GREEN
                    << "    ✔ " << duration << " ay | +"
                    << std::fixed << std::setprecision(2)
                    << (months * ScoreConstants::EXP_SCORE_PER_MONTH) << "%\n"
                    << Color::RESET;
            }
            else {
                double years = months / ScoreConstants::MONTHS_PER_YEAR;
                std::cout << Color::GREEN
                    << "    ✔ " << duration << " ay ≈ "
                    << std::fixed << std::setprecision(1) << years
                    << " yıl | +"
                    << std::fixed << std::setprecision(2)
                    << (months * ScoreConstants::EXP_SCORE_PER_MONTH) << "%\n"
                    << Color::RESET;
            }
        }

        (*m_langExpMonths)[lang] = months;
    }

    // ── Özet tablosu ──────────────────────────────────────────
    printSeparator('-', 58);
    std::cout << Color::BOLD << "  DENEYİM ÖZETİ\n" << Color::RESET;
    std::cout << "  " << std::left
        << std::setw(22) << "Dil / Araç"
        << std::setw(10) << "Ay"
        << std::setw(10) << "Yıl"
        << "Puan\n";
    std::cout << "  " << std::string(50, '-') << "\n";

    double totalMonths = 0.0;
    double totalExpScore = 0.0;
    for (const auto& entry : *m_langExpMonths) {
        double mos = entry.second;
        double score = mos * ScoreConstants::EXP_SCORE_PER_MONTH;
        totalMonths += mos;
        totalExpScore += score;
        std::cout << "  " << std::left
            << std::setw(22) << langName(entry.first)
            << std::setw(10) << static_cast<int>(mos)
            << std::fixed << std::setprecision(1)
            << std::setw(10) << (mos / ScoreConstants::MONTHS_PER_YEAR)
            << "%" << std::setprecision(2) << score << "\n";
    }
    std::cout << "  " << std::string(50, '-') << "\n";
    std::cout << Color::BOLD << "  " << std::left
        << std::setw(22) << "TOPLAM"
        << std::setw(10) << static_cast<int>(totalMonths)
        << std::fixed << std::setprecision(1)
        << std::setw(10) << (totalMonths / ScoreConstants::MONTHS_PER_YEAR)
        << Color::GREEN << "%" << std::setprecision(2) << totalExpScore
        << Color::RESET << "\n";
}

// ─────────────────────────────────────────────────────────────
//  Sertifika girişi
// ─────────────────────────────────────────────────────────────
void CVCalculator::inputCertificates() {
    printSeparator('-', 58);
    std::cout << Color::BOLD
        << "  [ SERTİFİKA ]\n"
        << Color::RESET
        << "  Bu alanda geçerli bir sertifikanız var mı? (evet/hayır)\n"
        << Color::BOLD << "  ► Cevabınız: " << Color::RESET;

    std::string ans;
    std::cin >> ans;
    clearInputBuffer();
    ans = toLower(ans);

    m_hasCert = (ans == "evet" || ans == "e" || ans == "yes" || ans == "y");

    if (m_hasCert) {
        std::cout << Color::BOLD << "  ► Kaç tane sertifikanız var? " << Color::RESET;
        while (true) {
            std::cin >> m_certCount;
            clearInputBuffer();
            if (m_certCount > 0) break;
            std::cout << Color::RED << "  ✘ En az 1 girmelisiniz: " << Color::RESET;
        }
        std::cout << Color::GREEN
            << "  ✔ " << m_certCount << " sertifika → +"
            << std::fixed << std::setprecision(1)
            << (m_certCount * ScoreConstants::CERT_SCORE_PER_CERT) << "%\n"
            << Color::RESET;
    }
    else {
        std::cout << Color::YELLOW << "  ✗ Sertifika yok.\n" << Color::RESET;
    }
}

// ─────────────────────────────────────────────────────────────
//  Proje girişi
// ─────────────────────────────────────────────────────────────
void CVCalculator::inputProjects() {
    printSeparator('-', 58);
    std::cout << Color::BOLD
        << "  [ PROJE DENEYİMİ ]\n"
        << Color::RESET;

    std::cout << Color::BOLD
        << "  ► Bu alanda kaç proje yaptınız? " << Color::RESET;
    while (true) {
        std::cin >> m_projectCount;
        clearInputBuffer();
        if (m_projectCount >= 0) break;
        std::cout << Color::RED << "  ✘ Negatif değer girilemez: " << Color::RESET;
    }

    std::cout << Color::BOLD
        << "  ► GitHub hesabınızda kaç projeniz var? " << Color::RESET;
    while (true) {
        std::cin >> m_githubRepos;
        clearInputBuffer();
        if (m_githubRepos >= 0) break;
        std::cout << Color::RED << "  ✘ Negatif değer girilemez: " << Color::RESET;
    }

    constexpr double maxProj = ScoreConstants::PROJECT_MAX_SCORE;
    double preview = m_projectCount * ScoreConstants::PROJECT_WEIGHT +
        m_githubRepos * ScoreConstants::GITHUB_WEIGHT;
    if (preview > maxProj) preview = maxProj;

    std::cout << Color::GREEN
        << "  ✔ Proje puanı tahmini: %" << std::fixed << std::setprecision(2) << preview
        << " (maks %" << maxProj << ")\n"
        << Color::RESET;
}

// ─────────────────────────────────────────────────────────────
//  Yarışma girişi
// ─────────────────────────────────────────────────────────────
void CVCalculator::inputCompetitions() {
    printSeparator('-', 58);
    std::cout << Color::BOLD
        << "  [ YARIŞMA & ÖDÜLLER ]\n"
        << Color::RESET;

    std::cout << Color::BOLD
        << "  ► Yarismalarda kac odul aldiniz? " << Color::RESET;
    while (true) {
        std::cin >> m_awardCount;
        clearInputBuffer();
        if (m_awardCount >= 0) break;
        std::cout << Color::RED << "  ✘ Negatif değer girilemez: " << Color::RESET;
    }

    if (m_awardCount > 0) {
        std::cout << Color::GREEN
            << "  ✔ " << m_awardCount << " odul → +"
            << std::fixed << std::setprecision(1)
            << (m_awardCount * ScoreConstants::COMP_PER_AWARD) << "%\n"
            << Color::RESET;
    }

    std::cout << Color::BOLD
        << "  ► yarismalarda derece elde ettiniz mi? (evet/hayir) "
        << Color::RESET;
    std::string ans;
    std::cin >> ans;
    clearInputBuffer();
    m_hasRanking = (toLower(ans) == "evet" || toLower(ans) == "e");

    if (m_hasRanking) {
        std::cout << "  Dereceyi girin (1=Birincilik, 2=İkincilik, 3=Üçüncülük)\n"
            << Color::BOLD << "  ► Derece: " << Color::RESET;
        while (true) {
            std::cin >> m_rankingPlace;
            clearInputBuffer();
            if (m_rankingPlace >= 1 && m_rankingPlace <= 3) break;
            std::cout << Color::RED << "  ✘ Lütfen 1, 2 veya 3 girin: " << Color::RESET;
        }

        double rankBonus = 0.0;
        switch (m_rankingPlace) {
        case 1: rankBonus = ScoreConstants::COMP_RANK_1ST; break;
        case 2: rankBonus = ScoreConstants::COMP_RANK_2ND; break;
        case 3: rankBonus = ScoreConstants::COMP_RANK_3RD; break;
        }
        std::cout << Color::GREEN
            << "  ✔ " << m_rankingPlace << ". derece → +"
            << rankBonus << "%\n"
            << Color::RESET;
    }
}

// ─────────────────────────────────────────────────────────────
//  ANA AKIŞ
// ─────────────────────────────────────────────────────────────
void CVCalculator::run() {
    welcome();
    inputName();
    inputGPA();
    inputProfession();
    inputSpecialization();
    inputLanguages();
    inputExperience();
    inputCertificates();
    inputProjects();
    inputCompetitions();
    calculateScores();
    showReport();
}

// ─────────────────────────────────────────────────────────────
//  HESAPLAMA FONKSİYONLARI
// ─────────────────────────────────────────────────────────────

double CVCalculator::calcGPAScore() const {
    if (m_gpa < ScoreConstants::GPA_CRITICAL_THRESHOLD)
        return ScoreConstants::GPA_SCORE_CRITICAL;
    if (m_gpa < ScoreConstants::GPA_NORMAL_THRESHOLD)
        return ScoreConstants::GPA_SCORE_NORMAL;
    if (m_gpa < ScoreConstants::GPA_COMPETITIVE_THRESHOLD)
        return ScoreConstants::GPA_SCORE_COMPETITIVE;
    return ScoreConstants::GPA_SCORE_HIGH;
}

double CVCalculator::calcLanguageScore() const {
    double score = 0.0;
    const auto& langs = *m_languages;

    for (const auto& lang : langs) {
        // Özel araç bonusu
        if (m_spec == ElectronicSpec::EMBEDDED_SOFTWARE && isEmbeddedSpecialTool(lang)) {
            score += ScoreConstants::LANG_SCORE_SPECIAL;
        }
        else if (m_spec == ElectronicSpec::DIGITAL_ELECTRONICS && isDigitalSpecialTool(lang)) {
            score += ScoreConstants::LANG_SCORE_SPECIAL;
        }
        else {
            score += ScoreConstants::LANG_SCORE_PER_LANG;
        }
    }
    return score;
}

double CVCalculator::calcExpScore() const {
    double total = 0.0;
    for (const auto& entry : *m_langExpMonths) {
        total += entry.second * ScoreConstants::EXP_SCORE_PER_MONTH;
    }
    return total;
}

double CVCalculator::calcCertScore() const {
    if (!m_hasCert) return 0.0;
    return m_certCount * ScoreConstants::CERT_SCORE_PER_CERT;
}

double CVCalculator::calcProjectScore() const {
    constexpr double maxScore = ScoreConstants::PROJECT_MAX_SCORE;
    double raw = m_projectCount * ScoreConstants::PROJECT_WEIGHT
        + m_githubRepos * ScoreConstants::GITHUB_WEIGHT;
    return raw < maxScore ? raw : maxScore;
}

double CVCalculator::calcCompScore() const {
    double score = m_awardCount * ScoreConstants::COMP_PER_AWARD;
    if (m_hasRanking) {
        switch (m_rankingPlace) {
        case 1: score += ScoreConstants::COMP_RANK_1ST; break;
        case 2: score += ScoreConstants::COMP_RANK_2ND; break;
        case 3: score += ScoreConstants::COMP_RANK_3RD; break;
        }
    }
    return score;
}


//  ÇOKLU THREAD İLE PARALEL HESAPLAMA

void CVCalculator::calculateScores() {
    printSeparator('=', 58);
    std::cout << Color::BOLD << Color::CYAN
        << "  SKORLAR HESAPLANIYOR...\n"
        << Color::RESET;

    // Her bileşen için geçici sonuç değişkenleri
    double r_gpa = 0.0, r_lang = 0.0, r_exp = 0.0;
    double r_cert = 0.0, r_proj = 0.0, r_comp = 0.0;

    // ── Thread tanımları ──────────────────────────────────────
    std::thread t_gpa([&]() {
        double v = calcGPAScore();
        std::lock_guard<std::mutex> lock(m_mutex);
        r_gpa = v;
        });

    std::thread t_lang([&]() {
        double v = calcLanguageScore();
        std::lock_guard<std::mutex> lock(m_mutex);
        r_lang = v;
        });

    std::thread t_exp([&]() {
        double v = calcExpScore();
        std::lock_guard<std::mutex> lock(m_mutex);
        r_exp = v;
        });

    std::thread t_cert([&]() {
        double v = calcCertScore();
        std::lock_guard<std::mutex> lock(m_mutex);
        r_cert = v;
        });

    std::thread t_proj([&]() {
        double v = calcProjectScore();
        std::lock_guard<std::mutex> lock(m_mutex);
        r_proj = v;
        });

    std::thread t_comp([&]() {
        double v = calcCompScore();
        std::lock_guard<std::mutex> lock(m_mutex);
        r_comp = v;
        });

    // ── Animasyon thread'i ────────────────────────────────────
    std::thread t_anim([this]() {
        loadingAnimation("Bileşen skorları işleniyor", 1200);
        });

    // ── Tüm thread'leri bekle ─────────────────────────────────
    t_gpa.join();
    t_lang.join();
    t_exp.join();
    t_cert.join();
    t_proj.join();
    t_comp.join();
    t_anim.join();

    // ── Sonuçları unique_ptr'a aktar ──────────────────────────
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_gpaScore = std::make_unique<double>(r_gpa);
        m_langScore = std::make_unique<double>(r_lang);
        m_expScore = std::make_unique<double>(r_exp);
        m_certScore = std::make_unique<double>(r_cert);
        m_projectScore = std::make_unique<double>(r_proj);
        m_compScore = std::make_unique<double>(r_comp);

        constexpr double maxScore = ScoreConstants::MAX_SCORE;
        double total = r_gpa + r_lang + r_exp + r_cert + r_proj + r_comp;
        m_totalScore = std::make_unique<double>(
            total < maxScore ? total : maxScore
        );
    }

    std::cout << Color::GREEN << Color::BOLD
        << "  ✔ Hesaplama tamamlandı!\n"
        << Color::RESET;
}

// ─────────────────────────────────────────────────────────────
//  operator<< (friend)
// ─────────────────────────────────────────────────────────────
std::ostream& operator<<(std::ostream& os, const CVCalculator& calc) {
    os << std::fixed << std::setprecision(2);
    os << "  [ " << *calc.m_name << " | GPA: " << calc.m_gpa;
    if (calc.m_totalScore)
        os << " | Toplam Skor: %" << *calc.m_totalScore;
    os << " ]\n";
    return os;
}

// ─────────────────────────────────────────────────────────────
//  showReport – ScoreReport'u çağırır
// ─────────────────────────────────────────────────────────────
void CVCalculator::showReport() const {
    ScoreReport::printFull(*this);
}

// =============================================================
//  ScoreReport implementasyonu
// =============================================================

void ScoreReport::printBar(const std::string& label, double score, double maxScore) {
    const int BAR_WIDTH = 30;
    int filled = static_cast<int>((score / maxScore) * BAR_WIDTH);
    filled = std::min(filled, BAR_WIDTH);

    // Renk seçimi
    const char* col = Color::GREEN;
    if (score / maxScore < 0.3)      col = Color::RED;
    else if (score / maxScore < 0.6) col = Color::YELLOW;

    std::cout << "  " << std::left << std::setw(30) << label;
    std::cout << col << "[";
    for (int i = 0; i < BAR_WIDTH; ++i)
        std::cout << (i < filled ? "█" : "░");
    std::cout << "] " << Color::RESET
        << Color::BOLD
        << std::right << std::fixed << std::setprecision(2)
        << std::setw(6) << score << "%" << Color::RESET << "\n";
}

void ScoreReport::printFull(const CVCalculator& calc) {
    printSeparator('=', 58);
    std::cout << Color::BOLD << Color::CYAN
        << "  ╔══════════════════════════════════════════════╗\n"
        << "  ║          CV BAŞARI ORANI RAPORU              ║\n"
        << "  ╚══════════════════════════════════════════════╝\n"
        << Color::RESET;

    std::cout << Color::BOLD
        << "\n  Aday  : " << Color::CYAN << *calc.m_name << Color::RESET << "\n"
        << Color::BOLD
        << "  GPA   : " << Color::CYAN
        << std::fixed << std::setprecision(2) << calc.m_gpa
        << Color::RESET << "\n";

    // Uzmanlık adı
    std::string specName;
    switch (calc.m_spec) {
    case ElectronicSpec::EMBEDDED_SOFTWARE:   specName = "Gömülü Yazılım";            break;
    case ElectronicSpec::DIGITAL_ELECTRONICS: specName = "Sayısal Elektronik Tasarım"; break;
    case ElectronicSpec::HARDWARE_DESIGN:     specName = "Donanım Tasarım";            break;
    case ElectronicSpec::POWER_ELECTRONICS:   specName = "Güç Elektroniği";            break;
    }
    std::cout << Color::BOLD
        << "  Alan  : " << Color::CYAN << specName << Color::RESET << "\n\n";

    printSeparator('-', 58);
    std::cout << Color::BOLD << "  PUAN DAĞILIMI\n" << Color::RESET;
    printSeparator('-', 58);

    double gpa = calc.m_gpaScore ? *calc.m_gpaScore : 0.0;
    double lang = calc.m_langScore ? *calc.m_langScore : 0.0;
    double exp = calc.m_expScore ? *calc.m_expScore : 0.0;
    double cert = calc.m_certScore ? *calc.m_certScore : 0.0;
    double proj = calc.m_projectScore ? *calc.m_projectScore : 0.0;
    double comp = calc.m_compScore ? *calc.m_compScore : 0.0;

    printBar("Not Ortalaması", gpa, ScoreConstants::GPA_SCORE_HIGH);
    printBar("Dil & Araç Bilgisi", lang, 15.0);
    printBar("Deneyim Süresi", exp, 24.0);
    printBar("Sertifikalar", cert, 10.0);
    printBar("Proje Deneyimi", proj, ScoreConstants::PROJECT_MAX_SCORE);
    printBar("Yarışma & Ödüller", comp, 6.0);

    printSeparator('=', 58);

    double total = calc.m_totalScore ? *calc.m_totalScore : 0.0;

    // Toplam puan rengi
    const char* totColor = Color::RED;
    std::string rating = "Zayıf";
    if (total >= 80.0) { totColor = Color::CYAN;    rating = "Mükemmel ★★★★★"; }
    else if (total >= 60.0) { totColor = Color::GREEN;  rating = "İyi       ★★★★☆"; }
    else if (total >= 40.0) { totColor = Color::YELLOW; rating = "Orta      ★★★☆☆"; }
    else if (total >= 20.0) { totColor = Color::RED;    rating = "Düşük     ★★☆☆☆"; }
    else { totColor = Color::RED;    rating = "Zayıf     ★☆☆☆☆"; }

    std::cout << Color::BOLD << "  TOPLAM CV BAŞARI ORANI : "
        << totColor << Color::BOLD
        << std::fixed << std::setprecision(2) << total << "%"
        << Color::RESET << "\n";
    std::cout << Color::BOLD << "  DEĞERLENDİRME          : "
        << totColor << rating << Color::RESET << "\n";

    printSeparator('=', 58);

    // Öneri
    std::cout << Color::BOLD << "\n  ÖNERİLER:\n" << Color::RESET;
    if (gpa < ScoreConstants::GPA_SCORE_NORMAL)
        std::cout << Color::RED << "  • Not ortalamanızı yükseltin (kritik seviyede!)\n" << Color::RESET;
    if (lang < 2.0)
        std::cout << Color::YELLOW << "  • Daha fazla programlama dili / araç öğrenin.\n" << Color::RESET;
    if (exp < 2.0)
        std::cout << Color::YELLOW << "  • Pratik çalışma sürenizi artırın.\n" << Color::RESET;
    if (cert == 0.0)
        std::cout << Color::YELLOW << "  • Alanınızla ilgili sertifika edinin.\n" << Color::RESET;
    if (proj < 3.0)
        std::cout << Color::YELLOW << "  • Daha fazla proje yapın ve GitHub'a yükleyin.\n" << Color::RESET;
    if (comp == 0.0)
        std::cout << Color::YELLOW << "  • Yarışmalara katılarak tecrübe kazanın.\n" << Color::RESET;
    if (total >= 60.0)
        std::cout << Color::GREEN << "  • Profiliniz rekabetçi görünüyor, başvuruya devam!\n" << Color::RESET;

    printSeparator('=', 58);
    std::cout << "\n";
}
