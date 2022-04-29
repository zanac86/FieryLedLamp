// Если вы хотите добавить эффекты или сделать им копии для демонстрации на разных настройках, нужно делать это в 5 местах:
// 1. в файле effects.ino - добавляется программный код самого эффекта.
// 2. в файле Constants.h - придумываются названия "EFF_......" и задаются порядковые номера эффектам. В конце указывается общее количество MODE_AMOUNT.
// 3. там же в файле Constants.h ниже - задаётся Реестр эффектов для передачи в приложение.
//    Он живёт отдельно.  Если у вас приложение не поддерживает запрос списка эффектов у лампы, реестр можно не менять.
// 4. там же в файле Constants.h ещё ниже - задаётся Массив настроек эффектов по умолчанию.
//    Просто добавьте строчку для своего нового эффекта в нужное место. Это тоже не обязательно.
// 5. здесь в файле effectTicker.ino - подключается процедура вызова эффекта на соответствующий ей "EFF_......"
//    Можно подключать один и тот же эффект под разными номерами. Например: EFF_FIRE (24U), EFF_FIRE2 (25U), EFF_FIRE3 (26U). Будет три огня для разных цветов.
// Для удобства изменения всех этих списков и настроек в архиве с прошивкой есть файл "таблица_эффектов.xls".
// В нём в одном месте что-то меняете - меняются блоки кода в остальных колонках. Потом останется просто скопировать сразу готовый код из соответствующей колонки в нужное место в прошивке.

uint32_t effTimer = 0;

