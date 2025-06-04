float pourcentagesousseuil = 0.05;


/**
 * @file TeamAT_IMUFusion_metriques_ex7_nano33.ino
 * @author Alexandre Lecours, Simon Latour
 * @brief  Montre ergonomie, permet de monitorer l'angle du bras
 * @version 1.0.0
 * @date 2021-09-02
 * 
 * @copyright Copyright (c) 2021
 * 
 * 
 * 
 * ParasportEstimeVitesse_v6
 * MB 2023
 */

//#define DEBUG_MODE 

#ifdef  DEBUG_MODE
  #define CODE_VERSION "6.6.6" 
#else
  #define CODE_VERSION "1.0.1" 
#endif

#include <ArduinoBLE.h>

#include "teamATbasic_V1_1.h"

#include "TeamAT_Filters_v1_1.h"

#include "oledDisplayManager_V1_1.h"  // Oled Display Manager
Adafruit_SSD1306 display;

#include "at_plaformAbstraction_V1_1.h"
#include "ButtonPollingManager_V2.3.h"

#include "SD_LOGGER_V1_3.h"

#include "cli_v1_0.h" //Command Line Interpreter library

#include "montre_light_arduino_defines.h"
#include "hapticFeedback_arduino.h"

//TODO compléter plus tard #include "triggerManager.h"

 #include "teamtAt_deviceID_V1.0.h"
 #include "backupManager_Arduino_NFR52_V1.1.h"
 #include "backupData_ModuleErgonomie.h"



#include "menuPage_main.h" // The Drink Reminder Menu


/////////////////////////////////////////////////////////////////////////
// !!!!!! INITIALISATION !!!!!!
/////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////
    // General
    //  *** Keep what you need
    /////////////////////////////////////////////////////////////////////////
      // Global variables init
      
      long counter = 0;      // Counter used for data
      int firstloop = 0;    // Global variable used to detect the first pass in the loop

      #define SPI_FREQ 20//40//20       // Fréquence du bus SPI à utiliser
      #define LOG_SERIAL 0    // Log data on serial port
      #define LOG_SD 1        // Log data on SD card
      int logPreferedDest = LOG_SD;
      int SerialSD = logPreferedDest;  // By default, we log data on SD card.
  
    /////////////////////////////////////////////////////////////////////////
    // IO
    //  *** Garder ce que vous avez besoin
    /////////////////////////////////////////////////////////////////////////
      // Pinout
        #define LED_ON 1    // Leds are on "pull down" on the arduino board 
        #define LED_OFF 0 
        const int led_alert = led_alert_pin;//7boarddrinkremind;//5boardacl;// pin de la led   
        const int led_sd = led_sd_pin; // 6;//5boarddrinkremind;//6boardacl;
        const int inBatVoltage = IN_BAT_VOLTAGE;
        const int audioread = audioread_pin;
        int sensor1_pin = A1;
        int sensor2_pin = A2;
     // ------------------------------------------------------------------- //

  /////////////////////////////////////////////////////////////////////////
  // Specific to this algos
  //  *** Keep what you need
  /////////////////////////////////////////////////////////////////////////

      // Variables to manage fix frequency tasks 
      //  *** you can change the time interval with txIntervalMillis
      unsigned long currentMillisStart;
      unsigned long prevMillis50ms;
      unsigned long currentMillisEnd;
      unsigned long loopintervalMillis=0;
      unsigned long prevMillisTaks10ms=0;
      unsigned long prevMillisTaks1000ms=0;
      unsigned long debut_temps_alert =0;
      unsigned long fin_temps_alert =0;
      unsigned long temps_total_alert=0;
      float temps_total_alert_secondes=0;
      ///////////////////////////////////////////////////////////////////
      // VARIABLE 
      //////////////////////////////////////////////////////////////////
        // Ceinture respiration
        int sensor1ValueZero = 0;
        int sensor2ValueZero = 0;
        float ys1_filtHPLP_m1 = 0;
        float ys2_filtHPLP_m1 = 0;
        float ys1_filtHPLP = 0;
        float ys2_filtHPLP = 0;
        float ys12_filtHPLP = 0;
        float repos_expiratoire = 0;
        int val_s1_read = 0;
        int val_s2_read = 0;
        
        float maxvalue=0;
        float minvalue=25000;
        int last_button = 0;
        
        int initdone = 0;
        int initdone2 = 0;
        int sens_zero_counter = 0;

        float audioval = 0;

        int alertonoff = 1;
      ////////////////////////////////////////////////////////////////
        int buttonvalue = 0;
        int buttonvalue3 = 0;
        int alert = 0;
        int derniere_alert = 0;
        int alert_count = 0;
      ////////////////////////////////////////////////////////////////
      
      // *** Fixed frequency for our program ***
      long txIntervalMillis = 50; // 50ms
      int fs=20;//100;
  // ------------------------------------------------------------------- //

  // ------------------------------------------------------------------- //
  /////////////////////////////////////////////////////////////////////////
  // TEAMAT FEEDBACK
  /////////////////////////////////////////////////////////////////////////
    bool gHapticMode = MODE_ERM;
    hapticFeedback_arduino haptic; 

    
  /////////////////////////////////////////////////////////////////////////
  // OLED display I2C
  /////////////////////////////////////////////////////////////////////////
    #define DISPLAY_REFRESH_PERIOD  50    // Screen will be refreshed every 50 ms
    #define DISPLAY_SLEEP_TIME_MS   5000  // If sleep mode enable, the unit will go in sleep mode after 5000 ms

    // Instance of the display manager. The display manager as its name implies manages the display for you
    // making it easier and letting you concentrate of the tasks of you program
    //oledDisplayManager dispManager; 
  // ------------------------------------------------------------------- //
    

  /////////////////////////////////////////////////////////////////////////
  // Button Polling Manager
  /////////////////////////////////////////////////////////////////////////
    // Button polling Manager setup
    enum buttonIndexList{
      BUTTON_UP_INDEX,
      BUTTON_DOWN_INDEX,
      BUTTON_ENTER_INDEX,
      BUTTON_RETURN_INDEX,
      BM_NB_BUTTON 
    };
     managedButton buttonList[BM_NB_BUTTON] ={
      {IN_BUTTON_UP, NO_PORT, ACTIVE_LOW, CLICK_ENABLE | LONGCLICK_ENABLE}, 
      {IN_BUTTON_DOWN, NO_PORT, ACTIVE_LOW, CLICK_ENABLE | LONGCLICK_ENABLE}, 
      {IN_BUTTON_ENTER, NO_PORT, ACTIVE_LOW, CLICK_ENABLE | HOLD_ENABLE}, 
      {IN_BUTTON_RETURN, NO_PORT, ACTIVE_LOW, CLICK_ENABLE | HOLD_ENABLE} 

     };

     ButtonPollingManager buttonManager;
     void humanInputDeviceManagement();
     
  // ------------------------------------------------------------------- //

  /////////////////////////////////////////////////////////////////////////
  // Tone player
  // *** Comment all if no IMU. Or choose the number of IMU and ports.
  /////////////////////////////////////////////////////////////////////////
    #include "tonePlayer.h"
    tonePlayerClass tonePlayer; 

    
    // Init NRF52 timer NRF_TIMER3
    //#if ( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )
    //  NRF52_MBED_Timer ITimer(NRF_TIMER_3);
    //#endif

  /////////////////////////////////////////////////////////////////////////
  // IMU
  // *** Comment all if no IMU. Or choose the number of IMU and ports.
  /////////////////////////////////////////////////////////////////////////
    #include "imu.h"
    // The SFE_LSM9DS1 library requires both Wire and SPI be included BEFORE including the 9DS1 library.
    #include <Wire.h>
    #include <SPI.h>
  
   // imuManagerClass imuManager; // Instance of the imu manager
  
    // IMU float data
    float imua_ax=0;float imua_ay=0;float imua_az=0;float imua_gx=0;float imua_gy=0;float imua_gz=0; //IMU 1
              
    #define NB_IMU 1      //Modify to select the number of actual IMUs you sish to use
    imuDataType imuData;
  
    // // Select IMU chip select (SD0 AG) digital IO attribution TABLE  *** Définir en fonction du nombre de IMU                
    //   // Chip select Table if usign Arduino board (CHANGE IF NEEDED):
    //   //  IMU 1 : Only device, no chip select
    //   //  IMU 2 : Not available
    //   //  IMU 3 : Not available
    //   int imuPinRef[NB_IMU_MAX] = {IMU_NO_CHIP_SELECT,
    //                                   IMU_NO_CHIP_SELECT,
    //                                   IMU_NO_CHIP_SELECT}; 

    
  // ------------------------------------------------------------------- //          


  /////////////////////////////////////////////////////////////////////////
  // SD CARD
  /////////////////////////////////////////////////////////////////////////
    bool log_onoff = false;              //Log On = true,Log Off = false  ( used to debounce button)
    int log_onOffInterrupt = false;     // Flag to communicate to the main loop that a button press was detected
    int last_log_onoff = -1;            // force detection of new state. on->off or off->on because some code only need to be executed
                                          // once when the logger is started or stopped.
    // SD chip select pin.  Be sure to disable any other SPI devices such as Enet.
       
    const uint8_t sdCardChipSelect = D10;  //SS1 pour MKRZero interne; SS pour autre; //SDCARD_SS_PIN pour ?;

 

    
    int flag_sdLogFileOpen=0; // Flag to indicate if the SD card has been initialized or not

  /////////////////////////////////////////////////////////////////////////
  // DATA Logger
  /////////////////////////////////////////////////////////////////////////
  // The data logger class is made to simplify logging data on SD card. byt taking care 
  // of the timing and communications for you. 
  /////////////////////////////////////////////////////////////////////////

  // * utilise sdfat.h qui est dans github\TeamAT_P_Arduino_MKRZero\SdFat. Devrait être dans le futur direct dans ce dossier.
    int loggindata = 0;
	#define logDataCount 10
    static char fileName[SDLOGGER_MAX_FILENAME_LEN];
    
    // Instance of the data logger
    //tow//sdlogger dataLogger;
    //tow//bool debug_newLineReceive = false;
    // Structure format for imu data logging
    struct __attribute__((packed)) datastore  // MB : modification des sorties
    {
       // int d_counter = 0;   // Data count
        int d_timestamp = 0; // Time in seconds
        float d_sensor1 = 0;    // float
        float d_sensor2 = 0;    // float
        float d_sensor12 = 0;    // float
        int d_alert = 0;
        float d_min = 0; // float
        float d_max = 0; // float
        float d_audio = 0; // float
        int d_alert_count = 0;
        float d_temps_total_alert_secondes=0;
        
        
    };

      char logDataTitles[logDataCount][32] = 
    					{
        "timestamp",
        "sensor1",
        "sensor2",
        "sensor12",
        "alert",
        "min",
        "max",
        "audio",
        "alert_count",
        "temps_total_alert_secondes"
    					};

    /*   
        char logDataTitles[logDataCount][32] =
              {
        "counter",
        "timestamp",
        "accx",
        "accy",
        "accz",
        "d_gyrx",
        "d_gyry",
        "accx_filt",//MB
        "d_Dist", //MB
        "d_speed1",
        "d_speed2",
        "d_Vit" //MB

              };*/
   uint8_t logDataTypes[logDataCount] =
					   {
					 //   logDataType_int,
              logDataType_int,
              logDataType_float,
              logDataType_float,
              logDataType_float,
              logDataType_int,
              logDataType_float,
              logDataType_float,
              logDataType_float,
				      logDataType_int,
              logDataType_float
                        };




    //  Because the data logger can be used for various data, it does not know
    //  the structure of the data you are using. You must specify in the following 
    //  callback function how to convert binary data to readable characters.
    //void logDataBinToChar(const uint8_t* binData, char* charData);



    // Instance of the data logger
    sdlogger dataLogger;
  
    /**
     * @brief Serial out function to be used for callbacks 
     * ( External libriries that needs to write on serial port)
     * 
     * @param outData The data to send
     */
    void serialOut(const char* outData);
   
