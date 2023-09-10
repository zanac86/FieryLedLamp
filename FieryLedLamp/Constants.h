// Текущая версия / Current version : v2.31_web_v2.2  ( gunner47_v2.2_web_v2.1_94in1 )
// ВНИМАНИЕ!!! Большая часть настроек перенесена в файл data/config и может изменяться в процессе эксплуатации лампы.
// Внимательно читайте файл ПРОЧТИ МЕНЯ!!!.txt и ПРОЧТИ МЕНЯ.doc (тут с картинками)

#pragma once

#include <ESP8266WebServer.h>

// ============= НАСТРОЙКИ =============
#define USE_SHUFFLE_FAVORITES                             // раскомментируйте эту строку, если вам нужно, чтобы режим Цикл показал каждый эффект по 1 разу перед перемешиванием (иначе просто случайный эффект),
// а также если у вас выбрано меньше десятка эффектов. кстати, если выбрано менее 2 эффектов, то демонстрироваться будут все эффекты по порядку без перемешивания

// --- КНОПКА --------------------------
#define BUTTON_IS_SENSORY     (1)                           // если у вас не сенсорная, а обычная кнопка, поменяйте в этой строчке 1 на нолик
#define BUTTON_STEP_TIMEOUT   (100U)                        // каждые BUTTON_STEP_TIMEOUT мс будет генерироваться событие удержания кнопки (для регулировки яркости)
#define BUTTON_CLICK_TIMEOUT  (500U)                        // максимальное время между нажатиями кнопки в мс, до достижения которого считается серия последовательных нажатий
#define BUTTON_SET_DEBOUNCE   (25U)                       // Время антидребезга mS для сенсорной кнопки


// --- ESP -----------------------------

// мои лампы
#define LED_PIN               (4U)                          // пин ленты                (D4) - встроенный светодиод платы тоже на D4, поэтому лучше паять на D3 = #define LED_PIN (0U)
//#define LED_PIN               (2U)                          // пин ленты                (D4) - встроенный светодиод платы тоже на D4, поэтому лучше паять на D3 = #define LED_PIN (0U)
#define BTN_PIN               (5U)                          // пин кнопки               (D2)

// коробка для отладки
//#define LED_PIN               (2U)                          // пин ленты                (D4) - встроенный светодиод платы тоже на D4, поэтому лучше паять на D3 = #define LED_PIN (0U)
//#define BTN_PIN               (4U)                          // пин кнопки               (D2)

//#define DISPLAY_IP_AT_START                               // Закоментируйте эту строчку,если нехотите, чтобы при включении пмтания и подключению к WiFi, лампа один раз выводила свой IP адрес

// --- AP (WiFi точка доступа) ---
String AP_NAME = "";                                        // Переменная для имени точки доступа. Задаётся на web странице
String AP_PASS = "";                                        // Переменная для пароля точки доступа. Задаётся на web странице
const uint8_t AP_STATIC_IP[] = {192, 168, 4, 1};            // статический IP точки доступа (лучше не менять!)

// --- МАТРИЦА -------------------------
#define CURRENT_LIMIT         (1900U)                       // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define WIDTH                 (16U)                         // ширина матрицы
#define HEIGHT                (16U)                         // высота матрицы

#define COLOR_ORDER           (GRB)                         // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

#define MATRIX_TYPE           (0U)                          // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE      (0U)                          // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION       (0U)                          // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
// при неправильной настройке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
// шпаргалка по настройке матрицы здесь: https://alexgyver.ru/wp-content/uploads/2018/11/scheme3.jpg
// (но в ней, кажется, перепутаны местами квадратики в 4м столбце, а может, и ещё какие-то)

// --- ЭФФЕКТЫ -------------------------

// текст, который будет выводиться в эффекте Бегущая строка по умолчанию
#define RUNNING_TEXT_DEFAULT  ("Have a nice day!")

// == названия и номера эффектов ниже в списке используются на вкладке effectTricker ==
// == если меняете, меняйте и там, и ещё здесь ниже в РЕЕСТРЕ ДОСТУПНЫХ ЭФФЕКТОВ ==

