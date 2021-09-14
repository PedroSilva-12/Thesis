//#define check_outputs_config 0
//#define check_device_config 1
#define check_output 2
//#define change_output 3
//#define check_status_init 5
#define important_pins_only 6
//#define non_important_pins_only 7
//#define check_prints_clean 8
//#define other_half

#include <Wire.h>
//Git info
#define IRPS5401_ADDR1  (0x43)
#define IRPS5401_ADDR2  (0x44)
#define IRPS38060_ADDR  (0x45) // Não usado

// ************************************************************************************************************************** //
// *************** Registos e pequena orientação **************************************************************************** //
// ************************************************************************************************************************** //  
// Baseado em  UN0049 e PMBus™ Specification Part II Revision 1.2 *********************************************************** //

#define PAGE  0x00                    // Mudar de pagina, diferentes outputs // USED

#define OPERATION 0x01                // Controla o modo de operação  //USED

#define ON_OFF_CONFIG 0x02            // Controla o enable

#define CLEAR_FAULTS  0x03            // Limpa o registo das falhas

#define PAGE_PLUS_READ 0x06           // Muda de pagina manda comando e le o resultado

#define WRITE_PROTECT 0x10            // Proteja os registods de escrita, para so leitura (+)

#define RESTORE_DEFAULT_ALL 0x12      // Poem o predefenido dos registos, existentes em memoria nao volatil (+)

#define STORE_USER_ALL 0x15           // Guarda o que neste momento esta no registos

#define RESTORE_USER_ALL 0x16         // Restora o que foi guardado como usiarios

#define CAPABILITY  0x19              // PMBus capabilities,sao expostas

#define SMBALERT_MASK 0x1B            // Mascara erros, de forma a nao alterar

#define VOUT_MODE 0x20                // Mode de output voltage , explicado no documento 2

#define VOUT_COMMAND  0x21            // Tensão de saída

#define VOUT_TRIM 0x22                // OFFSET para tensão de saída

#define VOUT_MAX  0x24                // Tensao maxima, nao permitindo um usuario de por a tensao alem de um limite (+)

#define VOUT_MARGIN_HIGH  0x25        // Tensao Margin High

#define VOUT_MARGIN_LOW 0x26          // Tensao Margin Low
  
#define VOUT_TRANSITION_RATE  0x27    // O racio de transicao entre uma tensão e outra por comando

#define VOUT_SCALE_LOOP 0x29          // Racio que escala a tensão
  
#define FREQUENCY_SWITCH  0x33        // A frequencia de switching

#define VIN_ON  0x35                  // Tensão de entrada para conversão de energia

#define VIN_OFF 0x36                  // Tensão de entrada para parar conversão de energia

#define IOUT_CAL_OFFSET 0x39          // O Offset para fazer reset no A output sensing

#define VOUT_OV_FAULT_LIMIT 0x40      // Limite para o Overvoltgate

#define VOUT_OV_FAULT_RESPONSE  0x41  // O que fazer em caso de Overvoltage

#define VOUT_OV_WARN_LIMIT  0x42      // A partir que tensão o warning entra em acção apesar de mascarado

#define VOUT_UV_WARN_LIMIT  0x43      // Semelhante Undervoltage

#define VOUT_UV_FAULT_LIMIT 0x44      // Semelhante Undervoltage

#define VOUT_UV_FAULT_RESPONSE  0x45  // Semelhante Undervoltage

#define IOUT_OC_FAULT_LIMIT 0x46      // Semelhante Overcurrent

#define IOUT_OC_FAULT_RESPONSE  0x47  // Semelhante Overcurrent

#define IOUT_OC_WARN_LIMIT  0x4A      // Semelhante Ovcercurrent

#define OT_FAULT_LIMIT  0x4F          // Semelhante Ovcer Temperature

#define OT_FAULT_RESPONSE 0x50        // Semelhante Ovcer Temperature

#define OT_WARN_LIMIT 0x51            // Semelhante Ovcer Temperature

#define VIN_OV_FAULT_LIMIT  0x55      // Semlhante Input voltage

#define VIN_OV_FAULT_RESPONSE 0x56    // Semlhante Input voltage

#define VIN_UV_WARN_LIMIT 0x58        // Semlhante Input voltage

#define POWER_GOOD_ON 0x5E            // Inicializacao tensoa, sinal opcional

#define POWER_GOOD_OFF  0x5F          // Tensoa negada, sinal opcional

#define TON_DELAY 0x60                // Delay antes de apartir de um comoando, a tensão começa a aumentar

#define TON_RISE  0x61                // Delay para chegar ao valor

#define TON_MAX_FAULT_LIMIT 0x62      // Tempo maximo para ultrapassar o undervoltage

#define TON_MAX_FAULT_RESPONSE  0x63  // Se demorar mais o que faz

#define TOFF_DELAY  0x64              // Semelhante

#define TOFF_FALL 0x65                // Semelhante

#define STATUS_BYTE 0x78              // Dita o erros

#define STATUS_WORD 0x79              // Dita mais

