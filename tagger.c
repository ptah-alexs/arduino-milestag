#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <SPI.h>
#include "U8glib.h"
U8GLIB_SSD1306_128X64 display(10, 9, 5);

static const unsigned char PROGMEM heart1[] ={
B00100100,
B01111110,
B11111111,
B01111110,
B00111100,
B00011000
};

static const unsigned char PROGMEM shield1[] ={
B11111111,
B11111111,
B11111111,
B01111110,
B00111100,
B00011000,
};

const char msg_0[] PROGMEM  = "\xbf\xd5\xe0\xd5\xd7\xd0\xe0\xef\xd4\xda\xd0";//"Reloading";
const char msg_1[] PROGMEM  = "\xbd\xd5\xe2 \xdf\xd0\xe2\xe0\xde\xdd\xde\xd2";//"No ammo";
const char msg_2[] PROGMEM  = "\xc1\xe2\xde\xdf";//"STOP";
const char msg_3[] PROGMEM  = "\xbf\xd0\xe3\xd7\xd0";//"Pause";
const char msg_4[] PROGMEM  = "\xbf\xde\xdb\xdd\xde\xd5 \xd7\xd4\xde\xe0\xde\xd2\xec\xd5";//"Full health";
const char msg_5[] PROGMEM  = "\xbf\xde\xdb\xdd\xd0\xef \xd1\xe0\xde\xdd\xef";//"Full armor";
const char msg_6[] PROGMEM  = "\xc0\xd0\xdd\xd5\xdd";//"You're hurt";
const char msg_7[] PROGMEM  = "\xc3\xd1\xd8\xe2!";//"You're dead";
const char msg_8[] PROGMEM  = "\xba\xe0\xde\xd2\xde\xe2\xd5\xe7\xd5\xdd\xd8\xd5";//"Bleeding";
const char msg_9[] PROGMEM  = "\xb4\xde\xdf. \xd7\xd4\xde\xe0\xde\xd2\xec\xd5";//"Health added";
const char msg_10[] PROGMEM = "\xb4\xde\xdf. \xdf\xd0\xe2\xe0\xde\xdd\xeb";//"Ammo added";
const char msg_11[] PROGMEM = "\xb2\xde\xd7\xe0\xde\xd6\xd4\xf1\xdd!";//"You're spawn";
const char msg_12[] PROGMEM = "\xc1\xd1\xe0\xe1 \xe3\xe1\xe2\xe0\xde\xd9\xe1\xe2\xd2\xd0";//"Reset data";
const char msg_13[] PROGMEM = "\xb8\xd3\xe0\xd0 \xdd\xd0\xe7\xd8\xdd\xd0\xd5\xe2\xe1\xef";//"Starting game";
const char msg_14[] PROGMEM = "\xbd\xde\xd2\xd0\xef \xd8\xd3\xe0\xd0";//"New game";
const char msg_15[] PROGMEM = "\xb8\xd3\xe0\xd0 \xdd\xd0\xe7\xd0\xe2\xd0";//"Game started";
const char msg_16[] PROGMEM = "\xbf\xde\xdb\xdd\xeb\xd5 \xde\xd1\xde\xd9\xdc\xeb";//"Full ammo";
const char msg_17[] PROGMEM = "\xb8\xd3\xe0\xd0 \xd7\xd0\xda\xde\xdd\xe7\xd5\xdd\xd0";//"Game over";
const char msg_18[] PROGMEM = "\xc1\xe2\xd0\xe2\xd8\xe1\xe2\xd8\xda\xd0 \xe7\xd8\xe1\xe2\xd0";//"Stat is clean";
const char msg_19[] PROGMEM = "\xc2\xd5\xe1\xe2 \xd4\xd0\xe2\xe7\xd8\xda\xde\xd2";//"Test sensors";
const char msg_20[] PROGMEM = "\xbe\xd1\xd5\xd7\xd4\xd2\xd8\xd6\xd5\xdd";//"You stunned";
const char msg_21[] PROGMEM = "\xb2\xd7\xef\xe2\xeb \xdf\xd0\xe2\xe0\xde\xdd\xeb";//"Got ammo box";
const char msg_22[] PROGMEM = "\xb2\xd7\xef\xe2\xde \xd7\xd4\xde\xe0\xde\xd2\xec\xd5";//"Got HP box";
const char msg_23[] PROGMEM = "\xc4\xdb\xd0\xd3 \xd7\xd0\xe5\xd2\xd0\xe7\xd5\xdd";//"Flag captured";
const char msg_24[] PROGMEM = "\xbf\xe0\xd8\xf1\xdc \xd4\xd0\xdd\xdd\xeb\xe5";//"Getting data";
const char msg_25[] PROGMEM = "\xbf\xd5\xe0\xd5\xd4\xd0\xe7\xd0 \xd4\xd0\xdd\xdd\xeb\xe5";//"Sending data";
const char msg_26[] PROGMEM = "\xbf\xd5\xe0\xd5\xd4\xd0\xe7\xd0 \xd2\xe1\xf1";//"Data sent";
const char msg_27[] PROGMEM = "\xb4\xd0\xe2\xe7\xd8\xda \xde\xe2\xda\xdb\xee\xe7\xd5\xdd";//"Sensors off";
const char* const messages[] PROGMEM = {
msg_0,msg_1,msg_2,msg_3,msg_4,msg_5,msg_6,msg_7,msg_8,msg_9,msg_10,msg_11,msg_12,msg_13,msg_14,msg_15,
msg_16,msg_17,msg_18,msg_19,msg_20,msg_21,msg_22,msg_23,msg_24,msg_25,msg_26,msg_27
};

const char team_0[] PROGMEM = "Red ";
const char team_1[] PROGMEM = "Blue ";
const char team_2[] PROGMEM = "Yellow ";
const char team_3[] PROGMEM = "Green ";
const char* const teams[] PROGMEM = {
team_0,team_1,team_2,team_3
};

const char st_0[] PROGMEM = "S";
const char st_1[] PROGMEM = "B";
const char st_2[] PROGMEM = "A";
const char* const states[] PROGMEM = {
st_0,st_1,st_2
};

const byte val_damage[16] PROGMEM = {1, 2, 4, 5, 7, 10, 15, 17, 20, 25, 30, 35, 40, 50, 75, 100};
const int val_hit_timeout[24] PROGMEM = {0, 5, 10, 15, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 320, 340, 360, 380, 400};
const int val_rpm[12] PROGMEM = {250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800};
const int val_health[72] PROGMEM = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, \
                                    80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195,\
                                    200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 999};

//global states
boolean tg_started        = true;
boolean tg_paused         = false;
boolean tg_is_overheat    = false;
boolean tg_is_inf_ammo    = false;

//timers
unsigned long start_timer = 0;
unsigned long fire_timer  = 0;
unsigned long gl_interval = 0;
unsigned long timer       = 0;

