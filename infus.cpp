#include <cmath>
#include <ctime>
#include <iostream>
#include <mysql/mysql.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>

using namespace cv;
using namespace std;

const char *id;
const char *vol_send;

void interrupt() {
  static int tombol = 0;
  tombol++;
  if (tombol == 1)
    system("sudo halt");
  // if(tombol==2)system("sudo reboot");
  if (tombol > 2)
    tombol = 0;
}

void database(const char *x, const char *y) {
  MYSQL mysql, *connection;
  MYSQL_RES *result;
  MYSQL_ROW row;

  int len, size;
  char data[1000 * 1024];
  char chunk[2 * 1000 * 1024 + 1];
  char query[1024 * 5000];

  FILE *fp;

  char *ip = (char *)"192.168.0.100";
  char *usr = (char *)"pi";
  char *pass = (char *)"josekroos10";
  char *db = (char *)"coba";

  int query_state;
  mysql_init(&mysql);
  connection = mysql_real_connect(&mysql, ip, usr, pass, db, 0, NULL, 0);
  if (connection == NULL) {
    cout << mysql_error(&mysql) << endl;
    return;
  } else {
    printf("Database connection successful\n");
  }
  query_state = mysql_query(connection, "SELECT id FROM tbinfus");
  result = mysql_store_result(connection);
  string cek_id;
  bool status = false;
  while ((row = mysql_fetch_row(result)) != NULL) {
    cek_id = row[0];
    if (cek_id == x) {
      status = true;
    }
  }
  fp = fopen("frame.jpg", "rb");
  size = fread(data, 1, 1024 * 1000, fp);
  mysql_real_escape_string(connection, chunk, data, size);
  const char *stat;
  if (status) {
    stat = "UPDATE tbinfus SET image='%s',volume='%s' WHERE id='%s'";
    len = snprintf(query, sizeof(stat) + sizeof(chunk), stat, chunk, y, x);
  } else {
    stat = "INSERT INTO tbinfus(id,image,volume)VALUES('%s','%s','%s')";
    len = snprintf(query, sizeof(stat) + sizeof(chunk), stat, x, chunk, y);
  }

  mysql_real_query(connection, query, len);
  fclose(fp);
  status = false;
  mysql_free_result(result);
  mysql_close(connection);
}
void scaling(int pixel) {
  if (pixel >= 575)
    vol_send = "500mL";
  else if (pixel >= 573 && pixel < 575)
    vol_send = "450mL";
  else if (pixel >= 570 && pixel < 572)
    vol_send = "400mL";
  else if (pixel >= 567 && pixel < 570)
    vol_send = "375mL";
  else if (pixel >= 562 && pixel <= 566)
    vol_send = "350mL";
  else if (pixel >= 559 && pixel < 562)
    vol_send = "300mL";
  else if (pixel >= 556 && pixel < 559)
    vol_send = "275mL";
  else if (pixel >= 554 && pixel <= 555)
    vol_send = "250mL";
  else if (pixel >= 547 && pixel < 554)
    vol_send = "225mL";
  else if (pixel >= 535 && pixel < 547)
    vol_send = "200mL";
  else if (pixel >= 525 && pixel < 535)
    vol_send = "175mL";
  else if (pixel >= 501 && pixel < 525)
    vol_send = "150mL";
  else if (pixel >= 458 && pixel < 501)
    vol_send = "125mL";
  else if (pixel >= 382 && pixel < 458)
    vol_send = "100mL";
  else if (pixel >= 308 && pixel < 382)
    vol_send = "75mL";
  else if (pixel >= 230 && pixel < 308)
    vol_send = "50mL";
  else if (pixel >= 181 && pixel < 230)
    vol_send = "25mL";
  else if (pixel < 162)
    vol_send = "Habis";
  printf("%s\n", vol_send);
  // return pixel;
}

void baca_tombol(bool &tombol1, bool &tombol2) {
  static bool prev_input1 = true, prev_input2 = true;
  static bool input_now1 = true, input_now2 = true;
  input_now1 = digitalRead(20);
  input_now2 = digitalRead(21);
  if (prev_input1 and (not input_now1)) {
    printf("Button Pressed\n");
    tombol1 ^= 1;
  }
  if (prev_input2 and (not input_now2)) {
    printf("Button Pressed2\n");
    tombol2 ^= 1;
  }

  prev_input1 = input_now1;
  prev_input2 = input_now2;
  delay(0.05);
}

