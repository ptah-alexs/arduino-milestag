const uint8_t CLONE_COUNT    = 36;
const int16_t lvl_0           = 600;
const int16_t lvl_1           = lvl_0<<1;
const uint8_t irfrequency    = 36;
const int16_t timeout         = 3200;
const int16_t irt             = (int) ((500/irfrequency) - 4);
const uint8_t irpulses       = (int) (lvl_0 / (1000/irfrequency));

// list of pin
int16_t dhead                 = 3;
int16_t triggerpin            = 4;
int16_t irtransmitpin         = 2;

/*
0  - последовательный порт. mp3-модуль
1  - последовательный порт. mp3-модуль
2  - ик-передатчик.
3  - ик-приемник. (ШИМ)
4  - спусковой крючок огня.
5  - индикатор патронов. (ШИМ)
6  - несущий уровень для индикатора брони и здоровья (ШИМ)
7  - переключения режима огня 1.
8  - переключения режима огня 2.
9  - кнопка перезарядки. (ШИМ)
10 - SPI.
11 - SPI.
12 - SPI.
13 - SPI.
14 - индикатор попадания. Аналоговый вход 0
15 - индикатор огня на стволе. Аналоговый вход 1
16 - индикатор активности (перезарядка, там еще какая-нибудь фигня). Аналоговый вход 2
17 - индикатор жизни. Аналоговый вход 3
18 - индикатор брони. Аналоговый вход 4
19 - Античит. Аналоговый вход 5
 */

// other data
uint8_t clone_data[CLONE_COUNT]; 
uint16_t command      = 33536;//instakill

//Global clone data values
uint8_t cl_team_id         = 0;
uint8_t cl_muzzle_flash    = 0;
uint8_t cl_overheat_on     = 0;
uint8_t cl_hit_led_enabled = 0;
uint8_t cl_friendly_fire   = 0;
uint8_t cl_unlimited_clips = 0;
uint8_t cl_zombie_mod      = 0;
uint8_t cl_medpack_enabled = 0;
uint8_t cl_gamebox_reset   = 0;
uint8_t cl_gamemox_limit   = 0;
uint8_t cl_ctf_display_on  = 0;
uint8_t cl_respawn_on      = 0;
uint8_t cl_tagger_nick_on  = 0;
uint8_t cl_old_ir_lvl      = 0;
uint8_t cl_ammo_reset_resp = 0;
uint8_t cl_game_mode_on    = 1;

uint8_t cl_ammo_box_clips  = 0;
uint8_t cl_medpack_size    = 0;
uint8_t cl_hit_led_timeout = 0;
uint8_t cl_sound_set       = 0;
uint8_t cl_overheat_limit  = 0;
uint8_t cl_damage_id       = 0;
uint8_t cl_clip_size       = 0;
uint8_t cl_clip_numbers    = 0;
uint8_t cl_fire_selector   = 0;
uint8_t cl_burst_size      = 0;
uint8_t cl_rpm             = 0;
uint8_t cl_reload_delay    = 0;
uint8_t cl_ir_power        = 0;
uint8_t cl_ir_range        = 0;
uint8_t cl_health_respawn  = 0;
uint8_t cl_respawn_delay   = 0;
uint8_t cl_armor           = 0;
uint8_t cl_hit_delay       = 0;
uint8_t cl_start_delay     = 0;
uint8_t cl_death_delay     = 0;
uint8_t cl_time_limit      = 0;
uint8_t cl_max_respawns    = 0;

void setup(){
    Serial.begin(9600); 
    pinMode(dhead,INPUT);
    pinMode(triggerpin,INPUT);
    pinMode(irtransmitpin,OUTPUT);
    digitalWrite(triggerpin, HIGH);
    fill_clone_data(0);
    Serial.println("Ready");
    
}

void loop(){
    triggers();
}

void triggers(){
    static uint8_t tr  = 0;
    static uint8_t ltr = 0;
        ltr = tr;
        tr = digitalRead(triggerpin);
        if ( tr != ltr && tr == LOW){
            sendcommand(0x8307);
            sendclonedata();
            sendcommand(0x8302);
        }
}

void sendcommand(uint16_t comm){
    uint8_t parity = 0;
    sendPulse(irtransmitpin,2);
    delayMicroseconds(lvl_0);
    for (int16_t i = 15; i >= 0; i--){
        if (((comm >> i) & B1) == 0){sendPulse(irtransmitpin,0);}
        else {
            sendPulse(irtransmitpin,1);
            parity = parity + 1;
        }
        delayMicroseconds(lvl_0);        
    }
    for (int16_t i = 7; i >= 0; i--){
        if (((0xE8 >> i) & B1) == 0){sendPulse(irtransmitpin,0);}
        else {sendPulse(irtransmitpin,1);}
        delayMicroseconds(lvl_0);        
    }
    parity += 4;
    if (cl_game_mode_on == 0){parity = 24 - parity;}
    if (parity%2 == 0){sendPulse(irtransmitpin,0);}
    else {sendPulse(irtransmitpin,1);}
    delayMicroseconds(lvl_0); 
}

void sendclonedata(){
    sendcommand(0x8701);
    for (int16_t j = 0; j <= CLONE_COUNT; j++){
        for (int16_t i = 7; i >= 0; i--){
            if (((clone_data[j] >> i) & B1) == 0){sendPulse(irtransmitpin,0);}
            else {sendPulse(irtransmitpin,1);}
            delayMicroseconds(lvl_0);        
        }
    }
}

