#include "esp32_wifi.hpp"
#include <stdio.h>

#if MODE_AP_STA
#define WIFI_MODE '2'

#elif MODE_STA
#define WIFI_MODE '1'

#elif MODE_AP
#define WIFI_MODE '0'

#else
#define WIFI_MODE '2'
#endif

#define APIP "ap_ip"
#define AP_WIFI_SSID "ESP32_WIFI_TEST"
#define AP_WIFI_PD ""

#define STAIP "sta_ip"
#define STA_WIFI_SSID "Yahboom2"
#define STA_WIFI_PD "yahboom890729"

AI_mode runmode = Nornal_AI;
ESP32_AI_Msg esp32_ai_msg;
QR_AI_Msg QR_msg;

char send_buf[35] = { 0 };
char recv_buf[50] = { 0 };
char data_buff[50] = { 0 };
uint8_t cmd_flag = 0;

void serial_init(void) {
  ESPWIFISerial.begin(115200);
}

void SET_STA_WIFI(void) {
  sprintf(send_buf, "sta_ssid:%s", STA_WIFI_SSID);
  ESPWIFISerial.print(send_buf);
  memset(send_buf, 0, sizeof(send_buf));
  delay(300);

  sprintf(send_buf, "sta_pd:%s", STA_WIFI_PD);
  ESPWIFISerial.print(send_buf);
  memset(send_buf, 0, sizeof(send_buf));

  delay(2000);
}

void SET_AP_WIFI(void) {
  sprintf(send_buf, "ap_ssid:%s", AP_WIFI_SSID);
  ESPWIFISerial.print(send_buf);
  memset(send_buf, 0, sizeof(send_buf));
  delay(300);

  sprintf(send_buf, "ap_pd:%s", AP_WIFI_PD);
  ESPWIFISerial.print(send_buf);
  memset(send_buf, 0, sizeof(send_buf));

  delay(2000);
}

void SET_ESP_WIFI_MODE(void) {
  sprintf(send_buf, "wifi_mode:%c", WIFI_MODE);
  ESPWIFISerial.print(send_buf);
  memset(send_buf, 0, sizeof(send_buf));

  delay(2000);
}

void SET_ESP_AI_MODE(AI_mode Mode) {

  sprintf(send_buf, "ai_mode:%d", Mode);
  ESPWIFISerial.print(send_buf);
  memset(send_buf, 0, sizeof(send_buf));
  runmode = Mode;

  delay(2000);
}

void Get_STAIP(void) {
  sprintf(send_buf, STAIP);
  ESPWIFISerial.print(send_buf);
  memset(send_buf, 0, sizeof(send_buf));
  cmd_flag = 1;
}

void Get_APIP(void) {
  sprintf((char *)send_buf, APIP);
  ESPWIFISerial.print(send_buf);
  memset(send_buf, 0, sizeof(send_buf));
  cmd_flag = 1;
}

void Get_Version(void) {
  sprintf((char *)send_buf, "wifi_ver");
  ESPWIFISerial.print(send_buf);
  memset(send_buf, 0, sizeof(send_buf));
  cmd_flag = 1;
}

void recv_data(void) {
  char strr;
  if (ESPWIFISerial.available()) {
    strr = char(ESPWIFISerial.read());
    //Serial.println(strr);
    Data_Deal(strr);
  }
}

uint8_t end_falg = 0;
uint8_t i_index = 0;

void Data_Deal(char RXdata) {
  if (cmd_flag == 1) {
    recv_buf[i_index] = RXdata;
    if (RXdata == 0x0D) {
      end_falg = 1;
    }

    if (end_falg == 1 && RXdata == 0x0A) {
      cmd_flag = 0;
      end_falg = 0;
      memcpy(data_buff, recv_buf, i_index);
      memset(recv_buf, 0, sizeof(recv_buf)); 
      i_index = 0;    
    } else
      i_index++;
  }

  else if (cmd_flag == 2) {
    recv_tcp_data(RXdata);
  } else if (cmd_flag == 3) {
    recv_face_data(RXdata); 
  } else if (cmd_flag == 4) {
    recv_QR_data(RXdata);  
  } else {
    recv_AI_data(RXdata); 
  }
}

uint8_t g_new_flag = 0;
uint8_t g_index = 0;
uint8_t newlines = 0;  

