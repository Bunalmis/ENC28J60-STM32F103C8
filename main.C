/*
    www.cncdesigner.com 
    facebook.com/Cnc-Designer-337826782908966
    Dikkat: TexCopy ile yerlestireceginiz datalarin toplam uzunlugu
    en fazla 1460 byte olabilir. Program kontrol etmez
    Demo program
*/

#include "stm32f10x.h"
#include "Web.h"
#include "stdio.h"
#include "string.h"

unsigned counter=0;                                 // Ziyaretci sayaci
char  DestinationMAC [6];                           // Paketin geldigi cihazin MAC adresi
char  DestinationIP[4];                             // IP paketin geldigi cihazin IP nosu
short DestinationPort[2];                           // IP paketin gonderen cihazin port nosu

char text[128];                                     // Dinamik islemler icin yazboz alani

char Cikis=0;                                       // Cihazlari calistirma/durdurma bilgisi
char Giris=0;                                       // Switch bilgileri

unsigned short PortA; 
unsigned short PortB;
unsigned short PortC;
short ADC_CH0;
short ADC_CH1;

char MyMAC [6]={0x77,0x77,0x77,0x77,0x77,0x77};
char MyIP  [4]={192,168,1,7};                       // ipconfig ile ogreneceksiniz ya da modemden ayirdiginiz IP

void AnaSayfa(void);
void Sayfa1(char*, int);
void Sayfa2(char*, int);
void Sayfa3(char*, int);
void Sayfa4(char*, int);

void SystemInit()
{ 
           WebInit(MyMAC, MyIP);		    // Donanimi ayarla
}

int main() 
{ 
           WebStart();                              // Serveri calistir
}

/*************************************************************************************
           Sayfada Link veren html dizilimi
*************************************************************************************/

void  LinkVer(char SayfaNo)
{
           sprintf(text,"<a href=~http://%d.%d.%d.%d/%c~>",MyIP[0],MyIP[1],MyIP[2],MyIP[3],SayfaNo); TextCopy(text);
}

/*************************************************************************************
           Yeni html sayfaya baslarken html kod dizilisi 
*************************************************************************************/

