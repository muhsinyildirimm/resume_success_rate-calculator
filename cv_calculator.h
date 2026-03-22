#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <iostream>
#include <iomanip>
#include "constants.h"
#include "enums.h"

// ─────────────────────────────────────────────────────────────
//  Ön bildirim – ScoreReport friend class olarak kullanılacak
// ─────────────────────────────────────────────────────────────
class ScoreReport;

// =============================================================
//  CVCalculator  –  Ana hesaplama sınıfı
// =============================================================
class CVCalculator
{
private:
    // ── Kullanıcı Bilgileri ───────────────────────────────────
    std::unique_ptr<std::string> m_name;
    double                       m_gpa{ 0.0 };

    // ── Meslek / Uzmanlık ─────────────────────────────────────
    Profession    m_profession{ Profession::ELECTRONIC_ENGINEERING };
    ElectronicSpec m_spec{ ElectronicSpec::EMBEDDED_SOFTWARE };

    // ── Dil & Araç Seçimleri ──────────────────────────────────
    std::unique_ptr<std::vector<Language>> m_languages;
    // Her dil için ayrı ayrı deneyim süresi (ay cinsinden)
    std::unique_ptr<std::map<Language, double>> m_langExpMonths;

    // ── Sertifika ─────────────────────────────────────────────
    bool m_hasCert{ false };
    int  m_certCount{ 0 };

    // ── Proje ─────────────────────────────────────────────────
    int m_projectCount{ 0 };
    int m_githubRepos{ 0 };

    // ── Yarışma ───────────────────────────────────────────────
    int  m_awardCount{ 0 };
    bool m_hasRanking{ false };
    int  m_rankingPlace{ 0 };

    // ── Puan Bileşenleri (unique_ptr) ─────────────────────────
    std::unique_ptr<double> m_gpaScore;
    std::unique_ptr<double> m_langScore;
    std::unique_ptr<double> m_expScore;
    std::unique_ptr<double> m_certScore;
    std::unique_ptr<double> m_projectScore;
    std::unique_ptr<double> m_compScore;
    std::unique_ptr<double> m_totalScore;

    // ── Thread Güvenliği ──────────────────────────────────────
    mutable std::mutex m_mutex;

    // ── Özel Hesaplama Metodları ──────────────────────────────
    double calcGPAScore()     const;
    double calcLanguageScore() const;
    double calcExpScore()     const;
    double calcCertScore()    const;
    double calcProjectScore() const;
    double calcCompScore()    const;

    // ── Özel Giriş Metodları ──────────────────────────────────
    void inputName();
    void inputGPA();
    void inputProfession();
    void inputSpecialization();
    void inputLanguages();
    void inputExperience();
    void inputCertificates();
    void inputProjects();
    void inputCompetitions();

    // ── Yardımcı Metodlar ─────────────────────────────────────
    void  printLanguageMenu()                  const;
    bool  isEmbeddedSpecialTool(Language lang) const;
    bool  isDigitalSpecialTool(Language lang)  const;
    bool  isHardwareTool(Language lang)        const;
    void  loadingAnimation(const std::string& msg, int ms = 600) const;

    // ── Geçersiz Kopyalama – tekil nesne ──────────────────────
    CVCalculator(const CVCalculator&) = delete;
    CVCalculator& operator=(const CVCalculator&) = delete;

public:
    CVCalculator();
    ~CVCalculator() = default;

    CVCalculator(CVCalculator&&) = default;

    void welcome()  const;
    void run();
    void calculateScores();
    void showReport() const;

    // ── Getter'lar (ScoreReport erişimi için) ─────────────────
    const std::string& getName()       const { return *m_name; }
    double             getGPA()        const { return m_gpa; }
    ElectronicSpec     getSpec()       const { return m_spec; }
    double             getTotalScore() const { return m_totalScore ? *m_totalScore : 0.0; }

    // ── Friend Bildirimleri ───────────────────────────────────
    friend class ScoreReport;
    friend std::ostream& operator<<(std::ostream& os, const CVCalculator& calc);
};

// =============================================================
//  ScoreReport  –  Rapor görselleştirme yardımcı sınıfı
//  CVCalculator'ın private üyelerine friend olarak erişir.
// =============================================================
class ScoreReport
{
public:
    static void printFull(const CVCalculator& calc);
    static void printBar(const std::string& label, double score, double maxScore);
};
