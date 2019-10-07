/*
 * rtcc_config.h
 *
 * Created: 13/09/2019 04:02:16 PM
 *  Author: Loay Ashraf
 */ 

#ifndef RTCC_CONFIG_H_
#define RTCC_CONFIG_H_

#include "hal/ecu/rtcc/driver/rtcc_driver.h"

/*------------DEFINE RTCC CONTROLLER------------*/

#define RTCC_CONTROLLER		RTCC_DS3231

/*------------DEFINE RTCC MILLENNIUM AND CLOCK FORMAT------------*/

#define RTCC_MILLENNIUM		2000
#define RTCC_CLOCK_FORMAT	"%02d:%02d:%02d %s"

#endif /* RTCC_CONFIG_H_ */