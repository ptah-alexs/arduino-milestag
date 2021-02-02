const int lnk_lvl_0       = 600;
const int lnk_lvl_1       = lnk_lvl_0<<1;
const byte lnk_ir_freq    = 36;
const int lnk_timeout     = 3200;
const byte lnk_ir_speriod = (int) ((500/lnk_ir_freq) - 4);
const byte lnk_ir_pulses  = (int) (lnk_lvl_0 / (1000/lnk_ir_freq));

byte cl_game_mode_on      = 1;
byte dvc_id               = 1; //id 0x0 - 0xF

// list of pin
int ctrl_ir_transmit      = 2;
int ctrl_btn              = 3;

void setup(){
    Serial.begin(9600); 
    pinMode(ctrl_btn,INPUT);
    pinMode(ctrl_ir_transmit,OUTPUT);
    digitalWrite(ctrl_btn, HIGH);
    Serial.println("Ready");
}

void loop(){
    triggers();
}

void triggers(){
    static byte trig_btn_state = 0;
    static byte trig_btn_state_old = 0;
    trig_btn_state_old = trig_btn_state;
    trig_btn_state = digitalRead(ctrl_btn);
    if ( trig_btn_state != trig_btn_state_old && trig_btn_state == LOW){
        sendcommand((0x8A << 8) + dvc_id);
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
