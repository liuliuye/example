/* !!! Do not manually modify this file in the production stage !!!
 * This file is automatically created by OSDEdit.exe,
 * and used in the production stage.*/

#ifndef __OSD_SOURCE_H
#define __OSD_SOURCE_H

//Custom color index information
#define       PALETTE_R_COLOR_ERROR                   	0xff
#define       PALETTE_R_COLOR_RED                     	0xfe
#define       PALETTE_R_COLOR_GREEN                   	0xfd
#define       PALETTE_R_COLOR_BLUE                    	0xfc
#define       PALETTE_R_COLOR_WHITE                   	0xfb
#define       PALETTE_R_COLOR_BLACK                   	0xfa
#define       PALETTE_R_COLOR_TRANSFER                	0xf9
#define       PALETTE_R_COLOR_TBLACK                  	0xf8
#define       PALETTE_R_COLOR_YELLOW                  	0xf7
#define       PALETTE_R_COLOR_GARY1                   	0xf6
#define       PALETTE_R_COLOR_BLUE1                   	0xf5
#define       PALETTE_R_COLOR_DBLUE                   	0xf4
#define       PALETTE_R_COLOR_BLUE2                   	0xf3
#define       PALETTE_R_COLOR_GARY2                   	0xf2
#define       PALETTE_R_COLOR_GARY3                   	0xf1

//OSD Icon index information
#define       OSD_ICONMENUAUDIO                   	0
#define       OSD_ICONMENUBATTERY                 	1
#define       OSD_ICONMENUCLOCK                   	2
#define       OSD_ICONMENUDELALL                  	3
#define       OSD_ICONMENUDELONE                  	4
#define       OSD_ICONMENUEV                      	5
#define       OSD_ICONMENUFLASH_AUTO              	6
#define       OSD_ICONMENUFLASH_OFF               	7
#define       OSD_ICONMENUFLASH_ON                	8
#define       OSD_ICONMENUFORMAT                  	9
#define       OSD_ICONMENUGSENSOR                 	10
#define       OSD_ICONMENUHZ                      	11
#define       OSD_ICONMENUIMAGEQ                  	12
#define       OSD_ICONMENULANGUAGE                	13
#define       OSD_ICONMENULIGHTNORMAL             	14
#define       OSD_ICONMENULOCK                    	15
#define       OSD_ICONMENULOOPRECORD              	16
#define       OSD_ICONMENUMONITOR                 	17
#define       OSD_ICONMENUMOON                    	18
#define       OSD_ICONMENUMOREPHOTO               	19
#define       OSD_ICONMENUMOTION                  	20
#define       OSD_ICONMENUMUSIC                   	21
#define       OSD_ICONMENUMUSICPAUSE              	22
#define       OSD_ICONMENUMUSICPLAY               	23
#define       OSD_ICONMENUPOWEROFF                	24
#define       OSD_ICONMENUPRINTDENSITY            	25
#define       OSD_ICONMENURESET                   	26
#define       OSD_ICONMENURESOLUTION              	27
#define       OSD_ICONMENUSCRENNOFF               	28
#define       OSD_ICONMENUSTRAMP                  	29
#define       OSD_ICONMENUTV                      	30
#define       OSD_ICONMENUUNLOCK                  	31
#define       OSD_ICONMENUVERSION                 	32
#define       OSD_ICONMENUVOLUME                  	33
#define       OSD_ICONMTBACKWARD2                 	34
#define       OSD_ICONMTBACKWARD4                 	35
#define       OSD_ICONMTBATCHARGE1                	36
#define       OSD_ICONMTBATCHARGE2                	37
#define       OSD_ICONMTBATCHARGE3                	38
#define       OSD_ICONMTBATCHARGE4                	39
#define       OSD_ICONMTBATTERY0                  	40
#define       OSD_ICONMTBATTERY1                  	41
#define       OSD_ICONMTBATTERY2                  	42
#define       OSD_ICONMTBATTERY3                  	43
#define       OSD_ICONMTBATTERY4                  	44
#define       OSD_ICONMTBATTERY5                  	45
#define       OSD_ICONMTFORWARD2                  	46
#define       OSD_ICONMTFORWARD4                  	47
#define       OSD_ICONMTGAME                      	48
#define       OSD_ICONMTIROFF                     	49
#define       OSD_ICONMTIRON                      	50
#define       OSD_ICONMTLOCK                      	51
#define       OSD_ICONMTLOOP1                     	52
#define       OSD_ICONMTLOOP10                    	53
#define       OSD_ICONMTLOOP5                     	54
#define       OSD_ICONMTMENU                      	55
#define       OSD_ICONMTMICOFF                    	56
#define       OSD_ICONMTMICON                     	57
#define       OSD_ICONMTMORE                      	58
#define       OSD_ICONMTMOTION                    	59
#define       OSD_ICONMTOFF                       	60
#define       OSD_ICONMTON                        	61
#define       OSD_ICONMTON1                       	62
#define       OSD_ICONMTON2                       	63
#define       OSD_ICONMTPARKOFF                   	64
#define       OSD_ICONMTPARKON                    	65
#define       OSD_ICONMTPAUSE                     	66
#define       OSD_ICONMTPHOTO                     	67
#define       OSD_ICONMTPHOTO3                    	68
#define       OSD_ICONMTPHOTO5                    	69
#define       OSD_ICONMTPHOTOFOCUSRED             	70
#define       OSD_ICONMTPHOTOFOCUSYELLOW          	71
#define       OSD_ICONMTPHOTOSKETCH               	72
#define       OSD_ICONMTPHOTOSKETCHCOLOR          	73
#define       OSD_ICONMTPHOTOTIME10               	74
#define       OSD_ICONMTPHOTOTIME3                	75
#define       OSD_ICONMTPHOTOTIME5                	76
#define       OSD_ICONMTPHOTOTIME5P3              	77
#define       OSD_ICONMTPLAY                      	78
#define       OSD_ICONMTPRINTER                   	79
#define       OSD_ICONMTPRTDLY1                   	80
#define       OSD_ICONMTPRTDLY2                   	81
#define       OSD_ICONMTPRTDLY3                   	82
#define       OSD_ICONMTRECORD                    	83
#define       OSD_ICONMTSDCNORMAL                 	84
#define       OSD_ICONMTSDCNULL                   	85
#define       OSD_ICONNUM1                        	86
#define       OSD_ICONNUM10                       	87
#define       OSD_ICONNUM2                        	88
#define       OSD_ICONNUM3                        	89
#define       OSD_ICONNUM4                        	90
#define       OSD_ICONNUM5                        	91
#define       OSD_ICONNUM6                        	92
#define       OSD_ICONNUM7                        	93
#define       OSD_ICONNUM8                        	94
#define       OSD_ICONNUM9                        	95
#define       OSD_ICON_MUSIC_1                    	96
#define       OSD_ICON_MUSIC_2                    	97
#define       OSD_ICON_MUSIC_3                    	98


#endif
