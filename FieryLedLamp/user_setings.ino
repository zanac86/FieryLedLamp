/*
Обработка команд web интерфейса

*/

void HTTP_init()
{
    HTTP.on("/random_on", handle_random);  // случайных настроек эффектов в режиме цикл без сохранения в EEPROM
    HTTP.on("/button_on", handle_button_on);  // Вкл\Выкл кнопки лампы (дублирует в приложении, но на виду)
    //HTTP.on("/User_set", handle_User_set); // Сохранение random_on, time_always, button_on и favorit в EEPROM (Файл)
    HTTP.on("/ESP_mode", handle_ESP_mode); // Установка ESP Mode
    HTTP.on("/eff_reset", handle_eff_reset);  //сброс настроек эффектов по умолчанию
    HTTP.on("/run_text", handle_run_text);  // Текст для бегущей строки
    HTTP.on("/off_after_shutdown", handle_off_after_shutdown);  // Не возобновлять работу эффектов
    HTTP.on("/eff_sel", handle_eff_sel);  // Выбор эффекта из списка
    HTTP.on("/eff", handle_eff);  // Следующий / Предыдущий
    HTTP.on("/br", handle_br);  // Яркость
    HTTP.on("/sp", handle_sp);  // Скорость
    HTTP.on("/sc", handle_sc);  // Масштаб / Цвет
    HTTP.on("/brm", handle_brm);  // Пошаговая яркость  минус
    HTTP.on("/brp", handle_brp);  // Пошаговая яркость плюс
    HTTP.on("/spm", handle_spm);  // Пошаговая  скорость минус
    HTTP.on("/spp", handle_spp);  // Пошаговая скорость  плюс
    HTTP.on("/scm", handle_scm);  // Пошаговый масштаб  минус
    HTTP.on("/scp", handle_scp);  // Пошаговый мвсштаб  плюс
    HTTP.on("/Power", handle_Power);          // устройство вкл/выкл
    HTTP.on("/cycle_on", handle_cycle_on);   // Вкл/выкл режима Цикл
    HTTP.on("/time_eff", handle_time_eff);   // Время переключения цикла + Dispersion добавочное случайное время от 0 до disp
    HTTP.on("/rnd_cycle", handle_rnd_cycle);   // Перемешать выбранные или по порядку
    HTTP.on("/favorites_at_start", handle_favorites_at_start);   // Запускать режим цыкл после выкл/вкл лампы или нет
    HTTP.on("/cycle_set", handle_cycle_set);   // Выбор эффектов
    HTTP.on("/eff_all", handle_eff_all);   // Выбрать все
    HTTP.on("/eff_clr", handle_eff_clr);   // сбросить Выбор
    HTTP.on("/def", handle_def);   //  Установка настроек эффекта по умолчанию
    HTTP.on("/rnd", handle_rnd);   // Установка случайных настроек эффектов
    HTTP.on("/all_br", handle_all_br);  // Общая яркость
    HTTP.on("/eff_save", handle_eff_save);  // Сохранить настройки эффектов в файл
    HTTP.on("/eff_read", handle_eff_read);  // Загрузить настройки эффектов из файла

    // --------------------Получаем SSID со страницы
    HTTP.on("/ssid", HTTP_GET, []()
    {
        jsonWrite(configSetup, "ssid", HTTP.arg("ssid"));
        jsonWrite(configSetup, "password", HTTP.arg("password"));
        jsonWrite(configSetup, "TimeOut", HTTP.arg("TimeOut").toInt());
        ESP_CONN_TIMEOUT = jsonReadtoInt(configSetup, "TimeOut");
        saveConfig();                 // Функция сохранения данных во Flash
        HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
    });
    // --------------------Получаем SSID AP со страницы
    HTTP.on("/ssidap", HTTP_GET, []()
    {
        jsonWrite(configSetup, "ssidAP", HTTP.arg("ssidAP"));
        jsonWrite(configSetup, "passwordAP", HTTP.arg("passwordAP"));
        saveConfig();                 // Функция сохранения строки конфигурации в файл
        HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
    });

    // -------------------Выдаем данные configSetup
    HTTP.on("/config.setup.json", HTTP_GET, []()
    {
        HTTP.send(200, "application/json", configSetup);
        LOG.println("Sending from memory");
        LOG.println(configSetup);
    });

    // -------------------Обработка Restart
    HTTP.on("/restart", HTTP_GET, []()
    {
        String restart = HTTP.arg("device");          // Получаем значение device из запроса
        if (restart == "ok")                           // Если значение равно Ок
        {
            LOG.println("restart from web request");
            HTTP.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
            yield();
            delay(100);
            while (1)
            {

            }
            ESP.restart();                                // перезагружаем модуль
        }
        else                                          // иначе
        {
            HTTP.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
        }
    });

    httpUpdater.setup(&HTTP);    // Добавляем функцию Update для перезаписи прошивки по WiFi при 4М(1M SPIFFS) и выше
    HTTP.begin();                // Запускаем HTTP сервер

}