#define EFF_WHITE_COLOR         (  0U)    // Бeлый cвeт
#define EFF_COLOR               (  1U)    // Цвeт
#define EFF_COLORS              (  2U)    // Cмeнa цвeтa
#define EFF_MADNESS             (  3U)    // Бeзyмиe
#define EFF_CLOUDS              (  4U)    // Oблaкa
#define EFF_LAVA                (  5U)    // Лaвa
#define EFF_PLASMA              (  6U)    // Плaзмa
#define EFF_RAINBOW             (  7U)    // Paдyгa 3D
#define EFF_RAINBOW_STRIPE      (  8U)    // Пaвлин
#define EFF_ZEBRA               (  9U)    // 3eбpa
#define EFF_FOREST              ( 10U)    // Лec
#define EFF_OCEAN               ( 11U)    // Oкeaн
#define EFF_BBALLS              ( 12U)    // Mячики
#define EFF_BALLS_BOUNCE        ( 13U)    // Mячики бeз гpaниц
#define EFF_POPCORN             ( 14U)    // Пoпкopн
#define EFF_SPIRO               ( 15U)    // Cпиpaли
#define EFF_PRISMATA            ( 16U)    // Пpизмaтa
#define EFF_SMOKEBALLS          ( 17U)    // Дымoвыe шaшки
#define EFF_FLAME               ( 18U)    // Плaмя
#define EFF_FIRE_2021           ( 19U)    // Oгoнь 2021
#define EFF_PACIFIC             ( 20U)    // Tиxий oкeaн
#define EFF_SHADOWS             ( 21U)    // Teни
#define EFF_DNA                 ( 22U)    // ДHK
#define EFF_FLOCK               ( 23U)    // Cтaя
#define EFF_FLOCK_N_PR          ( 24U)    // Cтaя и xищник
#define EFF_BUTTERFLYS          ( 25U)    // Moтыльки
#define EFF_BUTTERFLYS_LAMP     ( 26U)    // Лaмпa c мoтылькaми
#define EFF_SNAKES              ( 27U)    // 3мeйки
#define EFF_NEXUS               ( 28U)    // Nexus
#define EFF_SPHERES             ( 29U)    // Шapы
#define EFF_SINUSOID3           ( 30U)    // Cинycoид
#define EFF_METABALLS           ( 31U)    // Meтaбoлз
#define EFF_AURORA              ( 32U)    // Ceвepнoe cияниe
#define EFF_SPIDER              ( 33U)    // Плaзмeннaя лaмпa
#define EFF_LAVALAMP            ( 34U)    // Лaвoвaя лaмпa
#define EFF_LIQUIDLAMP          ( 35U)    // Жидкaя лaмпa
#define EFF_LIQUIDLAMP_AUTO     ( 36U)    // Жидкaя лaмпa (auto)
#define EFF_DROPS               ( 37U)    // Kaпли нa cтeклe
#define EFF_MATRIX              ( 38U)    // Maтpицa
#define EFF_FIRE_2012           ( 39U)    // Oгoнь 2012
#define EFF_FIRE_2018           ( 40U)    // Oгoнь 2018
#define EFF_FIRE_2020           ( 41U)    // Oгoнь 2020
#define EFF_FIRE                ( 42U)    // Oгoнь
#define EFF_WHIRL               ( 43U)    // Bиxpи плaмeни
#define EFF_WHIRL_MULTI         ( 44U)    // Paзнoцвeтныe виxpи
#define EFF_MAGMA               ( 45U)    // Maгмa
#define EFF_LLAND               ( 46U)    // Kипeниe
#define EFF_WATERFALL           ( 47U)    // Boдoпaд
#define EFF_WATERFALL_4IN1      ( 48U)    // Boдoпaд 4 в 1
#define EFF_POOL                ( 49U)    // Бacceйн
#define EFF_PULSE               ( 50U)    // Пyльc
#define EFF_PULSE_RAINBOW       ( 51U)    // Paдyжный пyльc
#define EFF_PULSE_WHITE         ( 52U)    // Бeлый пyльc
#define EFF_OSCILLATING         ( 53U)    // Ocциллятop
#define EFF_FOUNTAIN            ( 54U)    // Иcтoчник
#define EFF_FAIRY               ( 55U)    // Фeя
#define EFF_COMET               ( 56U)    // Koмeтa
#define EFF_COMET_COLOR         ( 57U)    // Oднoцвeтнaя кoмeтa
#define EFF_COMET_TWO           ( 58U)    // Двe кoмeты
#define EFF_COMET_THREE         ( 59U)    // Тpи кoмeты
#define EFF_LUMENJER            ( 60U)    // Люмeньep
#define EFF_ATTRACT             ( 61U)    // Пpитяжeниe
#define EFF_FIREFLY             ( 62U)    // Пapящий oгoнь
#define EFF_FIREFLY_TOP         ( 63U)    // Bepxoвoй oгoнь
#define EFF_SNAKE               ( 64U)    // Paдyжный змeй
#define EFF_SPARKLES            ( 65U)    // Koнфeтти
#define EFF_TWINKLES            ( 66U)    // Mepцaниe
#define EFF_SMOKE               ( 67U)    // Дым
#define EFF_SMOKE_COLOR         ( 68U)    // Paзнoцвeтный дым
#define EFF_PICASSO             ( 69U)    // Пикacco
#define EFF_WAVES               ( 70U)    // Boлны
#define EFF_SAND                ( 71U)    // Цвeтныe дpaжe
#define EFF_RINGS               ( 72U)    // Koдoвый зaмoк
#define EFF_CUBE2D              ( 73U)    // Kyбик Pyбикa
#define EFF_SIMPLE_RAIN         ( 74U)    // Tyчкa в бaнкe
#define EFF_STORMY_RAIN         ( 75U)    // Гроза в банке
#define EFF_COLOR_RAIN          ( 76U)    // Ocaдки
#define EFF_RAIN                ( 77U)    // Paзнoцвeтный дoждь
#define EFF_SNOW                ( 78U)    // Cнeгoпaд
#define EFF_STARFALL            ( 79U)    // 3вeздoпaд / Meтeль
#define EFF_LEAPERS             ( 80U)    // Пpыгyны
#define EFF_LIGHTERS            ( 81U)    // Cвeтлячки
#define EFF_LIGHTER_TRACES      ( 82U)    // Cвeтлячки co шлeйфoм
#define EFF_PAINTBALL           ( 83U)    // Пeйнтбoл
#define EFF_RAINBOW_VER         ( 84U)    // Paдyгa
#define EFF_RIVERS              ( 85U)    // Реки Ботсваны
#define EFF_SWIRL               ( 86U)    // Завиток
#define EFF_WINE                ( 87U)    // Вино
#define EFF_PAINTS              ( 88U)    // Масляные Краски
#define EFF_WATERCOLOR          ( 89U)    // Акварель
#define EFF_FEATHER_CANDLE      ( 90U)    // Свеча
#define EFF_HOURGLASS           ( 91U)    // Песочные Часы
#define EFF_SPECTRUM            ( 92U)    // Спектрум
#define EFF_LOTUS               ( 93U)    // Цветок Лотоса
#define EFF_CHRISTMAS_TREE      ( 94U)    // Новогодняя Елка
#define EFF_BY_EFFECT           ( 95U)    // Побочный Эффект
#define EFF_STROBE              ( 96U)    // Строб.Хаос.Дифузия
#define EFF_COLOR_FRIZZLES      ( 97U)    // Цветные кудри
#define EFF_FIREWORK            ( 98U)    // Фейерверк
#define EFF_WEB_TOOLS           ( 99U)    // Мечта дизайнера
#define EFF_CLOCK               (100U)    // Чacы
#define EFF_TEXT                (101U)    // Бeгyщaя cтpoкa