// ------------------------------------------------------------------- //

  /////////////////////////////////////////////////////////////////////////
  // SERIAL PORT COMMAND LINE INTERPRETER (CLI)
  /////////////////////////////////////////////////////////////////////////
  // 
  //  The CLI enables to sen command from the serial port. To create you own 
  //  command here are the steps
  // 
  // 1. Create the function that will be called when command is received
  // 2. Put a pointer to you function in the list
  // 3. Define what text needs to by typed to call the function. 
  // 
  // More details on each steps below
  ///////////////////////////////////////////////////////////////////////  
   

    String ReadSerialPortVal; // Initialize variable to read on serial port
    String serialPortLineBuffer = "";
    bool debug_newLineReceived = false;  // whether the string is complete
    #define SERIAL_ENABLED  1 
    #define SERIAL_DISABLED 0 
    int startstopserial = SERIAL_DISABLED;//SERIAL_ENABLED;//SERIAL_DISABLED; // Serial COM enabled by default 
    int startstopserial2 = SERIAL_DISABLED;//SERIAL_ENABLED;//SERIAL_DISABLED; // Serial COM enabled by default 

    // CLI Step 1: All the functions to be called by the Command Line interpreter
    //
    // -All function must return positive int values ( negative reserved for the CLI error codes)
    // -All function mus have one char* parameter (even if not needed).  If the command has an argument
    //  it will be passed to the function 
    //  example command:  "readdata 1234" would call function cmd_readdata and 
    //  pass character array  "1234" as parameter
    int cmd_exit(const char* argString);        // Function to quit the loop
    int cmd_serialLog(const char* argString);   // Turn serial display of the log on/off when no sd card
    int cmd_listfiles(const char* argString);   // Display the list of log files on the sd card.
    int cmd_readdata(const char* argString);    // Read the content of a file on the sd card
    int cmd_deletesdcard(const char* argString);// Delete everything on the SD card 
    int cmd_restoreFactoryDefault(const char* argString);// Delete everything on the SD card 
    int cmd_test(const char* argString);// used for tests on command
    int cmd_downloadData(const char* argString);  // used to download the data in a faster way using
    // CLI STEP 2: Put all functions in a function pointer list.
    CLI_FUNC_PTR commands_func[]{
        &cmd_exit,
        &cmd_serialLog,
        &cmd_listfiles,
        &cmd_readdata,
        &cmd_deletesdcard,
        &cmd_restoreFactoryDefault,
        &cmd_test,
        &cmd_downloadData
        // Add new functions
    };

    //CLI STEP 3: List of command text for each functions.
    const char *commands_str[] = {
        "exit",         // This command is used to qui the program.
        "seriallog",    // use "seriallog on" or "seriallog off" to enable / disable sending data on serial port when no sd card is present
        "listfiles",    // This command is used to list the log files present on the sd card
        "readdata",     // This command is use dto read the contect of a file and display it on the serial port as ASCII ex: readdata 0001 prints the content of Data0001.dat 
        "delete", // Delete ALL CONTENT from the Sd card.
        "restoreFD",
		"test",
   "downloaddata" // Download data in binary usign atComm
        // Add new text here
    };

    int num_cli_commands = sizeof(commands_str) / sizeof(char *); // Used to calcule the number of commands in the list.

    cli myCLI; // our Command Line instance