const byte MAX_COUNT      = 39;
const int lnk_lvl_0       = 600;
const int lnk_lvl_1       = lnk_lvl_0<<1;
const byte lnk_ir_freq    = 36;
const int lnk_timeout     = 3200;
const byte lnk_ir_speriod = (int) ((500/lnk_ir_freq) - 4);
const byte lnk_ir_pulses  = (int) (lnk_lvl_0 / (1000/lnk_ir_freq));

// list of pin
int ctrl_ir_transmit      = 2;
int sens_head             = 3;
int ctrl_fire             = 4;
int ctrl_fire_selector1   = 7;
int ctrl_fire_selector2   = 8;
int ind_hit               = 14;
int ind_muz_flash         = 15;
//int ind_busy              = 16;
int ctrl_reload           = 17;
/*Последняя ревизия
0  - последовательный порт. mp3-модуль
1  - последовательный порт. mp3-модуль
2  - ик-передатчик.
3  - ик-приемник. (ШИМ)
4  - спусковой крючок огня.
5  - Display RES. (ШИМ)
6  - пока не используется. (ШИМ)
7  - переключения режима огня 1.
8  - переключения режима огня 2.
9  - Display DC. (ШИМ)
10 - пока не используется. SPI.
11 - Display SDA. SPI.
12 - пока не используется. SPI.
13 - Display SCL. SPI.
14 - индикатор попадания. Аналоговый вход 0
15 - индикатор огня на стволе. Аналоговый вход 1
16 - пока не используется. Аналоговый вход 2
17 - кнопка перезарядки. Аналоговый вход 3
18 - пока не используется. Аналоговый вход 4. I2C SDA
19 - пока не используется. Аналоговый вход 5. I2C SCL
 */
/*
0  - последовательный порт. mp3-модуль
1  - последовательный порт. mp3-модуль
2  - ик-передатчик.
3  - ик-приемник. (ШИМ)
4  - спусковой крючок огня.
5  - Display D/C. (ШИМ)
6  - Display CSE. (ШИМ)
7  - переключения режима огня 1.
8  - переключения режима огня 2.
9  - Display RST. (ШИМ)
10 - SPI.
11 - Display DIN. SPI.
12 - SPI.
13 - Display SCK. SPI.
14 - индикатор попадания. Аналоговый вход 0
15 - индикатор огня на стволе. Аналоговый вход 1
16 - индикатор активности (перезарядка, там еще какая-нибудь фигня). Аналоговый вход 2
17 - кнопка перезарядки. Аналоговый вход 3
18 - пока не используется. Аналоговый вход 4
19 - Античит. Аналоговый вход 5
 */
/*
Распиновка nokia5110
VCC - 3v3 (14)
GND - "-"
CSE - 6
RST - 9
D/C - 5
DIN - 11
SCK - 13
LED - 3v3
Инициализация:
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
Adafruit_PCD8544 display = Adafruit_PCD8544(5, 6, 9);
 */
/*
Распиновка oled 0.96'
GND - "-"
VCC - 3v3 (14)
SCL - 13
SDA - 11
DC  - 9
RES - 5
Инициализация:
#include <SPI.h>
#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(10, 9, 5);
 */
/* распиновка с i2c
0  - последовательный порт. mp3-модуль
1  - последовательный порт. mp3-модуль
2  - ик-передатчик.
3  - ик-приемник. (ШИМ)
4  - спусковой крючок огня.
5  - кнопка перезарядки. (ШИМ)
6  - индикатор попадания. (ШИМ)
7  - переключения режима огня 1.
8  - переключения режима огня 2.
9  - индикатор огня на стволе. (ШИМ)
10 - SPI.
11 - SPI.
12 - SPI.
13 - SPI.
14 - индикатор активности (перезарядка, там еще какая-нибудь фигня). Аналоговый вход 0
15 - пока не используется. Аналоговый вход 1
16 - пока не используется. Аналоговый вход 2
17 - Античит. Аналоговый вход 3
18 - i2c. Аналоговый вход 4
19 - i2c. Аналоговый вход 5
 */
// other data
byte data_recieved[MAX_COUNT];
byte tg_stat_02[214];
byte tg_stat_03[214];
int hit_interval          = 0;
int ctrl_fire_delay       = 0;
int ctrl_index            = 0;
int tg_player_data        = 0;
int tg_max_life           = 0;
int tg_cur_life           = 0;
int tg_max_ammo           = 0;
int tg_cur_ammo           = 0;
int tg_medpack_mask       = 0;
int tg_ammobox_mask       = 0;
int tg_flag_mask          = 0;
int tg_overheat_counter   = 0;
int tg_overheat_int       = 0;
int tg_ind_shadow         = 0;

byte tg_overheat_cnt      = 0;
byte cheat_count          = 0;
word tg_wrk_int           = 0;
word tg_zombie_cnt        = 0;
byte tg_ani_cnt           = 0;
word tg_hit_led_cnt       = 0;
word tg_msg_cnt           = 0;
byte msg                  = 0;

byte tg_cur_clip          = 0;
byte tg_rd_trig           = 0;
byte tg_snd_shot          = 0;
byte tg_snd_reload        = 0;

byte tg_flag_count        = 0;
byte tg_respawn_count     = 0;
byte tg_armor             = 0;
byte tg_burst_counter     = 0;
byte tg_tagged_out        = 0;
byte tg_min_of_game       = 0;
byte tg_sec_of_game       = 0;
int  tg_shot_count        = 0;
int  tg_hit_count         = 0;

byte cl_team_id           = 0;
byte tg_player_id         = 0;

byte cl_muzzle_flash      = 0;
byte cl_overheat_on       = 0;
byte cl_hit_led_enabled   = 0;
byte cl_friendly_fire     = 0;
byte cl_unlimited_clips   = 0;
byte cl_zombie_mod        = 0;
byte cl_medpack_enabled   = 0;
byte cl_gamebox_reset     = 0;
byte cl_gamemox_limit     = 0;
byte cl_ctf_display_on    = 0;//Not implemented in hardware of tagger
byte cl_respawn_on        = 0;
byte cl_tagger_nick_on    = 0;//Not implemented in hardware
byte cl_old_ir_lvl        = 0;//Not implemented in hardware
byte cl_ammo_reset_resp   = 0;
byte cl_game_mode_on      = 1;
byte cl_ammo_box_clips    = 0;
byte cl_medpack_size      = 0;
byte cl_hit_led_timeout   = 0;
byte cl_sound_set         = 0;
byte cl_overheat_limit    = 0;
byte cl_damage_id         = 0;
byte cl_clip_size         = 0;
byte cl_clip_numbers      = 0;
byte cl_fire_selector     = 0;
byte cl_burst_size        = 0;
byte cl_rpm               = 0;
byte cl_reload_delay      = 0;
byte cl_ir_power          = 0;//Not implemented in hardware
byte cl_ir_range          = 0;//Not implemented in hardware
byte cl_health_respawn    = 0;
byte cl_respawn_delay     = 0;
byte cl_armor             = 0;
byte cl_hit_delay         = 0;
byte cl_start_delay       = 0;
byte cl_death_delay       = 0;
byte cl_time_limit        = 0;
byte cl_max_respawns      = 0;