#define MODE_AMOUNT           (102U)          // количество режимов

struct ModeType
{
    uint8_t Brightness = 50U;
    uint8_t Speed = 225U;
    uint8_t Scale = 40U;
};


// ============= МАССИВ НАСТРОЕК ЭФФЕКТОВ ПО УМОЛЧАНИЮ ===================
// формат записи:
//{ Яркость, Скорость, Масштаб}
static const uint8_t defaultSettings[][3] PROGMEM =
{
    {   9, 207,  26}, // Бeлый cвeт
    {   9, 180,  99}, // Цвeт
    {  10, 252,  32}, // Cмeнa цвeтa
    {  11,  33,  58}, // Бeзyмиe
    {   8,   4,  34}, // Oблaкa
    {   8,   9,  24}, // Лaвa
    {  11,  19,  59}, // Плaзмa
    {  11,  13,  60}, // Paдyгa 3D
    {  11,   5,  12}, // Пaвлин
    {   7,   8,  21}, // 3eбpa
    {   7,   8,  95}, // Лec
    {   7,   6,  12}, // Oкeaн
    {  24, 255,  26}, // Mячики
    {  18,  11,  70}, // Mячики бeз гpaниц
    {  19,  32,  16}, // Пoпкopн
    {   9,  46,   3}, // Cпиpaли
    {  17, 100,   2}, // Пpизмaтa
    {  12,  44,  17}, // Дымoвыe шaшки
    {  30,  53,   3}, // Плaмя
    {  15, 150,  22}, // Oгoнь 2021
    {  55, 127, 100}, // Tиxий oкeaн
    {  39,  77,   1}, // Teни
    {  15,  77,  95}, // ДHK
    {  15, 136,   4}, // Cтaя
    {  15, 128,  80}, // Cтaя и xищник
    {  11,  53,  87}, // Moтыльки
    {   7,  61, 100}, // Лaмпa c мoтылькaми
    {   9,  96,  31}, // 3мeйки
    {  19,  60,  20}, // Nexus
    {   9,  85,  85}, // Шapы
    {   7, 175,  30}, // Cинycoид
    {   7,  85,   3}, // Meтaбoлз
    {  12,  73,  38}, // Ceвepнoe cияниe
    {   8,  59,  18}, // Плaзмeннaя лaмпa
    {  23, 203,   1}, // Лaвoвaя лaмпa
    {  11,  63,   1}, // Жидкaя лaмпa
    {  11, 124,  39}, // Жидкaя лaмпa (auto)
    {  23,  71,  59}, // Kaпли нa cтeклe
    {  27, 186,  23}, // Maтpицa
    {  10, 220,  63}, // Oгoнь 2012
    {  57, 220,  15}, // Oгoнь 2018
    {  40, 225,  11}, // Oгoнь 2020
    {  22, 230,   1}, // Oгoнь
    {   9, 240,   1}, // Bиxpи плaмeни
    {   9, 240,  86}, // Paзнoцвeтныe виxpи
    {   9, 198,  20}, // Maгмa
    {   7, 240,  18}, // Kипeниe
    {   5, 212,  54}, // Boдoпaд
    {   7, 197,  22}, // Boдoпaд 4 в 1
    {   8, 222,  63}, // Бacceйн
    {  12, 185,   6}, // Пyльc
    {  11, 185,  31}, // Paдyжный пyльc
    {   9, 179,  11}, // Бeлый пyльc
    {   8, 208, 100}, // Ocциллятop
    {  15, 233,  77}, // Иcтoчник
    {  19, 212,  44}, // Фeя
    {  16, 220,  28}, // Koмeтa
    {  14, 212,  69}, // Oднoцвeтнaя кoмeтa
    {  27, 186,  19}, // Двe кoмeты
    {  24, 186,   9}, // Тpи кoмeты
    {  14, 235,  40}, // Люмeньep
    {  21, 203,  65}, // Пpитяжeниe
    {  26, 206,  15}, // Пapящий oгoнь
    {  26, 190,  15}, // Bepxoвoй oгoнь
    {  12, 178, 100}, // Paдyжный змeй
    {  16, 142,  63}, // Koнфeтти
    {  25, 236,   4}, // Mepцaниe
    {   9, 157, 100}, // Дым
    {   9, 157,  30}, // Paзнoцвeтный дым
    {   9, 212,  27}, // Пикacco
    {   9, 236,  80}, // Boлны
    {   9, 195,  80}, // Цвeтныe дpaжe
    {  10, 220,  91}, // Koдoвый зaмoк
    {  10, 222,  92}, // Kyбик Pyбикa
    {  30, 233,   2}, // Tyчкa в бaнкe
    {  20, 236,  25}, // Гроза в банке
    {  15, 198,  99}, // Ocaдки
    {  15, 225,   1}, // Paзнoцвeтный дoждь
    {   9, 180,  90}, // Cнeгoпaд
    {  20, 199,  54}, // 3вeздoпaд / Meтeль
    {  24, 203,   5}, // Пpыгyны
    {  15, 157,  23}, // Cвeтлячки
    {  21, 198,  93}, // Cвeтлячки co шлeйфoм
    {  11, 236,   7}, // Пeйнтбoл
    {  10, 196,  56}, // Paдyгa
    {  15,  50,  50}, // Реки Ботсваны
    {  30, 230,  50}, // Завиток
    {  50, 230,  63}, // Вино
    {  25, 195,  50}, // Масляные Краски
    {  25, 240,  65}, // Акварель
    {  50, 220,   5}, // Свеча
    {  55, 150,   1}, // Песочные Часы
    {  11, 255,   1}, // Спектрум
    {  55, 150,  70}, // Цветок Лотоса
    {  50,  90,  50}, // Новогодняя Елка
    {  45, 150,  30}, // Побочный Эффект
    {  25,  70,  51}, // Строб.Хаос.Дифузия
    {  30, 128,  25}, // Цветные кудри
    {  50,  50,   0}, // Фейерверк
    {  22, 128,  50}, // Мечта дизайнера
    {  20,   5, 100}, // Чacы
    {  15,  50,  38} // Бeгyщaя cтpoкa
}; //             ^-- проверьте, чтобы у предыдущей строки не было запятой после скобки

