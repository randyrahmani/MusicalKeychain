#include <Arduino.h>
#include <Wire.h>

#define SSD1306_NO_SPLASH
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <avr/pgmspace.h>

#include "korean_font.h"

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define Gb5 831
#define F5 698
#define Ab5 932
#define C6 1047
#define Cb6 1109
#define Db5 622
#define E5 659
#define BUZZER_PIN 13

static const uint8_t SCREEN_WIDTH = 128;
static const uint8_t SCREEN_HEIGHT = 64;
static const uint8_t OLED_ADDRESS = 0x3C;
static const int8_t OLED_RESET_PIN = -1;
static const uint8_t HANGUL_GLYPH_WIDTH = 16;
static const uint8_t HANGUL_GLYPH_HEIGHT = 16;
static const uint8_t LATIN_GLYPH_WIDTH = 18;
static const uint8_t LATIN_GLYPH_HEIGHT = 20;
static const uint8_t KOREAN_SPACE_WIDTH = 8;
static const uint8_t LINE_HEIGHT = 20;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);

struct NoteEvent {
  uint16_t frequency;
  uint16_t soundMs;
  uint16_t gapMs;
};

// All song and lyric data stays in flash. The OLED framebuffer already uses
// half of the Uno's 2 KB SRAM, so keeping this table out of RAM is essential.
static const NoteEvent SONG[] PROGMEM = {
 {Gb5, 88, 88},
 {Gb5, 88, 88},
 {F5, 88, 88},
 {Gb5, 176, 176},
 {Gb5, 88, 88},
 {Ab5, 265, 88},
 {Gb5, 176, 353},
 {Ab5, 176, 176},
 {Ab5, 88, 88},
 {Ab5, 176, 176},
 {C6, 176, 176},
 {Cb6, 529, 353},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Gb5, 88, 88},
 {Gb5, 88, 88},
 {Gb5, 88, 88},
 {Ab5, 176, 0},
 {C6, 176, 0},
 {Db5, 265, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Gb5, 88, 88},
 {Ab5, 88, 88},
 {C6, 88, 88},
 {Db5, 265, 265},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {C6, 265, 88},
 {Gb5, 265, 88},
 {F5, 265, 88},
 {Db5, 265, 88},
 {C6, 88, 88},
 {C6, 88, 265},
 {C6, 265, 88},
 {Db5, 88, 88},
 {Db5, 88, 88},
 {Db5, 88, 88},
 {E5, 265, 88},
 {E5, 265, 88},
 {Gb5, 265, 88},
 {Gb5, 265, 88},
 {Ab5, 265, 88},
 {Ab5, 265, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Gb5, 88, 88},
 {Gb5, 88, 88},
 {Gb5, 88, 88},
 {Ab5, 88, 88},
 {C6, 88, 88},
 {Db5, 265, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Gb5, 88, 88},
 {Gb5, 88, 88},
 {Gb5, 88, 88},
 {Ab5, 88, 88},
 {C6, 88, 88},
 {Db5, 265, 265},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {C6, 265, 88},
 {Gb5, 265, 88},
 {F5, 265, 88},
 {Db5, 265, 88},
 {C6, 88, 88},
 {C6, 88, 265},
 {C6, 265, 88},
 {Db5, 88, 88},
 {Db5, 88, 88},
 {Db5, 88, 88},
 {E5, 265, 88},
 {E5, 265, 88},
 {Gb5, 265, 88},
 {Gb5, 265, 88},
 {Ab5, 265, 88},
 {Ab5, 265, 88},
 {Ab5, 265, 88},
 {Gb5, 265, 88},
 {Gb5, 706, 0},
};

// Each entry is one physical OLED line. Longer phrases are deliberately
// wrapped so the enlarged, centered text never exceeds 128 pixels.
static const char LYRIC_00[] PROGMEM = u8"그게 언제쯤일까?";
static const char LYRIC_01[] PROGMEM = u8"어떤 사람일까?";
static const char LYRIC_02[] PROGMEM = "I wanna know";
static const char LYRIC_03[] PROGMEM = u8"사탕처럼";
static const char LYRIC_04[] PROGMEM = u8"달콤하다는데";
static const char LYRIC_05[] PROGMEM = "I wanna know";
static const char LYRIC_06[] PROGMEM = u8"하늘을 나는 것";
static const char LYRIC_07[] PROGMEM = u8"같다는데";
static const char LYRIC_08[] PROGMEM = "I wanna know";
static const char LYRIC_09[] PROGMEM = "know";
static const char LYRIC_10[] PROGMEM = "know";
static const char LYRIC_11[] PROGMEM = "What is";
static const char LYRIC_12[] PROGMEM = "love?";
static const char LYRIC_13[] PROGMEM = u8"사랑이 어떤";
static const char LYRIC_14[] PROGMEM = u8"느낌인지";
static const char LYRIC_15[] PROGMEM = "I wanna know";
static const char LYRIC_16[] PROGMEM = u8"하루 종일 웃고";
static const char LYRIC_17[] PROGMEM = u8"있다는데";
static const char LYRIC_18[] PROGMEM = "I wanna know";
static const char LYRIC_19[] PROGMEM = u8"세상이 다";
static const char LYRIC_20[] PROGMEM = u8"아름답다는데";
static const char LYRIC_21[] PROGMEM = "I wanna know";
static const char LYRIC_22[] PROGMEM = "know";
static const char LYRIC_23[] PROGMEM = "know";
static const char LYRIC_24[] PROGMEM = "What is";
static const char LYRIC_25[] PROGMEM = "love?";
static const char LYRIC_26[] PROGMEM = u8"언젠간 나에게도";
static const char LYRIC_27[] PROGMEM = u8"사랑이 올까";

