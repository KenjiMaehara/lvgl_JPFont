#include <Arduino.h>
#include <SPI.h>
//#include <SD.h>
#include <env.h>
#include <Preferences.h>
#include "common.h"

const uint16_t mStrInitNameS_01[] = {0x76d7,0x96e3,0x0000}; /* 盗難 */
const uint16_t mStrInitNameS_02[] = {0x706b,0x707d,0x0000}; /* 火災 */
const uint16_t mStrInitNameS_03[] = {0x4e88,0x5099,0x0031,0x0000}; /* 予備1 */
const uint16_t mStrInitNameS_04[] = {0x4e88,0x5099,0x0032,0x0000}; /* 予備2 */
const uint16_t mStrInitNameS_05[] = {0x4e88,0x5099,0x0033,0x0000}; /* 予備3*/
const uint16_t mStrInitNameS_06[] = {0x4e88,0x5099,0x0034,0x0000}; /* 予備4*/
const uint16_t mStrInitNameS_07[] = {0x4e88,0x5099,0x0035,0x0000}; /* 予備5*/
const uint16_t mStrInitNameS_08[] = {0x4e88,0x5099,0x0036,0x0000}; /* 予備6*/

const uint16_t *mStrInitSensor[] = {mStrInitNameS_01,mStrInitNameS_02,mStrInitNameS_03,mStrInitNameS_04,mStrInitNameS_05,mStrInitNameS_06,mStrInitNameS_07,mStrInitNameS_08};


const uint16_t mStrInitNameM_01[] = {0x0043,0x0048,0x0031,0x0000}; /* CH1 */
const uint16_t mStrInitNameM_02[] = {0x0043,0x0048,0x0032,0x0000}; /* CH2 */
const uint16_t mStrInitNameM_03[] = {0x0043,0x0048,0x0033,0x0000}; /* CH3 */
const uint16_t mStrInitNameM_04[] = {0x0043,0x0048,0x0034,0x0000}; /* CH4 */

const uint16_t *mStrInitMulti[] = {mStrInitNameM_01,mStrInitNameM_02,mStrInitNameM_03,mStrInitNameM_04};


const uint16_t mStrInitNameT_01[] = {0x5185,0x90e8,0x30bf,0x30f3,0x30d1,0x0000}; /* ?部タンパ */  
const uint16_t mStrInitNameT_02[] = {0x5916,0x90e8,0x30bf,0x30f3,0x30d1,0x0000}; /* 外部タンパ */
const uint16_t mStrInitNameT_03[] = {0xff37,0x30fb,0x30bf,0x30f3,0x30d1,0x0000}; /* Ｗ?タンパ */  
const uint16_t mStrInitNameT_04[] = {0x505c,0x96fb,0x0000}; /* 停電 */                              

const uint16_t mStrInitNameW_01[] = {0x0057,0x0043,0x0048,0x0031,0x0000}; /* CH1 */
const uint16_t mStrInitNameW_02[] = {0x0057,0x0043,0x0048,0x0032,0x0000}; /* CH2 */

const uint16_t *mStrInitWire[] = {mStrInitNameW_01,mStrInitNameW_02};

const uint16_t *mStrInitTampa[] = {mStrInitNameT_01,mStrInitNameT_02,mStrInitNameT_03,mStrInitNameT_04};


void env_default(env_t * env);


env_t   Env;
env_hdr_t hdrEnv;