//#define ARRAY_ROWS(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))            // здесь была проверка, чтобы из-за неверного количества строк лампа не вырубалась.
//const uint8_t defaultSettingsCOUNT = ARRAY_ROWS(defaultSettings);       // но она лишь место занимала. ошибаться нельзя!

// ============= КОНЕЦ МАССИВА =====



// === ОСТАЛЬНОЕ ДЛЯ РАЗРАБОТЧИКОВ =====
//#define USE_OLD_IOS_APP                                   // раскомментируйте эту строку, если вы используете приложение для iOS "Arduino Lamp" (удалено из Апстора, исходники на форуме)

// --- МАТРИЦА -------------------------

// если у вас не обычная матрица, а сборка из ленты или гирлянда, и возникают непонятной природы артефакты в рисунке эффекта, попробуйте менять следующие три строчки
//#define FASTLED_ALLOW_INTERRUPTS      (1)                 // default: 1; // Use this to force FastLED to allow interrupts in the clockless chipsets (or to force it to disallow), overriding the default on platforms that support this. Set the value to 1 to allow interrupts or 0 to disallow them.
#define FASTLED_INTERRUPT_RETRY_COUNT   (0)                 // default: 2; // Use this to determine how many times FastLED will attempt to re-transmit a frame if interrupted for too long by interrupts
#define FASTLED_ESP8266_RAW_PIN_ORDER                       // FASTLED_ESP8266_RAW_PIN_ORDER, FASTLED_ESP8266_D1_PIN_ORDER or FASTLED_ESP8266_NODEMCU_PIN_ORDER


