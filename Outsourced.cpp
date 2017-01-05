


int mcp3008(){

    /*
     * CS Pin = GPIO 25, HW# 22
     * sclk Pin = GPIO 18, HW# 12
     * miso pin = GPIO 23, HW# 16
     * mosi pin = GPIO 24, HW# 18
     */




    float max_volt = 3.3;
    int bit_resolution = 1023; // 10bit
    float voltresolution = max_volt / bit_resolution;

    int cs_pin = 25;
    int sclk_pin = 18;
    int miso_pin = 23;
    int mosi_pin = 24;


    int mosi_hexZahl = 0x18;  // 0101 0101
    int hexZahl = mosi_hexZahl;
    float taktrate = 500; // Herz  1 Mhz = 1 000 000 Hz
    float periodendauer = 1 / taktrate;
    string inputstate;
    int mask = 0;

    float actual_voltage = 0;
    int AD_Channel = 0;
    // todo auf MOSI Bit draufrechnen

    int treshold = 2;
    int prev_mask = 0;
    // todo treshold prev_voltage einbauen

    // Chip Select Pin initieren
    GPIOHandle gpio_CS(std::to_string(cs_pin));
    gpio_CS.export_gpio();
    gpio_CS.setdir_gpio("out");
    gpio_CS.setval_gpio("1");

    // Clock initietein
    GPIOHandle gpio_SCLK( std::to_string(sclk_pin) );
    gpio_SCLK.export_gpio();
    gpio_SCLK.setdir_gpio("out");
    gpio_SCLK.setval_gpio("0");

    GPIOHandle gpioMOSI(std::to_string(mosi_pin) );
    gpioMOSI.export_gpio();
    gpioMOSI.setdir_gpio("out");
    gpioMOSI.setval_gpio("0");

    GPIOHandle gpioMISO("23");
    gpioMISO.export_gpio();
    gpioMISO.setdir_gpio("in");

    std::cout << "SendMSG binary: " << std::bitset<8>(hexZahl)  << std::endl;
    std::cout << "Periodendauer: " << periodendauer << " seconds";
    std::cout << "| Voltresolution: " << voltresolution << " V" << endl;

    size_t halfperiod = periodendauer/2 * 1000000;
    size_t t_sucs = 0; // Minimale Wartezeit nach Chip Select High 100 ns = 0,1 Mikrosekunden;  = 0,0001, Milisekunden
    size_t t_csh = 0; // Minimale Chip Select High Zeit




    for (int x = 0; x < 10; x++) {
        //std::cout << "**** Do Measurement ****" << endl;

        gpio_CS.setval_gpio("0");
        usleep(periodendauer);

        // std::cout << "MOSI: ";
        hexZahl = mosi_hexZahl;
        for (int i = 0; i < 8; i++) {
            if ((hexZahl & 0x80) == 0x80) { //0x80 = 128
                // std::cout << " H | ";
                gpioMOSI.setval_gpio("1");
            }
            else {
                // std::cout << " L | ";
                gpioMOSI.setval_gpio("0");
            }

            // Clock generieren
            gpio_SCLK.setval_gpio("1");
            usleep(halfperiod);
            //std::cout << "periodendauer/2 * 1000000: " << periodendauer/2 * 1000000 << " seconds";
            gpio_SCLK.setval_gpio("0");
            usleep(halfperiod);
            hexZahl <<= 1;
        }
        // cout << endl;

        // Bitauslen
        // Bit generieren
        int genBitLength = 8;
        int fakewert = 1;
        int genHexZahl = 0;
        mask = 0;

        // std::cout << "MISO: ";
        for (int x = 0; x < 12; x++) {

            mask <<= 1;
            // Clock Rais genererieren
            gpio_SCLK.setval_gpio("1");

            gpioMISO.getval_gpio(inputstate);
            //std::cout << inputstate << " | ";
            // std::cout << " |" << x <<  ":" << inputstate ;


            if (inputstate=="1") {
                //std::cout << x << ".Generate 1" << std::endl;
                mask |= 0x01;
                //mask |= 0x01 << x;
            } else {
                //std::cout << x << ".Generate 0" << std::endl;
                mask |= 0x00;
                //mask |= 0x00 << x;
            }
            usleep(halfperiod);
            gpio_SCLK.setval_gpio("0");
            usleep(halfperiod);
        }

        actual_voltage = mask * voltresolution;

        if ( (abs(mask*1 - prev_mask*1)) > treshold)
        {
            // cout << std::endl;
            // std::cout << "MISO HEX: " << std::hex << mask;
            // std::cout << " | binary: " << std::bitset<10>(mask) << std::endl;
            std::cout << "Final decimal: " << std::dec << mask;
            std::cout << " | Voltage: " << actual_voltage << " V " << std::endl;
        }
        else {
            std::cout << "Final decimal: " << std::dec << mask;
            std::cout << " | Voltage: " << actual_voltage << " V " << " | treshold: " << treshold << std::endl;

        }

        prev_mask = mask;

        gpio_CS.setval_gpio("1");
        usleep(periodendauer);

    }

    gpio_CS.unexport_gpio();
    gpio_SCLK.unexport_gpio();
    gpioMOSI.unexport_gpio();
    gpioMISO.unexport_gpio();

    return 0;

}



