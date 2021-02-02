#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

const byte MAX_COUNT      = 39;
const int lnk_lvl_0       = 600;
const int lnk_lvl_1       = lnk_lvl_0<<1;
const byte lnk_ir_freq    = 36;
const int lnk_timeout     = 3200;
const byte lnk_ir_speriod = (int) ((500/lnk_ir_freq) - 4);
const byte lnk_ir_pulses  = (int) (lnk_lvl_0 / (1000/lnk_ir_freq));

int ctrl_ir_transmit      = 2;
int sens_head             = 3;
int ind_busy              = 4;
int ind_team0             = 5;
int ind_team1             = 6;
int ind_team2             = 7;
int ind_team3             = 8;

word timer_team[4];
unsigned long timer_cap   = 0; 
unsigned long one_sec     = 0;

byte data_recieved[MAX_COUNT];
byte dvc_id               = 1;//идентификатор флага 0-15.
byte cl_game_mode_on      = 1;
int state                 = -1;
int ctrl_index            = 0;
byte timer_win            = 15 * 60;// Время удержания флага в секундах.
boolean captured          = false;
boolean started           = true;


void setup(){
    Serial.begin(9600);
    mp3_set_serial (Serial);
    for (int i = 0; i < 4; i++){timer_team[i] = 0;}
    pinMode(sens_head,INPUT);
    pinMode(ctrl_ir_transmit,OUTPUT);
    pinMode(ind_busy,OUTPUT);
    pinMode(ind_team0,OUTPUT);
    pinMode(ind_team1,OUTPUT);
    pinMode(ind_team2,OUTPUT);
    pinMode(ind_team3,OUTPUT);
    mp3_set_volume (30);
    delay(100);
    mp3_play(1);
}

void loop(){
    if (started){
        receive_command();
        timers();
    }
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
        digitalWrite(ind_busy,HIGH);
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
            digitalWrite(ind_busy,LOW);
            if (cl_game_mode_on == 0){parity = (size_of_command << 3) - parity;}
            if ((parity%2)==temp_parity_bit){parse_protocol();}
            for (int i = 0; i < (ctrl_index >>3)+1; i++){data_recieved[i]=0;}
        }
    }
}

void parse_protocol(){
    if (data_recieved[0] < 128){
        captured = true;
        digitalWrite(ind_busy,HIGH);
        timer_cap = millis();
        state = data_recieved[1] >> 6;
    }
}

void timers(){
    if (state != -1 && !captured) {
        if (millis() - one_sec >= 1000){
            timer_team[state]++;
            one_sec = millis();
        }
    }
    if (captured){
        if (millis() - timer_cap >= 60000){// Здесь можно задать срок захвата флага
            captured = false;
            sendcommand((0x8C << 8) + dvc_id);
            digitalWrite(ind_busy,LOW);
            mp3_play(20);
        }
    }
    for (int i = 0; i < 4; i++){
        if (timer_team[i] >= timer_win) {
            mp3_play(12);
            started = false;
            switch (i){
                case 0:
                    digitalWrite(ind_team0,HIGH);
                    break;
                case 1:
                    digitalWrite(ind_team1,HIGH);
                    break;
                case 2:
                    digitalWrite(ind_team2,HIGH);
                    break;
                case 3:
                    digitalWrite(ind_team3,HIGH);
                    break;
            }
            break;
        }
    }
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