void Baslik()
{
           TextCopy((char*) "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
           TextCopy((char*) "<div align=~center~>");
           TextCopy((char*) "<table border=~0~ width=~620~><tr><td colspan=~4~ align=~center~ height=~50~>");
           TextCopy((char*) "<p><font size=~6~ color=~blue~>STM32F103C8-ENC28J60 Web Server</font></p></td></tr>");
//         TextCopy((char*) "<tr><td colspan=~4~ align=~center~><img src=~http://www.cncdesigner.com/asm.gif~></td></tr>");
}

/*************************************************************************************
           Basligi kapatan bölüm 
*************************************************************************************/

void Bitir(char ver)
{
           if (ver) 
              {
                TextCopy((char*)"<tr><td height=~100~colspan=~4~ align=~center~>");
                LinkVer(' ');
                TextCopy(text); 
                TextCopy((char*)"<p><font size=~4~ color=~green~>Ana Sayfaya geri dön</font></p>");
              }
           TextCopy((char*)"</td></tr></table></div>");
}

/*************************************************************************************
           Serverde bulunmayan bir sayfa izlenmek isteniyor 
*************************************************************************************/

void SayfaYok()
{
           Baslik();
           TextCopy((char*) "<tr><td colspan=~4~ align=~center~><b><p><font size=~4~ color=~red~>Hata 404</font></p></b>");
           TextCopy((char*) "<tr><td colspan=~4~ align=~center~><b><p><font size=~4~ color=~red~>Böyle bir sayfa bulunmamaktadir</font></p></b>");
           Bitir(1); 
}          

/*************************************************************************************
           Servera gelen sayfa izleme talebi bu rutine yonlendirilir
           Datalar TCP paket icinde gelen verileri saklayan array dir
           Len, Datalar alanindaki verilerin byte cinsinden uzunlugudur
           Not: Datalar alani siz sayfa hazirlamaya basladiginizda bozulur
*************************************************************************************/

void WebPage(char* Datalar, int len) 
{
short SayfaNo;

           counter++;

           SayfaNo=' ';
           if      (ThereAre((char*)"GET " )) SayfaNo=Datalar[5];     // Sayfa adlarini tek karakterle olusturun
           else if (ThereAre((char*)"POST ")) SayfaNo=Datalar[6];     // Yada bu kismi yeniden kodlayin
             
           switch(SayfaNo)
             {
               case ' ':  AnaSayfa(); break;
               case '1':  Sayfa1(Datalar, len)  ; break;
               case '2':  Sayfa2(Datalar, len)  ; break;
               case '3':  Sayfa3(Datalar, len)  ; break;
               case '4':  Sayfa4(Datalar, len)  ; break;
               default:   SayfaYok();
             }
}


/*************************************************************************************
           Ana sayfamiz
*************************************************************************************/

void AnaSayfa()
{
           Baslik();
           TextCopy((char*)"<tr><td width=~155~ align=~center~>"); LinkVer('1'); TextCopy(text); TextCopy((char*)"<p><font size=~4~ color=~green~>MCU Bilgileri</font></p></td>");
           TextCopy((char*)"<td width=~155~ align=~center~>"); LinkVer('2'); TextCopy(text);     TextCopy((char*)"<p><font size=~4~ color=~green~>Cihazlari ac kapat</font></p></td>");
           TextCopy((char*)"<td width=~155~ align=~center~>"); LinkVer('3'); TextCopy(text);     TextCopy((char*)"<p><font size=~4~ color=~green~>Sifreli menu</font></p></td>");
           TextCopy((char*)"<td width=~155~ align=~center~>"); LinkVer('4'); TextCopy(text);     TextCopy((char*)"<p><font size=~4~ color=~green~>Bakim menusu</font></p></td></tr>");
           Bitir(0);
}

/*************************************************************************************
           1. sayfa
*************************************************************************************/

void Sayfa1(char* Datalar, int len)
{

           Baslik();
           TextCopy((char*) "<tr><td colspan=~4~ align=~center~height=~100~><b><p><font size=~4~ color=~black~>MCU Bilgileri</font></p></b>");
           TextCopy((char*)"<tr><td colspan=~4~ align=~center~><table border=~0~ width=~200~ align=~center~>");
           sprintf(text,"<tr><td width=~80~ align=~center~><p>GPIO A</p></td> <td width=~120~ align=~center~><p>0x%04X</p></td></tr>",PortA);TextCopy(text);
           sprintf(text,"<tr><td width=~80~ align=~center~><p>GPIO B</p></td> <td width=~120~ align=~center~><p>0x%04X</p></td></tr>",PortB);TextCopy(text);
           sprintf(text,"<tr><td width=~80~ align=~center~><p>GPIO C</p></td> <td width=~120~ align=~center~><p>0x%04X</p></td></tr>",PortC);TextCopy(text);
           sprintf(text,"<tr><td width=~80~ align=~center~><p>An.Ch1</p></td> <td width=~120~ align=~center~><p>0x%04X</p></td></tr>",ADC_CH0);TextCopy(text);
           sprintf(text,"<tr><td width=~80~ align=~center~><p>An.Ch2</p></td> <td width=~120~ align=~center~><p>0x%04X</p></td></tr>",ADC_CH1);TextCopy(text); TextCopy((char*)"</table></td></tr>");
           Bitir(1);	
}

/*************************************************************************************
           2. sayfa
*************************************************************************************/

void Sayfa2(char* Datalar, int len)
{
          
           if      (ThereAre((char*)"A=0")) Cikis=(Cikis & ~0x01);
           else if (ThereAre((char*)"A=1")) Cikis=(Cikis |  0x01);

           if      (ThereAre((char*)"B=0")) Cikis=(Cikis & ~0x02);
           else if (ThereAre((char*)"B=1")) Cikis=(Cikis |  0x02);


           Baslik();
           TextCopy((char*) "<tr><td colspan=~4~ align=~center~height=~100~><b><p><font size=~4~ color=~black~>Cihazlari acip kapama ve durum ogrenme menusu</font></p></b></td></tr>");
           TextCopy((char*) "<form method=~post~>");
            
           TextCopy((char*) "<tr><td width=~200~><b><p>Kombi ");
           if (Cikis&0x01) TextCopy((char*) "Calisiyor</p></b></td>");
                      else TextCopy((char*) "Calismiyor</p></b></td>");
           TextCopy((char*) "<td><input type=~radio~; name=~A~; value=~1~> Calistir</td><td><input type=~radio~; name=~A~; value=~0~> Durdur</td></tr>");

           TextCopy((char*) "<tr><td><b><p>Kettle ");
           if (Cikis&0x02) TextCopy((char*) "Calisiyor</p></b></td>");
                      else TextCopy((char*) "Calismiyor</p></b></td>");
           TextCopy((char*) "<td><input type=~radio~; name=~B~; value=~1~> Calistir</td><td><input type=~radio~; name=~B~; value=~0~> Durdur</td></tr>");

           TextCopy((char*) "<tr><td><b><p>Pencere ");
           if (Giris&0x01) TextCopy((char*) "Acik</p></b></td></tr>");         else TextCopy((char*) "kapali</p></b></td></tr>");

           TextCopy((char*) "<tr><td><b><p>Kapi ");
           if (Giris&0x02) TextCopy((char*) "Acik</p></b></td></tr>");         else TextCopy((char*) "kapali</p></b></td></tr>");
													 
           TextCopy((char*) "<tr><td><b><p>Kedi ");
           if (Giris&0x04) TextCopy((char*) "disari cikti</p></b></td></tr>"); else TextCopy((char*)"içeri girdi</p></b></td></tr>");
													 
           TextCopy((char*) "<tr><td><b><p>Iceride ");
           if (Giris&0x08) TextCopy((char*) "birisi var</p></b></td></tr>");   else TextCopy((char*) "kimse yok</p></b></td></tr>");

           TextCopy((char*) "<tr><td colspan=~4~ align=~center~height=~100~><b><p><font size=~4~ color=~black~>Kombi ve Kettle PA0 ve PA1 pinlerine baglidir</font></p></b></td></tr>");

           TextCopy((char*) "<tr><td colspan=~4~ align=~center~height=~80~><b><p><input type=~submit~ value=~Gonder~></td></tr>");
           Bitir(1);
}

/*************************************************************************************
           3. sayfa
*************************************************************************************/

void Sayfa3(char* Datalar, int len)
{
           if ((ThereAre((char*)"Z=123456"))&&(ThereAre((char*)"W=Hirsizi+Vur")))
             {
               Baslik();
               TextCopy((char*) "<tr><td colspan=~4~ align=~center~ height=~150~><b><p><font size=~4~ color=~black~>Gizli Gorev Sayfasi</font></p></b>");
               TextCopy((char*) "<tr><td colspan=~4~ align=~center~><b><p>Isteginiz yerine getirildi</p></b></td></tr>");
             }
           else if ((ThereAre((char*)"Z="))||(ThereAre((char*)"W=")))
             {
               Baslik();
               TextCopy((char*) "<tr><td colspan=~4~ align=~center~ height=~150~><b><p><font size=~4~ color=~black~>Gizli Gorev Sayfasi</font></p></b>");
               TextCopy((char*) "<tr><td colspan=~4~ align=~center~><b><p>Yetkiniz yok ya da yanlis emir verdiniz</p></b></td></tr>");
             }
           else
             {
               Baslik();
               TextCopy((char*) "<tr><td colspan=~4~ align=~center~ height=~150~><b><p><font size=~4~ color=~black~>Gizli Gorev Sayfasi ( Gorev: Hirsizi Vur  Sifre: 123456 )</font></p></b>");
             }

           TextCopy((char*) "<form method=~post~>");
           TextCopy((char*) "<tr><td></td></tr>");
           TextCopy((char*) "<tr><td colspan=~4~ align=~center~height=~50~><b><p>Gorev  <input type=~text~; name=~W~></p></b></td></tr>");
           TextCopy((char*) "<tr><td colspan=~4~ align=~center~height=~50~><b><p>Sifre  <input type=~password~; name=~Z~></p></b></td></tr>");
           TextCopy((char*) "<tr><td colspan=~4~ align=~center~height=~50~><b><b><p><input type=~submit~ value=~Gonder~></td></tr>");
           TextCopy((char*) "<tr><td></td></tr>");
           Bitir(1);	 
}

/*************************************************************************************
           4. sayfa
*************************************************************************************/

void Sayfa4(char* Datalar, int len)
{
           Baslik();
           TextCopy((char*) "<tr><td colspan=~4~ align=~center~height=~100~><b><p><font size=~4~ color=~black~>Bakim menusu</font></p></b>");
           Bitir(1);
}

/*************************************************************************************
           Serverdan bagimsiz calisan kullanici kodlari

           Mevcut yapida asagidaki yapilandirmalar yapilmistir fakat degistirebilirsiniz

           A0..7 Output
           B8..B9 ADC 
           B10..B15 Input
           PC15..PC13 Input

           A15, B3, B4, B5, B6 pin tanimlarini degistiremezsiniz. (SPI1 reserve)
           
*************************************************************************************/

void STM32F103()
{
           while(1)
             {
               PortA=GPIOA->IDR&0x0FFFF;
               PortB=GPIOB->IDR&0x0FFFF;
               PortC=GPIOC->IDR&0x0FFFF;

               ADC_CH0= ADC1->JDR1;
               ADC_CH1= ADC1->JDR2;
               if (!(ADC1->CR2&0x200000)) ADC1->CR2|=0x200000;

               GPIOA->BSRR=Cikis&~0x8000;                     // A15 Sisteme ait (reserve)
               GPIOA->BRR=~Cikis&~0x8000;                     // A15 Sisteme ait (reserve)

               Giris=(GPIOB->IDR>>8)&~0x03;                   // B8 B9 ADC
             }  
}
 