void recv_tcp_data(char tcpdata) {
  if (tcpdata == '$' && g_new_flag == 0) {
    g_new_flag = 1;
    memset(recv_buf, 0, sizeof(recv_buf)); 
    return;
  }
  if (g_new_flag == 1) {
    if (tcpdata == '#') {
      g_new_flag = 0;
      g_index = 0;
      //newlines = 1; 
      memcpy(data_buff, recv_buf, sizeof(recv_buf));
      memset(recv_buf, 0, sizeof(recv_buf)); 
    }

    else if (tcpdata == '$') 
    {
      g_index = 0;
      g_new_flag = 0;
      memset(recv_buf, 0, sizeof(recv_buf));
    }

    else {
      recv_buf[g_index++] = tcpdata;
    }

    if (g_index > 50)  
    {
      g_index = 0;
      g_new_flag = 0;
      memset(recv_buf, 0, sizeof(recv_buf)); 
    }
  }
}

void recv_AI_data(char AIdata) 
{
  if (AIdata == '$' && g_new_flag == 0) {
    g_new_flag = 1;
    memset(recv_buf, 0, sizeof(recv_buf)); 
    return;
  }
  if (g_new_flag == 1) {
    if (AIdata == '#') {
      g_new_flag = 0;
      g_index = 0;
      memcpy(data_buff, recv_buf, sizeof(recv_buf));
      //ESPWIFISerial.println(data_buff);
      Get_AI_msg(data_buff);
      newlines = 1;                           
      memset(recv_buf, 0, sizeof(recv_buf));  
    }

    else if (AIdata == '$')
    {
      g_index = 0;
      g_new_flag = 0;
      memset(recv_buf, 0, sizeof(recv_buf)); 
    }

    else {
      recv_buf[g_index++] = AIdata;
    }

    if (g_index > 50) 
    {
      g_index = 0;
      g_new_flag = 0;
      memset(recv_buf, 0, sizeof(recv_buf));  
    }
  }
}

void Get_AI_msg(char *buf) {
  char databuf[3] = { "\0" };
  uint8_t len = 0;
  if (strlen(buf) != 16)  
  {
    //ESPWIFISerial.println(strlen(buf));
    return;
  }
  if (buf[3] != ',' && buf[7] != ',' && buf[11] != ',' && buf[15] != ',') 
  {
    return;
  }

  for (uint8_t i = 0; i < 16; i++) {
    if (buf[i] == ',')
      len++;
  }
  if (len != 4)  
  {
    return;
  }


  sprintf(databuf, "%c%c%c", buf[0], buf[1], buf[2]);
  esp32_ai_msg.lx = atof(databuf);

  sprintf(databuf, "%c%c%c", buf[4], buf[5], buf[6]);
  esp32_ai_msg.ly = atof(databuf);

  sprintf(databuf, "%c%c%c", buf[8], buf[9], buf[10]);
  esp32_ai_msg.rx = atof(databuf);

  sprintf(databuf, "%c%c%c", buf[12], buf[13], buf[14]);
  esp32_ai_msg.ry = atof(databuf);

  memset(buf, 0, sizeof(buf));

  if ((esp32_ai_msg.lx > esp32_ai_msg.rx) || (esp32_ai_msg.ly > esp32_ai_msg.ry)) 
  {
    return;
  }

  esp32_ai_msg.cx = (esp32_ai_msg.rx - esp32_ai_msg.lx) / 2 + esp32_ai_msg.lx;
  esp32_ai_msg.cy = (esp32_ai_msg.ry - esp32_ai_msg.ly) / 2 + esp32_ai_msg.ly;

  esp32_ai_msg.area = (esp32_ai_msg.rx - esp32_ai_msg.lx) / 10 * (esp32_ai_msg.ry - esp32_ai_msg.ly);

  //  ESPWIFISerial.println(esp32_ai_msg.cx);
  //  ESPWIFISerial.println(esp32_ai_msg.cy);
  //  ESPWIFISerial.println(esp32_ai_msg.area);
}


void recv_QR_data(char QRdata) 
{
  if (QRdata == '$' && g_new_flag == 0) {
    g_new_flag = 1;
    memset(recv_buf, 0, sizeof(recv_buf)); 
    return;
  }
  if (g_new_flag == 1) {
    if (QRdata == '#') {
      g_new_flag = 0;
      g_index = 0;
      newlines = 1;  
      memcpy(data_buff, recv_buf, sizeof(recv_buf));
      memcpy(QR_msg.QR_msg, data_buff, sizeof(QR_msg.QR_msg));  

      memset(recv_buf, 0, sizeof(recv_buf));    // 清除旧数据
      memset(data_buff, 0, sizeof(data_buff));  // 清除旧数据
    }

    else if (QRdata == '$')  //中途出现丢包
    {
      g_index = 0;
      g_new_flag = 0;
      memset(recv_buf, 0, sizeof(recv_buf));  //清除旧数据
    }

    else {
      recv_buf[g_index++] = QRdata;
    }

    if (g_index > 50)  //数组溢出
    {
      g_index = 0;
      g_new_flag = 0;
      memset(recv_buf, 0, sizeof(recv_buf));  //清除旧数据
    }
  }
}