#define STATUS_VOUT 0x7A              // Dita em relacao a tensao de saida

#define STATUS_IOUT 0x7B              // Indentifica em relacao a saida de corrente

#define STATUS_INPUT  0x7C            // Identifica em relacao a entrada

#define STATUS_TEMPERATURE  0x7D      // Em relacao a temperatura

#define STATUS_CML  0x7E              // Em relacao a comunicacao logica e memoria

#define READ_VIN  0x88                // Leitura de tensao de entrada

#define READ_IIN  0x89                // Leitura de corrente de entrada

#define READ_VOUT 0x8B                // Semelhante   

#define READ_IOUT 0x8C                // Semelhante

#define READ_TEMPERATURE_1  0x8D      // Semelhante   

#define READ_POUT 0x96                // Semelhante

#define READ_PIN  0x97                // Semelhante

#define PMBUS_REVISION  0x98          // Leitura de Versao

#define MFR_ID  0x99                  // Manufacturer ID

#define MFR_MODEL 0x9A                // Manufacturer Model

#define MFR_REVISION  0x9B            // Manufacturer Revision Number

#define IC_DEVICE_ID  0xAD            // Device ID

#define IC_DEVICE_REV 0xAE            // Device REV

#define MFR_REG_ACCESS 0x0            // Acesso aos registos de I2C

#define MFR_I2C_ADDRESS 0xD6          // O endereco do I2C

#define MFR_TPGDLY  0xD8              // Tem a haver com o POWER GOOD

#define MFR_FCCM  0xD9                // Forced Continuious Conduction Mode

#define MFR_VOUT_PEAK 0xDB            // Highets VOUT visto

#define MFR_IOUT_PEAK 0xDC            // Corrente mais alta vista

#define MFR_TEMPERATURE_PEAK  0xDD    // Temperatura mais alta

#define MFR_LDO_MARGIN 0xDE           // Margem no LDO

// **************************************************************************************************** //


#if defined(check_device_config)
const char* Operation_Strings[] = {"Immediate Off", "Soft Off", "On", "On Margin Low (Ignore Fault)", "On Margin Low (Act on Fault)", "On Margin High (Ignore Fault)", "On Margin High (Act on Fault)"}; 
const char* OnOffConfigs_Strings[] = { "Turn Off with delay", "Turn Off",
                                       "Enable at Low", "Enable at High",
                                       "Enable pin ignored, dependent only on Operation", "Enable requiered",
                                       "Bit 7 from OPERATION ignored", "Needs bit 7 from OPERATION",
                                       "Conversion as soon as PVin and VCC on, dont need Enable or OPERATION command", "Conversion only with Enable and OPERATION command"};
#endif


// **************************************************************************************************** //
// *************** Classe objeto ********************************************************************** //
// **************************************************************************************************** //  
class IRPS5401{
private:
  uint8_t irps5401_i2caddr;
  int8_t voutModeExponent;
  float iOut[5];
  float iIn[5];
  float vOut[5];
  float vIn[5];
  float pOut[5];
  float pIn[5];
  float pOutAux[5];
  float pInAux[5];
  float pOutTotal;
  float pOutTotalAux;
  float pInTotal; 
  float pInTotalAux;
public:
  IRPS5401(uint8_t addr = IRPS5401_ADDR1);
// i2c
  void wireReadWord(uint8_t reg, uint16_t *value);
  void wireReadByte(uint8_t reg, uint8_t *value);
  void wireReadBlock(uint8_t reg, uint8_t value[6]);
  void wireWriteWord(uint8_t reg, uint16_t value);
  void wireWriteByte (uint8_t reg, uint8_t value);
  void wireSendCmd(uint8_t reg);
  float linearDataFormat11(uint16_t toFormat);

// power supply
  float getVIN(uint8_t page) {
      uint16_t r_data16 = 0;
      float r = 0.0;
      wireReadWord(READ_VIN, &r_data16);
      r=linearDataFormat11(r_data16);
      vIn[page]=r;
      return r;
  }
  float getIIN(uint8_t page) {    
      uint16_t r_data16 = 0;
      float r = 0.0;
      wireReadWord(READ_IIN, &r_data16);
      r = linearDataFormat11(r_data16);    
      iIn[page]=r;
      return r;
  }
  float getPIN(uint8_t page) {  
      uint16_t r_data16 = 0;
      float r = 0.0;
      wireReadWord(READ_PIN, &r_data16);
      r = linearDataFormat11(r_data16);    
      pIn[page]=r;
      return r;
  }
  float getVOUT(uint8_t page) {
      uint16_t r_data16 = 0;
      float r = 0.0;
      wireReadWord(READ_VOUT, &r_data16); 
      r= r_data16*(pow(2,voutModeExponent));
      vOut[page]=r;
      return r;
  }
  float getIOUT(uint8_t page){
    uint16_t r_data16 = 0;
    float r = 0.0;
    wireReadWord(READ_IOUT, &r_data16);
    r=linearDataFormat11(r_data16);
    iOut[page]=r;   
    return r;    
  }
  float getPOUT(uint8_t page){
    uint16_t r_data16 = 0;
    float r = 0.0;    
    wireReadWord(READ_POUT, &r_data16);      
    r=linearDataFormat11(r_data16);
    pOut[page]=r;
    return r;    
  }
  float getPOUT_Total() {
    int i;
    pOutTotal=0.0;
    for(i = 0; i < 5; i++) {
      pOutTotal += pOut[i];
    }
    return pOutTotal;        
  }

