/***************************************************************************************

  WebInit   fonksiyonu STM32F103C8 ve ENC28J60 cipinin init islemlerini yapar
  WebStart  fonksiyonu Server yazilimini baslatir
  AddString fonksiyonu verilen stringi hedef rama yazar ve son adresi dondurur
  PageLoad  fonksiyonu hazirladiginiz web sayfasinini yayinlar ve ziyaretci sayisini okur
  WebPage   fonksiyonu icine kendi kodlarimizi yaziyoruz

  Dikkat: Yayinladiginiz MyWeb alani (array'i) ustunde yapacaginiz her degisiklik aninda
  yayinlanan sayfada da degisime neden olur. Sayfayi bir kez hazirlayip bir daha 
  degistirmeyecekseniz tek MyWeb gibi bir array ile calisabilirsiniz.

  Dinamik sayfa uygulamasi yapacaksaniz iki ayri MyWeb arayi ile calismalisiniz

  Dikkat: Sayfa hazirlamadan ziyaret sayacini okumamalisiniz
  
  Devre semasi ve aciklamalar icin : www.cncdesigner.com/wordpress/?p=6424

****************************************************************************************/

#include "stdio.h"
#include "Web.h"

char MyWeb1[1460];                                  // MYyWeb1 degistiriken MyWeb2'i yayinla
char MyWeb2[1460];                                  // MYyWeb2 degistiriken MyWeb1'i yayinla

char MyMAC [6]={0x77,0x77,0x77,0x77,0x77,0x77};
char MyIP  [4]={95,0,177,1};                        // ipconfig ile ogreneceksiniz ya da modemden ayirdiginiz IP

void SystemInit()
{ 
        WebInit(MyMAC, MyIP);		    // Donanimi ayarla
}

int main() 
{ 
        WebStart();                                 // Serveri calistir
}

void WebPage() 
{
char* index;
char dinamik[50];
int ziyaret=1;
int TextSize=1;

        while(1) 
         {
           index= AddString(MyWeb1,&StartHtml);      
           index= AddString(index, (char*) "<H1>STM32F103C8 - ENC28J60</H1>");
           sprintf(dinamik,"<H%d>Text Size=%d</H%d>",TextSize,TextSize,TextSize);
           index=AddString(index, dinamik);            
           if (++TextSize>6) TextSize=1;
           sprintf(dinamik,"<H4>Ziyaretci: %d </H4>",ziyaret);
           index=AddString(index, dinamik);
           index=AddString(index,&Ver);
           *index=0;                                 // Sayfanin sonuna null karakteri ekler
           ziyaret=PageLoad(MyWeb1);                 // MyWeb1 alani gosterilecek

//         Su anda MyWeb1 yayinda. MyWeb1 uzerindeki html kodlari bozarsak sayfa da bozulur

//         Radikal degisiklikleri MyWeb2 uzerinde yapalim

           index= AddString(MyWeb2,&StartHtml);      
           index= AddString(index, (char*) "<H1>Merhaba, Picproje uyeleri</H1>");
           index=AddString(index,&Ver);
           sprintf(dinamik,"<H2>Ziyaretci: %d </H2>",ziyaret);
           index=AddString(index, dinamik);
           sprintf(dinamik,"<H%d>Text Size=%d</H%d>",TextSize,TextSize,TextSize);
           index=AddString(index, dinamik);            
           if (++TextSize>6) TextSize=1;
           *index=0;                                 // Sayfanin sonuna null karakteri ekler

           ziyaret=PageLoad(MyWeb2);                 // MyWeb2 alani yayinda

//         Su anda MyWeb2 gosterimde. Artik MyWeb1 uzerinde oynayabilirsiniz.  

         }
}