void setup(){
    Serial.begin(9600);
    mp3_set_serial (Serial);
    display.begin();
    if ( display.getMode() == U8G_MODE_R3G3B2 ) {
        display.setColorIndex(255);     // white
    }
    else if ( display.getMode() == U8G_MODE_GRAY2BIT ) {
        display.setColorIndex(3);         // max intensity
    }
    else if ( display.getMode() == U8G_MODE_BW ) {
        display.setColorIndex(1);         // pixel on
    }
    else if ( display.getMode() == U8G_MODE_HICOLOR ) {
        display.setHiColorByRGB(255,255,255);
    }
    pinMode(sens_head,INPUT);
    pinMode(ctrl_fire,INPUT);
    pinMode(ctrl_reload,INPUT);
    pinMode(ctrl_fire_selector1,INPUT);
    pinMode(ctrl_fire_selector2,INPUT);
    pinMode(ind_hit,OUTPUT);
    pinMode(ind_muz_flash,OUTPUT);
    pinMode(ctrl_ir_transmit,OUTPUT);
    //pinMode(ind_busy,OUTPUT);
    digitalWrite(ctrl_fire, HIGH);
    digitalWrite(ctrl_reload, HIGH);
    digitalWrite(ctrl_fire_selector1, HIGH);
    digitalWrite(ctrl_fire_selector2, HIGH);
    set_default();
    set_player_id();
    clean_stat();
    mp3_set_volume (30);
    delay(100);
    mp3_play(1);
    new_game();
    
}

void loop(){
    receive_command();
    triggers();
    timers();
}

void set_default(){
    tg_player_id   = 20;//ID игрока
    cl_team_id   = 2;//ID команды
    /////////////////////////////////
    cl_muzzle_flash    = 1;    //muzzle flash 0- выкл, 1 - вкл
    cl_overheat_on     = 0;    //Учитывать перегрев ствола 0 - выкл, 1 - вкл
    cl_hit_led_enabled = 1;    //Включить индикацию попадания  0- выкл, 1 - вкл
    cl_friendly_fire   = 1;    //Дружественный огонь  0- выкл, 1 - вкл
    cl_unlimited_clips = 0;    //Безлимитные обоймы  0- выкл, 1 - вкл
    cl_zombie_mod      = 0;    //Режим игры зомби  0- выкл, 1 - вкл
    cl_medpack_enabled = 1;    //Медпаки включены  0- выкл, 1 - вкл
    cl_gamebox_reset   = 1;    //Сброс использованных устройств при возрождении  0- выкл, 1 - вкл
    cl_gamemox_limit   = 0;    //Одно устройство можно использовать только один раз  0- выкл, 1 - вкл
    cl_ctf_display_on  = 0;    //Включить экран на флаге  0- выкл, 1 - вкл
    cl_respawn_on      = 1;    //Включить возрождения  0- выкл, 1 - вкл
    cl_tagger_nick_on  = 0;    //Включить ники таггеров  0- выкл, 1 - вкл
    cl_old_ir_lvl      = 0;    //old IR level field   0- выкл, 1 - вкл
    cl_ammo_reset_resp = 1;    //Восстанавливать патроны при возрождении  0- выкл, 1 - вкл
    cl_game_mode_on    = 1;    //Game mode on 0- выкл, 1 - вкл  !!! возможно настройка бита четности.
    cl_ammo_box_clips  = 5;    //Количество обойм из ящика патронов 1-200
    cl_medpack_size    = 30;   //Количество здоровья из медпака 1-200
    cl_hit_led_timeout = 3;    //Тайм-аут свечения светодиода попадания в секундах 1 -240
    cl_sound_set       = 1;    //Набор звуков 0 - mil-sim, 1 - sci-fi, 2 - silence
    cl_overheat_limit  = 60;   //Лимит перегрева от непрерывной стрельбы в патронах
    cl_damage_id       = 1;    //Урон от выстрела 0x0 - 0xF
    cl_clip_size       = 30;   //Размер обоймы 0xFF - безлимит 1-240
    cl_clip_numbers    = 2;    //Количество обойм 0xCA - безлимит 1-200
    cl_fire_selector   = 1;    //Режим огня 0 - одиночные, 1 - очереди с отсечением, 2 - автомат 
    cl_burst_size      = 3;    //Количество выстрелов в очереди 3,4,5,6 выстрелов
    cl_rpm             = 0;    //Скорострельность выстрелов в минуту 0x0 - 0xb  
    cl_reload_delay    = 3;    //Задержка перезарядки обоймы в секундах
    cl_ir_power        = 0;    //Режим работы ИК передатчика 0 - внутри помещения, 1 - снаружи
    cl_ir_range        = 0;    //Дальность передачи ик передатчика 0x01 - 0x12
    cl_health_respawn  = 0x10; //Здоровье после возрождения 0x01(1) - 0x48(999)
    cl_respawn_delay   = 2;    //Задержка перед возрождением в десятках секунд //не по стандарту - 0xFF отключена
    cl_armor           = 5;    //Броня при каждом попадании снимается единица брони, также урон здоровью режется вдвое 
    cl_hit_delay       = 0x06; //Задержка при попадании в секундах 0x00(0) - 0x17(20)
    cl_start_delay     = 30;   //Задержка старта игры в секундах
    cl_death_delay     = 10;   //Задержка при смерти в секундах
    cl_time_limit      = 240;  //Время игры в минутах 5 - 240
    cl_max_respawns    = 10;   //Максимальное количество возрождений
    /////////////////////////////////
    tg_max_ammo  = cl_clip_numbers * cl_clip_size;
    tg_cur_ammo  = tg_max_ammo;
    tg_respawn_count = 0;
    tg_flag_count = 0;
    tg_max_life = code2health(cl_health_respawn);
    tg_cur_life  = tg_max_life;
    tg_armor = cl_armor;
    tg_medpack_mask = 0;
    tg_ammobox_mask = 0;
    tg_flag_mask = 0;
    tg_rd_trig = 0;
    tg_tagged_out = 0;
    tg_shot_count = 0;
    tg_hit_count = 0;
    ctrl_fire_delay = (int) (60000/code2rpm(cl_rpm));
    gl_interval = cl_time_limit * 60000;
    if (cl_zombie_mod == 1){
        set_zombie(cl_team_id);
    }
    if (cl_clip_numbers == 0xCA||cl_unlimited_clips == 1||cl_clip_size == 0xFF){tg_is_inf_ammo = true;} else {tg_is_inf_ammo = false;};
    tg_snd_shot = 25 + cl_sound_set;
    tg_snd_reload = 30 + cl_sound_set;
    hit_interval = cl_hit_led_timeout * 20;
    tg_overheat_int = 2000 / cl_overheat_limit;
    show_info_led();
}