static const char *const LYRIC_LINES[] PROGMEM = {
    LYRIC_00, LYRIC_01, LYRIC_02, LYRIC_03, LYRIC_04, LYRIC_05,
    LYRIC_06, LYRIC_07, LYRIC_08, LYRIC_09, LYRIC_10, LYRIC_10,
    LYRIC_11, LYRIC_12, LYRIC_13, LYRIC_14, LYRIC_15, LYRIC_16,
    LYRIC_17, LYRIC_18, LYRIC_19, LYRIC_20, LYRIC_21, LYRIC_22,
    LYRIC_23, LYRIC_23, LYRIC_24, LYRIC_25, LYRIC_26, LYRIC_27,
};

struct LyricCue {
  uint8_t startNote;
  uint8_t firstLine;
  uint8_t lineCount;
};

// Each cue starts on the note that sings its text. Tying frames to note
// indices, rather than a separate lyric timer, prevents sound/text drift.
static const LyricCue LYRIC_CUES[] PROGMEM = {
    {0, 0, 1},    // 그게 언제쯤일까?
    {7, 1, 1},    // 어떤 사람일까?
    {12, 2, 1},   // I wanna know
    {17, 3, 2},   // 사탕처럼 달콤하다는데
    {27, 5, 1},   // I wanna know
    {31, 6, 2},   // 하늘을 나는 것 같다는데
    {42, 8, 1},   // I wanna know
    {46, 9, 3},   // know / know / know
    {49, 12, 2},  // What is love?
    {52, 14, 2},  // 사랑이 어떤 느낌인지
    {61, 16, 1},  // I wanna know
    {66, 17, 2},  // 하루 종일 웃고 있다는데
    {76, 19, 1},  // I wanna know
    {80, 20, 2},  // 세상이 다 아름답다는데
    {91, 22, 1},  // I wanna know
    {95, 23, 3},  // know / know / know
    {98, 26, 2},  // What is love?
    {101, 28, 2}, // 언젠간 나에게도 사랑이 올까
};

static uint32_t nextUtf8CodePoint(PGM_P &text) {
  const uint8_t first = pgm_read_byte(text++);
  if (first < 0x80) {
    return first;
  }

  if ((first & 0xE0) == 0xC0) {
    const uint8_t second = pgm_read_byte(text++);
    return ((uint32_t)(first & 0x1F) << 6) | (second & 0x3F);
  }

  if ((first & 0xF0) == 0xE0) {
    const uint8_t second = pgm_read_byte(text++);
    const uint8_t third = pgm_read_byte(text++);
    return ((uint32_t)(first & 0x0F) << 12) |
           ((uint32_t)(second & 0x3F) << 6) | (third & 0x3F);
  }

  return '?';
}

static const HangulGlyph *findHangulGlyph(uint16_t codePoint) {
  uint8_t low = 0;
  uint8_t high = HANGUL_GLYPH_COUNT;

  while (low < high) {
    const uint8_t middle = low + (high - low) / 2;
    const uint16_t candidate =
        pgm_read_word(&HANGUL_GLYPHS[middle].codePoint);
    if (candidate < codePoint) {
      low = middle + 1;
    } else if (candidate > codePoint) {
      high = middle;
    } else {
      return &HANGUL_GLYPHS[middle];
    }
  }

  return nullptr;
}

static const LatinGlyph *findLatinGlyph(uint8_t codePoint) {
  uint8_t low = 0;
  uint8_t high = LATIN_GLYPH_COUNT;

  while (low < high) {
    const uint8_t middle = low + (high - low) / 2;
    const uint8_t candidate =
        pgm_read_byte(&LATIN_GLYPHS[middle].codePoint);
    if (candidate < codePoint) {
      low = middle + 1;
    } else if (candidate > codePoint) {
      high = middle;
    } else {
      return &LATIN_GLYPHS[middle];
    }
  }

  return nullptr;
}