#if 0
static const uint8_t crc8_table[256] =
{
     0,  94, 188, 226,  97,  63, 221, 131, 194, 156, 126,  32, 163, 253,  31,  65,
   157, 195,  33, 127, 252, 162,  64,  30,  95,   1, 227, 189,  62,  96, 130, 220,
    35, 125, 159, 193,  66,  28, 254, 160, 225, 191,  93,   3, 128, 222,  60,  98,
   190, 224,   2,  92, 223, 129,  99,  61, 124,  34, 192, 158,  29,  67, 161, 255,
    70,  24, 250, 164,  39, 121, 155, 197, 132, 218,  56, 102, 229, 187,  89,   7,
   219, 133, 103,  57, 186, 228,   6,  88,  25,  71, 165, 251, 120,  38, 196, 154,
   101,  59, 217, 135,   4,  90, 184, 230, 167, 249,  27,  69, 198, 152, 122,  36,
   248, 166,  68,  26, 153, 199,  37, 123,  58, 100, 134, 216,  91,   5, 231, 185,
   140, 210,  48, 110, 237, 179,  81,  15,  78,  16, 242, 172,  47, 113, 147, 205,
    17,  79, 173, 243, 112,  46, 204, 146, 211, 141, 111,  49, 178, 236,  14,  80,
   175, 241,  19,  77, 206, 144, 114,  44, 109,  51, 209, 143,  12,  82, 176, 238,
    50, 108, 142, 208,  83,  13, 239, 177, 240, 174,  76,  18, 145, 207,  45, 115,
   202, 148, 118,  40, 171, 245,  23,  73,   8,  86, 180, 234, 105,  55, 213, 139,
    87,   9, 235, 181,  54, 104, 138, 212, 149, 203,  41, 119, 244, 170,  72,  22,
   233, 183,  85,  11, 136, 214,  52, 106,  43, 117, 151, 201,  74,  20, 246, 168,
   116,  42, 200, 150,  21,  75, 169, 247, 182, 232,  10,  84, 215, 137, 107,  53
};



uint8_t crc8( uint8_t *data , long size )  
{  
    long lp;
    uint8_t crc = 0;

	for( lp = 0; lp < size; lp++ )
	{
		 crc = crc8_table[ crc ^ data[lp] ] ;
	}
    
    return crc;
} 
#endif


uint8_t crc8(const uint8_t *data, size_t size) {
  uint8_t crc = 0xFF; // 初期値
  for (size_t i = 0; i < size; i++) {
    crc ^= data[i]; // データバイトでCRCをXOR
    for (uint8_t j = 0; j < 8; j++) { // 各ビットについて
      if (crc & 0x80) { // 最上位ビットが1かどうかをチェック
        crc = (crc << 1) ^ 0x07; // 1の場合は左にシフトしてポリノミアルでXOR
      } else {
        crc <<= 1; // 0の場合は左にシフトのみ
      }
    }
  }
  return crc; // 計算されたCRC値
}


void env_update_dev()
{
//    flash_task_write((u32)FLASH_DEV_ADDR,(u8 *)&g_dev,sizeof(dev_t));
//    OSTimeDly(1000);
//    env_init_dev();
}


#if 0
void env_save(env_t * src,env_hdr_t * hdr)
{
    int err= 0;
    uint8_t  crc;
    uint32_t size = sizeof(env_t) + sizeof(env_hdr_t);


    hdr->crc = crc8((uint8_t *)src,sizeof(env_t));

    File envFile = SD.open("env.bin", FILE_WRITE);

    envFile.seek(0);
    envFile.write((uint8_t * )hdr,sizeof(env_hdr_t));
    envFile.write((uint8_t * )src,sizeof(env_t));
    envFile.close();
}
#endif

void env_save(env_t *src, env_hdr_t *hdr) {
    // データのシリアライズ
    size_t srcSize = sizeof(env_t);
    size_t hdrSize = sizeof(env_hdr_t);
    uint8_t* envData = new uint8_t[srcSize + hdrSize]; // 動的にメモリ割り当て

    // CRC計算とヘッダーの設定
    hdr->crc = crc8((uint8_t*)src, srcSize);
    memcpy(envData, hdr, hdrSize);
    memcpy(envData + hdrSize, src, srcSize);

    // Preferencesに保存
    preferences.begin("my-app", false);
    preferences.putBytes("env", envData, srcSize + hdrSize);
    preferences.end();

    delete[] envData; // 動的に割り当てたメモリの解放
}