void receive_command(){
    static int recieved_pulse   = 0;
    static byte recieved_bit    = 0;
    static byte parity_bit      = 0;
    static byte temp_parity_bit = 0; 
    static byte size_of_command = 0;
    static byte parity          = 0;
    recieved_pulse = pulseIn(sens_head, LOW,lnk_timeout);
    if (recieved_pulse > 1800) {
        ctrl_index  = 0;
        parity = 0;
        //digitalWrite(ind_busy,HIGH);
        while (recieved_pulse != 0){
            recieved_pulse = pulseIn(sens_head, LOW,lnk_timeout);
            if ((recieved_pulse > lnk_lvl_0 - 200) && (recieved_pulse < lnk_lvl_0 + 200)) {recieved_bit = 0;}
            else if ((recieved_pulse > lnk_lvl_1 - 200) && (recieved_pulse < lnk_lvl_1 + 200)) {recieved_bit = 1;}
            else if (recieved_pulse == 0){break;}
            data_recieved[ctrl_index >> 3] += (recieved_bit<< (7 - ctrl_index%8));
            if (ctrl_index == 0){
                if (recieved_bit == 0){
                    parity_bit = 16;
                    size_of_command = 2;
                }
                else {
                    parity_bit = 24;
                    size_of_command = 3;
                }
            } else if (ctrl_index == parity_bit){
                parity_bit = 0;
                temp_parity_bit = recieved_bit;
                data_recieved[ctrl_index >> 3] = 0;
                ctrl_index--;
            }
            ctrl_index++;
        }
        if (ctrl_index > 15){
            for (int j = 0;j < size_of_command; j++){
                for (int i=0;i < 8; i++){
                    if ((data_recieved[j]>>i)&B1 == 1){parity++;}
                }
            }
            //digitalWrite(ind_busy,LOW);
            if (cl_game_mode_on == 0){parity = (size_of_command << 3) - parity;}
            if ((parity%2)==temp_parity_bit){parse_protocol();}
            for (int i = 0; i < (ctrl_index >>3)+1; i++){data_recieved[i]=0;}
        }
    }
}

void show_message(byte msg1,word duration){
    if (duration != 0xFFFF){
        tg_msg_cnt = duration * 20;
    } else {tg_msg_cnt = 0xFFFF;};
    msg = msg1;
}

void reload(){
    if (tg_cur_ammo > 0){
        //digitalWrite(ind_busy,HIGH);
        tg_rd_trig = 3;
        tg_cur_clip = 0;
        tg_wrk_int = cl_reload_delay * 20;
        show_message(0,cl_reload_delay);
        mp3_play(4);
    } else {noammo();};
}

void triggers(){
    static byte trig_fire_state = 0;
    static byte trig_fire_state_old = 0;
    static byte trig_reload_state = 0;
    static byte trig_reload_state_old = 0;
    trig_fire_state_old = trig_fire_state;
    trig_fire_state = digitalRead(ctrl_fire);
    trig_reload_state_old = trig_reload_state;
    trig_reload_state = digitalRead(ctrl_reload);
    if (tg_started && !tg_paused){
        if (trig_fire_state == LOW){
            switch (cl_fire_selector){
                case 0:
                    if (trig_fire_state != trig_fire_state_old){shoot();}
                    break;
                case 1:
                    if (trig_fire_state != trig_fire_state_old){tg_burst_counter = 0;}
                    if (((millis()-fire_timer) >= ctrl_fire_delay) && (tg_burst_counter < cl_burst_size)){
                        fire_timer = millis();
                        tg_burst_counter++;
                        shoot();
                    }
                    break;
                case 2:
                    if ((millis()-fire_timer) >= ctrl_fire_delay){
                        fire_timer = millis();
                        shoot();
                    }
                    break;
                default:
                    break;
            }
        }
        if (trig_reload_state != trig_reload_state_old && trig_reload_state == LOW){
            reload();
            show_info_led();
        }
        if (digitalRead(ctrl_fire_selector1) == HIGH && digitalRead(ctrl_fire_selector2) == HIGH) {cl_fire_selector = 0;}
        else if (digitalRead(ctrl_fire_selector1) == LOW) {cl_fire_selector = 1;}
        else if (digitalRead(ctrl_fire_selector2) == LOW) {cl_fire_selector = 2;}
    } else {//если игра не активка то одновременное нажатие на огонь и перезарядку отправляет статистику
        if (trig_fire_state == LOW && trig_reload_state == LOW){send_stat();}
    }
}

void timers(){
    if (millis() - timer >= 50){
        timer = millis();
        if (cl_overheat_on == 1) {
            if (tg_overheat_counter > 1000){
                tg_is_overheat = true;
                //digitalWrite(ind_busy,HIGH);
            }
            if (tg_overheat_counter > 0){tg_overheat_counter-=10;}
            else {
                //digitalWrite(ind_busy,LOW);
                tg_overheat_counter =0;
                tg_is_overheat = false;
            }
        }
        if (tg_ind_shadow > 0){tg_ind_shadow--;}
        else if (tg_ind_shadow == 0) {
            display.firstPage();  
            do {
                display.setColorIndex(0);
                display.drawBox(0,0,128,64);
            } while( display.nextPage() );
            tg_ind_shadow = -1;
        }
        if (cl_zombie_mod == 1 && cl_team_id == 0 && tg_started){
            tg_zombie_cnt++;
            if (tg_zombie_cnt >= 600){
                mp3_play(5);
                digitalWrite(ind_hit,HIGH);
                tg_hit_led_cnt = hit_interval;
                tg_zombie_cnt = 0;
            }
        }
        if (tg_ani_cnt >= 20){
            show_info();
            if (start_timer != 0){
                if ((timer > start_timer) && (timer - start_timer >= gl_interval)) {
                    end_game();
                }
            }
            tg_ani_cnt = 0;
        } else {tg_ani_cnt++;}
        if (tg_hit_led_cnt > 0){
            tg_hit_led_cnt--;
            if (tg_hit_led_cnt == 0){
                digitalWrite(ind_hit,LOW);
            }
        }
        if (tg_wrk_int > 0){
            tg_wrk_int--;
            if (tg_wrk_int == 0){
                tg_paused = false;
                //digitalWrite(ind_busy,LOW);
                switch (tg_rd_trig){
                    case 1://Death delay
                        dead();
                        break;
                    case 2://Respawn delay
                        respawn();
                        break;
                    case 3://Reload delay
                        mp3_play(tg_snd_reload);
                        show_info_led();
                      break;
                    case 4://Starting game
                        show_message(15,3);
                        show_info_led();
                        break;
                }
                tg_rd_trig = 0;
            }
        }
        if (tg_msg_cnt > 0){
            if (tg_msg_cnt != 0xFFFF){tg_msg_cnt--;}
            if (tg_msg_cnt == 0){
                msg = 0;
                show_info_led();
            }
        }
        if (digitalRead(sens_head) == LOW){
            cheat_count++;
        } else if (cheat_count >= 20){
            cheat_count = 0;
            tg_paused = false;
            tg_ind_shadow = 200;
            show_message(27,0);
            digitalWrite(ind_hit,LOW);
        };
        if (cheat_count >= 20){
            cheat_count = 25;
            tg_ind_shadow = 0x6FFF;
            tg_paused = true;
            digitalWrite(ind_hit,HIGH);
            show_message(27,0xFFFF);
        }
    }
}