void handle_random()
{
    jsonWrite(configSetup, "random_on", HTTP.arg("random_on").toInt());
    saveConfig();
    random_on = jsonReadtoInt(configSetup, "random_on");
    HTTP.send(200, "text/plain", "OK");
}

void handle_button_on()
{
    jsonWrite(configSetup, "button_on", HTTP.arg("button_on").toInt());
    saveConfig();
    buttonEnabled = jsonReadtoInt(configSetup, "button_on");
    HTTP.send(200, "text/plain", "OK");
}

void handle_ESP_mode()
{
    jsonWrite(configSetup, "ESP_mode", HTTP.arg("ESP_mode").toInt());
    saveConfig();
    espMode = jsonReadtoInt(configSetup, "ESP_mode");
    HTTP.send(200, "text/plain", "OK");
}

void handle_eff_reset()
{
    restoreSettings();
    loadingFlag = true;  // Перезапуск Эффекта
    jsonWrite(configSetup, "br", modes[currentMode].Brightness);
    jsonWrite(configSetup, "sp", modes[currentMode].Speed);
    jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    HTTP.send(200, "text/plain", "OK");
}

void handle_run_text()
{
    jsonWrite(configSetup, "run_text", HTTP.arg("run_text"));
    saveConfig();                 // Функция сохранения данных во Flash
    (jsonRead(configSetup, "run_text")).toCharArray(TextTicker, (jsonRead(configSetup, "run_text")).length() + 1);
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}

void handle_off_after_shutdown()
{
    jsonWrite(configSetup, "off_after_shutdown", HTTP.arg("off_after_shutdown").toInt());
    saveConfig();
    HTTP.send(200, "text/plain", "OK");
}

