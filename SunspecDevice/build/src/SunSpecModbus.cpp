#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include "include/SunSpecModbus.h"

SunSpecModbus::SunSpecModbus (std::map <std::string, std::string>& configs)
    : model_path_(configs["path"]), sunspec_key_(stoul(configs["key"])) {
    // create modbus context pointer and connect to device at
    // given ip address and port number.
    const char* ip = configs["ip"].c_str ();
    unsigned int port = stoul(configs["port"]);
    context_ptr_ = modbus_new_tcp(ip, port);
    if (modbus_connect(context_ptr_) == -1) {
        std::cout << "[ERROR] : " << modbus_strerror(errno) << '\n';
    }

    unsigned int did = stoul(configs["did"]);
    SunSpecModbus::Query (did);
}

SunSpecModbus::~SunSpecModbus () {
    std::cout << "Closing modbus connection" << std::endl;
    models_.clear ();
    int status = modbus_flush(context_ptr_);
    std::cout << "flush: " << status << std::endl;
    if (status == -1) {
        std::cout << "[ERROR]\t"
            << "Modbus Flush: " << modbus_strerror(errno) << '\n';
        modbus_flush(context_ptr_);
    }
    modbus_close(context_ptr_);
    modbus_free(context_ptr_);
}

// Querry
// - Read all available registers to find sunspec compliant blocks.
// - The first step is to read the first two register and see if the modbus
// - device is a sunspec complient device.
// - Then traverse register array and find sunspec DID values and create the
// - sunspec model.
// - If it is not a sunspec complient device then use the given did to simulate
// - a sunspec model.
void SunSpecModbus::Query (unsigned int did) {
    uint16_t sunspec_id[2];
    // TODO (TS): sunspec states the holding register start can be
    // - 30, 40, or 50,000.
    // - 40000 is the preferece starting location
    unsigned int id_offset = 40000;
    SunSpecModbus::ReadRegisters(id_offset, 2, sunspec_id);
    uint32_t device_key = (sunspec_id[1] << 16) | sunspec_id[0];
    std::cout << "Sunspec ID: " << device_key << std::endl;
    // If match then increment offset by id length and read next two registers
    // to get DID and lenth of sunspec block
    if (device_key == sunspec_key_) {
        id_offset += 2;
        uint16_t did_and_length[2];
        SunSpecModbus::ReadRegisters(id_offset, 2, did_and_length);
        std::string filepath = SunSpecModbus::FormatModelPath (
                did_and_length[0]
        );

        // while the next model file exists, create model and increment offset
        struct stat buffer;  // used to check if file exists
        while ((stat (filepath.c_str (), &buffer) == 0)) {
            std::shared_ptr <SunSpecModel> model (
                new SunSpecModel (did_and_length[0], id_offset, filepath)
            );
            models_.push_back (std::move (model));
            std::map <std::string, std::string> props = SunSpecModbus::ReadBlock(did_and_length[0]);
            id_offset += did_and_length[1] + 2; // block length not model length
            SunSpecModbus::ReadRegisters(id_offset, 2, did_and_length);
            filepath = SunSpecModbus::FormatModelPath (did_and_length[0]);
        }

    } else {
        id_offset += 2;
        std::string filepath = SunSpecModbus::FormatModelPath (did);
        std::shared_ptr <SunSpecModel> model (
            new SunSpecModel (did, id_offset, filepath)
        );
        models_.push_back (std::move (model));
    }

}

// Read Registers
// - the register array is passed to the function as a pointer so the
// - modbus method call can operate on them.
void SunSpecModbus::ReadRegisters (unsigned int offset,
                                   unsigned int length,
                                   uint16_t *reg_ptr) {
    unsigned int reg_left = length;
    unsigned int new_offset = offset;
    int status;
    std::vector <uint16_t> a_block;

    // while the length of registers to read is greater than 100, read in each
    // block of 100 and append to vector.
    while (reg_left > 100) {
        uint16_t partial_reg[100];
        status = modbus_read_registers (context_ptr_,
                                            new_offset,
                                            100,
                                            partial_reg);
        if (status == -1) {
            std::cout << "[ERROR]\t"
                << "Read Registers: " << modbus_strerror(errno) << '\n';
                *reg_ptr = 0;
            status = modbus_flush(context_ptr_);
            if (status == -1) {
                std::cout << "[ERROR]\t"
                    << "Modbus Flush: " << modbus_strerror(errno) << '\n';
                modbus_flush(context_ptr_);
            }
        }

        // convert raw registers to vector to pass to other functions
        std::vector <uint16_t> b_block (partial_reg, partial_reg + 100);
        a_block.insert(
            std::end(a_block), std::begin(b_block), std::end(b_block)
        );

        // decrement registers left to read and increment offset to read the
        // next block of registers
        reg_left -= 100;
        new_offset += 100;
    }

    // read remaining register block once less than 100
    uint16_t partial_reg[reg_left];
    status = modbus_read_registers (context_ptr_,
                                        new_offset,
                                        reg_left,
                                        partial_reg);
    if (status == -1) {
        std::cout << "[ERROR]\t"
            << "Read Registers: " << modbus_strerror(errno) << '\n';
            *reg_ptr = 0;
        status = modbus_flush(context_ptr_);
        if (status == -1) {
            std::cout << "[ERROR]\t"
                << "Modbus Flush: " << modbus_strerror(errno) << '\n';
            modbus_flush(context_ptr_);
        }
    }
    // convert raw registers to vector to pass to other functions
    std::vector <uint16_t> b_block (partial_reg, partial_reg + reg_left);
    a_block.insert(
        std::end(a_block), std::begin(b_block), std::end(b_block)
    );

    // copy full register block vector into the register array
    memcpy(reg_ptr, &a_block.front (), a_block.size () * sizeof (uint16_t));
}