void shoot(){
    byte parity = 0;
    if (tg_cur_ammo > 0){
        if (tg_rd_trig != 3 && !tg_is_overheat){
            if (cl_muzzle_flash == 1) {digitalWrite(ind_muz_flash,HIGH);}
            mp3_play(tg_snd_shot);
            sendPulse(ctrl_ir_transmit,2);
            delayMicroseconds(lnk_lvl_0);
            for (int i = 15; i >= 0; i--){
                if (((tg_player_data >> i) & B1) == 0){sendPulse(ctrl_ir_transmit,0);}
                else {
                    sendPulse(ctrl_ir_transmit,1);
                    parity++;
                }
                delayMicroseconds(lnk_lvl_0);        
            }
            if (cl_game_mode_on == 0){parity = 16 - parity;}
            if ((parity%2) == 0){sendPulse(ctrl_ir_transmit,0);}
            else {sendPulse(ctrl_ir_transmit,1);}
            delayMicroseconds(lnk_lvl_0);
            if (cl_clip_size != 0xFF){
                tg_cur_ammo--;
                tg_cur_clip++;
                if (tg_cur_clip == cl_clip_size){
                    mp3_stop();
                    delay(10);
                    reload();
                    if ((cl_unlimited_clips == 1) || (cl_clip_numbers == 0xCA)){tg_cur_ammo += cl_clip_size;}
                }
            }
            digitalWrite(ind_muz_flash,LOW);
            tg_shot_count++;
            if (cl_overheat_on == 1) {tg_overheat_counter += tg_overheat_int;};
            show_info_led();
        }
    } else {
        noammo();
    }
}

void show_info_led(){
    tg_ind_shadow = 200;
    show_info();
};

void sendPulse(int pin, byte length){
  int i = 0;
    while(i < lnk_ir_pulses<<length){
      i++;
      digitalWrite(pin, HIGH);
      delayMicroseconds(lnk_ir_speriod);
      digitalWrite(pin, LOW);
      delayMicroseconds(lnk_ir_speriod);
    }
}

void set_player_id(){
    tg_player_data = (tg_player_id << 8) + (cl_team_id << 6) + (cl_damage_id << 2);
}

void parse_protocol(){
    if (data_recieved[0] < 128){
        if (tg_started && !tg_paused){hit(((data_recieved[1] >> 2) & 0xF), (data_recieved[1] >> 6), data_recieved[0]);}
    }
    else {
        if (data_recieved[2] == 0xE8){// if 3rd byte equal constant 0xE8
            switch (data_recieved[0]) {
                case 0x80 :
                    add_health(data_recieved[1]);
                    break;
                case 0x81 :
                    add_ammopack(data_recieved[1]);
                    break;
                case 0x82 :
                    break;//reserved
                case 0x83 :
                    switch (data_recieved[1]){
                        case 0x00 :
                            instakill();
                            break;
                        case 0x01 :
                            set_pause();
                            break;
                        case 0x02 :
                            start_game();
                            break;
                        case 0x03 :
                            reset_default();
                            break;
                        case 0x04 :
                            respawn();
                            break;
                        case 0x05 :
                            new_game();
                        case 0x06 :
                            restore_ammo();
                            break;
                        case 0x07 :
                            end_game();
                            break;
                        case 0x08 :
                            reset_timer();
                            break;
                        case 0x09 :
                            break; //reserved
                        case 0x0A :
                            player_init();
                            break;
                        case 0x0B :
                            explode_player();
                            break;
                        case 0x0C :
                            new_game_cont();
                            break;
                        case 0x0D :
                            restore_health();
                            break;
                        case 0x0E :
                            break; //reserved
                        case 0x0F :
                            restore_armor();
                            break;
                        case 0x10 :
                            break; //reserved
                        case 0x11 :
                            break; //reserved
                        case 0x12 :
                            break; //reserved
                        case 0x13 :
                            break; //reserved
                        case 0x14 :
                            reset_score();
                            break;
                        case 0x15 :
                            test_detectors();
                            break;
                        case 0x16 :
                            stun_player();
                            break;
                        case 0x17 :
                            ungun_player();
                            break;
                        default :
                            break;
                    }
                    break;
                case 0x84 :
                    break;//reserved
                case 0x85 :
                    break;//reserved
                case 0x86 :
                    break;//reserved
                case 0x87 :
                    switch (data_recieved[1]){
                        case 0x00:
                            break;//reserved
                        case 0x01:
                            getclonedata();
                            break;
                        case 0x02:
                            break;//reserved
                        case 0x03:
                            break;//get score data part 1 (not processing in tagger)
                        case 0x04:
                            break;//get score data part 2 (not processing in tagger)
                        case 0x05:
                            break;//get score data part 3 (not processing in tagger)
                        default :
                            break;
                    }
                    break;
                case 0x88 :
                    break;//reserved
                case 0x89 :
                    break;//reserved
                case 0x8A :
                    ammo_box(data_recieved[1]);
                    break;
                case 0x8B :
                    medpack(data_recieved[1]);
                    break;
                case 0x8C :
                    ctf(data_recieved[1]);
                    break;
                default :
                    break;
            }
        }
    }
}

void hit(byte damage, byte team, byte player){
    //hit to player. damage code - hp:
    byte dmg = 0;
    dmg = code2damage(damage);
    if (cl_friendly_fire == 0 && team == cl_team_id){}
    else {
        if (tg_armor > 0){
            tg_armor--;
            dmg = dmg >> 1;
        }
        tg_cur_life -= dmg;
        show_message(6,code2hit_timeout(cl_hit_delay)/20);
        show_info_led();
        tg_hit_count++;
        tg_stat_02[player]++;
        if (team == cl_team_id){tg_stat_03[player]++;}
        if (tg_armor + 1 > 0) {mp3_play(6);}
        else {mp3_play(7);}
        if (cl_hit_led_enabled == 1){
            digitalWrite(ind_hit,HIGH);
            tg_hit_led_cnt = hit_interval;
        }
        if (tg_cur_life <= 0){
            tg_rd_trig = 1;
            tg_cur_life = 0;
            tg_paused = true;
            //digitalWrite(ind_busy,HIGH);
            tg_wrk_int = cl_death_delay * 20;
            show_message(8,tg_wrk_int);
            mp3_play(8);
        } else {
            tg_paused = true;
            //digitalWrite(ind_busy,HIGH);
            tg_wrk_int = code2hit_timeout(cl_hit_delay);
        }
    }
}

