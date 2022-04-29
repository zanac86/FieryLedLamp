bool brightDirection;
static bool startButtonHolding = false;                     // флаг: кнопка удерживается для изменения яркости/скорости/масштаба лампы кнопкой
static bool Button_Holding = false;


void buttonTick()
{
    if (!buttonEnabled)                                       // события кнопки не обрабатываются, если она заблокирована
    {
        return;
    }

    touch.tick();
    uint8_t clickCount = touch.hasClicks() ? touch.getClicks() : 0U;


    // однократное нажатие
    if (clickCount == 1U)
    {
        ONflag = !ONflag;
        jsonWrite(configSetup, "Power", ONflag);
        saveConfig();
        changePower();
        loadingFlag = true;

    }


    // двухкратное нажатие
    if (ONflag && clickCount == 2U)
    {
        if (FavoritesManager::FavoritesRunning)
        {
            uint8_t lastMode = currentMode;
            do
            {
                if (++currentMode >= MODE_AMOUNT)
                {
                    currentMode = 0;
                }
            }
            while (FavoritesManager::FavoriteModes[currentMode] == 0 && currentMode != lastMode);
            if (currentMode == lastMode) // если ни один режим не добавлен в избранное, всё равно куда-нибудь переключимся
                if (++currentMode >= MODE_AMOUNT)
                {
                    currentMode = 0;
                }
        }
        else
            if (++currentMode >= MODE_AMOUNT)
            {
                currentMode = 0;
            }

        jsonWrite(configSetup, "eff_sel", currentMode);
        jsonWrite(configSetup, "br", modes[currentMode].Brightness);
        jsonWrite(configSetup, "sp", modes[currentMode].Speed);
        jsonWrite(configSetup, "sc", modes[currentMode].Scale);
        FastLED.setBrightness(modes[currentMode].Brightness);
        loadingFlag = true;

        if (random_on && FavoritesManager::FavoritesRunning)
        {
            selectedSettings = 1U;
        }
        LOG.printf("Button change currentMode to %d\n", currentMode);
    }


    // трёхкратное нажатие
    if (ONflag && clickCount == 3U)
    {
        if (FavoritesManager::FavoritesRunning)
        {
            uint8_t lastMode = currentMode;
            do
            {
                if (--currentMode >= MODE_AMOUNT)
                {
                    currentMode = MODE_AMOUNT - 1;
                }
            }
            while (FavoritesManager::FavoriteModes[currentMode] == 0 && currentMode != lastMode);
            if (currentMode == lastMode) // если ни один режим не добавлен в избранное, всё равно куда-нибудь переключимся
                if (--currentMode >= MODE_AMOUNT)
                {
                    currentMode = MODE_AMOUNT - 1;
                }
        }
        else
            if (--currentMode >= MODE_AMOUNT)
            {
                currentMode = MODE_AMOUNT - 1;
            }

        jsonWrite(configSetup, "eff_sel", currentMode);
        jsonWrite(configSetup, "br", modes[currentMode].Brightness);
        jsonWrite(configSetup, "sp", modes[currentMode].Speed);
        jsonWrite(configSetup, "sc", modes[currentMode].Scale);
        FastLED.setBrightness(modes[currentMode].Brightness);
        loadingFlag = true;

        if (random_on && FavoritesManager::FavoritesRunning)
        {
            selectedSettings = 1U;
        }
        LOG.printf("Button change currentMode to %d\n", currentMode);
    }


    // четырёхкратное нажатие
    if (clickCount == 4U)
    {
    }


    // пятикратное нажатие
    if (clickCount == 5U)                                     // вывод IP на лампу
    {
        if (espMode == 1U)
        {
            loadingFlag = true;
            while (!fillString(WiFi.localIP().toString().c_str(), CRGB::White, false))
            {
                delay(1);
                ESP.wdtFeed();
            }
            loadingFlag = true;
        }
    }


    // шестикратное нажатие
    if (clickCount == 6U)
    {
    }


    // семикратное нажатие
    if (clickCount == 7U)  // if (ONflag &&                   // смена рабочего режима лампы: с WiFi точки доступа на WiFi клиент или наоборот
    {
#ifdef RESET_WIFI_ON_ESP_MODE_CHANGE
        if (espMode)
        {
            wifiManager.resetSettings();    // сброс сохранённых SSID и пароля (сброс настроек подключения к роутеру)
        }
#endif
        espMode = (espMode == 0U) ? 1U : 0U;
        jsonWrite(configSetup, "ESP_mode", (int)espMode);
        saveConfig();

        ESP.restart();
    }


    // кнопка только начала удерживаться
    //if (ONflag && touch.isHolded())
    if (touch.isHolded()) // пускай для выключенной лампы удержание кнопки включает белую лампу
    {
        brightDirection = !brightDirection;
        startButtonHolding = true;
    }


    // кнопка нажата и удерживается
    //  if (ONflag && touch.isStep())
    if (touch.isStep())
        if (ONflag && !Button_Holding)
        {

            int8_t but = touch.getHoldClicks();
            //Serial.println (but);

            switch (but)
            {
                case 0U:                                              // просто удержание (до удержания кнопки кликов не было) - изменение яркости
                    {
                        uint8_t delta = modes[currentMode].Brightness < 10U // определение шага изменения яркости: при яркости [1..10] шаг = 1, при [11..16] шаг = 3, при [17..255] шаг = 15
                                        ? 1U
                                        : 5U;
                        modes[currentMode].Brightness =
                            constrain(brightDirection
                                      ? modes[currentMode].Brightness + delta
                                      : modes[currentMode].Brightness - delta,
                                      1, 255);
                        jsonWrite(configSetup, "br", modes[currentMode].Brightness);
                        FastLED.setBrightness(modes[currentMode].Brightness);
                        break;
                    }
                case 1U:                                              // удержание после одного клика - изменение скорости
                    {
                        modes[currentMode].Speed = constrain(brightDirection ? modes[currentMode].Speed + 1 : modes[currentMode].Speed - 1, 1, 255);
                        jsonWrite(configSetup, "sp", modes[currentMode].Speed);
                        loadingFlag = true; // без перезапуска эффекта ничего и не увидишь


                        break;
                    }
                case 2U:                                              // удержание после двух кликов - изменение масштаба
                    {
                        modes[currentMode].Scale = constrain(brightDirection ? modes[currentMode].Scale + 1 : modes[currentMode].Scale - 1, 1, 100);
                        jsonWrite(configSetup, "sc", modes[currentMode].Scale);
                        loadingFlag = true; // без перезапуска эффекта ничего и не увидишь
                        break;
                    }
                default:
                    break;
            }
        }
        else
        {
            if (!Button_Holding)
            {
                int8_t but = touch.getHoldClicks();
                switch (but)
                {
                    case 0U:                                              // просто удержание (до удержания кнопки кликов не было) - белый свет
                        {
                            Button_Holding = true;
                            currentMode = EFF_WHITE_COLOR;
                            jsonWrite(configSetup, "eff_sel", currentMode);
                            jsonWrite(configSetup, "br", modes[currentMode].Brightness);
                            jsonWrite(configSetup, "sp", modes[currentMode].Speed);
                            jsonWrite(configSetup, "sc", modes[currentMode].Scale);
                            ONflag = true;
                            jsonWrite(configSetup, "Power", ONflag);
                            changePower();
                            break;
                        }
                }
            }
        }

    // кнопка отпущена после удерживания
    if (ONflag && !touch.isHold() && startButtonHolding)      // кнопка отпущена после удерживания, нужно отправить MQTT сообщение об изменении яркости лампы
    {
        startButtonHolding = false;
        Button_Holding = false;
        loadingFlag = true;

    }
}