// Write Registers
// - the registers to write are passed by reference to reduce memory
void SunSpecModbus::WriteRegisters (unsigned int offset,
                                    unsigned int length,
                                    const uint16_t *reg_ptr) {
    // convert raw registers to vector to pass to other functions
    std::vector <uint16_t> block (reg_ptr, reg_ptr + length);
    unsigned int new_offset = offset;
    int status;
    int ctr = 1;
    int max_len = 50;
    while (ctr*max_len < length) {
        uint16_t temp_reg[max_len];
        std::cout << "HERE" << std::endl;
        // copy register block vector into the register array
        memcpy(temp_reg, &block.front () + ctr*max_len, max_len * sizeof (uint16_t));
        status = modbus_write_registers (context_ptr_,
                                         new_offset,
                                         max_len,
                                         temp_reg);
        if (status == -1) {
            std::cout << "[ERROR]\t"
                << "Write Registers: " << modbus_strerror(errno) << '\n';
            status = modbus_flush(context_ptr_);
            if (status == -1) {
                std::cout << "[ERROR]\t"
                    << "Modbus Flush: " << modbus_strerror(errno) << '\n';
                modbus_flush(context_ptr_);
            }
        }

        new_offset += max_len;
        ctr++;
    }

    unsigned int reg_left = length - ctr*max_len;
    uint16_t temp_reg[reg_left];
    // copy register block vector into the register array
    memcpy(temp_reg, &block.front () + ctr*max_len, reg_left * sizeof (uint16_t));
    status = modbus_write_registers (context_ptr_,
                                     new_offset,
                                     reg_left,
                                     temp_reg);
    if (status == -1) {
        std::cout << "[ERROR]\t"
            << "Write Registers: " << modbus_strerror(errno) << '\n';
        status = modbus_flush(context_ptr_);
        if (status == -1) {
            std::cout << "[ERROR]\t"
                << "Modbus Flush: " << modbus_strerror(errno) << '\n';
            modbus_flush(context_ptr_);
        }
    }
}

std::map <std::string, std::string> SunSpecModbus::ReadBlock (unsigned int did){
    for (const auto model : models_) {
        if (*model == did) {
            // read register block
            unsigned int offset = model->GetOffset ();
            unsigned int length = model->GetLength ();
            uint16_t raw[length];
            SunSpecModbus::ReadRegisters (offset, length, raw);

            // convert raw registers to vector to pass to other functions
            std::vector <uint16_t> block (raw, raw + length);
            return model->BlockToPoints (block);
        }
    }
    std::cout << "[ERROR]\t" << "Read Block: model not found\n";
    std::map <std::string, std::string> empty_map;
    return empty_map;
}

void SunSpecModbus::WriteBlock (unsigned int did,
                                std::map <std::string, std::string>& points) {
    for (const auto model : models_) {
        if (*model == did) {
            // read register block
            unsigned int offset = model->GetOffset ();
            unsigned int length = model->GetLength ();
            std::vector <uint16_t> block = model->PointsToBlock (points);

            // convert vector to array for writing to modbus registers
            uint16_t* raw = block.data();
            SunSpecModbus::WriteRegisters(offset, length, raw);
            return;
        }
    }
    std::cout << "[ERROR]\t" << "Write Block: model not found\n";
}

// Write Point
// - using the same format as writing a block, it will pass a string map
// - to the points to block function and only write the desired register
void SunSpecModbus::WritePoint (unsigned int did,
                                std::map <std::string, std::string>& points) {
    for (const auto model : models_) {
        if (*model == did) {
            // read register block
            unsigned int offset = model->GetOffset ();
            unsigned int length = model->GetLength ();
            std::vector <uint16_t> block = model->PointsToBlock (points);

            for (unsigned int i = 0; i < length; i++) {
                if (block[i] > 0) {
                    std::cout << offset + i << " : " << block[i] << std::endl;
                }
            }
            //convert vector to array for writing to modbus registers
            //uint16_t* raw = block.data();
            //SunSpecModbus::WriteRegisters(offset, length, raw);
            //return;
            return;
        }
    }
    std::cout << "[ERROR]\t" << "Write Point: model not found\n";
}

std::string SunSpecModbus::FormatModelPath (unsigned int did) {
    // create filename using a base path, then pad the did number and append
    // to the base path. The sunspec models are provided as xml so that will be
    // the file type that is appended ot the end of the filename.
    std::stringstream ss;
    ss << model_path_ << "smdx_";
    ss << std::setfill ('0') << std::setw(5) << did;
    ss << ".xml";
    return ss.str();
}
