//
// ======================= ВНИМАНИЕ !!! =============================
//  Все настройки делаются на вкладке Constants.h
//  Почитайте там то, что на русском языке написано.
//  Либо ничего не трогайте, если собирали, по схемам из этого архива.
//  В любом случае ВНИМАТЕЛЬНО прочитайте файл ПРОЧТИ МЕНЯ!!!.txt из этого архива.
//
//  решение проблем можно поискать тут под спойлерами:
//  https://community.alexgyver.ru/goto/post?id=73929
// ==================================================================

// Ссылка для менеджера плат:
// https://arduino.esp8266.com/stable/package_esp8266com_index.json
// При установке выбираем версию 2.7.4

/*
  Скетч к проекту "Многофункциональный RGB светильник"
  Страница проекта (схемы, описания): https://alexgyver.ru/GyverLamp/
  Исходники авторской (старой) прошивки на GitHub: https://github.com/AlexGyver/GyverLamp/
  ==========================================================================================
  Исходники новой версии: https://community.alexgyver.ru/goto/post?id=73929
  ==========================================================================================
  Автор идеи и первой реализации: AlexGyver, AlexGyver Technologies, 2019
  https://AlexGyver.ru/
*/

#define FASTLED_USE_PROGMEM 1 // просим библиотеку FASTLED экономить память контроллера на свои палитры

#include "pgmspace.h"
#include "Constants.h"
#include <FastLED.h>
#include <ESP8266WiFi.h>

#include <EEPROM.h>
#include <GyverButton.h>

#include "fonts.h"
//#include <TimeLib.h>
#include "FavoritesManager.h"
#include <ArduinoJson.h>        //Установить из менеджера библиотек версию 5.13.5 !!!. https://arduinojson.org/
#include <ESP8266HTTPUpdateServer.h>    // Обновление с web страницы

#include <LittleFS.h>
#define SPIFFS LittleFS

// --- ИНИЦИАЛИЗАЦИЯ ОБЪЕКТОВ ----------
CRGB leds[NUM_LEDS];

uint8_t selectedSettings = 0U;

GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);  // для сенсорной кнопки LOW_PULL
// для физической (не сенсорной) кнопки HIGH_PULL
// ну и кнопку нужно ставить без резистора в разрыв между пинами D2 и GND
// GButton touch(BTN_PIN, HIGH_PULL, NORM_OPEN);

ESP8266HTTPUpdateServer httpUpdater;  // Объект для обнавления с web страницы
ESP8266WebServer HTTP(ESP_HTTP_PORT); //ESP8266WebServer HTTP;  // Web интерфейс для устройства
File fsUploadFile;  // Для файловой системы



// --- ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ -------
static const uint8_t maxDim = max(WIDTH, HEIGHT);


uint8_t currentMode = 3;

bool loadingFlag = true;

// ПРИЗНАК включенной лампы
// Если в json есть off_after_shutdown=1, то при включении в сеть лампа не светится
// Если в json есть off_after_shutdown=0, то восстанавливается запомненное состояние
bool ONflag = false;

// при старте проверяется нажатие кнопки и если она нажата или замкнута, то кнопка временно отключается
// в работе используется настройка из json - button_on
bool buttonEnabled = true;

uint8_t FavoritesManager::FavoritesRunning = 0;          // флаг "работает режим автоматической смены избранных эффектов"
uint16_t FavoritesManager::Interval = 0;                 // статический интервал (время между сменами эффектов)
uint16_t FavoritesManager::Dispersion = 0;               // дополнительный динамический (случайный) интервал (время между сменами эффектов)
uint8_t FavoritesManager::UseFavoritesAtStart = 0;  // флаг, определяющий, нужно ли использовать сохранённое значение FavoritesRunning при перезапуске; еслин нет, "избранное" будет выключено при старте
uint8_t FavoritesManager::FavoriteModes[MODE_AMOUNT] = {0}; // массив, каждый элемент которого соответствует флагу "эффект №... добавлен в избранные"
uint32_t FavoritesManager::nextModeAt = 0;               // ближайшее время переключения на следующий избранный эффект (millis())
bool FavoritesManager::rndCycle = false;                     // Перемешивать цикл или нет

FavoritesManager fav;

unsigned char matrixValue[8][16]; //это массив для эффекта Огонь

char TextTicker [80];
int Painting = 0;
CRGB DriwingColor = CRGB(255, 255, 255);

uint8_t FPSdelay = DYNAMIC;

uint8_t espMode ;
uint8_t random_on;
uint32_t my_timer;
bool connect = false;
uint8_t ESP_CONN_TIMEOUT;