  void changePage(uint8_t page_n = 0x00){
     wireWriteByte(PAGE, page_n);
  }

  void getOnOffConfig (uint8_t *r_data8B ){
      uint8_t temp;
      uint8_t onOffConfig;
      
      wireReadByte(ON_OFF_CONFIG, &onOffConfig);
      
#if defined(check_prints_clean)
      Serial.print("ON_OFF_CONFIG clean:   "); Serial.println(onOffConfig,BIN);
#endif      

      temp=onOffConfig>>4 & 0x1;
      r_data8B[4]= temp;
      
      temp=onOffConfig>>3 & 0x1;
      r_data8B[3]= temp;

      temp=onOffConfig>>2 & 0x1;
      r_data8B[2]= temp;
      
      temp=onOffConfig>>1 & 0x1;
      r_data8B[1]= temp;

      temp=onOffConfig & 0x1;
      r_data8B[0]= temp;
  }

  void getOperation (uint8_t *r_data8 = 0){
     wireReadByte(OPERATION, r_data8);

#if defined(check_prints_clean)
     Serial.print("OPERATION clean:  "); Serial.println(*r_data8,BIN);
#endif      
     
     if((*r_data8 >> 6)==0){
        *r_data8=0;
     }
     else if((*r_data8 >> 6)==1){
        *r_data8=1;
     }
     else if((*r_data8 >> 4)==8){
        *r_data8=2;
     }
     else if((*r_data8 >> 2)==37){
        *r_data8=3;
     }
     else if((*r_data8 >> 2)==38){
        *r_data8=4;
     }
     else if((*r_data8 >> 2)==41){
        *r_data8=5;
     }
     else if((*r_data8 >> 2)==42){
        *r_data8=6;
     }
  }  

  void myVoutMode(uint8_t *r_data8){
     wireReadByte(VOUT_MODE, r_data8);
#if defined(check_prints_clean)
     Serial.print("VOUT_MODE clean:  "); Serial.println(*r_data8,HEX);
#endif
     if( (*r_data8 & 0xE0) == 0){ //linear
       *r_data8 = *r_data8 & 0x1F;
       voutModeExponent = (*r_data8) | 0xE0; 
     }
  }

  void getStatus(uint8_t *r_data8B){
     uint16_t r_data16;
     
#if defined(check_prints_clean)
     uint8_t r_data8;
#endif

     wireReadWord(STATUS_WORD, &r_data16);
     //s_word=r_data16;
     r_data8B[0]=r_data16 & 0xFF; //byte
     r_data8B[1]=(r_data16 >>8) & 0xFF;; //word
 
#if defined(check_prints_clean)     
     Serial.print("ST WR "); Serial.println(r_data16,BIN);
#endif

     wireReadByte(STATUS_VOUT, &r_data8B[2]);
     //s_vout=r_data8B[3];
     
#if defined(check_prints_clean)
     wireReadByte(STATUS_VOUT, &r_data8);
     Serial.print("ST VOUT "); Serial.println(r_data8,BIN);
#endif
          
     wireReadByte(STATUS_IOUT, &r_data8B[3]);
     
#if defined(check_prints_clean)
     wireReadByte(STATUS_IOUT, &r_data8);
     Serial.print("ST IOUT "); Serial.println(r_data8,BIN);
#endif
     
     wireReadByte(STATUS_INPUT, &r_data8B[4]);
     //s_input=r_data8B[4];
     
#if defined(check_prints_clean)
     wireReadByte(STATUS_INPUT, &r_data8);
     Serial.print("ST INPUT "); Serial.println(r_data8,BIN);
#endif
     
     wireReadByte(STATUS_TEMPERATURE, &r_data8B[5]);
     //s_temperature=r_data8B[5];

#if defined(check_prints_clean)
     wireReadByte(STATUS_TEMPERATURE, &r_data8);
     Serial.print("ST T "); Serial.println(r_data8,BIN); 
     Serial.println();   
#endif
  }

  float linearDataFormat16(uint16_t toFormat){
     float r= 0.0;
     r=toFormat*(pow(2,voutModeExponent));
     return r;
  }

  float linearDataFormat16_signed(int16_t toFormat){
     float r= 0.0;
     r=toFormat*(pow(2,voutModeExponent));
     return r;
  }
  
};


IRPS5401::IRPS5401(uint8_t addr) {
  irps5401_i2caddr = addr;
}
// **************************************************************************************************** //  


// **************************************************************************************************** //
// *************** I2C Functions ********************************************************************** //
// **************************************************************************************************** //

