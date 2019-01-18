/*
 * codeplug.h
 *
 *  Created on: 14.12.2018
 *      Author: SP5WWP
 */

#ifndef CODEPLUG_H_
#define CODEPLUG_H_

#define MAX_CHANNELS_PER_BANK			20			//max channels allowed in a bank
#define MAX_NAME_LEN					12+1		//max text length (except for VERSION -> 20)
#define MAX_BANK_NUM					100			//maximum bank number

#define K_AUTHOR						"author"
#define AUTHOR							1
#define K_VERSION						"version"
#define VERSION							2

#define K_NUM_BANKS						"num_banks"
#define NUM_BANKS						3
#define K_NUM							"num"
#define NUM								4
#define K_NAME							"name"
#define NAME							5
#define K_NUM_CHANNELS					"num_channels"
#define NUM_CHANNELS					6
#define K_DESCR							"descr"
#define DESCR							7
#define K_FREQ							"freq"
#define FREQ							8
#define K_ENCR							"encr"
#define ENCR							9
#define K_GPS							"gps"
#define GPS								10

#define EMPTY_OR_COMMENT				255

#define K_CODEPLUG						"codeplug"
#define CODEPLUG						100
#define K_BANK							"bank"
#define BANK							101
#define K_CHANNEL						"channel"
#define CHANNEL							102
#define K_END							"end"
#define END								200

#endif /* CODEPLUG_H_ */