void dead(){
    //Dead
    if (tg_started){
        tg_started = false;
        tg_cur_life = 0;
        mp3_play(13);
        tg_tagged_out++;
        if (cl_hit_led_enabled == 1){
            digitalWrite(ind_hit,HIGH);
            tg_hit_led_cnt = hit_interval;
        }
        show_message(7,0xFFFF);
        show_info_led();
        if (cl_zombie_mod == 1){
            //set_zombie(cl_team_id);
            switch (cl_team_id){
                case 1:
                    cl_team_id = 0;
                    set_zombie(cl_team_id);
                    break;
                case 0:
                    //set_zombie(cl_team_id);
                    cl_respawn_delay = 0xFF;
                    tg_respawn_count = cl_max_respawns;
                    break;
            }
            set_player_id();
        }
        digitalWrite(ind_hit,HIGH);
        if (cl_respawn_delay != 0xFF){
            tg_wrk_int = cl_respawn_delay * 200;
            tg_rd_trig = 2;
            //digitalWrite(ind_busy,HIGH);
        }
    }
}

void align(int st, int x, int y){
    byte offcet  = display.getStrWidth("9");
    byte offcet1 = 0; 
    if (st < 100){offcet1+=offcet;};
    if (st < 10){offcet1+=offcet;};
    display.setPrintPos(x+offcet1,y);
    display.print(st);
}

void align2(int st, int x, int y){
    byte offcet  = display.getStrWidth("9");
    byte offcet1 = offcet*3; 
    if (st < 100){offcet1 = offcet*2;};
    if (st < 10){offcet1 = offcet;};
    display.setPrintPos(x+((63 - offcet1)/2),y);
    display.print(st);
}

void noammo(){
    mp3_play(3);
    show_message(1,2);
    show_info_led();
}
void draw(){
    unsigned long tog = 0;
    int cl = 0;
    char buf[20];
    display.setColorIndex(1);
    display.setFont(u8g_font_courB08);
    display.drawBitmapP(0,0,1,6,heart1);
    tog = (tg_cur_life*100ul)/tg_max_life;
    display.drawFrame(9,0,94,7);
    display.drawBox(9,0,(tog*94)/100,7);
    if (tg_cur_life > 0){align(tog,105,7);}
    else {align(0,105,7);}
    display.print("%");
    display.drawBitmapP(0,8,1,6,shield1);
    tog = (tg_armor*100ul)/cl_armor;
    display.drawFrame(9,8,94,7);
    display.drawBox(9,8,(tog*94)/100,7);
    if (tg_cur_life > 0){align(tog,105,15);}
    else {align(0,105,15);}
    display.print("%");
    strcpy_P(buf,(char *) pgm_read_word (&(teams[cl_team_id])));
    display.setPrintPos(0,64);
    display.print(buf);
    display.print(tg_player_id);
    if (start_timer !=0){
        tog = (gl_interval + start_timer) - timer;
        byte tom = (tog/1000)/60;
        align(tom,92,64);
        display.print(":");
        byte tos = (tog/1000)%60;
        if (tos <10) {display.print(0);}
        display.print(tos);
    } else {
        display.setPrintPos(92,64);
        strcpy_P(buf,(char *) pgm_read_word (&(messages[2])));
        display.print(buf);
    }//u8g_font_courB20r u8g_font_timB24r
    display.setFont(u8g_font_timB24r);
    if (cl_clip_size != 0xFF){
        cl = cl_clip_size - tg_cur_clip;
        if (tg_cur_ammo <= cl_clip_size && cl > tg_cur_ammo){cl = tg_cur_ammo;};
        align2(cl,0,40);
    } else {
        display.setPrintPos(7,40);
        display.print(F("Inf"));
    }
    if (!tg_is_inf_ammo && tg_cur_ammo < 1000){
        align2(tg_cur_ammo,63,40);
    } else {
        display.setPrintPos(71,40);
        display.print(F("Inf"));
    }
    display.setFont(u8g_font_unifont_0_8);
    if (tg_msg_cnt != 0){
        strcpy_P(buf,(char *) pgm_read_word (&(messages[msg])));
        display.setPrintPos((128-display.getStrWidth(buf))>>1,54);
        display.print(buf);
    }
    display.setPrintPos(59,35);
    strcpy_P(buf,(char *) pgm_read_word (&(states[cl_fire_selector])));
    display.print(buf);
}

void show_info(){
    if (tg_ind_shadow > 0) {
        display.firstPage();  
        do {draw();} while( display.nextPage() );
    };
}

void add_health(byte data){
    //add health 0-100
    mp3_play(8);
    if (data > 100) {data = 100;}
    tg_cur_life += data;
    if (tg_cur_life > tg_max_life){tg_cur_life = tg_max_life;}
    show_message(9,3);
    show_info_led();
}

void add_ammopack(byte data){
    //add ammo 0-100
    mp3_play(10);
    if (data > 100) {data = 100;}
    tg_cur_ammo += data;
    if (tg_cur_ammo > tg_max_ammo){tg_cur_ammo = tg_max_ammo;}
    show_message(10,3);
    show_info_led();
}

void instakill(){
    //instant kill by admin
    dead();
}

void set_pause(){
    //set/unset pause
    mp3_play(11);
    tg_paused = !tg_paused;
    if (tg_paused){
        //digitalWrite(ind_busy,HIGH);
        show_message(3,0xFFFF);
    }
    else {
        //digitalWrite(ind_busy,LOW);
        show_message(3,0);
    }
}

void start_game(){
    //start game
    mp3_play(12);
    new_game();
    tg_wrk_int = cl_start_delay * 20;
    tg_rd_trig = 4;
    show_message(13,cl_start_delay);
    start_timer += cl_start_delay << 10;
    tg_paused = true;
    //digitalWrite(ind_busy,HIGH);
}

void reset_default(){
    //Reset to factory default 
    set_default();
    show_message(12,3);
    show_info_led();
}

void respawn(){
    //Player respawn 
    if ((cl_respawn_on == 1) && (tg_respawn_count < cl_max_respawns) && !tg_started){
        if (cl_zombie_mod != 1){tg_cur_life = cl_health_respawn;}
        if (cl_ammo_reset_resp == 0){
            tg_cur_ammo = 0;
        }
        if (cl_gamebox_reset == 1){
            tg_medpack_mask = 0;
            tg_ammobox_mask = 0;
            tg_flag_mask = 0;
        }
        tg_started = true;
        tg_respawn_count++;
        tg_armor = cl_armor;
        mp3_play(14);
        show_message(11,3);
        show_info_led();
    }
}