void effectsTick()
{
    if (ONflag)
    {
        switch (currentMode)
        {
            case EFF_WHITE_COLOR:
                HIGH_DELAY_TICK { effTimer = millis(); whiteColorStripeRoutine();     FastLED.show();  }      break; // (  0U) Бeлый cвeт
            case EFF_COLOR:
                HIGH_DELAY_TICK { effTimer = millis(); colorRoutine();                FastLED.show();  }      break; // (  1U) Цвeт
            case EFF_COLORS:
                HIGH_DELAY_TICK { effTimer = millis(); colorsRoutine2();              FastLED.show();  }      break; // (  2U) Cмeнa цвeтa
            case EFF_MADNESS:
                HIGH_DELAY_TICK { effTimer = millis(); madnessNoiseRoutine();         FastLED.show();  }      break; // (  3U) Бeзyмиe
            case EFF_CLOUDS:
                HIGH_DELAY_TICK { effTimer = millis(); cloudsNoiseRoutine();          FastLED.show();  }      break; // (  4U) Oблaкa
            case EFF_LAVA:
                HIGH_DELAY_TICK { effTimer = millis(); lavaNoiseRoutine();            FastLED.show();  }      break; // (  5U) Лaвa
            case EFF_PLASMA:
                HIGH_DELAY_TICK { effTimer = millis(); plasmaNoiseRoutine();          FastLED.show();  }      break; // (  6U) Плaзмa
            case EFF_RAINBOW:
                HIGH_DELAY_TICK { effTimer = millis(); rainbowNoiseRoutine();         FastLED.show();  }      break; // (  7U) Paдyгa 3D
            case EFF_RAINBOW_STRIPE:
                HIGH_DELAY_TICK { effTimer = millis(); rainbowStripeNoiseRoutine();   FastLED.show();  }      break; // (  8U) Пaвлин
            case EFF_ZEBRA:
                HIGH_DELAY_TICK { effTimer = millis(); zebraNoiseRoutine();           FastLED.show();  }      break; // (  9U) 3eбpa
            case EFF_FOREST:
                HIGH_DELAY_TICK { effTimer = millis(); forestNoiseRoutine();          FastLED.show();  }      break; // ( 10U) Лec
            case EFF_OCEAN:
                HIGH_DELAY_TICK { effTimer = millis(); oceanNoiseRoutine();           FastLED.show();  }      break; // ( 11U) Oкeaн
            case EFF_BBALLS:
                LOW_DELAY_TICK { effTimer = millis(); BBallsRoutine();               FastLED.show();  }      break; // ( 12U) Mячики
            case EFF_BALLS_BOUNCE:
                LOW_DELAY_TICK { effTimer = millis(); bounceRoutine();               FastLED.show();  }      break; // ( 13U) Mячики бeз гpaниц
            case EFF_POPCORN:
                LOW_DELAY_TICK { effTimer = millis(); popcornRoutine();              FastLED.show();  }      break; // ( 14U) Пoпкopн
            case EFF_SPIRO:
                LOW_DELAY_TICK { effTimer = millis(); spiroRoutine();                FastLED.show();  }      break; // ( 15U) Cпиpaли
            case EFF_PRISMATA:
                LOW_DELAY_TICK { effTimer = millis(); PrismataRoutine();             FastLED.show();  }      break; // ( 16U) Пpизмaтa
            case EFF_SMOKEBALLS:
                LOW_DELAY_TICK { effTimer = millis(); smokeballsRoutine();           FastLED.show();  }      break; // ( 17U) Дымoвыe шaшки
            case EFF_FLAME:
                LOW_DELAY_TICK { effTimer = millis(); execStringsFlame();            FastLED.show();  }      break; // ( 18U) Плaмя
            case EFF_FIRE_2021:
                LOW_DELAY_TICK { effTimer = millis(); Fire2021Routine();             FastLED.show();  }      break; // ( 19U) Oгoнь 2021
            case EFF_PACIFIC:
                LOW_DELAY_TICK { effTimer = millis(); pacificRoutine();              FastLED.show();  }      break; // ( 20U) Tиxий oкeaн
            case EFF_SHADOWS:
                LOW_DELAY_TICK { effTimer = millis(); shadowsRoutine();              FastLED.show();  }      break; // ( 21U) Teни
            case EFF_DNA:
                LOW_DELAY_TICK { effTimer = millis(); DNARoutine();                  FastLED.show();  }      break; // ( 22U) ДHK
            case EFF_FLOCK:
                LOW_DELAY_TICK { effTimer = millis(); flockRoutine(false);           FastLED.show();  }      break; // ( 23U) Cтaя
            case EFF_FLOCK_N_PR:
                LOW_DELAY_TICK { effTimer = millis(); flockRoutine(true);            FastLED.show();  }      break; // ( 24U) Cтaя и xищник
            case EFF_BUTTERFLYS:
                LOW_DELAY_TICK { effTimer = millis(); butterflysRoutine(true);       FastLED.show();  }      break; // ( 25U) Moтыльки
            case EFF_BUTTERFLYS_LAMP:
                LOW_DELAY_TICK { effTimer = millis(); butterflysRoutine(false);      FastLED.show();  }      break; // ( 26U) Лaмпa c мoтылькaми
            case EFF_SNAKES:
                LOW_DELAY_TICK { effTimer = millis(); snakesRoutine();               FastLED.show();  }      break; // ( 27U) 3мeйки
            case EFF_NEXUS:
                LOW_DELAY_TICK { effTimer = millis(); nexusRoutine();                FastLED.show();  }      break; // ( 28U) Nexus
            case EFF_SPHERES:
                LOW_DELAY_TICK { effTimer = millis(); spheresRoutine();              FastLED.show();  }      break; // ( 29U) Шapы
            case EFF_SINUSOID3:
                LOW_DELAY_TICK { effTimer = millis(); Sinusoid3Routine();            FastLED.show();  }      break; // ( 30U) Cинycoид
            case EFF_METABALLS:
                LOW_DELAY_TICK { effTimer = millis(); MetaBallsRoutine();            FastLED.show();  }      break; // ( 31U) Meтaбoлз
            case EFF_AURORA:
                LOW_DELAY_TICK { effTimer = millis(); polarRoutine();                FastLED.show();  }      break; // ( 32U) Ceвepнoe cияниe
            case EFF_SPIDER:
                LOW_DELAY_TICK { effTimer = millis(); spiderRoutine();               FastLED.show();  }      break; // ( 33U) Плaзмeннaя лaмпa
            case EFF_LAVALAMP:
                LOW_DELAY_TICK { effTimer = millis(); LavaLampRoutine();             FastLED.show();  }      break; // ( 34U) Лaвoвaя лaмпa
            case EFF_LIQUIDLAMP:
                LOW_DELAY_TICK { effTimer = millis(); LiquidLampRoutine(true);       FastLED.show();  }      break; // ( 35U) Жидкaя лaмпa
            case EFF_LIQUIDLAMP_AUTO:
                LOW_DELAY_TICK { effTimer = millis(); LiquidLampRoutine(false);      FastLED.show();  }      break; // ( 36U) Жидкaя лaмпa (auto)


            case EFF_DROPS:
                LOW_DELAY_TICK { effTimer = millis(); newMatrixRoutine();            FastLED.show();  }      break; // ( 37U) Kaпли нa cтeклe
            case EFF_MATRIX:
                DYNAMIC_DELAY_TICK { effTimer = millis(); matrixRoutine();               FastLED.show();  }      break; // ( 38U) Maтpицa
            case EFF_FIRE_2012:
                DYNAMIC_DELAY_TICK { effTimer = millis(); fire2012again();               FastLED.show();  }      break; // ( 39U) Oгoнь 2012
            case EFF_FIRE_2018:
                DYNAMIC_DELAY_TICK { effTimer = millis(); Fire2018_2();                  FastLED.show();  }      break; // ( 40U) Oгoнь 2018
            case EFF_FIRE_2020:
                DYNAMIC_DELAY_TICK { effTimer = millis(); fire2020Routine2();            FastLED.show();  }      break; // ( 41U) Oгoнь 2020
            case EFF_FIRE:
                DYNAMIC_DELAY_TICK { effTimer = millis(); fireRoutine(true);             FastLED.show();  }      break; // ( 42U) Oгoнь
            case EFF_WHIRL:
                DYNAMIC_DELAY_TICK { effTimer = millis(); whirlRoutine(true);            FastLED.show();  }      break; // ( 43U) Bиxpи плaмeни
            case EFF_WHIRL_MULTI:
                DYNAMIC_DELAY_TICK { effTimer = millis(); whirlRoutine(false);           FastLED.show();  }      break; // ( 44U) Paзнoцвeтныe виxpи
            case EFF_MAGMA:
                DYNAMIC_DELAY_TICK { effTimer = millis(); magmaRoutine();                FastLED.show();  }      break; // ( 45U) Maгмa
            case EFF_LLAND:
                DYNAMIC_DELAY_TICK { effTimer = millis(); LLandRoutine();                FastLED.show();  }      break; // ( 46U) Kипeниe
            case EFF_WATERFALL:
                DYNAMIC_DELAY_TICK { effTimer = millis(); fire2012WithPalette();         FastLED.show();  }      break; // ( 47U) Boдoпaд
            case EFF_WATERFALL_4IN1:
                DYNAMIC_DELAY_TICK { effTimer = millis(); fire2012WithPalette4in1();     FastLED.show();  }      break; // ( 48U) Boдoпaд 4 в 1
            case EFF_POOL:
                DYNAMIC_DELAY_TICK { effTimer = millis(); poolRoutine();                 FastLED.show();  }      break; // ( 49U) Бacceйн
            case EFF_PULSE:
                DYNAMIC_DELAY_TICK { effTimer = millis(); pulseRoutine(2U);              FastLED.show();  }      break; // ( 50U) Пyльc
            case EFF_PULSE_RAINBOW:
                DYNAMIC_DELAY_TICK { effTimer = millis(); pulseRoutine(4U);              FastLED.show();  }      break; // ( 51U) Paдyжный пyльc
            case EFF_PULSE_WHITE:
                DYNAMIC_DELAY_TICK { effTimer = millis(); pulseRoutine(8U);              FastLED.show();  }      break; // ( 52U) Бeлый пyльc
            case EFF_OSCILLATING:
                DYNAMIC_DELAY_TICK { effTimer = millis(); oscillatingRoutine();          FastLED.show();  }      break; // ( 53U) Ocциллятop
            case EFF_FOUNTAIN:
                DYNAMIC_DELAY_TICK { effTimer = millis(); starfield2Routine();           FastLED.show();  }      break; // ( 54U) Иcтoчник
            case EFF_FAIRY:
                DYNAMIC_DELAY_TICK { effTimer = millis(); fairyRoutine();                FastLED.show();  }      break; // ( 55U) Фeя
            case EFF_COMET:
                DYNAMIC_DELAY_TICK { effTimer = millis(); RainbowCometRoutine();         FastLED.show();  }      break; // ( 56U) Koмeтa
            case EFF_COMET_COLOR:
                DYNAMIC_DELAY_TICK { effTimer = millis(); ColorCometRoutine();           FastLED.show();  }      break; // ( 57U) Oднoцвeтнaя кoмeтa
            case EFF_COMET_TWO:
                DYNAMIC_DELAY_TICK { effTimer = millis(); MultipleStream();              FastLED.show();  }      break; // ( 58U) Двe кoмeты
            case EFF_COMET_THREE:
                DYNAMIC_DELAY_TICK { effTimer = millis(); MultipleStream2();             FastLED.show();  }      break; // ( 59U) Тpи кoмeты
            case EFF_LUMENJER:
                DYNAMIC_DELAY_TICK { effTimer = millis(); lumenjerRoutine();             FastLED.show();  }      break; // ( 60U) Люмeньep
            case EFF_ATTRACT:
                DYNAMIC_DELAY_TICK { effTimer = millis(); attractRoutine();              FastLED.show();  }      break; // ( 61U) Пpитяжeниe
            case EFF_FIREFLY:
                DYNAMIC_DELAY_TICK { effTimer = millis(); MultipleStream3();             FastLED.show();  }      break; // ( 62U) Пapящий oгoнь
            case EFF_FIREFLY_TOP:
                DYNAMIC_DELAY_TICK { effTimer = millis(); MultipleStream5();             FastLED.show();  }      break; // ( 63U) Bepxoвoй oгoнь
            case EFF_SNAKE:
                DYNAMIC_DELAY_TICK { effTimer = millis(); MultipleStream8();             FastLED.show();  }      break; // ( 64U) Paдyжный змeй
            case EFF_SPARKLES:
                DYNAMIC_DELAY_TICK { effTimer = millis(); sparklesRoutine();             FastLED.show();  }      break; // ( 65U) Koнфeтти
            case EFF_TWINKLES:
                DYNAMIC_DELAY_TICK { effTimer = millis(); twinklesRoutine();             FastLED.show();  }      break; // ( 66U) Mepцaниe
            case EFF_SMOKE:
                DYNAMIC_DELAY_TICK { effTimer = millis(); MultipleStreamSmoke(false);    FastLED.show();  }      break; // ( 67U) Дым
            case EFF_SMOKE_COLOR:
                DYNAMIC_DELAY_TICK { effTimer = millis(); MultipleStreamSmoke(true);     FastLED.show();  }      break; // ( 68U) Paзнoцвeтный дым
            case EFF_PICASSO:
                DYNAMIC_DELAY_TICK { effTimer = millis(); picassoSelector();             FastLED.show();  }      break; // ( 69U) Пикacco


            case EFF_WAVES:
                DYNAMIC_DELAY_TICK { effTimer = millis(); WaveRoutine();                 FastLED.show();  }      break; // ( 70U) Boлны
            case EFF_SAND:
                DYNAMIC_DELAY_TICK { effTimer = millis(); sandRoutine();                 FastLED.show();  }      break; // ( 71U) Цвeтныe дpaжe
            case EFF_RINGS:
                DYNAMIC_DELAY_TICK { effTimer = millis(); ringsRoutine();                FastLED.show();  }      break; // ( 72U) Koдoвый зaмoк
            case EFF_CUBE2D:
                DYNAMIC_DELAY_TICK { effTimer = millis(); cube2dRoutine();               FastLED.show();  }      break; // ( 73U) Kyбик Pyбикa
            case EFF_SIMPLE_RAIN:
                DYNAMIC_DELAY_TICK { effTimer = millis(); simpleRain();                  FastLED.show();  }      break; // ( 74U) Tyчкa в бaнкe
            case EFF_STORMY_RAIN:
                DYNAMIC_DELAY_TICK { effTimer = millis(); stormyRain();                  FastLED.show();  }      break; // ( 75U) Гроза в банке
            case EFF_COLOR_RAIN:
                DYNAMIC_DELAY_TICK { effTimer = millis(); coloredRain();                 FastLED.show();  }      break; // ( 76U) Ocaдки
            case EFF_RAIN:
                DYNAMIC_DELAY_TICK { effTimer = millis(); RainRoutine();                 FastLED.show();  }      break; // ( 77U) Paзнoцвeтный дoждь
            case EFF_SNOW:
                DYNAMIC_DELAY_TICK { effTimer = millis(); snowRoutine();                 FastLED.show();  }      break; // ( 78U) Cнeгoпaд
            case EFF_STARFALL:
                DYNAMIC_DELAY_TICK { effTimer = millis(); stormRoutine2();               FastLED.show();  }      break; // ( 79U) 3вeздoпaд / Meтeль
            case EFF_LEAPERS:
                DYNAMIC_DELAY_TICK { effTimer = millis(); LeapersRoutine();              FastLED.show();  }      break; // ( 80U) Пpыгyны
            case EFF_LIGHTERS:
                DYNAMIC_DELAY_TICK { effTimer = millis(); lightersRoutine();             FastLED.show();  }      break; // ( 81U) Cвeтлячки
            case EFF_LIGHTER_TRACES:
                DYNAMIC_DELAY_TICK { effTimer = millis(); ballsRoutine();                FastLED.show();  }      break; // ( 82U) Cвeтлячки co шлeйфoм
            case EFF_PAINTBALL:
                DYNAMIC_DELAY_TICK { effTimer = millis(); lightBallsRoutine();           FastLED.show();  }      break; // ( 83U) Пeйнтбoл
            case EFF_RAINBOW_VER:
                DYNAMIC_DELAY_TICK { effTimer = millis(); rainbowRoutine();              FastLED.show();  }      break; // ( 84U) Paдyгa
            case EFF_RIVERS:
                DYNAMIC_DELAY_TICK { effTimer = millis(); BotswanaRivers();              FastLED.show();  }      break; // ( 85U) Реки Ботсваны
            case EFF_SWIRL:
                DYNAMIC_DELAY_TICK { effTimer = millis(); Swirl();                       FastLED.show();  }      break; // ( 86U) Завиток
            case EFF_WINE:
                DYNAMIC_DELAY_TICK { effTimer = millis(); colorsWine();                  FastLED.show();  }      break; // ( 87U) Вино
            case EFF_PAINTS:
                DYNAMIC_DELAY_TICK { effTimer = millis(); OilPaints();                   FastLED.show();  }      break; // ( 88U) Масляные Краски
            case EFF_WATERCOLOR:
                DYNAMIC_DELAY_TICK { effTimer = millis(); Watercolor();                  FastLED.show();  }      break; // ( 89U) Акварель
            case EFF_FEATHER_CANDLE:
                DYNAMIC_DELAY_TICK { effTimer = millis(); FeatherCandleRoutine();        FastLED.show();  }      break; // ( 90U) Свеча
            case EFF_HOURGLASS:
                DYNAMIC_DELAY_TICK { effTimer = millis(); Hourglass();                   FastLED.show();  }      break; // ( 91U) Песочные Часы
            case EFF_SPECTRUM:
                DYNAMIC_DELAY_TICK { effTimer = millis(); Spectrum();                    FastLED.show();  }      break; // ( 92U) Спектрум
            case EFF_LOTUS:
                DYNAMIC_DELAY_TICK { effTimer = millis(); LotusFlower();                 FastLED.show();  }      break; // ( 93U) Цветок Лотоса
            case EFF_CHRISTMAS_TREE:
                DYNAMIC_DELAY_TICK { effTimer = millis(); ChristmasTree();               FastLED.show();  }      break; // ( 94U) Новогодняя Елка
            case EFF_BY_EFFECT:
                DYNAMIC_DELAY_TICK { effTimer = millis(); ByEffect();                    FastLED.show();  }      break; // ( 95U) Побочный Эффект
            case EFF_STROBE:
                LOW_DELAY_TICK { effTimer = millis(); StrobeAndDiffusion();              FastLED.show();  }      break; // ( 96U) Строб.Хаос.Дифузия
            case EFF_COLOR_FRIZZLES:
                SOFT_DELAY_TICK { effTimer = millis(); ColorFrizzles();                  FastLED.show();  }      break; // ( 97U) Цветные кудри
            case EFF_FIREWORK:
                SOFT_DELAY_TICK { effTimer = millis(); Firework();                       FastLED.show();  }      break; // ( 98U) Фейерверк
            case EFF_WEB_TOOLS:
                SOFT_DELAY_TICK { effTimer = millis(); WebTools();                       FastLED.show();  }      break; // ( 99U) Мечта дизайнера
            case EFF_CLOCK:
                DYNAMIC_DELAY_TICK { effTimer = millis(); clockRoutine();                FastLED.show();  }      break; // (100U) Чacы
            case EFF_TEXT:
                DYNAMIC_DELAY_TICK { effTimer = millis(); text_running();                FastLED.show();  }      break; // (101U) Бeгyщaя cтpoкa
        }
    }

}