void sendPulse(int16_t pin, uint8_t length){
  int16_t i = 0;
    while(i < irpulses<<length){
      i++;
      digitalWrite(pin, HIGH);
      delayMicroseconds(irt);
      digitalWrite(pin, LOW);
      delayMicroseconds(irt);
    }
}

void fill_clone_data(uint8_t game_mode){
    int16_t checksum = 0;
    clone_data[0]  =0x08;//marker
    clone_data[1]  =0xE8;//marker
    clone_data[2]  =0x21;//marker
    clone_data[3]  = cl_team_id;//команда 0-3
    clone_data[4]  =0x00;//reserved
    clone_data[7]  =0x03;
    clone_data[11] =0x00;//reserved
    clone_data[12] =0x00;//reserved
    clone_data[24] =0x00;//reserved
    clone_data[34] =0xFF;
    switch (game_mode){
        case 0:
            cl_muzzle_flash    = 1; //Вспышки из ствола 0- выкл, 1 - вкл
            cl_overheat_on     = 0; //Учитывать перегрев ствола 0 - выкл, 1 - вкл
            clone_data[22]     = (cl_muzzle_flash & B1) + ((cl_overheat_on & B1) << 1);// Настройки таггера
            cl_hit_led_enabled = 1; //Включить индикацию попадания  0- выкл, 1 - вкл
            cl_friendly_fire   = 0; //Дружественный огонь  0- выкл, 1 - вкл
            cl_unlimited_clips = 0; //Безлимитные обоймы  0- выкл, 1 - вкл
            cl_zombie_mod      = 0; //Режим игры зомби  0- выкл, 1 - вкл
            cl_medpack_enabled = 1; //Медпаки включены  0- выкл, 1 - вкл
            cl_gamebox_reset   = 1; //Сброс использованных устройств при возрождении  0- выкл, 1 - вкл
            cl_gamemox_limit   = 0; //Одно устройство можно использовать только один раз  0- выкл, 1 - вкл
            clone_data[27]     = ((cl_hit_led_enabled & B1) << 1) + ((cl_friendly_fire & B1) << 2) + ((cl_unlimited_clips &B1) << 3) + ((cl_zombie_mod &B1) << 4) + ((cl_medpack_enabled & B1) << 5) + ((cl_gamebox_reset & B1) << 6) + ((cl_gamemox_limit & B1) << 7);
            cl_ctf_display_on  = 0; //Включить экран на флаге  0- выкл, 1 - вкл
            cl_respawn_on      = 1; //Включить возрождения  0- выкл, 1 - вкл
            cl_tagger_nick_on  = 0; //Включить ники таггеров  0- выкл, 1 - вкл
            cl_old_ir_lvl      = 0; //TODO узнать что такое old IR level field   0- выкл, 1 - вкл
            cl_ammo_reset_resp = 1; //Восстанавливать патроны при возрождении  0- выкл, 1 - вкл
            cl_game_mode_on    = 1; //TODO не понятно что имелось ввиду Game mode on 0- выкл, 1 - вкл
            clone_data[28]     = ((cl_ctf_display_on & B1) << 2) + ((cl_respawn_on & B1) << 3) + ((cl_tagger_nick_on & B1) << 4) + ((cl_old_ir_lvl & B1) << 5) + ((cl_ammo_reset_resp & B1) << 6) + ((cl_game_mode_on & B1) << 7);
            clone_data[5]   = 5;    //Количество обойм из ящика патронов 1-200
            clone_data[6]   = 30;   //Количество здоровья из медпака 1-200
            clone_data[8]   = 3;    //Тайм-аут свечения светодиода попадания в секундах 1 -240
            clone_data[9]   = 0;    //Набор звуков 0 - mil-sim, 1 - sci-fi, 2 - silence
            clone_data[10]  = 60;   //Лимит перегрева от непрерывной стрельбы в патронах
            clone_data[13]  = 1;    //Урон от выстрела 0x0 - 0xF
            clone_data[14]  = 15;   //Размер обоймы 0xFF - безлимит 1-240
            clone_data[15]  = 4;    //Количество обойм 0xCA - безлимит 1-200
            clone_data[16]  = 1;    //Режим огня 0 - одиночные, 1 - очереди с отсечением, 2 - автомат 
            clone_data[17]  = 3;    //Количество выстрелов в очереди 3,4,5,6 выстрелов
            clone_data[18]  = 0;    //Скорострельность выстрелов в минуту 0x0 - 0xb  
            clone_data[19]  = 3;    //Задержка перезарядки обоймы в секундах
            clone_data[20]  = 0;    //Режим работы ИК передатчика 0 - внутри помещения, 1 - снаружи
            clone_data[21]  = 0;    //Дальность передачи ик передатчика 0x01 - 0x12
            clone_data[23]  = 0x24; //Здоровье после возрождения 0x01(1) - 0x48(999)
            clone_data[25]  = 9;    //Задержка перед возрождением в десятках секунд 
            clone_data[26]  = 5;    //Броня при каждом попадании снимается единица брони, также урон здоровью режется вдвое 
            clone_data[29]  = 0x05; //Задержка при попадании??? в секундах 0x00(0) - 0x17(20)
            clone_data[30]  = 30;   //Задержка старта игры в секундах
            clone_data[31]  = 10;   //Задержка при смерти в секундах
            clone_data[32]  = 240;  //Время игры в минутах 5 - 240
            clone_data[33]  = 10;   // Максимальное количество возрождений
            break;
        default:
            break;
    }
    checksum = 0;
    for (int16_t i = 0; i < CLONE_COUNT - 1; i++){checksum += clone_data[i];}
    clone_data[35] = checksum%0x100;
}