void new_game(){
    //Start new game immediate
    clean_stat();
    digitalWrite(ind_hit,LOW);
    tg_cur_life = tg_max_life;
    tg_cur_ammo = tg_max_ammo;
    tg_armor = cl_armor;
    tg_started = true;
    tg_medpack_mask = 0;
    tg_ammobox_mask = 0;
    tg_flag_mask = 0;
    start_timer = millis();
    mp3_play(15);
    show_message(14,3);
    show_info_led();
}

void restore_ammo(){
    // restore full ammo
    mp3_play(10);
    tg_cur_ammo = tg_max_ammo;
    tg_cur_clip = 0;
    show_message(16,3);
    show_info_led();
}

void end_game(){
    //end game
    mp3_play(12);
    mp3_play(9);
    unsigned long time_of_game = 0;
    tg_started = false;
    time_of_game = millis() - start_timer;
    start_timer = 0;
    tg_sec_of_game = (time_of_game/1000)%60;
    tg_min_of_game = (time_of_game/1000)/60;
    show_message(17,0xFFFF);
    show_info_led();
}

void reset_timer(){
    //reset timer
    mp3_play(11);
    start_timer = millis();
}

void player_init(){
    //Reset tagger
    mp3_play(15);
    set_default();
    set_player_id();
    clean_stat();
    show_info_led();
}

void explode_player(){
    //Explode player;
    mp3_play(16);
    dead();
}

void new_game_cont(){
    //new game
    start_game();
}

void restore_health(){
    //restore health to max
    mp3_play(8);
    tg_cur_life = tg_max_life;
    show_message(4,3);
    show_info_led();
}

void restore_armor(){
    //restore armor
    mp3_play(17);
    tg_armor = cl_armor;
    show_message(5,3);
    show_info_led();
}

void reset_score(){
    //reset statistics
    show_message(18,3);
    show_info_led();
    clean_stat();
}

void test_detectors(){
    digitalWrite(ind_hit,HIGH);
    tg_hit_led_cnt = hit_interval;
    show_message(19,3);
    show_info_led();
}

void stun_player(){
    //Stun player for 5 seconds
    mp3_play(18);
    tg_paused = true;
    //digitalWrite(ind_busy,HIGH);
    tg_wrk_int = 100;
    show_message(20,5);
    show_info_led();
}

void ungun_player(){
    //set ammo to 0
    mp3_play(19);
    tg_cur_ammo = 0;
    show_message(1,3);
    show_info_led();
}

void ammo_box(byte data){
    //Ammo box (interactive); data 0x00-0x0F box's ID (0-15)
    mp3_play(10);
    if (cl_gamemox_limit == 0){
        tg_cur_ammo += cl_ammo_box_clips * cl_clip_size;
        tg_cur_clip = 0;
    } else {
        if (((tg_ammobox_mask >> data) & B1) == 0){
            tg_cur_ammo += cl_ammo_box_clips * cl_clip_size;
            tg_ammobox_mask = tg_ammobox_mask | (B1 << data);
            tg_cur_clip = 0;
        }
    }
    show_message(21,3);
    show_info_led();
}

void medpack(byte data){
    //Medical pack (interactive); data 0x00-0x0F medpack's ID (0-15)
    if (cl_medpack_enabled == 1){
        mp3_play(8);
        if (cl_gamemox_limit == 0){
            tg_cur_life +=cl_medpack_size;
        } else {
            if (((tg_medpack_mask >> data) & B1) == 0){
                tg_cur_life +=cl_medpack_size;
                tg_medpack_mask = tg_medpack_mask | (B1 << data);
            }
        }
        show_message(22,3);
        show_info_led();
    }
}

void ctf(byte data){
    //Capture the flag (interactive); 0x00-0x0F ID флага (0-15)
    mp3_play(20);
    if (((tg_flag_mask >> data) & B1) == 0){
        tg_flag_mask = tg_flag_mask | (B1 << data);
    }
    tg_flag_count++;
    show_message(23,3);
    show_info_led();
}

void getclonedata(){
    int checksum = 0;
    if (!tg_started){
        show_message(24,3);
        show_info_led();
        for (int i = 3; i < MAX_COUNT - 1; i++){
            checksum += data_recieved[i];
        }
        if (checksum%0x100 == data_recieved[MAX_COUNT - 1]){
            mp3_play(21);
            cl_team_id = data_recieved[6];
            cl_ammo_box_clips = data_recieved[8];
            cl_medpack_size = data_recieved[9];
            cl_hit_led_timeout = data_recieved[11];
            cl_sound_set = data_recieved[12];
            cl_overheat_limit = data_recieved[13];
            cl_damage_id = data_recieved[16];
            cl_clip_size = data_recieved[17];
            cl_clip_numbers = data_recieved[18];
            cl_fire_selector = data_recieved[19];
            cl_burst_size = data_recieved[20];
            cl_rpm = data_recieved[21];
            cl_reload_delay = data_recieved[22];
            cl_ir_power = data_recieved[23];
            cl_ir_range = data_recieved[24];
            cl_health_respawn = data_recieved[26];
            cl_respawn_delay = data_recieved[28];
            cl_armor = data_recieved[29];
            cl_hit_delay = data_recieved[32];
            cl_start_delay = data_recieved[33];
            cl_death_delay = data_recieved[34];
            cl_time_limit = data_recieved[35];
            cl_max_respawns = data_recieved[36];
            cl_muzzle_flash = data_recieved[25] & B1;
            cl_overheat_on = (data_recieved[25] >> 1) & B1;
            cl_hit_led_enabled = (data_recieved[30] >> 1) & B1;
            cl_friendly_fire = (data_recieved[30] >> 2) & B1;
            cl_unlimited_clips = (data_recieved[30] >> 3) & B1;
            cl_zombie_mod = (data_recieved[30] >> 4) & B1;
            cl_medpack_enabled = (data_recieved[30] >> 5) & B1;
            cl_gamebox_reset = (data_recieved[30] >> 6) & B1;
            cl_gamemox_limit = (data_recieved[30] >> 7) & B1;
            cl_ctf_display_on = (data_recieved[31] >> 2) & B1;
            cl_respawn_on = (data_recieved[31] >> 3) & B1;
            cl_tagger_nick_on = (data_recieved[31] >> 4) & B1;
            cl_old_ir_lvl = (data_recieved[31] >> 5) & B1;
            cl_ammo_reset_resp = (data_recieved[31] >> 6) & B1;
            cl_game_mode_on = (data_recieved[31] >> 7) & B1;
            ////////////
            tg_max_life = code2health(cl_health_respawn);
            tg_cur_life = tg_max_life;
            tg_max_ammo = cl_clip_numbers * cl_clip_size;
            tg_cur_ammo = tg_max_ammo;
            tg_armor = cl_armor;
            ctrl_fire_delay = (int) (60000/code2rpm(cl_rpm));
            gl_interval = cl_time_limit * 60000;
            tg_overheat_int = 2000 / cl_overheat_limit;
            tg_rd_trig = 0;
            if (cl_zombie_mod == 1){
                set_zombie(cl_team_id);
            }
            if (cl_clip_numbers == 0xCA||cl_unlimited_clips == 1||cl_clip_size == 0xFF){tg_is_inf_ammo = true;} else {tg_is_inf_ammo = false;};
            tg_snd_shot = 25 + cl_sound_set;
            tg_snd_reload = 30 + cl_sound_set;
            hit_interval = cl_hit_led_timeout * 20;
            set_player_id();
            show_info_led();
        } 
    }
}

