#ifndef MODBUS_H
#define MODBUS_H


class Modbus
{
    public:
        Modbus();
        virtual ~Modbus();
        void WriteBlock ();
        void ReadBlock ();

    protected:

    private:
};

#endif // MODBUS_H