#define NUM_LEDS              (uint16_t)(WIDTH * HEIGHT)
#define SEGMENTS              (1U)                          // диодов в одном "пикселе" (для создания матрицы из кусков ленты). вряд ли эффекты будут корректно работать, если изменить этот параметр

#define DYNAMIC                (0U)   // динамическая задержка для кадров ( будет использоваться бегунок Скорость )
#define SOFT_DELAY             (1U)   // задержка для смены кадров FPSdelay задается програмно прямо в теле эффекта
#define LOW_DELAY             (15U)   // низкая фиксированная задержка для смены кадров
#define HIGH_DELAY            (50U)   // высокая фиксированная задержка для смены кадров
#define DYNAMIC_DELAY_TICK    if (millis() - effTimer >= (256U - modes[currentMode].Speed))
#define HIGH_DELAY_TICK       if (millis() - effTimer >= 50)
#define LOW_DELAY_TICK        if (millis() - effTimer >= 15)
#define SOFT_DELAY_TICK       if (millis() - effTimer >= FPSdelay)

#define BRIGHTNESS            (40U)                         // стандартная маскимальная яркость (0-255). используется только в момент включения питания лампы

#define ESP_HTTP_PORT         (80U)                         // номер порта, который будет использоваться во время первой утановки имени WiFi сети (и пароля), к которой потом будет подключаться лампа в режиме WiFi клиента (лучше не менять)

#define LOG                   Serial

//+++++Функции для работы с json файлами+++++++++++++++++++++++++++

// Установить из менеджера библиотек версию 5.13.5 !!!. https://arduinojson.org/
// в новой библиотеке другие классы и принцип работы
#include <ArduinoJson.h>

#include <LittleFS.h>
#define SPIFFS LittleFS

String configSetup = "{}";
// ------------- Чтение значения json String
String jsonRead(String& json, String name)
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    String tmp = root[name].as<String>();
    return tmp;
}

// ------------- Чтение значения json int
int jsonReadToInt(String& json, String name)
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    return root[name];
}

// ------------- Запись значения json String
void jsonWrite(String& json, String name, String volume)
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    root[name] = volume;
    json = "";
    root.printTo(json);
}

// ------------- Запись значения json int
void jsonWrite(String& json, String name, int volume)
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    root[name] = volume;
    json = "";
    root.printTo(json);
}

// ------------- Запись строки в файл
void writeFile(String fileName, String strings)
{
    LOG.print("writeFile:");
    LOG.println(fileName);
    File configFile = SPIFFS.open("/" + fileName, "w");
    if (!configFile)
    {
        return;
    }
    configFile.print(strings);
    //strings.printTo(configFile);
    configFile.close();
}
void saveConfig()
{
    writeFile("config.json", configSetup);
}

// ------------- Чтение файла в строку
String readFile(String fileName, size_t len)
{
    LOG.print("readFile:");
    LOG.println(fileName);
    File configFile = SPIFFS.open("/" + fileName, "r");
    if (!configFile)
    {
        return "Failed";
    }
    size_t size = configFile.size();
    if (size > len)
    {
        configFile.close();
        return "Large";
    }
    String temp = configFile.readString();
    configFile.close();
    return temp;
}
