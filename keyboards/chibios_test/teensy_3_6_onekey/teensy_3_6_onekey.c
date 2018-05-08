#include "teensy_3_6_onekey.h"

/*===========================================================================*/
/* Card insertion monitor.                                                   */
/*===========================================================================*/

#define POLLING_INTERVAL                10
#define POLLING_DELAY                   10

/*
 * Working area for driver.
 */
static uint8_t sd_scratchpad[512];

/*
 * SDIO configuration.
 */
static const SDCConfig sdccfg = {
  sd_scratchpad,
  SDC_MODE_4BIT
};

/**
 * @brief   Card monitor timer.
 */
//static virtual_timer_t tmr;
static bool init_done = FALSE;
//static bool inserted = FALSE;

/* FS mounted and ready.*/
static bool fs_ready = FALSE;
mutex_t fs_mutex;

//static void InsertHandler(void);
//static void RemoveHandler(void);

static inline bool is_fs_ready(void) {
  bool res;
  chMtxLock(&fs_mutex);
  res = fs_ready;
  chMtxUnlock(&fs_mutex);
  return res;
}

static inline void fs_is_ready(bool res) {
  chMtxLock(&fs_mutex);
  fs_ready = res;
  chMtxUnlock(&fs_mutex);
}

//static bool toggy = FALSE;
//#define togger do{ mikedebug((toggy ? 255 : 0)); toggy=!toggy;} while(0)

/**
 * @brief   Debounce counter.
 */
//static unsigned cnt;

/**
 * @brief   Insertion monitor timer callback function.
 *
 * @param[in] p         pointer to the @p BaseBlockDevice object
 *
 * @notapi
 */
/*
static void tmrfunc(void *p) {
  BaseBlockDevice *bbdp = p;

  chSysLockFromISR();
  if (cnt > 0) {
    if (!inserted && blkIsInserted(bbdp)) {
      if (--cnt == 0) {
        inserted = TRUE;
        InsertHandler();
      }
    }
    else
      cnt = POLLING_INTERVAL;
  }
  else {
    if (inserted && !blkIsInserted(bbdp)) {
      cnt = POLLING_INTERVAL;
      inserted = FALSE;
      RemoveHandler();
    }
  }
  chVTSetI(&tmr, MS2ST(POLLING_DELAY), tmrfunc, bbdp);
  chSysUnlockFromISR();
}
*/

/**
 * @brief   Polling monitor start.
 *
 * @param[in] p         pointer to an object implementing @p BaseBlockDevice
 *
 * @notapi
 */
/*
static void tmr_init(void *p) {
  chSysLock();
  cnt = POLLING_INTERVAL;
  chVTSetI(&tmr, MS2ST(POLLING_DELAY), tmrfunc, p);
  chSysUnlock();
}
*/

/*===========================================================================*/
/* FatFs related.                                                            */
/*===========================================================================*/

/**
 * @brief FS object.
 */
static FATFS SDC_FS;


static FRESULT scan_files(char *path) {
  static FILINFO fno;
  FRESULT res;
  DIR dir;
  size_t i;
  char *fn;
  char buf[80];

  debug_enable = true;
  debug_matrix = true;

  print("Entering scan_files\n");

  if(!init_done) local_fatfs_init();

  if(!is_fs_ready()) {
    print("Can't scan files, fs not ready!\n");
    return FR_NOT_READY;
  }

  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    print("opendir succeeded\n");
    i = strlen(path);
    while (((res = f_readdir(&dir, &fno)) == FR_OK) && fno.fname[0]) {
      if (FF_FS_RPATH && fno.fname[0] == '.')
        continue;
      fn = fno.fname;
      if (fno.fattrib & AM_DIR) {
        *(path + i) = '/';
        strcpy(path + i + 1, fn);
        res = scan_files(path);
        *(path + i) = '\0';
        if (res != FR_OK)
          break;
      }
      else {
        sprintf(buf, "%s/%s\r\n", path, fn);
        print(buf);
      }
    }
  }
  else {
    print("opendir FAILED\n");
  }
  return res;
}

/*===========================================================================*/
/* Main and generic code.                                                    */
/*===========================================================================*/

/*
 * Card insertion event.
 */
/*
static void InsertHandler(void) {
  FRESULT err;

  print("Insertion detected\n");
  if (sdcConnect(&SDCD1)){
    print("Connect failed\n");
    return;
  }
  print("Connect succeeded, attempting to mount\n");

  err = f_mount(&SDC_FS, "/", 1);
  if (err != FR_OK) {
    print("Mount failed\n");
    sdcDisconnect(&SDCD1);
    return;
  }
  else {
  }
  print("Mount succeeded\n");
  fs_is_ready(TRUE);
}

static void RemoveHandler(void) {
  print("Removal detected\n");
  sdcDisconnect(&SDCD1);
  fs_is_ready(FALSE);
}
*/

int local_fatfs_init(void) {
  FRESULT err;
  char buf[64];
  chMtxObjectInit(&fs_mutex);
  print("entering local_fatfs_init\n");
  print("doing sdcStart\n");
  sdcStart(&SDCD1, &sdccfg);
  //print("doing tmr_init\n");
  //tmr_init(&SDCD1);
  init_done = true;

  if ((err=sdcConnect(&SDCD1))){
    sprintf(buf, "Connect failed with code %d\n", err);
    print(buf);
    return -1;
  }
  print("Connect succeeded, attempting to mount\n");

  err = f_mount(&SDC_FS, "/", 1);
  if (err != FR_OK) {
    print("Mount failed\n");
    sdcDisconnect(&SDCD1);
    return -1;
  }
  else {
  }
  print("Mount succeeded\n");
  fs_is_ready(TRUE);




  return 0;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//  FRESULT local_result;
//  char debug_string[64];

  switch (keycode) {
    case KC_CAPS:
      if (record->event.pressed) {
        print("Keypress\n");
        //local_result = scan_files("/");
        //sprintf(debug_string, "Result: %d\n", (int)local_result);
        //print(debug_string);
//        local_fatfs_init();
        //print("fatfs init\n");
          scan_files("/");
      }
      break;
  }
  return true;
}


