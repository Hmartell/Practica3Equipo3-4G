#include <18F4620.h>
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)

#use fast_io (b)
#use fast_io (d)

   #define RX_232        PIN_C7
   #define TX_232        PIN_C6
   #use RS232(BAUD=9600, XMIT=TX_232, RCV=RX_232, BITS=8,PARITY=N, STOP=1)
   //#use fast_io(c)

void main (void){
   setup_oscillator(OSC_16MHZ);
   printf("Hola Mundo\n");//Puedes usar putc o printf. Revisa la documentaciÃ³n de CCS para ver que mas puedes hacer.

   set_tris_c(0x00);   
   set_tris_b(0x00);
   set_tris_a(0x00);
   
   char cadena[11];
   int contador=0, contadorAuxiliar=1;
   int banderaOperacion=0, banderaImprimir=0, banderaLeer=0;
   char operador=00;
   unsigned int operando1=0, operando2=0;
   signed int32 resultado=0;
   //<000,000,+>
   while(1){
      if(kbhit()){
         cadena[contador]=getch();
         if(cadena[contador]>=48 && cadena[contador]<=57){
            printf("%c", cadena[contador]);
            contador++;
         }
         else{
            if(cadena[contador]== 42 || cadena[contador]== 43  || cadena[contador]== 45 || cadena[contador] == 47){
               printf("%c", cadena[contador]);
               contador++;
            }
            else{
               if((cadena[contador]==60 || cadena[contador]== 62) || (cadena[contador]==44)){
                  if(cadena[contador]==62){
                  banderaLeer=1;
                  }
                  printf("%c", cadena[contador]);
                  contador++;
                  }
               }
            }
         }
         if(banderaLeer==1){
            printf("Evaluando cadena\n");
            contadorAuxiliar=1;
            while (cadena[contadorAuxiliar] != ','){
               printf("entro a obtener numero\n");
               operando1 = operando1 * 10 + (cadena[contadorAuxiliar] - '0');
               contadorAuxiliar++;
            }
            contadorAuxiliar++;
            while (cadena[contadorAuxiliar] != ','){
               operando2 = operando2 * 10 + (cadena[contadorAuxiliar] - '0');
               contadorAuxiliar++;
            }
            contadorAuxiliar++;
            if(cadena[contadorAuxiliar]=='+' || cadena[contadorAuxiliar]=='-' || (cadena[contadorAuxiliar]=='*' || cadena[contadorAuxiliar]=='/')){
               operador = cadena[contadorAuxiliar];
               banderaOperacion=1;
            }
            contadorAuxiliar=0;
            banderaLeer=0;
      }
      
      if(banderaOperacion == 1){
         switch(operador){
            case '+':
            banderaImprimir=1;
               printf("Sumando\n");
               resultado=(long)operando1 + (long)operando2;
               
               banderaOperacion=0;
               
            break;
            
            case '-':  
            banderaImprimir=1;
               resultado=(long)operando1 - (long)operando2;
             
               banderaOperacion=0;
               
            break;
            
            case '*':
            banderaImprimir=1;
               resultado=(long)operando1 * (long)operando2;
               
               banderaOperacion=0;
            break;
            
            case '/':
               if(operando2==0){
                  printf(" !!ERROR¡¡\n ");
                  output_a(0xff);
                  output_b(0xff>>6);
                  output_c(0xff>>14);
                  delay_ms(500);
                  output_a(0x00);
                  output_b(0x00>>6);
                  output_c(0x00>>14);
                  delay_ms(500);
                  contador=0;
                  banderaOperacion=0;
               }
               else{
                  resultado=operando1 / operando2;
                  banderaImprimir=1;
                  banderaOperacion=0;
               }
            break;
         }
      }
      if(banderaImprimir==1){
         printf("%f", (float) resultado);
         output_a((long)resultado);
         output_b((long)resultado >> 6);
         output_c((long)resultado >> 14)
         delay_ms(500);
         banderaImprimir=0;
         resultado=0;
         operando1=0;
         operando2=0;
         for(int i=0; i<=contador; i++){
            cadena[i]=00;
         }
         contador=0;
         
      }
   }
}