void setup()  //==================================================================  void setup()  =========================================================================
{
    delay(250);
    Serial.begin(115200);
    LOG.println();
    ESP.wdtDisable();
    ESP.wdtEnable(WDTO_8S);

    LOG.print(F("\nСтарт файловой системы\n"));
    FS_init();  //Запускаем файловую систему
    LOG.print(F("Чтение файла конфигурации\n"));
    configSetup = readFile("config.json", 1024);
    LOG.println(configSetup);
    //Настраиваем и запускаем HTTP интерфейс
    LOG.print(F("Старт WebServer\n"));
    HTTP_init();


    //-----------Инициализируем переменные, хранящиеся в файле config.json--------------
    AP_NAME = jsonRead(configSetup, "ssidAP");
    AP_PASS = jsonRead(configSetup, "passwordAP");
    random_on = jsonReadToInt(configSetup, "random_on");
    espMode = jsonReadToInt(configSetup, "ESP_mode");
    buttonEnabled = jsonReadToInt(configSetup, "button_on");
    ESP_CONN_TIMEOUT = jsonReadToInt(configSetup, "TimeOut");
    (jsonRead(configSetup, "run_text")).toCharArray(TextTicker, (jsonRead(configSetup, "run_text")).length() + 1);

    // КНОПКА
    touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
    touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
    touch.setDebounce(BUTTON_SET_DEBOUNCE);
    delay(350);                                            // ожидание инициализации модуля кнопки ttp223 (по спецификации 250мс)
    // если после включения и задержки кнопка нажата, то отключить ее
    if (digitalRead(BTN_PIN))
    {
        buttonEnabled = false;
    }
    ESP.wdtFeed();

    // ЛЕНТА/МАТРИЦА
    FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)/*.setCorrection(TypicalLEDStrip)*/;
    //FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(0xB0FFE0); // Калибровка баланса белого цвета. Последовательность байт RGB (B0-R FF-G E0-B)
    FastLED.setBrightness(BRIGHTNESS);
    if (CURRENT_LIMIT > 0)
    {
        FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    }
    FastLED.clear();
    FastLED.show();

    // тест подключения и порядка цветов
    leds[0] = CRGB::Red;
    leds[1] = CRGB::Red;
    leds[2] = CRGB::Green;
    leds[3] = CRGB::Green;
    leds[4] = CRGB::Blue;
    leds[5] = CRGB::Blue;
    FastLED.show();
    delay(350);

    FastLED.clear();
    FastLED.show();


#ifdef USE_SHUFFLE_FAVORITES // первоначальная очередь избранного до перемешивания
    for (uint8_t i = 0; i < MODE_AMOUNT; i++)
    {
        shuffleFavoriteModes[i] = i;
    }
