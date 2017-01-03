# 123Duino Arduino IDE 開發環境安裝 "簡易版" (OS:Window)
<HR size=3>
<h3>123Duino Board 安裝</h3>
<BR>1. 請由以下Arduino 官網連結下載Arduino 開發環境
https://www.arduino.cc/en/Main/Donate

<BR>2.點選以上網址後,您可選擇您要捐助的金額,或是直接點選"JUST DOWNLOAD"下載
<img  src="https://github.com/123duino/123duino/blob/master/Software/img/1_download_ide.png?raw=true"  width="70%" height="70%">

<BR>3.下載完成請直接執行安裝檔 如:arduino-1.8.0-windows.exe

<BR>4.安裝時請依照安裝程式指示即可完成安裝

<BR>5.完成後,點選Arduino 開啟Arduino開發環境

<BR><B>6.在Arduino IDE 功能表：File > Preferences. 複製/貼上以下的超連結到 "Additional Boards Manager URLs" </B>
https://raw.githubusercontent.com/123duino/123duino/master/package_123duino_index.json

<img  src="https://github.com/123duino/123duino/blob/master/Software/img/3_board_url.png"  width="75%" height="75%">


<BR>7.再到 Tools > Board >Boards Manager 中看，就可看到123Duino 開發板安裝項目並點選install
<img  src="https://github.com/123duino/123duino/blob/master/Software/img/4_board_manager_install.png"  width="75%" height="75%">

<BR>8.完成以上步驟,即可在Tools > Board 中點選123Duino 
<img  src="https://github.com/123duino/123duino/blob/master/Software/img/5_board_123duino.png"  width="60%" height="60%">

<HR size=3>
<h3>123Duino 相關Library安裝</h3>
<BR>因為123Duino上面有許多的感測器與OLED等周邊.這些周邊都需要額外安裝程式庫才能正常控制
<BR>以下幾個步驟分別說明各個程式庫的安裝方法與注意事項
<hr>
<BR>1. SHT30 溫度感測器程式庫安裝 (PS:因SHT30與SHT31使用同一個程式庫,所以直接安裝SHT31程式庫即可)
<BR>1-1. Arduino IDE 中選擇 Sketch > Include Library > Manage Libraries ...
<img  src="https://github.com/123duino/123duino/blob/master/Software/img/lib-1.png"  width="70%" height="70%">
<BR>1-2. 在Library Manager 中的filter欄位輸入SHT3,就可找到SHT31.按下install完成安裝
<img src="https://github.com/123duino/123duino/blob/master/Software/img/lib-2.png">
<BR>
<BR>2. 繪圖與OLED 程式庫. 同樣使用如以上SHT30程式庫安裝的方式,分別安裝Adafruit_GFX_Library、Adafruit_SSD1306.
<img  src="https://github.com/123duino/123duino/blob/master/Software/img/lib-3.png"  width="70%" height="70%">
<img  src="https://github.com/123duino/123duino/blob/master/Software/img/lib-4.png"  width="70%" height="70%">
<BR>
<BR>特別注意!!!因Adafruit_SSD1306程式庫預設高度設定為32像素,而123Duino的OLED尺寸的高度是64像素,所以需要以下修改.
<BR>Adafruit_SSD1306.h，改成如下的樣子：
<BR> #define SSD1306_128_64
<BR> // #define SSD1306_128_32
<BR> // #define SSD1306_96_16

<hr>
<BR>
<B>接下來這幾個程式庫無法使用Library Manager 來安裝,須分別下載後自行移動到Arduino Library目錄中</B>
<BR>3. 