int main(int, char **) {
  clock_t start;
  clock_t time2;
  double duration, led_timer;

  Rect rect; /* bounding box */
  Mat frame_setting;
  Mat frame, gray, edges, otsu, equ;
  Mat template_img, outimage, filter, canny;
  Mat histImage;
  Rect bounding_rect;
  Mat frame2;
  // char filename="jose.jpg";
  bool addwFlag = false;
  bool histoFlag = false;
  bool equalizeFlag = false;
  bool infusFlag = true;
  bool tombol_kanan = false, tombol_kiri = false;
  bool positionFlag = false;
  bool baca = false;
  bool time_count = true;
  int index = 0;
  int volume = 0, volume_baru = 0, volume_lama = 0;
  int volume_control = 720;
  int cnt = 0;
  double brightness = 1;
  double value;
  int batas_kanan, batas_kiri;
  int index_h = 0;
  int index_h1 = 0;
  int largest_index;
  int largest_area = 0;
  int batas_miring = 1;
  int height, width;
  int volume_hitung, level, norm;
  int batas_rendah;
  wiringPiSetupGpio();

  pinMode(3, INPUT);
  pinMode(20, INPUT);
  pinMode(21, INPUT);
  pinMode(12, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(22, OUTPUT);

  pullUpDnControl(20, PUD_UP);
  pullUpDnControl(21, PUD_UP);
  pullUpDnControl(3, PUD_UP);

  digitalWrite(4, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(22, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(16, LOW);
  wiringPiISR(3, INT_EDGE_FALLING, interrupt);

  VideoCapture cap;
  int deviceID = 0;
  int apiID = cv::CAP_ANY;

  system("v4l2-ctl -c exposure_auto=1");
  system("v4l2-ctl -c exposure_absolute=2500");
  cap.open(deviceID + apiID);
  cap.set(CAP_PROP_FRAME_WIDTH, 1280);
  cap.set(CAP_PROP_FRAME_HEIGHT, 720);
  cap.read(frame);
  delay(100);
  cap.release();

  digitalWrite(12, HIGH);
  digitalWrite(16, HIGH);
  // delay(1000);
  // digitalWrite(12,LOW);
  // digitalWrite(16,LOW);
  // digitalWrite(17,LOW);
  // digitalWrite(4,LOW);

  cap.open(deviceID + apiID);
  cap.set(CAP_PROP_FRAME_WIDTH, 1280);
  cap.set(CAP_PROP_FRAME_HEIGHT, 720);
  namedWindow("frame", WINDOW_AUTOSIZE);
  // namedWindow("Output",WINDOW_AUTOSIZE);
  template_img = imread("/home/pi/frame.jpg", 0);
  if (template_img.empty()) {
    template_img = imwrite("/home/pi/frame.jpg", frame);
  }

  start = clock();

  while (1) {
    baca_tombol(tombol_kanan, tombol_kiri);

    if ((tombol_kanan || tombol_kiri) && time_count) {
      time2 = clock();
      time_count = false;
    }
    led_timer = (clock() - time2) / (double)CLOCKS_PER_SEC;
    if (led_timer >= 1 && tombol_kanan) {
      digitalWrite(16, LOW);
      time2 = clock();

    } else
      digitalWrite(16, HIGH);
    if (led_timer >= 1 && tombol_kiri) {
      digitalWrite(12, LOW);
      time2 = clock();
    } else
      digitalWrite(12, HIGH);

    if (positionFlag == true && tombol_kanan == false)
      infusFlag = false;
    if (positionFlag == true && tombol_kiri == false)
      infusFlag = true;
    if (tombol_kanan || tombol_kiri) {
      duration = (clock() - start) / (double)CLOCKS_PER_SEC;
      cap.read(frame);
      if (infusFlag and tombol_kanan) {
        rect = Rect(760, 0, 380, 720); // infus kanan
        id = "RS1R";
        baca = true;
        norm = 0;
        batas_rendah = 40;

      } else {
        rect = Rect(150, 0, 460, 720); // infus kiri
        id = "RS1L";
        baca = false;
        norm = 0;
        batas_rendah = 50;
      }
      frame = frame(rect);
      frame2 = frame.clone();
      cvtColor(frame, gray, COLOR_BGR2GRAY);
      gray.convertTo(gray, -1, brightness, 0);
      if (equalizeFlag)
        equalizeHist(gray, gray);
      medianBlur(gray, filter, 5);
      value = (double)threshold(filter, otsu, 0, 255,
                                CV_THRESH_BINARY | CV_THRESH_OTSU);
      Canny(filter, canny, 200 + value, 600 - value, 5);
      bitwise_not(canny, outimage);
      vector<vector<Point>> contours;
      findContours(otsu, contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

      for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area > largest_area) {
          largest_area = area;
          largest_index = i;
          bounding_rect = boundingRect(contours[i]);
        }
      }
      rectangle(frame, bounding_rect, Scalar(0, 255, 0), 1, 8, 0);
      height = bounding_rect.height;
      width = bounding_rect.width;
      if (height >= 690)
        height = 720;
      batas_kanan = bounding_rect.x;
      batas_kiri = bounding_rect.width + bounding_rect.x;
      // printf("%d : %d\n",batas_kanan,batas_kiri);
      vector<Vec4i> lines;
      HoughLinesP(canny, lines, 1, CV_PI / 180, 5, 20, 5);
      for (size_t i = 0; i < lines.size(); i++) {
        Vec4i l = lines[i];
        for (int i = 0; i <= 15; i++) {
          if ((l[0] == l[2] + i) || (l[0] == l[2] - i)) {
            if ((l[0] <= batas_kanan + 15 && l[0] >= batas_kanan - 10) and
                baca) {
              line(frame, Point(l[0], l[1]), Point(l[2], l[3]),
                   Scalar(255, 0, 0), 3, CV_AA);
              index_h = l[0];
            }

            if ((l[0] >= batas_kiri - 20 && l[0] <= batas_kiri + 25) and
                (not baca)) {
              line(frame, Point(l[0], l[1]), Point(l[2], l[3]),
                   Scalar(255, 0, 0), 3, CV_AA);
              index_h1 = l[0];
            }
          }
        }

        for (int j = 0; j <= batas_miring; j++) {
          if ((l[1] == l[3] + j) || (l[1] == l[3] - j)) {
            if ((l[0] <= index_h + 25 and baca) ||
                (l[0] >= index_h1 - 60 and (not baca))) {

              if (l[1] < height - 120 && l[1] > 100) {
                index = (l[1] + l[3]) / 2;
                volume = height - index;
                if (abs(volume_control - volume) < 20) {
                  line(frame, Point(l[0], l[1]), Point(l[2], l[3]),
                       Scalar(0, 0, 255), 3, CV_AA);
                  volume_baru = volume + volume_lama;
                  cnt++;
                  volume_lama = volume_baru;
                }
                volume_control = volume;
              }
            }
          }
        }
      }

      if (addwFlag) {
        absdiff(gray, template_img, outimage);
        imshow("Output", outimage);
      }

      // rectangle(frame, rect, CV_RGB(255, 0, 0), 3, 8, 0);
      imshow("frame", frame);
      // imshow("Gray",gray);
      // imshow("Filter",filter);
      imshow("Canny", canny);
      // imshow("frame2",frame2);
      // imshow("Output",outimage);
      imshow("Otsu", otsu);
      // imshow("Histogram",histImage);
      // printf("auto = %8.3f\n",value);
      // printf("%d\n",batas_miring);
      if (duration >= 10 && cnt < 2) {
        batas_miring = (batas_miring <= 8) ? batas_miring + 2 : 8;
        start = clock();
      }

      if (cnt >= 2) {
        level = volume_baru / cnt;
        level = level - norm;
        printf("Tinggi Botol : %d\nLebar Botol : %d\nLevel Cairan %d\n", height,
               width, level);
        if (height > 650) {
          if (level >= 501) {
            volume_hitung = 3813.9 * log(level) - 23497;
            // volume_hitung = 0.0066*exp(0.0197*level);

            if (volume_hitung > 540)
              volume_hitung = 540;

          } else {
            volume_hitung = 129.11 * log(level) - 604.13;
            // printf("%d mL\n",volume_hitung);
          }
        } else {
          volume_hitung = 143.66 * log(level) - 677.2;
        }
        printf("%d mL\n", volume_hitung);
        imwrite("/home/pi/frame.jpg", frame);
        string s = to_string(volume_hitung) + "mL";
        vol_send = s.c_str();
        if (volume_hitung <= batas_rendah) {
          if (baca)
            digitalWrite(22, LOW);
          else
            digitalWrite(27, LOW);
          vol_send = "Habis";
        }
        /*if(level<=256 && level >166)
        {
                digitalWrite(17,HIGH);
                digitalWrite(4,LOW);
                //digitalWrite(4,LOW);
        }*/
        // scaling(volume_baru/cnt);
        database(id, vol_send);
        cnt = 0;
        volume_lama = 0;
        start = clock();
        if (tombol_kanan && tombol_kiri) {
          infusFlag ^= 1;
          batas_kiri = 0;
          batas_kanan = 0;
          largest_area = 0;
          positionFlag = true;
        }
      }
      // printf("%.3f",duration);

      int key = waitKey(1);
      if ((key == 27) || (key == 32))
        break;
      if ((key == 'a') || (key == 'A'))
        brightness = brightness + 0.05;
      if ((key == 'z') || (key == 'Z'))
        brightness = brightness - 0.05;
      if ((key == 's') || (key == 'S'))
        imwrite("/home/pi/frame.jpg", frame);
      if ((key == 'c') || (key == 'C')) {
        imwrite("/home/pi/canny_inv.jpg", outimage);
        imwrite("/home/pi/canny.jpg", canny);
        imwrite("/home/pi/frame2.jpg", frame2);
        imwrite("/home/pi/filter.jpg", filter);
        imwrite("/home/pi/gray.jpg", gray);
        imwrite("/home/pi/otsu.jpg", otsu);
      }
      if ((key == 'l') || (key == 'L'))
        template_img = imread("/home/pi/frame.jpg", 0);
      if ((key == 'w') || (key == 'W'))
        addwFlag ^= 1;
      if ((key == 'e') || (key == 'E'))
        equalizeFlag ^= 1;
      if ((key == 'm') || (key == 'M'))
        infusFlag ^= 1;
      if ((key == 'r') || (key == 'R')) {
        batas_kiri = 0;
        batas_kanan = 0;
        largest_area = 0;
        batas_miring = 1;
      }
      if ((key == 'p') || (key == 'P'))
        tombol_kanan ^= 1;
      // batas_kiri=0;
      // batas_kanan=0;
      largest_area = 0;
    }
  }
  cap.release();
  destroyAllWindows();
  digitalWrite(17, HIGH);
  return 0;
}
