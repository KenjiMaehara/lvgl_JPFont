/**
 * @file env.h
 *
 */

#ifndef ENV_H
#define ENV_H


#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/
#define 	SW_VER_MAJOR	0
#define 	SW_VER_MINOR	8
#define 	SW_VER_SUB		4

#define     MAX_AREA        8
#define     MAX_SENSOR      8
#define     MAX_MULTI       128
#define     MAX_RELAY       2
#define     MAX_CTL_OUT     8

#define     NO      0
#define     NC      1

#define     RESET   0
#define     SET     1

#define		C_ON	0
#define		C_OFF	1
/**********************
 *      TYPEDEFS
 **********************/
#pragma pack(push, 1)  
typedef struct _LAN_TYPE_
{
	uint8_t		type;
	uint8_t		area;

}lan_env_t;


typedef struct _SYSTEM_ENV_
{
    uint16_t     dev_name[20];              /* device name*/
    uint16_t     password; 		           /* maintence pass*/
    lan_env_t    lan_slave_enable[32];      /* 使用LAN SLAVE選択  : 1 ~ 31 check */
    uint8_t      multi_zone_enable[32];     /* マルチゾーン 選択 : 0 ~ 31 check   */ 
    uint8_t      sr_key_used;               /* S/R Key : 使用しない/使用する   */  
    uint8_t      sr_key_send;               /* S/R通報の可否 : 通報しない/１分後に通報する */ 
    uint8_t      polarity;                  /* SW NO/NC*/
	uint8_t      msg_sr_send;
    uint8_t      lan_address;               /* ＬＡＮアドレス : 00:HOST   01 - 31:SLAVE */
    uint8_t      schdule_call;              /* 定時通報の選択 : 使用しない/警備開始後/終日、設定時間で行う*/  
    uint8_t      schdule_time;              /* 定時通報時間間隔 : 0１～４８：１時間～４８時間(PSTN), ００：３０分(PVC)*/
	uint8_t      r_schdule_time;            /* 定時通報時間間隔 : 0１～４８：１時間～４８時間(PSTN), ００：３０分(PVC)*/
    uint8_t      relay[4];                  /* リレー出力の動作条件: ０：使用しない     */
                                       /*  移報出力の動作条件  １：１０秒間動作   */
                                       /*                     ２：２０秒間動作   */
                                       /*                     ３：３０秒間動作   */
                                       /*                     ４：４０秒間動作   */   
                                       /*                     ５：５０秒間動作   */
                                       /*                     ６：発生時１秒間動作 */
                                       /*                     ７：発生／復旧時１秒間動作 */
                                       /*                     ８：復旧時１秒間動作 */
                                       /*                     ９：動作通り */
    uint8_t      ctl[MAX_CTL_OUT];
}system_env_t;


typedef struct _SET_AREA_MSG_T
{
    uint8_t       set[6];
    uint8_t       reset[6];
    uint8_t       sec_set[6];
    uint8_t       sec_reset[6];
    uint8_t       setmiss[6];
    uint8_t       chk_key_nor[6];
    uint8_t       chk_key_err[6];
}a_msg_t;

typedef struct _AREA_ITEM_
{
    uint16_t     name[10];                  /* エリア名称 Max 10 */
    uint16_t     address[4];                /* アドレス : Mel1(pvc,tcp),Mel2,Mel3,Mel4*/
    uint8_t      share_area;                /* 共有部の割当 : 共有部割り当てなし/全エリアの共有部*/
    a_msg_t      msg;                       /* 音声: １～62：音声番号　6種類の組み合わせ*/
    uint8_t      relay;                     /* realy 1 bit 0, relay 2 bit 1*/
    uint8_t      method;                    /* Key bit 0, Card bit 1, SW bit 2, Remote bit3*/ 
}area_t;

typedef struct _AREA_
{
    uint8_t      area_num;                   /* 使用エリア数 : 1 ~ 8*/
    area_t      area[MAX_AREA];
    a_msg_t     all_msg;
    uint8_t      share_set_time;             /* 共有部の再開始 : ０：なし　１～９：２０秒単位*/
    uint8_t      delay_msg[2];               /* ディレー時のメッセージ : １～62：音声番号　2種類の組み合わせ*/
}area_env_t;


typedef struct _TCP_DESTINATION_
{
	uint8_t		used;
    uint32_t     ip;                         /* 相手先ＩＰアドレス */
    uint16_t     port;                       /* Port*/
    uint8_t      alive;                      /* 断線監視 : しない/10sec/20sec/30sec/40sec/50sec/01min/02min/03min/04min/05min*/
	uint8_t		mail[60];
}t_dest_t;

typedef struct _TCP_ENV_
{
    t_dest_t    dest[3];
    uint8_t     relay;                      /* */
    uint8_t     dhcp;                       /* */
    uint32_t    ip;                         /* */
    uint32_t    subnet;                     /* */
    uint32_t    gw;                         /* */
	uint16_t	port;
	uint8_t		pass[20];
}tcp_env_t;

