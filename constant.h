#pragma once

// =============================================================
//  ScoreConstants – tüm sabit katsayılar bu sınıfta saklanır.
//  Değerlerin yanlışlıkla değiştirilmesini engellemek için
//  static constexpr kullanılmıştır.
// =============================================================
class ScoreConstants
{
public:
    // ─── Not Ortalaması Eşik Değerleri ───────────────────────
    static constexpr double GPA_CRITICAL_THRESHOLD = 2.75;
    static constexpr double GPA_NORMAL_THRESHOLD = 3.00;
    static constexpr double GPA_COMPETITIVE_THRESHOLD = 3.30;
    static constexpr double GPA_MAX = 4.00;
    static constexpr double GPA_MIN = 0.00;

    // ─── Not Ortalaması Katkı Puanları (%) ───────────────────
    static constexpr double GPA_SCORE_CRITICAL = 10.0;
    static constexpr double GPA_SCORE_NORMAL = 15.0;
    static constexpr double GPA_SCORE_COMPETITIVE = 20.0;
    static constexpr double GPA_SCORE_HIGH = 35.0;

    // ─── Programlama / Uygulama Dilleri ──────────────────────
    static constexpr double LANG_SCORE_PER_LANG = 0.7;   // Her dil
    static constexpr double LANG_SCORE_SPECIAL = 1.4;   // Özel araçlar

    // ─── Deneyim (ay başına) ─────────────────────────────────
    static constexpr double EXP_SCORE_PER_MONTH = 0.5;
    static constexpr int    MONTHS_PER_YEAR = 12;

    // ─── Sertifika ───────────────────────────────────────────
    static constexpr double CERT_SCORE_PER_CERT = 2.0;

    // ─── Proje ───────────────────────────────────────────────
    static constexpr double PROJECT_MAX_SCORE = 10.0;
    static constexpr double PROJECT_WEIGHT = 0.80;  // proje başına
    static constexpr double GITHUB_WEIGHT = 0.20;  // repo başına

    // ─── Yarışma ─────────────────────────────────────────────
    static constexpr double COMP_PER_AWARD = 0.3;
    static constexpr double COMP_RANK_1ST = 2.5;
    static constexpr double COMP_RANK_2ND = 1.2;
    static constexpr double COMP_RANK_3RD = 0.8;

    // ─── Genel ───────────────────────────────────────────────
    static constexpr double MAX_SCORE = 100.0;
    static constexpr int    LANG_COUNT = 16;
};