int toggleButton(){

    cout << "< Info > my_Project_0.2 staaArting" << endl;
    string inputstate;
    int laststate = 0;
    int btnpushed = 0;
    int state = 0;
    int counter = 0;

    GPIOHandle gpioOUT("4");
    gpioOUT.export_gpio();
    gpioOUT.setdir_gpio("out");
    gpioOUT.blink(500000);

    GPIOHandle gpioIN("17");
    gpioIN.export_gpio();
    gpioIN.setdir_gpio("in");
    gpioIN.setdir_gpio("in");

    while(1) {
        usleep(10000);
        gpioIN.getval_gpio(inputstate);
        // cout << "< Info > Current input pin state is " << inputstate  << " laststate: " << laststate <<  " counter: " << counter << endl;
        if ((inputstate=="0") && (laststate == 0)) {
            cout << "< Info > Button is pushed" << endl;
            laststate=1; // turn LED ON
            counter++;

            if (counter % 2 == 1) {
                //cout << "< Info > turn on led" << endl;
                gpioOUT.setval_gpio("1"); // turn LED ON
            }

            if (counter% 2 == 0) {
                //cout << "< Info > turn off led" << endl;
                gpioOUT.setval_gpio("0"); // turn LED ON
            }

        }

        if ((inputstate=="1") && (laststate == 1)) {
            cout << "< Info > Button is losgelassen" << endl;
            laststate=0; // turn LED ON



        }


    }

    gpioIN.unexport_gpio();
    gpioOUT.unexport_gpio();


    return 0;

}


int bitschieber(){

    GPIOHandle gpioOUT("4");    // #Pin07 GPIO04
    gpioOUT.export_gpio();
    gpioOUT.setdir_gpio("out");
    //gpioOUT.blink(500000);

    GPIOHandle gpioClock("18");
    gpioClock.export_gpio();
    gpioClock.setdir_gpio("out");
//    gpioClock.blink(500000);

    GPIOHandle gpioMOSI("24");
    gpioMOSI.export_gpio();
    gpioMOSI.setdir_gpio("out");

    GPIOHandle gpioMISO("23");
    gpioMISO.export_gpio();
    gpioMISO.setdir_gpio("in");



    //int hexZahl = 0x87;  // 0000 0111
    int hexZahl = 0x87;  //  0000 0111
    float taktrate = 1; // Herz  1 Mhz = 1 000 000 Hz
    int periodendauer = 1 / taktrate;


    std::cout << "hexZahl als Dezimal = " << hexZahl  << std::endl;
    std::cout << "hexZahl als binÃ¤r = " << std::bitset<8>(hexZahl)  << std::endl;
    std::cout << "hexZahl als hex = " << std::hex << hexZahl  << std::endl;

    for (int x = 0; x < 8; x++) {
        hexZahl = 0x87;
        // Bit-Schieber von Bit 8 -> 0
        for (int i = 0; i < 8; i++) {
            if ((hexZahl & 128) == 0x80) { //0x80 = 128
                std::cout << " H | ";
                gpioMOSI.setval_gpio("1");

            }
            else {
                std::cout << " L | ";
                gpioMOSI.setval_gpio("0");
            }
            gpioClock.setval_gpio("1");
            usleep(5000);
            gpioClock.setval_gpio("0");

            hexZahl <<= 1;

        }
    }

    // Bitauslen
    string inputstate;
    for (int x = 0; x < 11; x++) {

        // gpioMISO.getval_gpio(inputstate);
        // cout << "< Info > Current input pin state is " << inputstate  << endl;

    }
    std::cout << std::endl;

    gpioOUT.unexport_gpio();

    return 0;
}