typedef struct _COMM_ENV_
{
    
    uint8_t          first;                  /* Not Used,PSTN, PVC, TCP/IP  */
    uint8_t          err1;                   /* */
    uint8_t          andor;                  /* AND/ OR */
    uint8_t          second;                 /* PSTN or Not used */
    uint8_t          err2;                   /* */

	uint8_t          set_msg_uesd;       	/*メッセージ通報時の開始/解除 : 通報しない/通報する */
	uint8_t          user_msg[3];        	/*メッセージ通報時のユーザー音声番号 : １～62：音声番号　3種類の組み合わせ */
	uint8_t      	err_msg[6];                 /* 通信エラー時の 通報メッセージ音声番号 : １～62：音声番号　6種類の組み合わせ*/

    tcp_env_t       tcp;
}comm_env_t;



typedef struct _EMG_OPER_
{
    uint8_t          used;
    uint8_t          type;                   /* 0 emg, 1 ctl*/
    uint8_t          area;                   /* エリア割当 : 1 ~ 8 エリア*/
    uint8_t          polarity;               /* NC/NO or wireless battory low*/
    uint8_t          led;                    /* sensor ０：CHｾﾝｻｰのみ表示１：エリア全体を表示, multi zone表示方法の選択 : 1 ~ 8 Red Led 選択*/
    uint8_t          led_mode;               /* 0 : 表示しない 1 : 動作通りの表示 2 : 5秒間表示 3 : 表示を保持 */
    uint8_t          hold;                   /* 無保持/保持*/
    uint8_t          method;                 /* 監視方法 : 開始に連動（Ｓ/Ｒ）/ 終日監視（２４Ｈ）*/
    uint8_t          delay;                  /* ディレータイマー : ０：なし　１～９：２０秒単位*/
    uint8_t          data_send;              /* 通報の可否 : 通報しない/通報する */
    uint8_t          jurnel;                 /* ジャーナル出力 : 出力しない/ 出力する*/
	uint8_t			ctl;
	uint8_t			ctl_jurnel;
	uint8_t			sch_out;
}e_oper_t;

typedef struct _EMG_MSG_
{
    uint8_t          output;                 /* 音声選択 : 出力しない/出力する*/
    uint8_t          msg[6];                 /* 音声番号の選択 : １～62：音声番号　6種類の組み合わせ*/
    uint8_t          repeat;                 /* 音声の再生回数 : ００：手動停止、 ０１～９９：繰り返し回数*/
}e_msg_t;

typedef struct _EMG_OUT_
{
	uint8_t          loop;		            /* ループチェック機能 : ０：チェックしない　１～９：２時間単位  */
										/*Wireless Sensor 定時通報監視 0 : ０：監視しない 1: １：監視する*/
    uint8_t          relay;                  /* relay 1 bit 0, realy 2 bit 1*/
    uint8_t          ctl;                    /* ctl 1 ~ ctl 8*/
    uint8_t          buzzer;                 /* チャイム : 使用しない/ 使用する*/
    uint8_t          force_call;             /* 強制発信 : 強制発信しない/強制発信する*/
}e_out_t;

typedef struct  _EMG_SIG_
{
    uint8_t          mel1;
    uint8_t          mel2;
    uint8_t          mel3[2];
    uint8_t          mel4[2];
}e_sig_t;

typedef struct _EMG_
{
	bool		used;
    e_oper_t    oper;
    e_msg_t     msg;
    e_out_t     out;
    e_sig_t     signal;
    uint16_t    name[10];
}emg_t;


typedef struct _WDEVICE_
{
    uint8_t			used;		    // 사퓖E여부
    uint8_t			SerialNum[6];	// 무선 unit serial number
    uint8_t			BatChaneg[3];	//  Bat 교환 날짜
    uint8_t       	BatSend;
    uint8_t        	SchSend;        // '0' 감시하햨E않는다, '1' 감시한다.
    uint8_t	        SchTime;
    e_sig_t 	    Tampa;
    e_sig_t 	    SBatlow;
    e_sig_t 	    Schdule;
    uint8_t		    Reserved[10];
} w_device_t;


typedef struct _EMG_ENV_
{
    emg_t       sensor[8];
    emg_t       tampa[4];
    emg_t       multi[128];
    emg_t       wire[64];
	w_device_t	dev[32];
}emg_env_t;


typedef struct _AE_ENV_
{
    system_env_t    system;
    area_env_t      area;
    comm_env_t      comm;
    emg_env_t       emg;
}env_t;

typedef struct _ENV_HDR_
{
    uint8_t			identify[3];
    uint8_t			crc;
}env_hdr_t;


typedef struct _AE_ENV_ALL_
{
	env_hdr_t	hdr;
	env_t		env;
}ae_env_t;

typedef struct _DEV_HDR_
{
	uint8_t		mac[6];
	uint8_t		bat[3];
	uint8_t		plevel[3];
	uint8_t		calc;
	uint8_t		model;
}dev_hdr_t;
#pragma pack(pop)

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void env_init(void);


/**
 *
 */

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*ENV_H*/