static uint16_t measureUtf8Line(PGM_P text) {
  uint16_t width = 0;
  uint32_t codePoint;

  while ((codePoint = nextUtf8CodePoint(text)) != 0) {
    if (codePoint == ' ') {
      width += KOREAN_SPACE_WIDTH;
    } else if (codePoint < 0x80) {
      const LatinGlyph *glyph =
          findLatinGlyph(static_cast<uint8_t>(codePoint));
      width += glyph == nullptr ? KOREAN_SPACE_WIDTH
                                : pgm_read_byte(&glyph->advance);
    } else {
      width += HANGUL_GLYPH_WIDTH;
    }
  }

  return width;
}

static void drawUtf8Line(PGM_P text, int16_t y) {
  const uint16_t width = measureUtf8Line(text);
  int16_t x = width < SCREEN_WIDTH ? (SCREEN_WIDTH - width) / 2 : 0;
  uint32_t codePoint;

  while ((codePoint = nextUtf8CodePoint(text)) != 0) {
    const LatinGlyph *latinGlyph = nullptr;
    uint8_t advance = HANGUL_GLYPH_WIDTH;
    if (codePoint == ' ') {
      advance = KOREAN_SPACE_WIDTH;
    } else if (codePoint < 0x80) {
      latinGlyph = findLatinGlyph(static_cast<uint8_t>(codePoint));
      advance = latinGlyph == nullptr
                    ? KOREAN_SPACE_WIDTH
                    : pgm_read_byte(&latinGlyph->advance);
    }

    if (x + advance > SCREEN_WIDTH) {
      break;  // Defensive clipping if a future line is accidentally too long.
    }

    if (codePoint == ' ') {
      // The display was cleared before drawing, so only advance the cursor.
    } else if (codePoint < 0x80) {
      if (latinGlyph != nullptr) {
        display.drawBitmap(x, y, latinGlyph->bitmap, LATIN_GLYPH_WIDTH,
                           LATIN_GLYPH_HEIGHT, SSD1306_WHITE);
      }
    } else {
      const HangulGlyph *glyph =
          findHangulGlyph(static_cast<uint16_t>(codePoint));
      if (glyph != nullptr) {
        display.drawBitmap(x, y, glyph->bitmap, HANGUL_GLYPH_WIDTH,
                           HANGUL_GLYPH_HEIGHT, SSD1306_WHITE);
      } else {
        display.drawRect(x + 1, y + 1, HANGUL_GLYPH_WIDTH - 2,
                         HANGUL_GLYPH_HEIGHT - 2, SSD1306_WHITE);
      }
    }
    x += advance;
  }
}

static void showLyric(uint8_t cueIndex) {
  const uint8_t firstLine =
      pgm_read_byte(&LYRIC_CUES[cueIndex].firstLine);
  const uint8_t lineCount =
      pgm_read_byte(&LYRIC_CUES[cueIndex].lineCount);
  const int16_t firstY = (SCREEN_HEIGHT - lineCount * LINE_HEIGHT) / 2;

  display.clearDisplay();
  for (uint8_t line = 0; line < lineCount; ++line) {
    PGM_P text = reinterpret_cast<PGM_P>(
        pgm_read_ptr(&LYRIC_LINES[firstLine + line]));
    drawUtf8Line(text, firstY + line * LINE_HEIGHT);
  }
  display.display();
}

static uint8_t cueStartNote(uint8_t cueIndex) {
  return pgm_read_byte(&LYRIC_CUES[cueIndex].startNote);
}

static void playSong() {
  // At 400 kHz the full OLED refresh takes about 25 ms. Refresh near the end
  // of the preceding silent gap so the page and next phrase begin together,
  // without stretching the melody's original note timing.
  static const uint8_t DISPLAY_REFRESH_BUDGET_MS = 40;
  uint8_t nextCue = 0;

  showLyric(nextCue++);

  for (uint8_t noteIndex = 0; noteIndex < ARRAY_LEN(SONG); ++noteIndex) {
    const uint16_t frequency =
        pgm_read_word(&SONG[noteIndex].frequency);
    const uint16_t soundMs = pgm_read_word(&SONG[noteIndex].soundMs);
    const uint16_t gapMs = pgm_read_word(&SONG[noteIndex].gapMs);

    tone(BUZZER_PIN, frequency);
    delay(soundMs);
    noTone(BUZZER_PIN);

    const uint32_t gapStartedAt = millis();
    const bool lyricChangesNext =
        nextCue < ARRAY_LEN(LYRIC_CUES) &&
        cueStartNote(nextCue) == noteIndex + 1;

    if (lyricChangesNext) {
      if (gapMs > DISPLAY_REFRESH_BUDGET_MS) {
        delay(gapMs - DISPLAY_REFRESH_BUDGET_MS);
      }
      showLyric(nextCue++);
      while (millis() - gapStartedAt < gapMs) {
        delay(1);
      }
    } else {
      delay(gapMs);
    }
  }
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    while (true) {
      delay(1000);
    }
  }

  Wire.setClock(400000UL);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextWrap(false);
  display.clearDisplay();
  display.display();

  playSong();
}

void loop() {
  // The song plays once at power-up; keep the final lyric on screen.
}
