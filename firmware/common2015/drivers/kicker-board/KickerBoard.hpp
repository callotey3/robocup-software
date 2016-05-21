#pragma once

#include <mbed.h>
#include <string>
#include "AVR910.hpp"

// ATtiny84a
#define AVR_FAMILY_MASK 0xF0
#define AVR_FAMILY_ID 0x90
#define ATTINY84A_DEVICE_ID 0x0C
#define ATTINY84A_PAGESIZE 32  // Size in words (word = 2 bytes)
#define ATTINY84A_NUM_PAGES 128

/**
 * @brief A class for interfacing with the kicker board, which is based on an
 * AVR chip.
 */
class KickerBoard : public AVR910 {
public:
    /**
     * @brief Constructor for KickerBoard
     *
     * @param sharedSPI A pointer to the shared spi bus
     * @param nReset mbed pin for not reset line on the ISP interface.
     * @param progFilename Path to kicker program binary file that will be
     *     loaded by the flash() method
     */
    KickerBoard(std::shared_ptr<SharedSPI> sharedSPI, PinName nCs,
                PinName nReset, const std::string& progFilename);

    /**
     * @brief Reflashes the program on the kicker board MCU with the file
     *     specified in the constructor.
     *
     * @param onlyIfDifferent If true, compares the program file to the MCU's
     *     flash and only goes through the flashing process if they're
     *     different.
     * @param verbose If verbose, debug log messages are printed to stdout
     * @return True if flashing was successful
     */
    bool flash(bool onlyIfDifferent = true, bool verbose = false);

    /**
     * @brief Sends the KickerBoard a command to kick for the allotted time in
     *     in milliseconds. This roughly corresponds to kick strength.
     *
     * @param time Millisecond kick time, can only range from 0 to 16 ms
     */
    void kick(uint8_t time);

    /**
     * @brief Sends the KickerBoard a command to chip for the allotted time in
     *     in milliseconds. This roughly corresponds to chip strength.
     *
     * @param time Millisecond chip time, can only range from 0 to 16 ms
     */
    void chip(uint8_t time);

    /**
     * @brief Reads the charge voltage back from the KickerBoard.
     *
     * @return Voltage between 0 (Kicker GND) and 255 (Kicker VD)
     */
    uint8_t read_voltage();

    /**
     * @brief Sets the charge pin (to high)
     */
    void charge();

    /**
     * @brief Clears the charge pin
     */
    void stop_charging();

protected:
    /**
     * @brief Uses the given function to check if it's return value equals the
     * expected value.
     *
     * @return True if the return value was the expected value, false otherwise
     */
    bool verify_param(const char* name, char expected,
                      int (AVR910::*paramMethod)(), char mask = 0xFF,
                      bool verbose = false);

private:
    std::string _filename;

    void send_to_kicker(const uint8_t cmd, const uint8_t arg);
};