void handle_eff_sel()
{
    jsonWrite(configSetup, "eff_sel", HTTP.arg("eff_sel").toInt());
    currentMode = jsonReadtoInt(configSetup, "eff_sel");
    jsonWrite(configSetup, "br", modes[currentMode].Brightness);
    jsonWrite(configSetup, "sp", modes[currentMode].Speed);
    jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    FastLED.setBrightness(modes[currentMode].Brightness);
    loadingFlag = true;
    if (random_on && FavoritesManager::FavoritesRunning)
    {
        selectedSettings = 1U;
    }
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_eff()
{
    jsonWrite(configSetup, "eff", HTTP.arg("eff").toInt());
    if (HTTP.arg("eff").toInt())
    {
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
    }
    else
    {
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
    }
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_br()
{
    jsonWrite(configSetup, "br", HTTP.arg("br").toInt());
    modes[currentMode].Brightness = jsonReadtoInt(configSetup, "br");
    FastLED.setBrightness(modes[currentMode].Brightness);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_sp()
{
    jsonWrite(configSetup, "sp", HTTP.arg("sp").toInt());
    modes[currentMode].Speed = jsonReadtoInt(configSetup, "sp");
    loadingFlag = true;  // Перезапуск Эффекта
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_sc()
{
    jsonWrite(configSetup, "sc", HTTP.arg("sc").toInt());
    modes[currentMode].Scale = jsonReadtoInt(configSetup, "sc");
    loadingFlag = true;  // Перезапуск Эффекта
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_brm()
{
    modes[currentMode].Brightness = constrain(modes[currentMode].Brightness - 1, 1, 255);
    jsonWrite(configSetup, "br", modes[currentMode].Brightness);
    FastLED.setBrightness(modes[currentMode].Brightness);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_brp()
{
    modes[currentMode].Brightness = constrain(modes[currentMode].Brightness + 1, 1, 255);
    jsonWrite(configSetup, "br", modes[currentMode].Brightness);
    FastLED.setBrightness(modes[currentMode].Brightness);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_spm()
{
    modes[currentMode].Speed = constrain(modes[currentMode].Speed - 1, 1, 255);
    jsonWrite(configSetup, "sp", modes[currentMode].Speed);
    loadingFlag = true;  // Перезапуск Эффекта
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_spp()
{
    modes[currentMode].Speed = constrain(modes[currentMode].Speed + 1, 1, 255);
    jsonWrite(configSetup, "sp", modes[currentMode].Speed);
    loadingFlag = true;  // Перезапуск Эффекта
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_scm()
{
    modes[currentMode].Scale = constrain(modes[currentMode].Scale - 1, 1, 100);
    jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    loadingFlag = true;  // Перезапуск Эффекта
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_scp()
{
    modes[currentMode].Scale = constrain(modes[currentMode].Scale + 1, 1, 100);
    jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    loadingFlag = true;  // Перезапуск Эффекта
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_Power()
{
    uint8_t tmp = HTTP.arg("Power").toInt();
    LOG.printf("handle_Power tmp=%d\n", tmp);
    ONflag = (tmp == 0) ? false : true;
    jsonWrite(configSetup, "Power", tmp);
    saveConfig();
    changePower();
    loadingFlag = true;
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_cycle_on()     // Вкл/выкл режима Цикл
{
    uint8_t tmp = HTTP.arg("cycle_on").toInt();
    jsonWrite(configSetup, "cycle_on", tmp);
    if (ONflag)
    {
        FavoritesManager::FavoritesRunning = tmp;
    }
    else
    {
        FavoritesManager::FavoritesRunning = 0;
        jsonWrite(configSetup, "cycle_on", 0);
    }
    saveConfig();
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");  //HTTP.send(200, "text/plain", "OK");
}

void handle_time_eff()      // Время переключения цикла + Dispersion добавочное случайное время от 0 до disp
{
    jsonWrite(configSetup, "time_eff", HTTP.arg("time_eff").toInt());
    FavoritesManager::Interval = jsonReadtoInt(configSetup, "time_eff");
    jsonWrite(configSetup, "disp", HTTP.arg("disp").toInt());
    FavoritesManager::Dispersion = jsonReadtoInt(configSetup, "disp");
    saveConfig();
    HTTP.send(200, "text/plain", "OK");
}

void handle_rnd_cycle()      // Перемешать выбранные или по порядку
{
    jsonWrite(configSetup, "rnd_cycle", HTTP.arg("rnd_cycle").toInt());
    FavoritesManager::rndCycle = jsonReadtoInt(configSetup, "rnd_cycle");
    saveConfig();
    HTTP.send(200, "text/plain", "OK");
}

void handle_favorites_at_start()      // Запускать режим цыкл после выкл/вкл лампы или нет
{
    uint8_t tmp = HTTP.arg("favorites_at_start").toInt();
    jsonWrite(configSetup, "favorites_at_start", tmp);
    FavoritesManager::UseFavoritesAtStart = tmp;
    if (!ONflag && !FavoritesManager::UseFavoritesAtStart)
    {
        // если лампа выключена и работает не по списку, то отключить цикл при старте
        FavoritesManager::FavoritesRunning = 0;
        jsonWrite(configSetup, "cycle_on", 0);
    }
    saveConfig();
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");  //HTTP.send(200, "text/plain", "OK");
}

void handle_eff_all()       //Выбрать все эффекты
{
    char i[3];
    String configCycle = readFile("cycle_config.json", 1024);
    // подготовка  строк с именами полей json
    for (uint8_t k = 0; k < MODE_AMOUNT; k++)
    {
        itoa((k), i, 10);
        String e = "e" + String(i) ;
        //сохранение параметров в строку
        jsonWrite(configCycle, e, 1U);
    }
    writeFile("cycle_config.json", configCycle);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_eff_clr()       //очостить все эффекты
{
    char i[3];
    String configCycle = readFile("cycle_config.json", 1024);
    // подготовка  строк с именами полей json
    for (uint8_t k = 0; k < MODE_AMOUNT; k++)
    {
        itoa((k), i, 10);
        String e = "e" + String(i) ;
        //сохранение параметров в строку
        jsonWrite(configCycle, e, 0U);
    }
    writeFile("cycle_config.json", configCycle);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_cycle_set()      // Выбор эффектов для Цикла
{
    LOG.println(F("handle_cycle_set"));
    char i[3];
    String configCycle = readFile("cycle_config.json", 1024);
    // подготовка  строк с именами полей json file
    for (uint8_t k = 0; k < MODE_AMOUNT; k++)
    {
        itoa((k), i, 10);
        String e = "e" + String(i) ;
        //сохранение параметров в строку
        jsonWrite(configCycle, e, HTTP.arg(e).toInt());
        //сохранение выбранных эффектов для Цикла
        FavoritesManager::FavoriteModes[k] = jsonReadtoInt(configCycle, e);
    }
    writeFile("cycle_config.json", configCycle);
    HTTP.send(200, "text/plain", "OK");
}

void cycle_get()     // Начальная инициализация выбранных эффектов из файла при включении питания
{
    LOG.println(F("cycle_get"));
    char i[3];
    bool cycle_change = false;
    String configCycle = readFile("cycle_config.json", 1024);
    // подготовка  строк с именами полей json file
    for (uint8_t k = 0; k < MODE_AMOUNT; k++)
    {
        itoa((k), i, 10);
        String e = "e" + String(i) ;

        FavoritesManager::FavoriteModes[k] = jsonReadtoInt(configCycle, e);

        //передача параметров из массива в строку json если значение в памяти не равно значению в файле
        if (FavoritesManager::FavoriteModes[k] != jsonReadtoInt(configCycle, e))
        {
            jsonWrite(configCycle, e, FavoritesManager::FavoriteModes[k]);
            cycle_change = true;
        }

    }
    if (cycle_change)
    {
        writeFile("cycle_config.json", configCycle);
    }
}

void handle_def()      // Сброс настроек текущего эффекта по умолчанию
{
    setModeSettings();
    loadingFlag = true;
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_rnd()      // Установка случайных настроек текущему эффекту
{
    selectedSettings = 1U;
    loadingFlag = true;
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_all_br()       //Общая яркость
{
    jsonWrite(configSetup, "all_br", HTTP.arg("all_br").toInt());
    uint8_t ALLbri = jsonReadtoInt(configSetup, "all_br");
    for (uint8_t i = 0; i < MODE_AMOUNT; i++)
    {
        modes[i].Brightness = ALLbri;
    }
    jsonWrite(configSetup, "br", ALLbri);
    FastLED.setBrightness(ALLbri);
    loadingFlag = true;
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_eff_save()
{
    eff_save();
    HTTP.send(200, "text/plain", "OK");
}

void handle_eff_read()
{
    eff_read();
    HTTP.send(200, "text/plain", "OK");
}

void eff_save()
{
    File file = SPIFFS.open("/effect.ini", "w");
    if (file)
    {
        for (uint8_t i = 0; i < MODE_AMOUNT; i++)
        {
            file.write(modes[i].Brightness);
            file.write(modes[i].Speed);
            file.write(modes[i].Scale);
        }
    }
    else
    {
    }
    file.close();
}

void eff_read()
{
    File file = SPIFFS.open("/effect.ini", "r");
    if (file)
    {
        for (uint8_t i = 0; i < MODE_AMOUNT; i++)
        {
            modes[i].Brightness = file.read();
            modes[i].Speed = file.read();
            modes[i].Scale = file.read();
        }
        loadingFlag = true;  // Перезапуск Эффекта
        jsonWrite(configSetup, "br", modes[currentMode].Brightness);
        jsonWrite(configSetup, "sp", modes[currentMode].Speed);
        jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    }
    else
    {
    }
    file.close();
}