//---------------------------------------------------------------------------------------//
//人脸识别协议
void recv_face_data(char facedata)  //协议基本是$xxx,yyy,zzz,hhh,#@ID:1!\r\n
{
  if (facedata == '$' && g_new_flag == 0) {
    g_new_flag = 1;
    memset(recv_buf, 0, sizeof(recv_buf));  // 清除旧数据
    return;
  }
  if (g_new_flag == 1) {
    if (facedata == '!') {
      g_new_flag = 0;
      g_index = 0;
      memcpy(data_buff, recv_buf, sizeof(recv_buf));
      //处理
      Get_faceAI_msg(data_buff);
      newlines = 1;                             //新数据接收完毕
      memset(data_buff, 0, sizeof(data_buff));  // 清除旧数据
      memset(recv_buf, 0, sizeof(recv_buf));    // 清除旧数据
    }

    else if (facedata == '$')  //中途出现丢包
    {
      g_index = 0;
      g_new_flag = 0;
      memset(recv_buf, 0, sizeof(recv_buf));  //清除旧数据
    }

    else {
      if ((facedata != '#') && (facedata != '@'))  //去掉#和@号，只要有效数据
        recv_buf[g_index++] = facedata;
    }

    if (g_index > 50)  //数组溢出
    {
      g_index = 0;
      g_new_flag = 0;
      memset(recv_buf, 0, sizeof(recv_buf));  //清除旧数据
    }
  }
}

//获取人脸数据
void Get_faceAI_msg(char *buf) {
  char databuf[3] = { "\0" };
  uint8_t len = 0;
  for (uint8_t i = 0; i < 16; i++) {
    if (buf[i] == ',')
      len++;
  }
  if (len != 4) {
    return;
  }


  if (buf[3] != ',' && buf[7] != ',' && buf[11] != ',' && buf[15] != ',') {
    return;
  }
  sprintf(databuf, "%c%c%c", buf[0], buf[1], buf[2]);
  esp32_ai_msg.lx = atof(databuf);

  sprintf(databuf, "%c%c%c", buf[4], buf[5], buf[6]);
  esp32_ai_msg.ly = atof(databuf);

  sprintf(databuf, "%c%c%c", buf[8], buf[9], buf[10]);
  esp32_ai_msg.rx = atof(databuf);

  sprintf(databuf, "%c%c%c", buf[12], buf[13], buf[14]);
  esp32_ai_msg.ry = atof(databuf);

  //   ESPWIFISerial.println(buf);
  //   ESPWIFISerial.println(strlen(buf));
  len = strlen(buf);

  if ((esp32_ai_msg.lx > esp32_ai_msg.rx) || (esp32_ai_msg.ly > esp32_ai_msg.ry))  //当坐标非法
  {
    return;
  }

  //中心点x,y
  esp32_ai_msg.cx = (esp32_ai_msg.rx - esp32_ai_msg.lx) / 2 + esp32_ai_msg.lx;
  esp32_ai_msg.cy = (esp32_ai_msg.ry - esp32_ai_msg.ly) / 2 + esp32_ai_msg.ly;

  //面积缩10倍吧  因为ardniuo是个16位的东西,这个65535是上限
  esp32_ai_msg.area = (esp32_ai_msg.rx - esp32_ai_msg.lx) / 10 * (esp32_ai_msg.ry - esp32_ai_msg.ly);


  char idbuf[len - 18] = { '\0' };
  //解析id
  if (buf[19] == '-')  //为负数
  {
    for (uint8_t i = 0; i < len - 19; i++) {
      idbuf[i] = buf[20 + i];
    }
    esp32_ai_msg.id = -atof(idbuf);
  } else  //为正数
  {
    for (uint8_t i = 0; i < len - 19; i++) {
      idbuf[i] = buf[19 + i];
    }
    esp32_ai_msg.id = atof(idbuf);
  }


  //  ESPWIFISerial.println(esp32_ai_msg.cx);
  //  ESPWIFISerial.println(esp32_ai_msg.cy);
  //  ESPWIFISerial.println(esp32_ai_msg.area);
  //ESPWIFISerial.println(esp32_ai_msg.id);
}