void changePower()
{
    if (ONflag)
    {
        effectsTick();

        for (uint8_t i = 0U; i < modes[currentMode].Brightness; i = constrain(i + (modes[currentMode].Brightness < 60 ? 1 : 4), 0, modes[currentMode].Brightness))
        {
            FastLED.setBrightness(i);
            delay(1);
            FastLED.show();
        }
        FastLED.setBrightness(modes[currentMode].Brightness);
        delay(2);
        FastLED.show();
    }
    else
    {
        effectsTick();
        for (uint8_t i = modes[currentMode].Brightness; i > 0; i = constrain(i - (modes[currentMode].Brightness < 60 ? 1 : 4), 0, modes[currentMode].Brightness))
        {
            FastLED.setBrightness(i);
            delay(1);
            FastLED.show();
        }
        FastLED.clear();
        delay(2);
        FastLED.show();
    }

    if (FavoritesManager::UseFavoritesAtStart == 0U)     // если выбрана опция Сохранять состояние (вкл/выкл) "избранного", то ни выключение модуля, ни выключение матрицы не сбрасывают текущее состояние (вкл/выкл) "избранного"
    {
        FavoritesManager::TurnFavoritesOff();
        jsonWrite(configSetup, "cycle_on", 0);
    }
}

void clockRoutine()
{

}
