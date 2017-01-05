//
// Created by Andreas on 03.01.16.
//

#include "GPIOHandle.h"
/*
 * GPIOHandle.cpp
 */

#include <iostream>
#include "GPIOHandle.h"
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <string>

#include <sstream>


using namespace std;


GPIOHandle::GPIOHandle() {
    cout << "< Info > GPIOHandle Konstruktor" << endl;

    sGPIO="0";
    //iGPIO=0;

}

GPIOHandle::GPIOHandle(string gpionum) {
    cout << "< Info > GPIOHandle ueberl. Konstruktor mit gpio: " << gpionum << endl;
    sGPIO= gpionum;

}

int GPIOHandle::setGPIO(std::string gpionum) {
    cout << "< Info > GPIOHandle::setGPIO mit gpio: " << gpionum << endl;
    sGPIO= gpionum;
    return 0;

}

int GPIOHandle::createSquareSignal( float tastgrad_perc, float clockrate) {


    float periodendauer_s = 1 / clockrate;
    float hightime = (tastgrad_perc / 100 ) * periodendauer_s;
    float lowtime = periodendauer_s - hightime;

    cout << "< Info > 1.createSquareSignal GPIO: " << this->sGPIO  << " tastgrad_perc: " << tastgrad_perc << " Taktrate: " << clockrate << endl;

    // cout << "< Info > Set High for " << hightime << " seconds" << endl;
    this->setval_gpio("1");
    usleep(hightime *  1000000);
    // cout << "< Info > Set Low  for " << lowtime << " seconds" << endl;
    this->setval_gpio("0");
    usleep(lowtime * 1000000);

    return 0;
}



int GPIOHandle::createZeroSquareSignal( float clockrate) {


    float periodendauer_s = 1 / clockrate;
    cout << "< Info > createZeroSignal gpionum: " << this->sGPIO  << " Periodendauer: " << periodendauer_s << " Taktrate: " << clockrate << endl;
    this->setval_gpio("0");
    usleep(clockrate *  1000000);

    return 0;
}

int GPIOHandle::blink(int delta) {

    cout << "< Info > GPIOHandle blink " << endl;
    usleep(delta);  // wait for 0.5 seconds
    this->setval_gpio("1"); // turn LED ON
    cout << "< Info > GPIOHandle blink LED ON" << endl;
    usleep(delta);  // wait for 0.5 seconds
    this->setval_gpio("0"); // turn LED ON
    cout << "< Info > GPIOHandle blink LED OFF" << endl;
    usleep(delta);  // wait for 0.5 seconds
}



int GPIOHandle::export_gpio()
{

    cout << "< Info > GPIOHandle::export_gpio : " << endl;
    string export_str = "/sys/class/gpio/export";
    ofstream exportgpio(export_str.c_str()); // Open "export" file. Convert C++ string to C string. Required for all Linux pathnames
    if (exportgpio < 0){
        cout << " OPERATION FAILED: Unable to export GPIO"<< this->sGPIO <<" ."<< endl;

    }

    exportgpio << this->sGPIO ; //write GPIO number to export
    exportgpio.close(); //close export file

    return 0;

}

int GPIOHandle::unexport_gpio()
{
     cout << "< Info > GPIOHandle::unexport_gpio : " << sGPIO << endl;
    string unexport_str = "/sys/class/gpio/unexport";
    ofstream unexportgpio(unexport_str.c_str()); //Open unexport file
    if (unexportgpio < 0){
        cout << " OPERATION FAILED: Unable to unexport GPIO"<< this->sGPIO <<" ."<< endl;

    }

    unexportgpio << this->sGPIO ; //write GPIO number to unexport
    unexportgpio.close(); //close unexport file
    return 0;

}

int GPIOHandle::setdir_gpio(std::string dir) {
    cout << "< Info > GPIOHandle::setdir_gpio direction: " << dir << endl;

    usleep(50000);

    string setdir_str ="/sys/class/gpio/gpio" + this->sGPIO + "/direction";
    // cout << "< Debug > setdir_str: " << setdir_str << endl;
    ofstream setdirgpio(setdir_str.c_str()); // open direction file for gpio
    // cout << "< Debug > setdirgpio: " << setdirgpio << endl;
    if (setdirgpio == 0){
        cout << " OPERATION FAILED: Unable to set direction of GPIO"<< this->sGPIO <<" ."<< endl;

    }

    setdirgpio << dir; //write direction to direction file
    setdirgpio.close(); // close direction file
    return 0;


}

int GPIOHandle::setval_gpio(string val)
{
    //cout << "< Info > GPIOHandle::setval_gpio pin: " << this->sGPIO << " .value: " << val << endl;

    string setval_str = "/sys/class/gpio/gpio" + this->sGPIO + "/value";
    ofstream setvalgpio(setval_str.c_str()); // open value file for gpio
    if (setvalgpio < 0){
        cout << " OPERATION FAILED: Unable to set the value of GPIO"<< this->sGPIO <<" ."<< endl;

    }

    setvalgpio << val ;//write value to value file
    setvalgpio.close();// close value file
    return 0;

}

int GPIOHandle::getval_gpio(string& val){

    string getval_str = "/sys/class/gpio/gpio" + this->sGPIO + "/value";
    ifstream getvalgpio(getval_str.c_str());// open value file for gpio
    if (getvalgpio < 0){
        cout << " OPERATION FAILED: Unable to get value of GPIO"<< this->sGPIO <<" ."<< endl;
        return -1;
    }

    getvalgpio >> val ;  //read gpio value


    if(val != "0")
        val = "1";
    else
        val = "0";

    getvalgpio.close(); //close the value file
    return 0;
}

void GPIOHandle::sayHello(){
    cout << "< Info > GPIOHandle started" << endl;
}

void GPIOHandle::initializeGPIO(){

}


std::string GPIOHandle::getsGPIO(){

    return sGPIO;

}
int GPIOHandle::getiGPIO(){

    return iGPIO;

}