bool env_load(env_t *dst, env_hdr_t *hdr) {
  // 構造体サイズの合計を計算
  uint32_t size = sizeof(env_t) + sizeof(env_hdr_t);
  uint8_t envData[size]; // データを格納するバイト配列

  // Preferencesからデータを読み出し
  preferences.begin("my-app", true); // 読み取り専用でPreferencesを開始
  if (preferences.getBytesLength("env") != size) {
    preferences.end(); // サイズが一致しない場合、失敗
    return false;
  }
  preferences.getBytes("env", envData, size);
  preferences.end(); // Preferencesを終了

  // ヘッダー情報をコピー
  memcpy(hdr, envData, sizeof(env_hdr_t));
  // CRCチェック
  uint8_t crc = crc8(envData + sizeof(env_hdr_t), sizeof(env_t));
  if (crc != hdr->crc) {
    return false; // CRCが一致しない場合、失敗
  }

  // 構造体データをコピー
  memcpy(dst, envData + sizeof(env_hdr_t), sizeof(env_t));
  return true; // 成功
}

#if 0
void env_init(void)
{

    if (!SD.begin(4)) 
    {
        Serial.println("SD initialization failed!");
        return;
    }

    if (SD.exists("env.bin")) 
    {
        Serial.println("env.bin doesn't exist.");
        hdrEnv.identify[0] = 0x19;
        hdrEnv.identify[1] = 0xf1;
        hdrEnv.identify[2] = 0xca;
        env_default(&Env);
        env_save(&Env,&hdrEnv);
    }

     
    Serial.println("SD initialization done.");

    File envFile = SD.open("env.bin", FILE_READ);

    //envFile.read(&hdrEnv,sizeof(env_hdr_t));
    //envFile.read(&Env,sizeof(env_hdr_t));
    //envFile.read(&hdrEnv,sizeof(hdrEnv));
    //envFile.read(&Env,sizeof(hdrEnv));

	envFile.read((uint8_t * )&hdrEnv,sizeof(env_hdr_t));
	envFile.read((uint8_t * )&Env,sizeof(env_t));

    envFile.close();

    if(hdrEnv.identify[0] != 0x19 && hdrEnv.identify[1] != 0xf1 && hdrEnv.identify[2] != 0xca)
    {
        hdrEnv.identify[0] = 0x19;
        hdrEnv.identify[1] = 0xf1;
        hdrEnv.identify[2] = 0xca;
        env_default(&Env);
        env_save(&Env,&hdrEnv);
    }

    Serial.println("Env initialization done.");
}
#endif


void env_init(void) {

  Serial.println("-------------env_init-------------start  1");
  env_default(&Env);
  Serial.println("-------------env_init-------------  2");
  env_save(&Env, &hdrEnv); 
  Serial.println("-------------env_init-------------  3");
  env_load(&Env, &hdrEnv);
  Serial.println("-------------env_init-------------  4");


  Serial.println("Env initialization done.");
}




void set_default_msg(int ch,a_msg_t * msg)
{	
	msg->set[0]   = 33;
	msg->set[1]   =  9;
	msg->set[2]   = (ch == 8) ? 11 : (12 + ch);
	msg->set[3]   = 31;
	msg->set[4]   =  0;
	msg->set[5]   =  0;
	msg->reset[0] = 33;
	msg->reset[1] = (ch == 8) ? 11 : (12 + ch);
	msg->reset[2] = 41;
	msg->reset[3] =  0;
	msg->reset[4] =  0;
	msg->reset[5] =  0;
	msg->sec_set[0]   = 33;
	msg->sec_set[1]   = 10;
	msg->sec_set[2]   = (ch == 8) ? 11 : (12 + ch);
	msg->sec_set[3]   = 41;
	msg->sec_set[4]   =  0;
	msg->sec_set[5]   =  0;
	msg->sec_reset[0] = 33;
	msg->sec_reset[1] = 43;
	msg->sec_reset[2] = (ch == 8) ? 11 : (12 + ch);
	msg->sec_reset[3] = 31;
	msg->sec_reset[4] =  0;
	msg->sec_reset[5] =  0;
	msg->setmiss[0]  = 39;                      
	msg->setmiss[1]  = 40 ;
	msg->setmiss[2]  = 27 ;     
	msg->setmiss[3]  = 0  ;     
	msg->setmiss[4]  = 0  ;    
	msg->setmiss[5]  = 0  ;    
	msg->chk_key_nor[0]   = 35;
	msg->chk_key_nor[1]   = 0;
	msg->chk_key_nor[2]   = 0;
	msg->chk_key_nor[3]   = 0;
	msg->chk_key_nor[4]   = 0;
	msg->chk_key_nor[5]   = 0;

	msg->chk_key_err[0]   =  34;                      
	msg->chk_key_err[1]   = 0; 
	msg->chk_key_err[2]   = 0;      
	msg->chk_key_err[3]   = 0;     
	msg->chk_key_err[4]   = 0;     
	msg->chk_key_err[5]   = 0;                      
}