void set_zombie(byte team_id){
    switch (team_id){
        case 0:
            tg_max_life = 200;
            tg_cur_life  = tg_max_life;
            cl_clip_size = 1;
            cl_clip_numbers = 200;
            tg_max_ammo = cl_clip_numbers * cl_clip_size;
            tg_cur_ammo = tg_max_ammo;
            cl_damage_id = 5;
            cl_reload_delay = 1;
            cl_hit_delay = 1;
            cl_fire_selector = 0;
            cl_respawn_on = 1;
            cl_max_respawns = 1;
            cl_respawn_delay = 60;
            break;
        case 1:
            tg_max_life = 100;
            tg_cur_life  = tg_max_life;
            cl_hit_delay = 1;
            cl_respawn_on = 1;
            cl_max_respawns = 1;
            cl_respawn_delay = 60;
            break;
    }
}

void clean_stat(){
//    for (int i = 0; i < 214; i++){
//        tg_stat_02[i] = 0;
//        tg_stat_03[i] = 0;
//    }
    memset(tg_stat_02,0,214);
    memset(tg_stat_03,0,214);
    tg_flag_count = 0;
    tg_respawn_count = 0;
    tg_tagged_out = 0;
    tg_shot_count = 0;
    tg_hit_count = 0;
}

void send_stat(){
    byte tg_stat_01[12];
    int checksum1 = 0;
    int checksum2 = 0;
//    for (int i = 0; i < 12; i++){
//        tg_stat_01[i] = 0;
//    }
    show_message(25,0xFFFF);
    show_info_led();    
    memset(tg_stat_01,0,12);
    tg_stat_01[0]  = tg_player_id;
    tg_stat_01[1]  = cl_team_id;
    tg_stat_01[2]  = (tg_shot_count >> 8) & 0xFF;
    tg_stat_01[3]  = tg_shot_count & 0xFF;
    tg_stat_01[4]  = (tg_hit_count >> 8) & 0xFF;
    tg_stat_01[5]  = tg_hit_count & 0xFF;
    tg_stat_01[6]  = tg_min_of_game;
    tg_stat_01[7]  = tg_sec_of_game;
    tg_stat_01[8]  = tg_respawn_count;
    tg_stat_01[9]  = tg_tagged_out;
    tg_stat_01[10] = tg_flag_count;
    tg_stat_01[11] = (tg_stat_01[0] + tg_stat_01[1] + tg_stat_01[2] + tg_stat_01[3] + tg_stat_01[4] + tg_stat_01[5] + tg_stat_01[6] + tg_stat_01[7] + tg_stat_01[8] + tg_stat_01[9] + tg_stat_01[10])%0x100;
    for (int j = 0; j <= 213; j++){
        checksum1 +=tg_stat_02[j];
        checksum2 +=tg_stat_03[j];
    }
    tg_stat_02[213] = checksum1%0x100;
    tg_stat_03[213] = checksum2%0x100;
    //digitalWrite(ind_busy,HIGH);
    sendcommand(0x8703);
    for (int j = 0; j <= 12; j++){
        for (int i = 7; i >= 0; i--){
            if (((tg_stat_01[j] >> i) & B1) == 0){sendPulse(ctrl_ir_transmit,0);}
            else {sendPulse(ctrl_ir_transmit,1);}
            delayMicroseconds(lnk_lvl_0);        
        }
    }
    delayMicroseconds(lnk_timeout << 1);
    sendcommand(0x8704);
    for (int j = 0; j <= 214; j++){
        for (int i = 7; i >= 0; i--){
            if (((tg_stat_02[j] >> i) & B1) == 0){sendPulse(ctrl_ir_transmit,0);}
            else {sendPulse(ctrl_ir_transmit,1);}
            delayMicroseconds(lnk_lvl_0);        
        }
    }
    delayMicroseconds(lnk_timeout << 1);
    sendcommand(0x8705);
    for (int j = 0; j <= 214; j++){
        for (int i = 7; i >= 0; i--){
            if (((tg_stat_03[j] >> i) & B1) == 0){sendPulse(ctrl_ir_transmit,0);}
            else {sendPulse(ctrl_ir_transmit,1);}
            delayMicroseconds(lnk_lvl_0);        
        }
    }
    mp3_play(22);
    //digitalWrite(ind_busy,LOW);
    show_message(26,3);
    show_info_led();  
}

void sendcommand(unsigned int comm){
    byte parity = 0;
    sendPulse(ctrl_ir_transmit,2);
    delayMicroseconds(lnk_lvl_0);
    for (int i = 15; i >= 0; i--){
        if (((comm >> i) & B1) == 0){sendPulse(ctrl_ir_transmit,0);}
        else {
            sendPulse(ctrl_ir_transmit,1);
            parity = parity + 1;
        }
        delayMicroseconds(lnk_lvl_0);        
    }
    for (int i = 7; i >= 0; i--){
        if (((0xE8 >> i) & B1) == 0){sendPulse(ctrl_ir_transmit,0);}
        else {sendPulse(ctrl_ir_transmit,1);}
        delayMicroseconds(lnk_lvl_0);        
    }
    parity += 4;
    if (cl_game_mode_on == 0){parity = 24 - parity;}
    if (parity%2 == 0){sendPulse(ctrl_ir_transmit,0);}
    else {sendPulse(ctrl_ir_transmit,1);}
    delayMicroseconds(lnk_lvl_0); 
}

int code2health(byte health){
    if (health < sizeof(val_health)){
        return val_health[health];}
    else return 0;
}

int code2rpm(byte rpm){
    if (rpm < sizeof(val_rpm)){
        return val_rpm[rpm];}
    else return 0;
}

int code2damage(byte damage){
    if (damage < sizeof(val_damage)){
        return val_damage[damage];}
    else return 0;
}

int code2hit_timeout(byte code){
    if (code < sizeof(val_hit_timeout)){
        return val_hit_timeout[code];}
    else return 0;
}

int code2ir_range(byte code){
    switch (code) {
        case 0x01:
            return 1;
        case 0x02:
            return 10;
        case 0x04:
            return 20;
        case 0x07:
            return 40;
        case 0x0A:
            return 60;
        case 0x0E:
            return 80;
        case 0x12:
            return 100;
    }
}
