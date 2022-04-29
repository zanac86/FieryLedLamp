#pragma once
#include <EEPROM.h>
#include "Constants.h"

#define DEFAULT_FAVORITES_INTERVAL           (300U)         // значение по умолчанию для интервала переключения избранных эффектов в секундах
#define DEFAULT_FAVORITES_DISPERSION         (0U)           // значение по умолчанию для разброса интервала переключения избранных эффектов в секундах

uint8_t shuffleFavoriteModes[MODE_AMOUNT];
uint8_t shuffleCurrentIndex = MODE_AMOUNT; // начальное значение увеличивается на 1 и затем сравнивается с MODE_AMOUNT, чтобы создать первоначальное перемешивание режимов

ModeType modes[MODE_AMOUNT];

class FavoritesManager
{
    public:
        static uint8_t FavoritesRunning;            // флаг "работает режим автоматической смены избранных эффектов"
        static uint16_t Interval;                   // статический интервал (время между сменами эффектов)
        static uint16_t Dispersion;                 // дополнительный динамический (случайный) интервал (время между сменами эффектов)
        static uint8_t UseFavoritesAtStart;    // флаг, определяющий, нужно ли использовать сохранённое значение FavoritesRunning при перезапуске; еслин нет, "избранное" будет выключено при старте
        static uint8_t FavoriteModes[MODE_AMOUNT];  // массив, каждый элемент которого соответствует флагу "эффект №... добавлен в избранные"
        static uint32_t nextModeAt;                 // ближайшее время переключения на следующий избранный эффект (millis())
        static bool rndCycle;                       // Перемешивать цикл или нет

        static bool HandleFavorites(                            // функция, обрабатывающая циклическое переключение избранных эффектов; возвращает true, если эффект был переключен
            bool* ONflag,
            uint8_t* currentMode,
            bool* loadingFlag,
            uint8_t* random_on,
            uint8_t* selectedSettings
        )
        {
            if (FavoritesRunning == 0 ||
                    !*ONflag                                          // лампа не переключается на следующий эффект при выключенной матрице
                    || *currentMode == EFF_WHITE_COLOR && FavoriteModes[EFF_WHITE_COLOR] == 0U // лампа не переключается на следующий эффект, если выбран режим Белый свет, и он не в списке режима Цикл
               )
            {
                return false;
            }

            // лампа не переключается на следующий эффект сразу после включения режима избранных эффектов
            if (nextModeAt == 0)
            {
                nextModeAt = getNextTime();
                return false;
            }

            // пришло время сменить эффект
            if (millis() >= nextModeAt)
            {
                // новый эффект с учетом списка, который может быть перемешан
                *currentMode = getNextFavoriteMode(currentMode);
                // установить его как текущий и запомнить его настройки
                jsonWrite(configSetup, "eff_sel", *currentMode);
                jsonWrite(configSetup, "br", modes[*currentMode].Brightness);
                jsonWrite(configSetup, "sp", modes[*currentMode].Speed);
                jsonWrite(configSetup, "sc", modes[*currentMode].Scale);
                // Признак перезапуска эффекта. Если нужно, то применятся случайные параметры
                *loadingFlag = true;
                if (*random_on)
                {
                    *selectedSettings = 1U;
                }
                // следующая смена эффекта
                nextModeAt = getNextTime();
                return true;
            }

            return false;
        }

        static void ReadFavoritesSettings()
        {
            nextModeAt = 0;
            Interval = jsonReadtoInt(configSetup, "time_eff");
            Dispersion = jsonReadtoInt(configSetup, "disp");
            rndCycle = jsonReadtoInt(configSetup, "rnd_cycle");
            UseFavoritesAtStart = jsonReadtoInt(configSetup, "favorites_at_start");
            FavoritesRunning = jsonReadtoInt(configSetup, "cycle_on");

            String configCycle = readFile("cycle_config.json", 1024);
            char i[3];
            for (uint8_t k = 0; k < MODE_AMOUNT; k++)
            {
                itoa((k), i, 10);
                String e = "e" + String(i) ;
                FavoriteModes[k] = jsonReadtoInt(configCycle, e);
                FavoriteModes[k] = FavoriteModes[k] > 0 ? 1 : 0;
            }
            LOG.println("ReadFavoritesSettings");
        }

        static void SaveFavoritesSettings()
        {
            jsonWrite(configSetup, "time_eff", Interval);
            jsonWrite(configSetup, "disp", Dispersion);
            jsonWrite(configSetup, "favorites_at_start", UseFavoritesAtStart);
            jsonWrite(configSetup, "cycle_on", FavoritesRunning);
            saveConfig();


            char i[3];
            String configCycle = readFile("cycle_config.json", 1024);
            for (uint8_t k = 0; k < MODE_AMOUNT; k++)
            {
                itoa((k), i, 10);
                String e = "e" + String(i) ;
                //сохранение параметров в строку
                jsonWrite(configCycle, e, FavoriteModes[k]);
            }
            writeFile("cycle_config.json", configCycle);
            LOG.println("SaveFavoritesSettings");
        }

        static void TurnFavoritesOff()
        {
            FavoritesRunning = 0;
            nextModeAt = 0;
        }

        static uint8_t getNextFavoriteMode(uint8_t* currentMode)      // возвращает следующий (случайный) включенный в избранные эффект
        {
            uint8_t result;
            uint8_t count = MODE_AMOUNT;// считаем в этом счётчике, есть ли вообще в наличии избранные режимы. хотя бы два
            do
            {
                shuffleCurrentIndex++;
                if (shuffleCurrentIndex >= MODE_AMOUNT)         // если достигнут предел количества режимов
                {
                    count = MODE_AMOUNT;// считаем в этом счётчике, есть ли вообще в наличии избранные режимы, кроме одного
                    if (rndCycle)
                    {
                        for (uint8_t i = 0; i < MODE_AMOUNT; i++)     // перемешиваем режимы
                        {
                            uint8_t j = random8(MODE_AMOUNT);
                            result = shuffleFavoriteModes[i];
                            shuffleFavoriteModes[i] = shuffleFavoriteModes[j];
                            shuffleFavoriteModes[j] = result;
                            if (FavoriteModes[i] == 0) // заодно считаем, вдруг нет избранных режимов, кроме одного
                            {
                                count--;
                            }
                        }
                    }
                    else
                    {
                        for (uint8_t i = 0; i < MODE_AMOUNT; i++)         //расставляем очередь по порядку, начиная от текущего эффекта
                        {
                            shuffleFavoriteModes[i] = (*currentMode + i + 1U) % MODE_AMOUNT;
                            if (FavoriteModes[i] == 0) // заодно считаем, вдруг нет избранных режимов, кроме одного
                            {
                                count--;
                            }
                        }
                    }
                    shuffleCurrentIndex = 0;
                }
            }
            while ((FavoriteModes[shuffleFavoriteModes[shuffleCurrentIndex]] == 0U || shuffleFavoriteModes[shuffleCurrentIndex] == *currentMode) && count > 1U);
            if (count > 1U)
            {
                result = shuffleFavoriteModes[shuffleCurrentIndex];
            }
            else
            {
                result = *currentMode + 1U < MODE_AMOUNT ? *currentMode + 1U : 0U;
            }
            LOG.printf("Favorites manager change currentMode %d to %d\n", *currentMode, result);
            return result;
        }

        static uint32_t getNextTime()                           // определяет время следующего переключения на следующий избранный эффект
        {
            return millis() + Interval * 1000 + random(0, Dispersion + 1) * 1000;
        }
};