#if 1
void env_default(env_t * env)
{
    int i;
    /* system env */
    memset(env->system.dev_name,0,sizeof(env->system.dev_name));

    env->system.password    = 1234;

	for( i = 0 ; i < 32; i++)
	{
        env->system.lan_slave_enable[i].type = 0;
		env->system.lan_slave_enable[i].area = 0;
	}
    memset(env->system.multi_zone_enable,0,sizeof(env->system.multi_zone_enable));

    env->system.sr_key_used = true;
    env->system.sr_key_send = true;
    env->system.polarity = NC;
	env->system.msg_sr_send = false;
    env->system.schdule_call = 1 ; /* */
    env->system.schdule_time = 1 ; 
    env->system.r_schdule_time = 1 ; 
    env->system.lan_address = 0;
    memset(env->system.relay,0,sizeof(env->system.relay));
    memset(env->system.ctl,0,sizeof(env->system.ctl));


    /* area env */
    env->area.area_num = 1;

    for(i = 0 ; i < MAX_AREA ; i++)
    {
        area_t * area = &env->area.area[i];
        
        memset(area->name,0,sizeof(area->name));
        area->name[0] = 0x30a8;
        area->name[1] = 0x30ea;
        area->name[2] = 0x30a2;
        area->name[3] = 0x0031 + (0x100 * i);
        area->address[0] = 0;
        area->address[1] = 0;
        area->address[2] = 0;
        area->address[3] = 0;
        area->share_area = false;
        set_default_msg(i,&area->msg);
        area->relay = 0;
        area->method = 0x07;
    }
    set_default_msg(MAX_AREA,&env->area.all_msg);
    env->area.share_set_time = 0;
    env->area.delay_msg[0] = 48;
    env->area.delay_msg[1] = 0;


    /* comm setup */
    env->comm.first = 0;
    env->comm.err1 = 0;
    env->comm.andor = 0;
    env->comm.second = 0;
    env->comm.err2 = 0;

    /* tcp */
    for(i = 0 ; i < 3 ; i++)
    {
        t_dest_t * dest = &env->comm.tcp.dest[i];
        dest->ip = (192L << 24) | (168L << 16) | (1L << 8) | 101L;
        dest->port = 5000;
        dest->alive = 2;
		memset(dest->mail,0,60);
    }
    env->comm.tcp.relay = 0;
    env->comm.tcp.dhcp = 1;
    env->comm.tcp.ip = ((uint32_t)192L << 24) | ((uint32_t)168L << 16) | ((uint32_t)1L << 8) | (uint32_t)110L;
    env->comm.tcp.subnet = ((uint32_t)255L << 24) | ((uint32_t)255L << 16) | ((uint32_t)255L << 8) | (uint32_t)0L;
    env->comm.tcp.gw = ((uint32_t)192L << 24) | ((uint32_t)168L << 16) | ((uint32_t)1L << 8) | (uint32_t)1L;
    env->comm.tcp.port = 5001;

    /* emg */
    for(i = 0 ; i < MAX_SENSOR ; i++ )
    {
        emg_t * sensor = &env->emg.sensor[i];
        sensor->used = true;
        sensor->oper.type = 0; // 0 emg, 1 ctl
        sensor->oper.area = 0;
        sensor->oper.polarity = NC;
        sensor->oper.led = 1;
		sensor->oper.led_mode = 1;
        sensor->oper.hold = 0;
        sensor->oper.method = 0;
        sensor->oper.delay = 0;
        sensor->oper.data_send = true;
        sensor->oper.jurnel = true;
        sensor->oper.ctl = 0;
        sensor->oper.ctl_jurnel = 0;
        sensor->oper.sch_out = 0;

        sensor->msg.output = 1;
        sensor->msg.repeat = 0;
        sensor->msg.msg[0] = 39;
        sensor->msg.msg[1] = 2;
        sensor->msg.msg[2] = 3;
        sensor->msg.msg[3] = 1;
        sensor->msg.msg[4] = 0;
        sensor->msg.msg[5] = 0;
        sensor->out.loop = 0;
        sensor->out.relay = 0;
        sensor->out.ctl = 0;
        sensor->out.buzzer = 0;
        sensor->out.force_call = 0;

        sensor->signal.mel1 = i;
        sensor->signal.mel2 = i;
        sensor->signal.mel3[0] = i < 5 ? '0' : '9';
        sensor->signal.mel3[1] = i < 5 ? '1' + i : '1' + (i - 5);
        sensor->signal.mel4[0] = '3';
        sensor->signal.mel4[1] = '0' + i;
        memcpy(sensor->name,mStrInitSensor[i],sizeof(mStrInitSensor[i]));
    }

	/* emg */
	for(i = 0 ; i < 4 ; i++ )
	{
		emg_t * sensor = &env->emg.tampa[i];
		if(i == 1 || i == 2)
			sensor->used =false;
		else
			sensor->used =true;
		sensor->oper.type = 0; // 0 emg, 1 ctl
		sensor->oper.area = 0;
		sensor->oper.polarity = NC;
		sensor->oper.led = 0;
		sensor->oper.hold = 0;
		sensor->oper.method = 0;
		sensor->oper.delay = 0;
		sensor->oper.data_send = true;
		sensor->oper.jurnel = true;
        sensor->oper.ctl = 0;
        sensor->oper.ctl_jurnel = 0;
        sensor->oper.sch_out = 0;
		sensor->msg.output = 1;
		sensor->msg.repeat = 0;
		sensor->msg.msg[0] = 39;

		if(i != 3)
			sensor->msg.msg[1] = 29;
		else
			sensor->msg.msg[1] = 30;

		if(i == 0)
			sensor->msg.msg[2] = 1;
		else if(i == 1)
			sensor->msg.msg[2] = 1;
		else if(i == 2)
			sensor->msg.msg[2] = 1;
		else
			sensor->msg.msg[2] = 0;

		sensor->msg.msg[3] = 0;
		sensor->msg.msg[4] = 0;
		sensor->msg.msg[5] = 0;

		sensor->out.loop = 0;
		sensor->out.relay = 0;
		sensor->out.ctl = 0;
		sensor->out.buzzer = 0;
		sensor->out.force_call = 0;

		if(i < 3)
		{
			sensor->signal.mel1 = 19;
			sensor->signal.mel2 = 19;
			sensor->signal.mel3[0] = '4';
			sensor->signal.mel3[1] = '9';
			sensor->signal.mel4[0] = '5';
			sensor->signal.mel4[1] = '9';
		}
		else
		{
			sensor->signal.mel1 = 37;
			sensor->signal.mel2 = 37;
			sensor->signal.mel3[0] = '1';
			sensor->signal.mel3[1] = '2';
			sensor->signal.mel4[0] = '1';
			sensor->signal.mel4[1] = '3';
		}
		memcpy(sensor->name,mStrInitTampa[i],sizeof(mStrInitTampa[i]));
	}


    for(i = 0 ; i < 128; i++)
    {
        emg_t * sensor = &env->emg.multi[i];


    	sensor->used = false;
    	sensor->oper.type = 0; // 0 emg, 1 ctl
    	sensor->oper.area = 0;
    	sensor->oper.polarity = NC;
    	sensor->oper.led = 0;
    	sensor->oper.hold = 0;
    	sensor->oper.method = 0;
    	sensor->oper.delay = 0;
    	sensor->oper.data_send = true;
    	sensor->oper.jurnel = true;
    	sensor->oper.ctl = 0;
    	sensor->oper.ctl_jurnel = 0;
    	sensor->oper.sch_out = 0;
    	sensor->msg.output = 1;
    	sensor->msg.repeat = 0;
    	sensor->msg.msg[0] = 39;
    	sensor->msg.msg[1] = 2;
    	sensor->msg.msg[2] = 8;
    	sensor->msg.msg[3] = 1;
    	sensor->msg.msg[4] = 0;
    	sensor->msg.msg[5] = 0;
    	sensor->out.loop = 0;
    	sensor->out.relay = 0;
    	sensor->out.ctl = 0;
    	sensor->out.buzzer = 0;
    	sensor->out.force_call = 0;
    
    	sensor->signal.mel1 = (i%4);
    	sensor->signal.mel2 = (i%4);
    	sensor->signal.mel3[0] = '0';
    	sensor->signal.mel3[1] = '1' + (i%4);
    	sensor->signal.mel4[0] = '3';
    	sensor->signal.mel4[1] = '0' + (i%4);
    	memset(sensor->name,0,20);
        memcpy(sensor->name,mStrInitMulti[i%4],sizeof(mStrInitMulti[i%4]));
    }

	for(i = 0 ; i < 64 ; i++)
	{
		emg_t * sensor = &env->emg.wire[i];


		sensor->used = false;
		sensor->oper.type = 0; // 0 emg, 1 ctl
		sensor->oper.area = 0;
		sensor->oper.polarity = NC;
		sensor->oper.led = 0;
		sensor->oper.hold = 0;
		sensor->oper.method = 0;
		sensor->oper.delay = 0;
		sensor->oper.data_send = true;
		sensor->oper.jurnel = true;
		sensor->oper.ctl = 0;
		sensor->oper.ctl_jurnel = 0;
		sensor->oper.sch_out = 0;
		sensor->msg.output = 1;
		sensor->msg.repeat = 0;
		sensor->msg.msg[0] = 39;
		sensor->msg.msg[1] = 2;
		sensor->msg.msg[2] = 8;
		sensor->msg.msg[3] = 1;
		sensor->msg.msg[4] = 0;
		sensor->msg.msg[5] = 0;
		sensor->out.loop = 0;
		sensor->out.relay = 0;
		sensor->out.ctl = 0;
		sensor->out.buzzer = 0;
		sensor->out.force_call = 0;

		sensor->signal.mel1 = (i%4);
		sensor->signal.mel2 = (i%4);
		sensor->signal.mel3[0] = '0';
		sensor->signal.mel3[1] = '1' + (i%4);
		sensor->signal.mel4[0] = '3';
		sensor->signal.mel4[1] = '0' + (i%4);
		memset(sensor->name,0,20);
		memcpy(sensor->name,mStrInitWire[i%2],sizeof(mStrInitWire[i%2]));
	}

	for(i=0 ; i < 32 ; i++)
	{
		w_device_t * dev;
		dev = &env->emg.dev[i];

        dev->used = 0;
		memset(dev->SerialNum,0,6);
		dev->BatChaneg[0]=0;		//  Bat 교환 날짜
		dev->BatChaneg[1]=0;		//  Bat 교환 날짜
		dev->BatChaneg[2]=0;		//  Bat 교환 날짜
        dev->BatSend = 1;
        dev->SchSend  = 1;
        dev->SchTime = 0;


		dev->Tampa.mel1=0;
		dev->Tampa.mel2=0;
		dev->Tampa.mel3[0]='0';
		dev->Tampa.mel3[1]='1';
		dev->Tampa.mel4[0]='3';
		dev->Tampa.mel4[1]='0';

		dev->SBatlow.mel1=21;
		dev->SBatlow.mel2=21;
		dev->SBatlow.mel3[0]='6';
		dev->SBatlow.mel3[1]='1';
		dev->SBatlow.mel4[0]='7';
		dev->SBatlow.mel4[1]='1';

		dev->Schdule.mel1=20;
		dev->Schdule.mel2=20;
		dev->Schdule.mel3[0]='6';
		dev->Schdule.mel3[1]='0';
		dev->Schdule.mel4[0]='7';
		dev->Schdule.mel4[1]='0';
	}
}
#endif