////////////////////////////////
// Menu manager
////////////////////////////////

 //menuManager menuManager;
 //menuPage_main mainMenu;
 void menuManagementTask();
// ------------------------------------------------------------------- //

////////////////////////////////
// Backup manager
////////////////////////////////
  // instance du backup manager pour le arduino nano ble 33
  backupManager_ArduinoNfr52 backupManager;
  backupStruct_t configDataBackup;
  void configurationBackupManager();
  void loadFlashConfig();
  void setDefaultConfigData(); // Put default valeus in the local struct;
  
  #define RESTORE_FACTORY_TIMER 20 // 20 * 50 ms = 2 sec of hold detect
  int restoreDefaultTimer1;
  int restoreDefaultTimer2;

  //void inductifinterrupt(); // MB
// ------------------------------------------------------------------- //

////////////////////////////////
// Bluetooth
////////////////////////////////

BLEService liveDataService("22B10000-E8F2-537E-4F6C-D104768A1215");
BLEService controlService("28B10000-E8F2-537E-4F6C-D104768A1215");    

BLECharacteristic liveDataCharacteristic("22B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLENotify,103);
BLEByteCharacteristic liveDataReadyCharacteristic("22B10002-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite| BLENotify);

BLEByteCharacteristic controlCharacteristic("28B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite);


bool gBluetoothConnected = false;
bool gSendDataEnabled = false;



// ------------------------------------------------------------------- //


// Project specific functions, regrouped Tasks of main
void alertManagement();
bool timeAlertEnable = true;
void logManagement();
void batteryManagement(); 

void bluetoothSetup();
void bluetoothTasks();
void sendConfigBluetooth(); // Send watch configuration to app
  
/**
 * @brief Configure Microcontroller inputs and outputs
 * 
 */
void IOSetup()
{

    // Analog resolution for Battery IN_BAT_VOLTAGE   A2
    analogReadResolution(12);

 //#if ( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )
    pinMode(OLED_DC, OUTPUT);
    pinMode(OLED_RESET, OUTPUT);
    pinMode(OLED_CS, OUTPUT);

    pinMode(led_sd, OUTPUT);
    digitalWrite(led_sd,LED_OFF); 

    pinMode(led_alert, OUTPUT);
    digitalWrite(led_alert,LED_OFF); 

    pinMode(sdCardChipSelect, OUTPUT);
    
    pinMode(IN_BUTTON_UP, INPUT_PULLUP); //tow?//
    pinMode(IN_BUTTON_DOWN, INPUT_PULLUP);//tow?//
    pinMode(IN_BUTTON_ENTER, INPUT_PULLUP);//tow?//
    pinMode(IN_BUTTON_RETURN, INPUT_PULLUP);//tow?//


  //pinMode(A0, INPUT);
  //attachInterrupt(digitalPinToInterrupt(A0), inductifinterrupt, FALLING);

}

//char bluetoothDeviceName[BLUTOOTH_NAME_MAX_SIZE];
void bluetoothSetup()
{

    // bluetooth initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
  }
    // set advertised local name and service UUID:
  
  //BLE.setDeviceName("Test 0002");
  
  //strcpy(bluetoothDeviceName,DEFAULT_BLUTOOTH_NAME);
  //Serial.println((const char *)bluetoothDeviceName);
  
  BLE.setDeviceName("Parasport BT1");
  //BLE.setDeviceName(DEFAULT_BLUTOOTH_NAME);
  BLE.setLocalName("Parasport BT1");

  BLE.advertise();

  BLE.setAdvertisedService(liveDataService);
  BLE.setAdvertisedService(controlService);

  // add the characteristic to the service

  liveDataService.addCharacteristic(liveDataCharacteristic);
  liveDataService.addCharacteristic(liveDataReadyCharacteristic);

  controlService.addCharacteristic(controlCharacteristic);

  // add service
  BLE.addService(liveDataService);
  BLE.addService(controlService);

  liveDataReadyCharacteristic.writeValue((uint8_t)0); // a 0 par défaut
  liveDataReadyCharacteristic.broadcast();

  // start advertising
  BLE.advertise();


}

/*
 * @brief Main SETUP, initialise variable and call needed peripherals setup functions 
 * 
 */
void setup() 
{
  #ifdef  DEBUG_MODE
    delay(1000); // comment after debug
  #endif 
  // Serial port configuration
  Serial.begin(115200);   //Select desired Baudrate
  //delay(3000);            // 3 sec delay to let type to the serial port to be ready, also usefull to be able to reprogram your device 
                          // if your code freezes the processor
  Serial.println("Begin");// Print on serial port to indicate that the system is starting
  
  //tow//serialPortLineBuffer.reserve(130);
  IOSetup(); // Setup IO function


  Wire.begin(); // I2C Initialization  (required for IMU, I2C OLED, etc)     

  #define SPI_FREQ_MHZ 8UL // A investiguer, le ble33 semble bloqué à 8Mhz, limit a 32 dans les specs...
  #define SPI_FREQ_HZ (SPI_FREQ_MHZ * 1000000UL)
  // Setup oled display  
    //dispManager.init_SPI(SCREEN_WIDTH_128, SCREEN_HEIGHT_64, &SPI, OLED_DC, OLED_RESET, OLED_CS, /*8000000UL*/SPI_FREQ_HZ,DISPLAY_REFRESH_PERIOD);  
    ////dispManager.init_I2C(SCREEN_WIDTH_128, SCREEN_HEIGHT_32, &Wire, OLED_RESET_SHARED,DISPLAY_REFRESH_PERIOD);                
    //dispManager.enableSleepMode(DISPLAY_SLEEP_TIME_MS); // After X milliseconds without being called, the display will switch to sleep mode
    //dispManager.disableAutoSleepMode();

    #define SCREEN_WIDTH_128 128 // OLED display width, in pixels
    #define SCREEN_HEIGHT_32 32 // OLED display height, in pixels   
    #define SCREEN_HEIGHT_64 64 // OLED display height, in pixels 

    //#define OLED_RESET -1 //4 // Reset pin # (Pin number or -1 if sharing Arduino reset pin)
delay(5000);
display = Adafruit_SSD1306(SCREEN_WIDTH_128, SCREEN_HEIGHT_64, &SPI, OLED_DC, OLED_RESET, OLED_CS, SPI_FREQ_HZ);
        if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
        {  
            //#ifdef DEBUG_INFO_SERIAL 
                Serial.println(F("initialisation SSD1306 non réussie"));
            //#endif

            //return DISPLAY_MANAGER_ERROR_BEGIN;
        }
        else {  
          // Vide le tampon de l'afficheur.  
          //display.clearDisplay();
          //display.display(); 
          //#ifdef DEBUG_INFO_SERIAL
            Serial.println(F("SSD1306 allocation success"));
          //#endif

        }

    
  // Button polling manager
    buttonManager.init(BM_NB_BUTTON,buttonList);

  // Setup imu manager
    imuInit();



  // Initialisation haptic drivers
  haptic.init(gHapticMode);
  
  //TODO compléter plus tard
  // Initialisation / création des triggers pour le haptic driver
  //triggerInit();
  

  // SD CARD
    //Setup the data logger
    dataLogger.init(sizeof(datastore),
                    logDataCount,
                    logDataTitles,
                    logDataTypes,
                    SDLOGGER_BINARY,      // Format to log data SDLOGGER_BINARY or SDLOGGER_ASCII
                    "LOG_CEINTURE1",            // Folder name for log on sd card //MB ICI
                    SDLOGGER_VERBOSE_ON   // Debug message print
                    );          
                 
    // Connect the function to use to convert binary data to ASCII (character) data
    //dataLogger.assignBinaryToCharConverter(logDataBinToChar);

   


  // Setup and start command line interpreter.
  myCLI.init(num_cli_commands,  // The number of implemented commands/functions
              commands_str,     // The string list for all the commands
              commands_func,    // The pointer lists for the commands
              &serialOut,       // Funtion to used to pu data on the serial port
              VERBOSE_OFF);     // Communicate status on serial port if OFF 
                                // ( on is useful for troubleshooting but slower)

  myCLI.start();  // Start to interpret commands on the serial port.
  

  // Menu and menu manager
    //mainMenu.connect(&menuManager,NULL);        // Connect teh page with the menu manager
    //mainMenu.connectDisplayManager(&dispManager); // Connec to the display manager so the menu can display on the Oled
    //menuManager.setRootPage(&mainMenu);         // Set the root page to the menu manager



    display.ssd1306_command(SSD1306_DISPLAYON);
    display.clearDisplay();
    display.setCursor(10,10);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.write("Maude \n Rules");
    //display.write(text1.c_str());
  //dispManager.widget_3Row(0,0,SCREEN_WIDTH_128,40,"Montre Ergonomie",1,"VERSION",1,CODE_VERSION,2);
    display.display(); 


    // Load config setting from flash
    setDefaultConfigData(); // Put default value in local struct. If first boot, thos values will be saved to backup memory
    int initResult = backupManager.init(&configDataBackup,sizeof(configDataBackup),PRODUCT_ID_MODULE_ERGO,0); // Init du backup manager.
    loadFlashConfig(); 

    //Force restor factory for now
    // setDefaultConfigData();
    // backupManager.saveConfig();
  
 //hereeeeeee
  /*dispManager.display.ssd1306_command(SSD1306_DISPLAYON);
  dispManager.display.clearDisplay();
  dispManager.widget_3Row(0,0,SCREEN_WIDTH_128,40,"Montre Ergonomie",1,"VERSION",1,CODE_VERSION,2);
  dispManager.widget_1row(0,40,SCREEN_WIDTH_128,24,String(mainMenu.page_config.bluetoothDeviceName).c_str(),1);
  dispManager.display.display();*/

  uint32_t timeStartMillis = millis();
  // Look on sd car for already present logs to avoir doing the work later when starting log.
  //  can be slow depending on the quantity og logs on the card
  dataLogger.resumeNumbering(); 
  
  // insures a 5 seconds delay minimum.  we done use a delay() function Because dataLogger.resumeNumbering() time 
  // vary depending on the number of log in memory so we don'T want to add another 5 seconds if this time is already passed
  while((millis() - timeStartMillis) < 3000);

  // dispManager.display.clearDisplay();
  // dispManager.widget_1row(0,0,SCREEN_WIDTH_128,SCREEN_HEIGHT_64,String(mainMenu.page_config.bluetoothDeviceName).c_str(),1);
  // dispManager.display.display();
  // timeStartMillis = millis();
  // while((millis() - timeStartMillis) < 3000);

  tonePlayer.loadSong(song_up);
  tonePlayer.start();

  bluetoothSetup();

}


/**
 * @brief Main program Loop
 * 
 */
void loop()
{  
  
  // Get the current time since startup. Used to do fixed frequency loop
  currentMillisStart = millis();

  tonePlayer.handle(currentMillisStart);

  unsigned long prevMillisTaks1000ms=0;

  if((currentMillisStart - prevMillisTaks1000ms) >= 1000)
  {
    prevMillisTaks1000ms = currentMillisStart; 
    //batteryManagement(); 
   
  }

  // 10 ms tasks
  if((currentMillisStart - prevMillisTaks10ms) >= 1)
  {
    prevMillisTaks10ms = currentMillisStart;
    //mainMenu.Callback10ms(); // Incrément compteurs internes seulement
    configurationBackupManager();
  }

  // 50 ms tasks
  if ( (currentMillisStart - prevMillis50ms) >= txIntervalMillis) 
  {
    prevMillis50ms = millis(); // Updat time with current time for next time.
    counter = counter + 1;
       
    //menuManager.handle(); //Execute menu related Tasks.

    // this function check if serial data was received, if so it shares the data with the CLI
    ManageSerialCommand();  

    //Serial.print(32);Serial.print(",");Serial.println(65);
  
    // The display manager needs to be cealled every loop. It will only do something
    // if the time elapsed matche the refresh frequency set befor and if new data has been receive.
    // If the refresh rate was se to zero, then you need to manage when to call this function.
    //dispManager.run(currentMillisStart); //heree

    buttonManager.handle(millis()); 
    humanInputDeviceManagement(); // Map butten event to menu and other effects if needed      



    /*display.clearDisplay();
    display.setCursor(10,10);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.write("Mon \n Rules");
    //display.write(text1.c_str());
  //dispManager.widget_3Row(0,0,SCREEN_WIDTH_128,40,"Montre Ergonomie",1,"VERSION",1,CODE_VERSION,2);
    display.display(); */
 

      
      /////////////////////////////////////////
      // ICI
        sens_zero_counter ++;
        
        float Ts = 0.02;
        float tau_ph = 30;
        float tau_lp = 0.06;

        val_s1_read = analogRead(sensor1_pin);
        val_s2_read = analogRead(sensor2_pin);
        if (sens_zero_counter <= 10)
        {
          sensor1ValueZero = val_s1_read;  
          sensor2ValueZero = val_s2_read;
        }
        int sensor1Value = val_s1_read - sensor1ValueZero;
        int sensor2Value = val_s2_read - sensor1ValueZero;
        
    /////////////////////////////////////////////////////////
        if (firstloop == 0)
        {
          ys1_filtHPLP_m1 = (float)sensor1Value;
        }
        ys1_filtHPLP = ys1_filtHPLP_m1*(1-Ts/tau_lp) + (float)sensor1Value*Ts/tau_lp;
        ys1_filtHPLP_m1 = ys1_filtHPLP;

        if (firstloop == 0)
        {
          ys2_filtHPLP_m1 = (float)sensor2Value;
        }
        ys2_filtHPLP = ys2_filtHPLP_m1*(1-Ts/tau_lp) + (float)sensor2Value*Ts/tau_lp;
        ys2_filtHPLP_m1 = ys2_filtHPLP;
        

        ys12_filtHPLP = ys1_filtHPLP*2 + ys2_filtHPLP*1;
    /////////////////////////////////////////////////////////
        if (buttonvalue == 1)
        {
          //alert = 1;
          if (ys12_filtHPLP > maxvalue)
          {
            maxvalue = ys12_filtHPLP;
          }
      
          if (ys12_filtHPLP < minvalue)
          {
            minvalue = ys12_filtHPLP;
          }
        }
        else
        {
          //alert = 0;
        }

    /////////////////////////////////////////////////////////
        if (buttonvalue3 == 1)
        {
          repos_expiratoire = ys12_filtHPLP;
          buttonvalue3=0;
        }
    /////////////////////////////////////////////////////////
        //if ((ys12_filtHPLP < (minvalue - pourcentagesousseuil*(maxvalue-minvalue))) && initdone)
        if (log_onoff==1)
        {
        if ((ys12_filtHPLP < ((repos_expiratoire) - pourcentagesousseuil*(maxvalue-minvalue))) && initdone && initdone2 && (buttonvalue==0))
        {
          alert = 1;
          //digitalWrite(D2,HIGH);
         if ((derniere_alert==0))
         {
          alert_count++ ;
          debut_temps_alert = currentMillisStart;
         }
        }
        else
        {
          alert = 0;
          //digitalWrite(D2,LOW);
          if (derniere_alert==1)
          {
           fin_temps_alert = currentMillisStart;
           temps_total_alert += (fin_temps_alert-debut_temps_alert);  
           temps_total_alert_secondes = static_cast<float>(temps_total_alert) / 1000.0;
          }
          
        }
        }
    /////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////

        audioval = analogRead(A3);
              
    /////////////////////////////////////////////////////////
      if (startstopserial == SERIAL_ENABLED)
      {
        Serial.print(audioval);
        Serial.print(" ");
        //Serial.print(sensor1Value);
        //Serial.print(" ");
        //Serial.print(sensor2Value);
        //Serial.print(" ");
        Serial.print(ys1_filtHPLP);
        Serial.print(" ");
        Serial.print(ys2_filtHPLP);
        Serial.print(" ");
        Serial.print(ys12_filtHPLP);
        Serial.print(" ");
        Serial.print(minvalue);
        Serial.print(" ");
        Serial.print(maxvalue);
        Serial.print(" ");
        Serial.print(repos_expiratoire);
        Serial.print(" ");
        Serial.print(temps_total_alert_secondes);
        Serial.print("\n");
      }

        // if alert not already signaled
        alertManagement(); 

      logManagement();

      //bluetoothTasks();
    // If some algorithm need to be initalized once when starting.
    if (firstloop == 0)
    {

    }
        
    loopintervalMillis = millis() - currentMillisStart;
    if (startstopserial || startstopserial2)
    {
      //Serial.println(loopintervalMillis);
      //Serial.println("");
    }
    firstloop=1; // Se flag to true so it is only false for the first pass in the loop.
  }
derniere_alert = alert;

} // loop()




void alertManagement()
{

      /*if(maxAngleAlertCooldown)
      {
        maxAngleAlertCooldown --;
      }*/

      
  if(alert == 1 && alertonoff == 1 && buttonvalue==0)
  {
    haptic.playWaveform(DRV2605_WF_ALERT_10000ms);

    digitalWrite(led_alert, LED_ON);
    //tone(SPEAKER_OUT, TONE_D5/3 );
  }
  else
  {
    digitalWrite(led_alert, LED_OFF);
    //noTone(SPEAKER_OUT);
  }


}

void logManagement()
{
    //log_onoff = mainMenu.getLogStatus();
    //log_onoff = true;
    if (log_onoff == true)
    {
      if(last_log_onoff != log_onoff) 
      {
        //Serial.println("Log start catch."); 
        //tonePlayer.loadSong(song_beep);
        //tonePlayer.start();
      }
      //-----------------SD CARD---------------------------------------------// 
        // if SD is selected for logging data and file not created yet, then we need to start a new log
        if ( (SerialSD == LOG_SD) && (flag_sdLogFileOpen == 0) )  
        {
          // SD CARD log //
          if(dataLogger.startNewLog()) //This function creates and open a new log file
          {
            // If no SD card present, we switch to serial port
            #ifdef  DEBUG_MODE
              Serial.print("SD error. Going to Serial.");
            #endif 
            SerialSD=0;
            // mainMenu.setLogMode(logModes::logMode_SERIAL);
          }
          else
          {
            
            digitalWrite(led_sd, LED_ON); // Turn Red Led On to indicate logger is active
            loggindata = 1;
            int logNumber = dataLogger.getActiveLogNumber();
            //MB ici mettre les zéros 
            #ifdef  DEBUG_MODE
              Serial.println("Sd card success");
              Serial.print("Log number: ");Serial.println(logNumber);
            #endif
            if(logNumber>=0)
            {
                //tow//mainMenu.setLogNumber(std::to_string(logNumber));
                //tow//mainMenu.setLogMode(logModes::logMode_SD);
            }
            
          }
                
          flag_sdLogFileOpen = 1;
        }

      // Log data ----------------------------------------------------------//
        if (SerialSD == LOG_SERIAL) // Log data on serial port
        {
            //mainMenu.setLogMode(logModes::logMode_SERIAL);
            // Choose number of IMU by setting NB_IMU in the defines at teh top of this file
            if (startstopserial == SERIAL_ENABLED)
            {
                //Serial.print(elevationResult.elevationRelative*180/PI,8);Serial.print(" * ");Serial.print(elevationResult.elevationAbsolute*180/PI,8);Serial.print(" ** ");Serial.print(loopintervalMillis);Serial.print(" ** ");Serial.print(elevationMetrics.elevTime_total);Serial.print(" * ");Serial.print(elevationMetrics.elevTime_finite);Serial.print(" * ");Serial.print(elevationMetrics.elevTime_finite_resetfeedback);Serial.print(" ** ");Serial.print(elevationMetrics.nbElev_total);Serial.print(" * ");Serial.print(elevationMetrics.nbElev_finite);Serial.print(" * ");Serial.print(elevationMetrics.nbElev_finite_resetfeedback);Serial.print(" ** ");Serial.print(vib_number_warning);
                /*Serial.print(imua_ax);Serial.print(" ");Serial.print(imua_ay);Serial.print(" ");Serial.print(imua_az);
                Serial.print(" * ");
                Serial.print(imua_gx);Serial.print(" ");Serial.print(imua_gy);Serial.print(" ");Serial.print(imua_gz);
                Serial.print(" * ");Serial.print(elevationResult.elevationRelative*180/PI);
                Serial.print(" * ");Serial.print(loopintervalMillis);
                Serial.println("");*/
                //Serial.print(elevationResult.elevationRelative*180/PI);Serial.print(" ");Serial.print(loopintervalMillis);Serial.println("");

              // Only write message one time on status change, no need to send it every time and loose processing time
              //if(last_log_onoff != log_onoff) 
              //{
              //  dispManager.writeMessage((String)"No SD. \nSending to serial"); // Disply status on oled.
              //}
            }
            else // Serial print disabled
            {
              //if(last_log_onoff != log_onoff) // Only write message ones on status change
              //{
              //  dispManager.writeMessage((String)"No SD. \nSerial disabled"); //Display status on oled.
              //}
            }
        }
        else // Log Data to SD Card
        {
          datastore myData; // Conted to get data
 
          //myData.d_counter = counter;
          myData.d_timestamp = currentMillisStart;//millis();       
          myData.d_sensor1 = ys1_filtHPLP; 
          myData.d_sensor2 = ys2_filtHPLP; 
          myData.d_sensor12 = ys12_filtHPLP; //MB Valeur de l'acceleration en x filtrée
          myData.d_alert = alert; 
          myData.d_min = minvalue; //MB vitesse calculée avec speedometre méthode 1
          myData.d_max = maxvalue; //MB vitesse calculée avec speedometre méthode 2
          myData.d_audio = audioval; //MB vitesse avec l'accélécation
          myData.d_alert_count= alert_count;
          myData.d_temps_total_alert_secondes= temps_total_alert_secondes;
   
          // Write data on sd card followng the format defined during setup
          if(dataLogger.write((const uint8_t *)&myData, // Pointer to the data to write
                              sizeof(myData),1))          // Data size in bytes
          {
            #ifdef  DEBUG_MODE
              Serial.println("Error writing to log");   
            #endif           
          }
                                        
          

          // Display active log file on the Oled display. 
          if(dataLogger.getActiveLogName(fileName) && (last_log_onoff != log_onoff)) // Only write message ones on status change
          {
              //dispManager.disableAutoSleepMode(); // Disable sleep mode when loggin to show information duril all log time
              ////dispManager.writeMessage((String)"Logging to: \n " + (String)fileName); // Print file on the Oled display. 
                  display.clearDisplay();
                  display.setCursor(10,10);
                  display.setTextSize(1);
                  display.setTextColor(WHITE);
                  String temptext = "Logging to: \n " + (String)fileName;
                  display.println(temptext);
                  display.display(); 
          }                      
        }
      // ------------------------------------------------------------------- //

    } // Log is not enable.
    else
    {
      // If a fil is opened, it means that the log was just stopped. We need to close the file and 
      // turn off the led that indicates that log is in progress
      if(flag_sdLogFileOpen)
      {
        dataLogger.stopLog();         // Close file
        digitalWrite(led_sd, LED_OFF); // Turn off led
        loggindata = 0;
        flag_sdLogFileOpen = 0;       // Flag to indicate that no file is opened
      }
      
      if(last_log_onoff != log_onoff && !log_onoff) // Only write message ones on status change
      {
          display.clearDisplay();
          display.setCursor(10,10);
          display.setTextSize(1);
          display.setTextColor(WHITE);
          display.write("System on. \nNot logging.");
          display.display(); 

          Serial.println("LOG ON - not logging");

          
         //dispManager.writeMessage((String)"System on. \nNot logging.");
         //TEMPORAIRE dispManager.enableSleepMode(DISPLAY_SLEEP_TIME_MS);  // Put back auto sleep timer
         //mainMenu.setLogMode(logModes::logMode_off);
         SerialSD = logPreferedDest; // Restor log destination to the prefered method
      }
      else if (!log_onoff)
      {
          //if (!initdone) 
          //{//heree
            display.clearDisplay();
            display.setCursor(0,0);
            display.setTextSize(1);
            display.setTextColor(WHITE);
            String phrase;
            String phrasealert;
            String phrasecalib;
            String phraserange;
            if ((val_s1_read <=100) || (val_s1_read >=3900) || (val_s2_read <=100) || (val_s2_read >=3900) )
            {
              phraserange = (String)"Alerte plage capteurs";
            }
            else
            {
              phraserange = (String)((int)ys1_filtHPLP) + (String)" , " + (String)((int)ys2_filtHPLP);
            }
            if (initdone2 && !initdone)
            {
              phrasecalib = (String)" Calib repos terminee\n"; 
            }
            else if(initdone)
            {
              phrasecalib = (String)" Calibration terminee\n";
            }
            else
            {
              phrasecalib = "";
            }
            if (alertonoff == 1)
            {
              phrasealert = "\n \n Vibration ON";
            }
            else
            {
              phrasealert = "\n \n Vibration OFF";
            }
            int volatgeMilli  = (int)((float)analogRead(IN_BAT_VOLTAGE)*3300*11.0/4095.0);
            String batteryInfo = (String)"\nBat: " + (String) batPercent; 

            phrase = (String)"System on. \nNot logging. \n \n" + phrasecalib + phraserange + phrasealert + batteryInfo;
            display.println(phrase);
            display.display(); 
          //}
      }

      
                  
    }

    // Update to track change
    last_log_onoff = log_onoff;
    //// END LOG MANAGEMENT

    // If some algorithm need to be initalized once when starting.
    if (firstloop == 0)
    {
      
    }
        
    loopintervalMillis = millis() - currentMillisStart;

    firstloop=1; // Se flag to true so it is only false for the first pass in the loop.

  
}

/**
 * @brief Get button event and assigne desired actions
 * 
 * @note Important: Make sur the event type is activated for the button 
 *        If for example you want to use a button hold functionality, the flah HOLD_ENABLE
 *        must be used when setting button manager
 * 
 */
 int value=0;
 int holdreturnfirst = 0;
 int firstbutton3 = 1;
void humanInputDeviceManagement()
{


  if(buttonManager.getClickEvent(BUTTON_RETURN_INDEX)) //Button 1
  {
    if(firstbutton3)
    {
     buttonvalue3=1;
     firstbutton3=0;
     initdone2 =1;
    }
    else
    {
     buttonvalue3=0; 
    }
  }

  if(buttonManager.getLongClickEvent(BUTTON_RETURN_INDEX))  //Button 1
  {


  }

  if(buttonManager.getDoubleClickEvent(BUTTON_RETURN_INDEX))  //Button 1
  { 

  }
  
  if(buttonManager.getHoldEvent(BUTTON_RETURN_INDEX))  //Button 1
  { 
    if (holdreturnfirst==1 && initdone2)
    {
      maxvalue=0;
      minvalue=25000;
      initdone = 1;
    }
    holdreturnfirst = 0;

    buttonvalue = 1;
  }
  else
  {
    buttonvalue = 0;
    holdreturnfirst = 1;
  }



  if(buttonManager.getClickEvent(BUTTON_UP_INDEX))  //Button 2
  {
     value=1-value;
     //digitalWrite(led_sd,value);
     //menuManager.onPressUp();
      log_onoff = 1-log_onoff;
  }

  if(buttonManager.getLongClickEvent(BUTTON_UP_INDEX))  //Button 2
  {

  }

  if(buttonManager.getDoubleClickEvent(BUTTON_UP_INDEX))  //Button 2
  {
  }

  if(buttonManager.getHoldEvent(BUTTON_UP_INDEX)) //Button 2 hold
  {
     
  }

  if(buttonManager.getClickEvent(BUTTON_DOWN_INDEX)) //Button 3 click
  {
  }

  if(buttonManager.getLongClickEvent(BUTTON_DOWN_INDEX)) //Button 3 long click
  {

  }

  if(buttonManager.getDoubleClickEvent(BUTTON_DOWN_INDEX)) //Button 3 double click
  {
  }

  if(buttonManager.getHoldEvent(BUTTON_DOWN_INDEX)) //Button 3 hold
  {

  }


  if(buttonManager.getClickEvent(BUTTON_ENTER_INDEX)) //Button 4 click
  {
    alertonoff = 1 - alertonoff;
  }

  if(buttonManager.getLongClickEvent(BUTTON_ENTER_INDEX)) //Button 4 click
  {
  }

  if(buttonManager.getDoubleClickEvent(BUTTON_ENTER_INDEX)) //Button 4 click
  {
  }

  if(buttonManager.getHoldEvent(BUTTON_ENTER_INDEX)) //Button 4 hold
  {

  }
  else
  {

  }


}






/**
 * @brief Outputs data on serial port
 * 
 * @note This function was implemented because some of the librarie are made to be used on 
 *       other systems as well and do not use Arduino specific function function. 
 * 
 * @param outData Data to send on the serial port
 */
void serialOut(const char* outData)
{
  Serial.print(outData);
}


/**
 * @brief This function takes the binary data and prints it in a human readable format.
 * 
 * @param binData the input binary data
 * @param charData teh data converted in char
 */
//void logDataBinToChar(const uint8_t* binData, char* charData)
//{
  //CONFIGURE: Adapt this code to match your data structure for logs
  //datastore *dataPtr;
  //dataPtr = (datastore *)binData;
//
 // sprintf(charData,"%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f"
 //         ,dataPtr->d_counter
 //         ,dataPtr->d_timestamp
   //       ,dataPtr->d_accx
     //     ,dataPtr->d_accy
 //         ,dataPtr->d_accz
 //         ,dataPtr->d_gyrx
 //         ,dataPtr->d_gyry
 //         ,dataPtr->d_gyrz
 //         ,dataPtr->d_proxsignal
 //         ,dataPtr->d_speed1
 //         ,dataPtr->d_speed2
 //         ,dataPtr->d_speed3);
//}

/**
 * @brief This function checks if serial data has been received and if so
 *        pass it along to the CLI
 */
void ManageSerialCommand()
{
  if (Serial.available())
  {
    String ReadSerialPortVal = Serial.readString(); // Read Serial port
    //Serial.println(ReadSerialPortVal);
    myCLI.run(ReadSerialPortVal.c_str()); // Call to command line interpreter. 
  }
}

/**
 * @brief This function is used to get out of the loop and quit program
 * 
 * @param argString not used
 * @return int 
 */
int cmd_exit(const char* argString){

    startstopserial = 0;
    
    return 0; // The retunerned vaue will be also return by cli.run()
}

/**
 * @brief This function is used to get out of the loop and quit program
 * 
 * @param argString not used
 * @return int 
 */
int cmd_serialLog(const char* argString){

  String argument;

  argument = argString;
    if(!strcmp("on",argString))
    {
        startstopserial = 1;
    }
    else if(!strcmp("off",argString))
    {
      startstopserial = 0;
    }
    
    return 0; // The retunerned vaue will be also return by cli.run()
}


/**
 * @brief CALLBACK function that lists all the file son a SD card
 * 
 * @param argString   Not used
 * @return int status
 */
int cmd_listfiles(const char* argString){

  uint8_t status;
  uint8_t displayTimeStamp = 0;

  if(NULL != strstr(argString,"time"))
  {
      displayTimeStamp = 1;
  }
  status = dataLogger.printFileList(displayTimeStamp);
  
  if(status)
  {
      Serial.print("Error listing files: ");
      Serial.print(status);
      Serial.print("\n\r");
  }

  serialOut("--END OF FILE LIST--\n\r");

  return status;
}


/**
 * @brief Callback to the function to read one file in on the sdCard
 * 
 * command Format: "readdata XXXX"  where XXXX is the number at the end of the file
 * 
 * Ex:   to open Data01.dat type: readdata 01
 *       to open Data001.dat type: readdata 001
 * 
 * @param argString is the argument XX 
 * @return int status
 */
int cmd_readdata(const char* argString)
{
  uint8_t status;

  status = dataLogger.printFile(argString/*,sizeof(datastore)*/);

  if(status)
  {
    Serial.print("Problem with SD card: ");
    Serial.print(status);
  }   
  
  serialOut("-- END OF DATA --\n\r");
  
  return status;
}




/**
 * @brief Callback from the CLI to the delete SD card function
 * 
 * @param argString The argument typed by the user
 * @return int error status
 */
int cmd_deletesdcard(const char* argString)
{
  uint8_t status;


  String argument;

    argument = argString;
    if(!strcmp("all",argString))
    {
        status = dataLogger.deleteSdCard();
        if(!status)
        {
          Serial.println("Sd Card cleaned");
        }
        else
        {
          Serial.println("Problem with SD card");
        }
    }
    else if(!strcmp("log",argString) )
    {
        status = dataLogger.deleteLogFolder();
        if(!status)
        {
          Serial.println("Log folder deleted");
        }
        else
        {
          if(status == SDLOGGER_ERROR_FILEOPEN)
          {
              Serial.print("Folder not present");
          }
          else
          {
              Serial.print("Problem with SD card:");
              Serial.println(status);
          }
          
        }
    }
    else
    {
        Serial.println("\nMissing param:");
        Serial.println("  delete log --> deletes the log folder only");
        Serial.println("  delete all --> deletes everythign on the sd card");

    }




  
  
  return status;

}

/**
 * @brief 
 * 
 * @param argString 
 * @return int 
 */
int cmd_restoreFactoryDefault(const char* argString)
{
  uint8_t status = ERROR_NO_ERROR;

    setDefaultConfigData();
    backupManager.saveConfig();
    loadFlashConfig();
  

  // if(status)
  // {
  //   Serial.print("Problem with SD card: ");
  //   Serial.print(status);
  // }   
  
  serialOut("\n\r--> Factory data restored\n\r");
  
  return status;
}

  int cmd_test(const char* argString)// used for tests on command
  {
    uint8_t status = ERROR_NO_ERROR;

     datastore myData; // Conted to get data
     char charData[500];
     
       myData.d_timestamp = 3;       
          myData.d_sensor1 = 1; 
          myData.d_sensor2 = 2; 
          myData.d_sensor12 = 20; 
          myData.d_alert = 30; 
          myData.d_min = 50;
          myData.d_max = 60;
          myData.d_audio = 7;
          myData.d_alert_count=70;
          myData.d_temps_total_alert_secondes=80;
      dataLogger.binToCharDataConvert((uint8_t*)&myData, charData, logDataTypes,logDataCount);

       serialOut(charData);
    
    return status;
  }

void setDefaultConfigData()
{
    configDataBackup.alertsOnOff[0] = DEFAULT_VIBRATION_ONOFF;                    
    configDataBackup.alertsOnOff[1] = DEFAULT_TONE_ONOFF;                         
    configDataBackup.alertsOnOff[2] = DEFAULT_REPALERT_ONOFF;                     
    configDataBackup.alertsOnOff[3] = DEFAULT_TIMEALERT_ONOFF;                    
    configDataBackup.alertsOnOff[4] = DEFAULT_ANGLEDETECT_ONOFF;                  
    configDataBackup.alertsOnOff[5] = DEFAULT_ANGLEMAX_ONOFF;     

    configDataBackup.angleElevation = ANGLE_DETECT_DEFAULT;
    configDataBackup.angleLimit = ANGLE_MAX_DEFAULT;    
    configDataBackup.periodLenght_lastXmin = REF_PERIOD_DEFAULT;
    configDataBackup.repLimit_lastXmin = REF_PERIOD_REP_DEFAULT;
    configDataBackup.timeLimit_lastXmin = REF_PERIOD_TIME_DEFAULT; 

    strcpy(configDataBackup.bluetoothDeviceName,DEFAULT_BLUTOOTH_NAME);        
}

void loadFlashConfig()
{

  int result = backupManager.loadConfig();
  
  if(result)
  {
    Serial.println("Error loading flash data");

    while(1); // TODO améliorer réponse
  }
  else{
    //mainMenu.page_config.pageAlerts.setAngleDetectAlertEnable(configDataBackup.alertsOnOff[INDEX_DEFAULT_ANGLEDETECT_ONOFF]);
    //mainMenu.page_config.pageAlerts.setAngleMaxAlertEnable(configDataBackup.alertsOnOff[INDEX_DEFAULT_ANGLEMAX_ONOFF]);
    //mainMenu.page_config.pageAlerts.setRepetitionAlertEnable( configDataBackup.alertsOnOff[INDEX_DEFAULT_REPALERT_ONOFF]);
    //mainMenu.page_config.pageAlerts.setTimeAlertEnable(configDataBackup.alertsOnOff[INDEX_DEFAULT_TIMEALERT_ONOFF]);
    //mainMenu.page_config.pageAlerts.setToneAlertEnable(configDataBackup.alertsOnOff[INDEX_DEFAULT_TONE_ONOFF]);
    //mainMenu.page_config.pageAlerts.setVibrationAlertEnable(configDataBackup.alertsOnOff[INDEX_DEFAULT_VIBRATION_ONOFF]);

    //mainMenu.page_config.page_rep.mRepLimit = configDataBackup.repLimit_lastXmin;
    //mainMenu.page_config.page_time.mTimeTargetMilliseconds = configDataBackup.timeLimit_lastXmin;
    //mainMenu.page_config.page_Length.mPeriodLength = configDataBackup.periodLenght_lastXmin;

    //mainMenu.page_config.page_angle_detect.mAngleLimit = configDataBackup.angleElevation;
    //mainMenu.page_config.page_angle_max.mAngleLimit = configDataBackup.angleLimit;  

    //strcpy(mainMenu.page_config.bluetoothDeviceName,configDataBackup.bluetoothDeviceName);  

    Serial.print("Device Name:");
    //Serial.println(mainMenu.page_config.bluetoothDeviceName);

    // char btData[8];
    // sprintf(btData," %d",mainMenu.getRepLimitXmin());
    // repTargetLastXminCharacteristic.writeValue(btData);
    // repTargetLastXminCharacteristic.broadcast();

    // sprintf(btData," %d", ((int)mainMenu.getTimeLimitXmin_milliseconds())/1000/60);
    // timeTargetLastXminCharacteristic.writeValue(btData);
    // timeTargetLastXminCharacteristic.broadcast();

  }


  
  
}

/**
 * @brief This functions checks if config has change and if some data need to be backed up in the flash.
 * 
 */
//TODO optimiser la logique dans les menus pour qu'on enregistre juste quand on quitte le mode config, pas a tous les changements
void configurationBackupManager()
{
    bool flashUpdateRequired = false;
    char btData[8];


    // Force Restore factory default by holding 2 buttons for 5 seconds
  if(restoreDefaultTimer1 > RESTORE_FACTORY_TIMER && restoreDefaultTimer2 > RESTORE_FACTORY_TIMER)
  {
    restoreDefaultTimer1 = 0;
    restoreDefaultTimer2 = 0;

    //loadDefaultConfig(&configData);
    setDefaultConfigData();
    backupManager.saveConfig();
    loadFlashConfig();
    //#ifdef DEBUG_SERIAL_LOW_LEVEL
      Serial.println("Default values loaded");
    //#endif

    //dispManager.display.clearDisplay();

    //dispManager.writeMessage("Factory settings restored");
    //dispManager.needRefresh();
    //dispManager.run();
    delay(5000);
  }


}        

/**
 * @brief This functions sets the time to produce the frequency of the note
 * 
 * @param note Is the comparator value required for the not. @see toneplayer.h
 */
void noteOn(unsigned int note)
{
   if(note > TONE_SILENCE )
   {
    tone(SPEAKER_OUT, note/3 );
   } 
    
}

/**
 * @brief disable Timer 4 to stop output.
 * 
 */
void noteOff()
{
  noTone(SPEAKER_OUT);
}

/**
 * @brief Thsi fucntion manage batterie value acquisition and conversion from ADC value to %
 * 
 */

int batPercent;

void  batteryManagement()
{


  //int  batValue = analogRead(IN_BAT_VOLTAGE)*112;  //112 vient de 3300/4095*1,4*100 , *100 pour garder de la précision sans prendre de float et 1.4 du diviseur de voltage avec les résistances
 // int  batValue = analogRead(IN_BAT_VOLTAGE)*1222;  //vient de 1,2217*1000 , *1000 pour garder de la précision sans prendre de float et 1.2217 du diviseur de voltage avec les résistances+ conversion analog/4095
 
  //int volatgeMilli = batValue / 1000;             // divise par 100 pour avoir al valeur en millivoltes
  int volatgeMilli  = (int)((float)analogRead(IN_BAT_VOLTAGE)*3300*11.0/4095.0);//*1240909; //  old:= *1000/3.3*4095;
  // Note: percetange offset vs real percentage to protect battery valeur estimé rapidement à partir d'un table, à préciser éventuellement
  int batteryLookupTable[10][2] = {{9000,100},  // 100%+ 
                                  {8700,80},   // 90%+ actual  
                                  {8400,70},   // 80%+ 
                                  {8100,60},   // 70%+ 
                                  {7800,50},   // 60%+ 
                                  {7500,40},   // 50%+ 
                                  {7200,30},   // 40%+ 
                                  {6900,20},   // 30%+ 
                                  {6600,10},   // 20%+ 
                                  {6300,0}};    // 10%+ 

    for(int i = 0 ; i < 10; i++ )
    {
      if(volatgeMilli >= batteryLookupTable[i][0])
      {
        batPercent = batteryLookupTable[i][1];
        i = 10;
      }
    }

  
  //Serial.print("Sensor: "); Serial.print(batValue); Serial.print(" Voltage milli: "); Serial.println(volatgeMilli);
  //Serial.print(" Voltage milli: "); Serial.println(mainMenu.batteryPercent);
  
}


int cmd_downloadData(const char* argString)
{
  uint8_t status;

  status = dataLogger.downloadFile(argString);

  return status;
}
// ***************************************************