/*void IRPS5401::wireSendCmd(uint8_t reg) {
  Wire.beginTransmission(irps5401_i2caddr);
  Wire.write(reg);                       // PMBus command
  Wire.endTransmission();
}*/

void IRPS5401::wireWriteByte (uint8_t reg, uint8_t value) {
  Wire.beginTransmission(irps5401_i2caddr);
  Wire.write(reg);                       // PMBus command
  Wire.write(value);                     // byte to write
  Wire.endTransmission();
}

void IRPS5401::wireWriteWord (uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(irps5401_i2caddr);

  Wire.write(reg);                       // PMBus command
  Wire.write(value & 0xFF);              // Lower 8-bits
  Wire.write((value >> 8) & 0xFF);       // Upper 8-bits

  Wire.endTransmission();
}

/*void IRPS5401::wireReadBlock(uint8_t reg, uint8_t value[6])
{
  
  int i;
  uint8_t block_size;

  Wire.beginTransmission(irps5401_i2caddr);
  Wire.write(reg);
  Wire.endTransmission(false);
  
  uint8_t ret=Wire.requestFrom(irps5401_i2caddr,(uint8_t)7);
  if( ret != 2)
  {
    Serial.println( "requestFrom failed");
  }
  else
  {
    block_size=Wire.read();
    for (i=0;i < block_size;i++)
    {
      value[i]=Wire.read();
    }
  }
  
}*/

void IRPS5401::wireReadWord(uint8_t reg, uint16_t *value)
{
  
  Wire.beginTransmission(irps5401_i2caddr);
  Wire.write(reg);
  Wire.endTransmission(false);
  
  uint8_t ret=Wire.requestFrom(irps5401_i2caddr,(uint8_t)2);
  if( ret != 2)
  {
    Serial.println( "requestFrom failed");
  }
  else
  {
    *value = Wire.read();
    *value=((Wire.read() << 8) | *value);
  }
  
}

void IRPS5401::wireReadByte(uint8_t reg, uint8_t *value)
{
  
  Wire.beginTransmission(irps5401_i2caddr);
  Wire.write(reg);
  Wire.endTransmission(false);
  
  uint8_t ret=Wire.requestFrom(irps5401_i2caddr,(uint8_t)1);
  if( ret != 1)
  {
    Serial.println( "RequestFrom failed");
  }
  else
  {
    *value = Wire.read();
  }
  
}


// **************************************************************************************************** //

  float IRPS5401::linearDataFormat11(uint16_t toFormat){
    float r = 0.0;
    int16_t y = 0;
    int8_t x = 0;
    
    y = toFormat & 0x7FF;
    if((y>>10 & 1) == 1){
      y=y|0xF800;
    }
  
    x = (toFormat & 0xF800)>>11;
    if((x>>4 & 1) == 1){
      x=x|0xE0;
    }
  
    return(y*(pow(2,x)));
  }

  float linearDataFormat11(uint16_t toFormat){
    float r = 0.0;
    int16_t y = 0;
    int8_t x = 0;
    
    y = toFormat & 0x7FF;
    if((y>>10 & 1) == 1){
      y=y|0xF800;
    }
  
    x = (toFormat & 0xF800)>>11;
    if((x>>4 & 1) == 1){
      x=x|0xE0;
    }
  
    return(y*(pow(2,x)));
  }


// **************************************************************************************************** //
// *************** APP MAIN *************************************************************************** //
// **************************************************************************************************** //

IRPS5401 REG1(IRPS5401_ADDR1); 
IRPS5401 REG2(IRPS5401_ADDR2); 

