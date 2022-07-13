#ifndef COMMANDS_h
#define  COMMANDS_h

// see docs.simplefoc.com for in depth explanation of the commands

// list of commands
 #define CMD_C_D_PID   'D' //!< current d PID & LPF
 #define CMD_C_Q_PID   'Q' //!< current d PID & LPF
 #define CMD_V_PID     'V' //!< velocity PID & LPF
 #define CMD_A_PID     'A' //!< angle PID & LPF
 #define CMD_STATUS    'E' //!< motor status enable/disable
 #define CMD_LIMITS    'L' //!< limits current/voltage/velocity
 #define CMD_MOTION_TYPE  'C' //!< motion control type
 #define CMD_TORQUE_TYPE  'T' //!< torque control type
 #define CMD_SENSOR    'S' //!< sensor offsets
 #define CMD_MONITOR   'M' //!< monitoring
 #define CMD_RESIST    'R' //!< motor phase resistance
 #define CMD_KV_RATING 'K' //!< motor kv rating
 #define CMD_PWMMOD   'W' //!< pwm modulation

 // commander configuration
 #define CMD_SCAN    '?' //!< command scaning the network - only for commander
 #define CMD_VERBOSE '@' //!< command setting output mode - only for commander
 #define CMD_DECIMAL '#' //!< command setting decimal places - only for commander

 // anti cogging
 #define CMD_ANTICOG 'N'

#define SCMD_ANTICOG_OFF '0'
#define SCMD_ANTICOG_POS '1'
#define SCMD_ANTICOG_VEL '2'
#define SCMD_ANTICOG_PARAMETERIZED '3'
#define SCMD_ANTICOG_HANDTUNED '4'

#define SCMD_ANTICOG_FREQ 'F'
#define SCMD_ANTICOG_PHASE 'P'
#define SCMD_ANTICOG_AMP 'A'


 // subcomands
 //pid - lpf
 #define SCMD_PID_P     'P' //!< PID gain P
 #define SCMD_PID_I     'I' //!< PID gain I
 #define SCMD_PID_D     'D' //!< PID gain D
 #define SCMD_PID_RAMP  'R' //!< PID ramp
 #define SCMD_PID_LIM   'L' //!< PID limit
 #define SCMD_LPF_TF    'F' //!< LPF time constant
 // limits
 #define SCMD_LIM_CURR  'C' //!< Limit current
 #define SCMD_LIM_VOLT  'U' //!< Limit voltage
 #define SCMD_LIM_VEL   'V' //!< Limit velocity
 //sensor
 #define SCMD_SENS_MECH_OFFSET 'M' //!< Sensor offset
 #define SCMD_SENS_ELEC_OFFSET 'E' //!< Sensor electrical zero offset
 // monitoring
 #define SCMD_DOWNSAMPLE 'D' //!< Monitoring downsample value
 #define SCMD_CLEAR      'C' //!< Clear all monitored variables
 #define SCMD_GET        'G' //!< Get variable only one value
 #define SCMD_SET        'S' //!< Set variables to be monitored

 #define SCMD_PWMMOD_TYPE   'T'  //!<< Pwm modulation type
 #define SCMD_PWMMOD_CENTER 'C'  //!<< Pwm modulation center flag


#endif