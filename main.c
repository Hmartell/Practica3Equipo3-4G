#include <18F4620.h>
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
#use fast_io(a)
#use fast_io(b)
#use fast_io(c)

#define RX_232        PIN_C7
#define TX_232        PIN_C6
#use RS232(BAUD=9600, XMIT=TX_232, RCV=RX_232, BITS=8,PARITY=N, STOP=1)

char trama[13];
int contadorDeTrama=0;
int flagTramallena=0,flagValidacionParte1=0,flagValidacionParte2=0;
int contadorNum=0,contadorPosicion=1;
unsigned int numeros[2];
char operacionAri;

void limpiar_Trama(){
   for(int i=0;i<contadorDeTrama;i++)
      trama[i]=0;  
   contadorDeTrama=0;
   flagTramallena=0;
   contadorNum=0;
   contadorPosicion=1;
   numeros[0]=0;
   numeros[1]=0;
}

void llenarTrama(){
   if(kbhit()){
      trama[contadorDeTrama]=getch();
   if(trama[contadorDeTrama]>= 42 && trama[contadorDeTrama]<=62){
      printf("%c",trama[contadorDeTrama]);
      if(trama[contadorDeTrama]== '<'){
         flagTramallena=1;
      }
      contadorDeTrama++;
   }
   }   
}
void validacionTramaParte1(){
   printf("\n\n entro a validacion parte 1 \n\r");
   if(trama[1]== '>' && trama[contadorDeTrama] == '<'){
       flagValidacionParte1=1;        
   }
   else{
        limpiar_Trama();
   }
}
void validacionTramaParte2(){
   printf("\n\n entro a validacion parte 2 \n\r");
   int contadorComas=0;
   for(int i=2;i<contadorDeTrama-2;i++){
      if(trama[i]==',')
         contadorComas++;
   }
   
   if(contadorComas == 2){
      flagValidacionParte2=1;
   }
   else{
      //mensajeError();
      limpiar_Trama();
      return;
   }
      
}
void obtenerNumeros(){
   printf("entro a numeros \r");
   while(trama[contadorPosicion] != ','){
      if(trama[contadorPosicion]>= '0' && trama[contadorPosicion]<='9'){
         printf("entro a while \r");
         numeros[contadorNum]= numeros[contadorNum] * 10 + (trama[contadorPosicion]-'0');
         printf("%i \r",numeros[contadorNum]);
         contadorPosicion++;
      }
   }
}
int operacion(char operacion,int num1[]){
   printf("entro a operacion \r");
   long resultado;
   switch(operacion){
      case '+':
         resultado = (long)num1[0] + (long)num1[1];
         return resultado;
         break;
      case '-':
         resultado = (long)num1[0] - (long)num1[1];
         return resultado;
         break;
      case '*':
         resultado = (long)num1[0] * (long)num1[1];
         return resultado;
         break;
      case '/':
         if(num1[1] == 0){
            resultado = 0x2AAA;
            return resultado;
         }
         else{
               resultado = (long)num1[0] / (long)num1[1];
               return resultado;
            }
         break;
   }
}


void main (void){
   setup_oscillator(OSC_16MHZ);  
   set_tris_c(0xCC);   
   set_tris_b(0xF0);
   set_tris_a(0xC0);
   signed long resultadoFinal;
   

   while(1){
      llenarTrama();
      if(flagTramallena=1){
         if(contadorDeTrama>=7){
            validacionTramaParte1();
            if(flagValidacionParte1=1){
               validacionTramaParte2();
               if(flagValidacionParte2==1){
                  obtenerNumeros();
                  contadorPosicion++;
                  contadorNum++;
                  obtenerNumeros();
                  contadorPosicion++;
                  operacionAri = trama[contadorPosicion];
                  
                  if(numeros[0]<256 && numeros[1]<256){
                     resultadoFinal = operacion(operacionAri,numeros);
                     printf("%li",resultadoFinal);
                     output_a(resultadoFinal);
                     output_b(resultadoFinal>>6);
                     output_c(resultadoFinal>>14);
                     limpiar_Trama();
                     
                  }
                  
               }
               else{
                  limpiar_Trama();
               }
            }
            else{
               limpiar_Trama();
            }
         }
         else{
            limpiar_Trama();
         }
      }
  }
}