#endif

    currentMode = jsonReadToInt(configSetup, "eff_sel");

    // если нет файла effect.ini, то восстановить из defaultSettings
    // и записать в файл
    // Иначе читать из файла и использовать
    if (SPIFFS.exists("/effect.ini"))
    {
        LOG.println("Loading effects settings from effect.ini");
        eff_read();
    }
    else
    {
        LOG.println("No file effect.ini - restoring default settings");
        restoreSettings();
        eff_save();
    }

    // флаг из файла настроек, которые обновляются по кнопке
    ONflag = jsonReadToInt(configSetup, "Power") == 0 ? 0 : 1;

    // но если стоит галочка "не включать после обесточивания", то выключаемся
    if (jsonReadToInt(configSetup, "off_after_shutdown") == 1)
    {
        ONflag = false;
    }

    jsonWrite(configSetup, "Power", ONflag);

    //    jsonWrite(configSetup, "eff_sel", currentMode);

    modes[currentMode].Brightness = jsonReadToInt(configSetup, "br");
    modes[currentMode].Speed = jsonReadToInt(configSetup, "sp");
    modes[currentMode].Scale = jsonReadToInt(configSetup, "sc");

    jsonWrite(configSetup, "br", modes[currentMode].Brightness);
    jsonWrite(configSetup, "sp", modes[currentMode].Speed);
    jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    jsonWrite(configSetup, "button_on", buttonEnabled);
    //    cycle_get();  // чтение выбранных эффектов

    FavoritesManager::ReadFavoritesSettings();

    // WI-FI

    LOG.printf_P(PSTR("\nРабочий режим лампы: ESP_MODE = %d\n"), espMode);
    //Запускаем WIFI
    LOG.println(F("Старуем WIFI"));

    WiFi.persistent(false);   // Побережём EEPROM

    if (espMode == 0U)                                        // режим WiFi точки доступа
    {
        // Отключаем WIFI
        WiFi.disconnect();
        // Меняем режим на режим точки доступа
        WiFi.mode(WIFI_AP);
        // Задаем настройки сети
        if (sizeof(AP_STATIC_IP))
        {
            WiFi.softAPConfig(
                IPAddress(AP_STATIC_IP[0], AP_STATIC_IP[1], AP_STATIC_IP[2], AP_STATIC_IP[3]),      // IP адрес WiFi точки доступа
                IPAddress(AP_STATIC_IP[0], AP_STATIC_IP[1], AP_STATIC_IP[2], 1),                    // первый доступный IP адрес сети
                IPAddress(255, 255, 255, 0));                                                       // маска подсети
        }
        // Включаем WIFI в режиме точки доступа с именем и паролем
        // хранящихся в переменных _ssidAP _passwordAP в фвйле config.json
        WiFi.softAP(AP_NAME, AP_PASS);
        LOG.print(F("Старт WiFi в режиме точки доступа\n"));
        LOG.print(F("IP адрес: "));
        LOG.println(WiFi.softAPIP());
        connect = true;
        delay(100);
    }
    else                                                      // режим WiFi клиента. Подключаемся к роутеру
    {
        LOG.println(F("Старт WiFi в режиме клиента (подключение к роутеру)"));
        //  WIFI_start_station_mode ();


        WiFi.persistent(false);

        // Попытка подключения к Роутеру
        WiFi.mode(WIFI_STA);
        String _ssid = jsonRead(configSetup, "ssid");
        String _password = jsonRead(configSetup, "password");
        if (_ssid == "" && _password == "")
        {
            espMode = 0;
            jsonWrite(configSetup, "ESP_mode", (int)espMode);
            saveConfig();
            ESP.restart();
        }
        else
        {
            WiFi.begin(_ssid.c_str(), _password.c_str());
        }

        delay(100);
    }     //if (espMode == 0U) {...} else {...

    ESP.wdtFeed();

    // ОСТАЛЬНОЕ
    memset(matrixValue, 0, sizeof(matrixValue)); //это массив для эффекта Огонь. странно, что его нужно залить нулями
    randomSeed(micros());
    changePower();
    loadingFlag = true;

    //TextTicker = RUNNING_TEXT_DEFAULT;
    delay(100);

    my_timer = millis();

    LOG.printf("ONflag=%d\n", ONflag);
}


void loop()  //====================================================================  void loop()  ===========================================================================
{

    if (espMode)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            if ((millis() - my_timer) >= 1000UL)
            {
                my_timer = millis();
                if (ESP_CONN_TIMEOUT--)
                {
                    LOG.print(".");
                    ESP.wdtFeed();
                }
                else
                {
                    // Если не удалось подключиться запускаем в режиме AP
                    espMode = 0;
                    jsonWrite(configSetup, "ESP_mode", (int)espMode);
                    saveConfig();
                    ESP.restart();
                }
            }
        }
        else
        {
            // Иначе удалось подключиться отправляем сообщение
            // о подключении и выводим адрес IP
            LOG.print(F("\nПодключение к роутеру установлено\n"));
            LOG.print(F("IP адрес: "));
            LOG.println(WiFi.localIP());
            long rssi = WiFi.RSSI();
            LOG.print(F("Уровень сигнала сети RSSI = "));
            LOG.print(rssi);
            LOG.println(F(" dbm"));
            connect = true;
            delay(100);
        }
    }

    if (connect || !espMode)
    {
        my_timer = millis();
    }

    do      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++========= Главный цикл ==========+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    {
        // Если не устойчивое подключение к WiFi, или не создаётся точка доступа, или лампа не хочет подключаться к вашей сети или вы не можете подключиться к точке доступа, то может быть у вас не качественная плата.
        delay(12);    //Для некоторых плат ( особенно без металлического экрана над ESP и Flash памятью ) эта задержка должна быть увеличена. Подбирается индивидуально в пределах 1-12 мс до устойчивой работы WiFi. Чем меньше, тем лучше. Качественные платы работают с задержкой 0.

        if (connect || !espMode)
        {
            HTTP.handleClient();
        }

        yield();

        if (Painting == 0)
        {
            effectsTick();
            buttonTick();
            yield();

            if (FavoritesManager::HandleFavorites(                    // обработка режима избранных эффектов
                        &ONflag,
                        &currentMode,
                        &loadingFlag,
                        &random_on,
                        &selectedSettings))
            {
                FastLED.setBrightness(modes[currentMode].Brightness);
            }

        }
        ESP.wdtFeed();
    }
    while (connect);

}