void setup() {
  uint8_t r_data8=0;
  uint16_t r_data16=0;
  uint8_t r_data8B[6];
  float tmp;

  Wire.begin();        
  Serial.begin(115200); 
  while(!Serial);

  REG1.changePage(0);
  REG2.changePage(0);
  //REG2.wireWriteByte(STATUS_IOUT,0x20);
  
#if defined(check_device_config) ////////////////////////////////////////////////////////////////////////////////////////

  Serial.println("\n\rConfigurations IRPS5401 - 1");
  REG1.getOperation(&r_data8);
  Serial.print("OPERATION REG_1:    "); Serial.print(r_data8,BIN); Serial.print("  "); Serial.println(Operation_Strings[r_data8]);
  REG1.getOnOffConfig(r_data8B);
  Serial.print("ON_OFF_CONFIG REG_1:    "); Serial.print(r_data8B[0],BIN); Serial.print("  "); Serial.print(OnOffConfigs_Strings[r_data8B[0]]); Serial.print(" | "); 
                                            Serial.print(r_data8B[1],BIN); Serial.print("  "); Serial.print(OnOffConfigs_Strings[r_data8B[1]+2]); Serial.print(" | "); 
                                            Serial.print(r_data8B[2],BIN); Serial.print("  "); Serial.print(OnOffConfigs_Strings[r_data8B[2]+4]); Serial.print(" | "); 
                                            Serial.print(r_data8B[3],BIN); Serial.print("  "); Serial.print(OnOffConfigs_Strings[r_data8B[3]+6]); Serial.print(" | "); 
                                            Serial.print(r_data8B[4],BIN); Serial.print("  "); Serial.println(OnOffConfigs_Strings[r_data8B[4]+8]); 

  //Serial.println();
  
#endif //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  REG1.myVoutMode(&r_data8);  

#if defined(check_device_config) ///////////////////////////////////////////////////////////////////////////////////////

  Serial.print("VOUT_MODE REG_1:   "); Serial.println(r_data8,HEX);
  
#endif //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
#if defined(check_device_config) ////////////////////////////////////////////////////////////////////////////////////////

  Serial.println("\n\rConfigurations IRPS5401 - 2");
  REG2.getOperation(&r_data8);
  Serial.print("Operation REG_2:    "); Serial.print(r_data8,BIN); Serial.print("  "); Serial.println(Operation_Strings[r_data8]); 
  REG2.getOnOffConfig(r_data8B);
  Serial.print("On Off Config REG_2:    "); Serial.print(r_data8B[0],BIN); Serial.print("  "); Serial.print(OnOffConfigs_Strings[r_data8B[0]]); Serial.print(" | "); 
                                            Serial.print(r_data8B[1],BIN); Serial.print("  "); Serial.print(OnOffConfigs_Strings[r_data8B[1]+2]); Serial.print(" | "); 
                                            Serial.print(r_data8B[2],BIN); Serial.print("  "); Serial.print(OnOffConfigs_Strings[r_data8B[2]+4]); Serial.print(" | "); 
                                            Serial.print(r_data8B[3],BIN); Serial.print("  "); Serial.print(OnOffConfigs_Strings[r_data8B[3]+6]); Serial.print(" | "); 
                                            Serial.print(r_data8B[4],BIN); Serial.print("  "); Serial.println(OnOffConfigs_Strings[r_data8B[4]+8]); 

  //Serial.println();
#endif ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  REG2.myVoutMode(&r_data8);  
  
#if defined(check_device_config) ////////////////////////////////////////////////////////////////////////////////////////

  Serial.print("VOUT_MODE 2 :   "); Serial.println(r_data8,HEX);

#endif ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(check_status_init) ///////////////////////////////////////////////////////////////////////////////////////////

  Serial.println("\n\rStatus Initial IRPS5401 - 1");
  
  REG1.getStatus(r_data8B);
  Serial.print("STATUS_BYTE REG_1:   "); Serial.println(r_data8B[0],BIN); 
  
  Serial.print("STATUS_WORD upper REG_1:   "); Serial.println(r_data8B[1],BIN);
  
  Serial.print("STATUS_VOUT REG_1:   "); Serial.println(r_data8B[2],BIN);

  Serial.print("STATUS_IOUT REG_1:   "); Serial.println(r_data8B[3],BIN);
 
  Serial.print("STATUS_INPUT REG_1:   "); Serial.println(r_data8B[4],BIN);
  
  Serial.print("STATUS_TEMPERATURE REG_1:   "); Serial.println(r_data8B[5],BIN);    

  Serial.println("\n\rStatus Initial IRPS5401 - 2");
  
  REG2.getStatus(r_data8B);
  Serial.print("STATUS_BYTE REG_2:   "); Serial.println(r_data8B[0],BIN); 
  
  Serial.print("STATUS_WORD upper REG_2:   "); Serial.println(r_data8B[1],BIN);
  
  Serial.print("STATUS_VOUT REG_2:   "); Serial.println(r_data8B[2],BIN);

  Serial.print("STATUS_IOUT REG_2:   "); Serial.println(r_data8B[3],BIN);
 
  Serial.print("STATUS_INPUT REG_2:   "); Serial.println(r_data8B[4],BIN);
  
  Serial.print("STATUS_TEMPERATURE REG_2:   "); Serial.println(r_data8B[5],BIN);    
  
#endif ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void loop() {  
  
  uint8_t r_data8=0;
  uint8_t r_dataB[6], p;
  uint16_t r_data16=0;
  uint16_t temp;
  float tmp=0.0;
  uint16_t r_data16_signed=0;

  
  for(p = 0; p < 5; p++) 
  {

#if defined(important_pins_only) //////////////////////////////////////////////////////////////////////////////////////////////////////////
 
    if(p != 1 && p != 3){
      continue;
    }
#endif ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(non_important_pins_only) //////////////////////////////////////////////////////////////////////////////////////////////////////
 
    if(p != 4 && p != 1 && p != 0)
      continue;

#endif ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    REG1.changePage(p);
    REG2.changePage(p);
    
    REG1.wireReadByte(PAGE, &r_data8);
    if(r_data8==4)
      Serial.print("LDO");
    else
      Serial.print("SW "); Serial.print(r_data8,DEC);Serial.println(" : ");
    
#if defined(check_outputs_config) //////////////////////////////////////////////////////////////////////////////////////////////////////////
    #if !defined(other_half)
    if(p!=4){
    REG1.wireReadWord(VOUT_COMMAND, &r_data16);
    tmp=REG1.linearDataFormat16(r_data16);
    Serial.print("VOUT_COMMAND 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(VOUT_COMMAND, &r_data16);
    tmp=REG2.linearDataFormat16(r_data16);
    Serial.print("VOUT_COMMAND 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");

    REG1.wireReadWord(VOUT_TRIM , &r_data16_signed);
    tmp=REG1.linearDataFormat16_signed(r_data16_signed);
    Serial.print("VOUT_TRIM 1:   "); Serial.print(r_data16_signed,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(VOUT_TRIM , &r_data16_signed);
    tmp=REG2.linearDataFormat16_signed(r_data16_signed);
    Serial.print("VOUT_TRIM 2:   "); Serial.print(r_data16_signed,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");

    REG1.wireReadWord(VOUT_MAX , &r_data16);
    tmp=REG1.linearDataFormat16(r_data16);
    Serial.print("VOUT_MAX 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(VOUT_MAX , &r_data16);
    tmp=REG2.linearDataFormat16(r_data16);
    Serial.print("VOUT_MAX 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");

    REG1.wireReadWord(VOUT_MARGIN_HIGH , &r_data16);
    tmp=REG1.linearDataFormat16(r_data16);
    Serial.print("VOUT_MARGIN_HIGH 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(VOUT_MARGIN_HIGH , &r_data16);
    tmp=REG2.linearDataFormat16(r_data16);
    Serial.print("VOUT_MARGIN_HIGH 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");

    REG1.wireReadWord(VOUT_MARGIN_LOW , &r_data16);
    tmp=REG1.linearDataFormat16(r_data16);
    Serial.print("VOUT_MARGIN_LOW 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(VOUT_MARGIN_LOW , &r_data16);
    tmp=REG2.linearDataFormat16(r_data16);
    Serial.print("VOUT_MARGIN_LOW 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");

    REG1.wireReadWord(VOUT_TRANSITION_RATE , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("VOUT_TRANSITION_RATE 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,2); Serial.println(" mV/uSec");
    REG2.wireReadWord(VOUT_TRANSITION_RATE , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("VOUT_TRANSITION_RATE 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,2); Serial.println(" mV/uSec");

    REG1.wireReadWord(VOUT_SCALE_LOOP , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("VOUT_SCALE_LOOP 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.println(tmp,1);
    REG2.wireReadWord(VOUT_SCALE_LOOP , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("VOUT_SCALE_LOOP 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.println(tmp,1);
    }
    REG1.wireReadWord(VIN_ON , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("VIN_ON 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(VIN_ON , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("VIN_ON 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");

    REG1.wireReadWord(VIN_OFF , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("VIN_OFF 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(VIN_OFF , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("VIN_OFF 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    
    REG1.wireReadWord(IOUT_CAL_OFFSET , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("IOUT_CAL_OFFSET 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" A");
    REG2.wireReadWord(IOUT_CAL_OFFSET , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("IOUT_CAL_OFFSET 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" A");

    REG1.wireReadWord(VOUT_OV_FAULT_LIMIT , &r_data16);
    tmp=REG1.linearDataFormat16(r_data16);
    Serial.print("VOUT_OV_FAULT_LIMIT 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(VOUT_OV_FAULT_LIMIT , &r_data16);
    tmp=REG2.linearDataFormat16(r_data16);
    Serial.print("VOUT_OV_FAULT_LIMIT 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");

    REG1.wireReadByte(VOUT_OV_FAULT_RESPONSE , &r_data8);
    Serial.print("VOUT_OV_FAULT_RESPONSE 1:   "); Serial.println(r_data8,HEX);
    REG2.wireReadByte(VOUT_OV_FAULT_RESPONSE , &r_data8);
    Serial.print("VOUT_OV_FAULT_RESPONSE 2:   "); Serial.println(r_data8,HEX);
    
    REG1.wireReadWord(VOUT_OV_WARN_LIMIT , &r_data16);
    tmp=REG1.linearDataFormat16(r_data16);
    Serial.print("VOUT_OV_WARN_LIMIT 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(VOUT_OV_WARN_LIMIT , &r_data16);
    tmp=REG2.linearDataFormat16(r_data16);
    Serial.print("VOUT_OV_WARN_LIMIT 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");

    REG1.wireReadWord(VOUT_UV_WARN_LIMIT , &r_data16);
    tmp=REG1.linearDataFormat16(r_data16);
    Serial.print("VOUT_UV_WARN_LIMIT 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(VOUT_UV_WARN_LIMIT , &r_data16);
    tmp=REG2.linearDataFormat16(r_data16);
    Serial.print("VOUT_UV_WARN_LIMIT 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");

    REG1.wireReadWord(VOUT_UV_FAULT_LIMIT , &r_data16);
    tmp=REG1.linearDataFormat16(r_data16);
    Serial.print("VOUT_UV_FAULT_LIMIT 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(VOUT_UV_FAULT_LIMIT , &r_data16);
    tmp=REG2.linearDataFormat16(r_data16);
    Serial.print("VOUT_UV_FAULT_LIMIT 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    if(p!=4){
    REG1.wireReadByte(VOUT_UV_FAULT_RESPONSE , &r_data8);
    Serial.print("VOUT_UV_FAULT_RESPONSE 1:   "); Serial.println(r_data8,HEX);
    REG2.wireReadByte(VOUT_UV_FAULT_RESPONSE , &r_data8);
    Serial.print("VOUT_UV_FAULT_RESPONSE 2:   "); Serial.println(r_data8,HEX);
    }
    
    REG1.wireReadWord(IOUT_OC_FAULT_LIMIT , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("IOUT_OC_FAULT_LIMIT 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" A");
    REG2.wireReadWord(IOUT_OC_FAULT_LIMIT , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("IOUT_OC_FAULT_LIMIT 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" A");

    REG1.wireReadByte(IOUT_OC_FAULT_RESPONSE , &r_data8);
    Serial.print("IOUT_OC_FAULT_RESPONSE 1:   "); Serial.println(r_data8,HEX);
    REG2.wireReadByte(IOUT_OC_FAULT_RESPONSE , &r_data8);
    Serial.print("IOUT_OC_FAULT_RESPONSE 2:   "); Serial.println(r_data8,HEX); //O LDO tem o valor 0, estranho
    #endif
    
    #if defined(other_half)
    REG1.wireReadWord(IOUT_OC_WARN_LIMIT , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("IOUT_OC_WARN_LIMIT 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" A");
    REG2.wireReadWord(IOUT_OC_WARN_LIMIT , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("IOUT_OC_WARN_LIMIT 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" A");

    REG1.wireReadWord(OT_FAULT_LIMIT , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("OT_FAULT_LIMIT 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,1); Serial.println(" ºC");
    REG2.wireReadWord(OT_FAULT_LIMIT , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("OT_FAULT_LIMIT 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,1); Serial.println(" ºC");

    REG1.wireReadByte(OT_FAULT_RESPONSE , &r_data8);
    Serial.print("OT_FAULT_RESPONSE 1:   "); Serial.println(r_data8,HEX);
    REG2.wireReadByte(OT_FAULT_RESPONSE , &r_data8);
    Serial.print("OT_FAULT_RESPONSE 2:   "); Serial.println(r_data8,HEX);
   
    REG1.wireReadWord(OT_WARN_LIMIT , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("OT_WARN_LIMIT 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,1); Serial.println(" ºC");
    REG2.wireReadWord(OT_WARN_LIMIT , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("OT_WARN_LIMIT 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,1); Serial.println(" ºC");

    REG1.wireReadWord(VIN_OV_FAULT_LIMIT , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("VIN_OV_FAULT_LIMIT 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(VIN_OV_FAULT_LIMIT , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("VIN_OV_FAULT_LIMIT 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");

    REG1.wireReadByte(VIN_OV_FAULT_RESPONSE , &r_data8);
    Serial.print("VIN_OV_FAULT_RESPONSE 1:   "); Serial.println(r_data8,HEX);
    REG2.wireReadByte(VIN_OV_FAULT_RESPONSE , &r_data8);
    Serial.print("VIN_OV_FAULT_RESPONSE 2:   "); Serial.println(r_data8,HEX);

    REG1.wireReadWord(VIN_UV_WARN_LIMIT , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("VIN_UV_WARN_LIMIT 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(VIN_UV_WARN_LIMIT , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("VIN_UV_WARN_LIMIT 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    
    REG1.wireReadWord(POWER_GOOD_ON , &r_data16);
    tmp=REG1.linearDataFormat16(r_data16);
    Serial.print("POWER_GOOD_ON 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(POWER_GOOD_ON , &r_data16);
    tmp=REG2.linearDataFormat16(r_data16);
    Serial.print("POWER_GOOD_ON 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");

    REG1.wireReadWord(POWER_GOOD_OFF , &r_data16);
    tmp=REG1.linearDataFormat16(r_data16);
    Serial.print("POWER_GOOD_OFF 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(POWER_GOOD_OFF , &r_data16);
    tmp=REG2.linearDataFormat16(r_data16);
    Serial.print("POWER_GOOD_OFF 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");

    REG1.wireReadWord(TON_DELAY , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("TON_DELAY 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" mS");
    REG2.wireReadWord(TON_DELAY , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("TON_DELAY 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" mS");
    
    if(p!=4){
    REG1.wireReadWord(TON_RISE , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("TON_RISE 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" mS");
    REG2.wireReadWord(TON_RISE , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("TON_RISE 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" mS");
    }
    
    REG1.wireReadWord(TON_MAX_FAULT_LIMIT , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("TON_MAX_FAULT_LIMIT 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" mS");
    REG2.wireReadWord(TON_MAX_FAULT_LIMIT , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("TON_MAX_FAULT_LIMIT 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" mS");

    REG1.wireReadByte(TON_MAX_FAULT_RESPONSE , &r_data8);
    Serial.print("TON_MAX_FAULT_RESPONSE 1:   "); Serial.println(r_data8,HEX);
    REG2.wireReadByte(TON_MAX_FAULT_RESPONSE , &r_data8);
    Serial.print("TON_MAX_FAULT_RESPONSE 2:   "); Serial.println(r_data8,HEX);

    if(p!=4){
    REG1.wireReadWord(TOFF_DELAY , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("TOFF_DELAY 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" mS");
    REG2.wireReadWord(TOFF_DELAY , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("TOFF_DELAY 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" mS");
     
    REG1.wireReadWord(TOFF_FALL , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("TOFF_FALL 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" mS");
    REG2.wireReadWord(TOFF_FALL , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("TOFF_FALL 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" mS");
    }
    
    REG1.wireReadWord(MFR_VOUT_PEAK , &r_data16);
    tmp=REG1.linearDataFormat16(r_data16);
    Serial.print("MFR_VOUT_PEAK 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");
    REG2.wireReadWord(MFR_VOUT_PEAK , &r_data16);
    tmp=REG2.linearDataFormat16(r_data16);
    Serial.print("MFR_VOUT_PEAK 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" V");

    REG1.wireReadWord(MFR_IOUT_PEAK , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("MFR_IOUT_PEAK 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" A");
    REG2.wireReadWord(MFR_IOUT_PEAK , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("MFR_IOUT_PEAK 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" A");


    REG1.wireReadWord(MFR_TEMPERATURE_PEAK , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("MFR_TEMPERATURE_PEAK 1:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" ºC");
    REG2.wireReadWord(MFR_TEMPERATURE_PEAK , &r_data16);
    tmp=linearDataFormat11(r_data16);
    Serial.print("MFR_TEMPERATURE_PEAK 2:   "); Serial.print(r_data16,HEX);Serial.print(" = "); Serial.print(tmp,5); Serial.println(" ºC");
    #endif
#endif /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(check_output) //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    tmp = REG1.getVIN(p);  
    Serial.print("VIN: "); Serial.print(tmp,5); Serial.print ("V  ");

    // O consumo de corrente nao e possivel ver no LDO.
    if(p!=4){
      tmp = REG1.getIIN(p);
      Serial.print("IIN: "); Serial.print(tmp,5); Serial.print("A  ");
    }
    
    tmp = REG1.getPIN(p);
    Serial.print("PIN: "); Serial.print(tmp,5);Serial.print("W  ");

    tmp = REG1.getPIN(p);
    Serial.print("PIN: "); Serial.print(tmp,5);Serial.print("W  ");
      
    tmp = REG1.getVOUT(p);
    Serial.print("VOUT: "); Serial.print(tmp,5);Serial.print ("V  ");
    
    tmp = REG1.getIOUT(p);
    Serial.print("IOUT: "); Serial.print(tmp,5);Serial.print ("A  ");
    
    tmp = REG1.getPOUT(p);
    Serial.print("POUT: "); Serial.print(tmp,5);Serial.print ("W  ");

    REG1.wireReadWord(READ_TEMPERATURE_1, &r_data16);
    Serial.print("READ_TEMPERATURE_1:   "); Serial.print(r_data16 & 0X7ff);   Serial.println("ºC");

    tmp = REG2.getVIN(p);  
    Serial.print("VIN: "); Serial.print(tmp,5); Serial.print ("V  ");

    // O consumo de corrente nao e possivel ver no LDO.
    if(p!=4){
      tmp = REG2.getIIN(p);
      Serial.print("IIN: "); Serial.print(tmp,5); Serial.print("A  ");
    }
    
    tmp = REG2.getPIN(p);
    Serial.print("PIN: "); Serial.print(tmp,5);Serial.print("W  ");

    tmp = REG2.getPIN(p);
    Serial.print("PIN: "); Serial.print(tmp,5);Serial.print("W  ");
      
    tmp = REG2.getVOUT(p);
    Serial.print("VOUT: "); Serial.print(tmp,5);Serial.print ("V  ");
    
    tmp = REG2.getIOUT(p);
    Serial.print("IOUT: "); Serial.print(tmp,5);Serial.print ("A  ");
    
    tmp = REG2.getPOUT(p);
    Serial.print("POUT: "); Serial.print(tmp,5);Serial.print ("W  ");

    REG2.wireReadWord(READ_TEMPERATURE_1, &r_data16);
    Serial.print("READ_TEMPERATURE_1:   "); Serial.print(r_data16 & 0X7ff);   Serial.println("ºC");

#endif /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    Serial.println();

  }
/*
#if defined(check_output) //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  tmp = REG1.getPOUT_Total();
  Serial.print("Total Pout by sum: "); Serial.print(tmp,8); Serial.println(" W"); 

#endif /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
  delay(100);
  while(analogRead(A0) < 1000){}
}

// ****************************************************************************************************************************************** //
