
#ifndef _SAKURAALPHA_H_
#define _SAKURAALPHA_H_

#include <Wire.h>
#include <SPI.h>

#define SAKURACC_SLAVE_ADDR 0x4F

#define CMD_NETWORKSTATUS  0x01

#define CMD_UPDATED     0x10
#define CMD_UNTRANSMITTED 0x11
#define CMD_TXCH_STATUS    0x12
#define CMD_RXCH_STATUS   0x13

#define CMD_READ      0x20

#define CMD_WRITE     0x30

#define CMD_TRANSMIT    0x40


#define TRANSMIT_NONE   0x00
#define TRANSMIT_AUTO   0x01
#define TRANSMIT_ONCE   0x02

#define TYPE_INT32      'i'
#define TYPE_UINT32     'I'
#define TYPE_INT64      'l'
#define TYPE_UINT64     'L'
#define TYPE_FLOAT      'f'
#define TYPE_DOUBLE     'd'
#define TYPE_BYTES      'b'


#define CH_SIZE 128

#define TX_STATUS_NONE    0x00
#define TX_STATUS_MODIFIED  0x01

#define RX_STATUS_NONE    0x00
#define RX_STATUS_UPDATED 0x01

class SakuraAlpha {
protected:

    virtual void begin(){}
    virtual void end(){}

    virtual void sendByte(uint8_t data){}
    virtual void finishSending(){}

    virtual void startReceiving(uint16_t num_recieve){}
    virtual uint8_t receiveByte(){}
    virtual void finishReceiving(){}

public:

    void writeChannel(uint8_t ch, int32_t data) {
        uint8_t *p = (uint8_t *)&data;

        begin();

        sendByte(CMD_WRITE);
        sendByte(ch);
        sendByte(TYPE_INT32);

        for ( int i = 0 ; i < 4 ; i++ ) {
            sendByte(p[i]);
        }

        finishSending();
        end();
    }

    void writeChannel(uint8_t ch, uint32_t data) {
        uint8_t *p = (uint8_t *)&data;

        begin();

        sendByte(CMD_WRITE);
        sendByte(ch);
        sendByte(TYPE_UINT32);

        for ( int i = 0 ; i < 4 ; i++ ) {
            sendByte(p[i]);
        }

        finishSending();
        end();
    }

    void writeChannel(uint8_t ch, int64_t data) {
        uint8_t *p = (uint8_t *)&data;

        begin();

        sendByte(CMD_WRITE);
        sendByte(ch);
        sendByte(TYPE_INT64);

        for ( int i = 0 ; i < 8 ; i++ ) {
            sendByte(p[i]);
        }

        finishSending();
        end();
    }

    void writeChannel(uint8_t ch, uint64_t data) {
        uint8_t *p = (uint8_t *)&data;

        begin();

        sendByte(CMD_WRITE);
        sendByte(ch);
        sendByte(TYPE_UINT64);

        for ( int i = 0 ; i < 8 ; i++ ) {
            sendByte(p[i]);
        }

        finishSending();
        end();
    }

    void writeChannel(uint8_t ch, float data) {
        uint8_t *p = (uint8_t *)&data;

        begin();

        sendByte(CMD_WRITE);
        sendByte(ch);
        sendByte(TYPE_FLOAT);

        for ( int i = 0 ; i < 4 ; i++ ) {
            sendByte(p[i]);
        }

        finishSending();
        end();
    }

    void writeChannel(uint8_t ch, double data) {
        uint8_t *p = (uint8_t *)&data;

        begin();

        sendByte(CMD_WRITE);
        sendByte(ch);
        sendByte(TYPE_DOUBLE);

        for ( int i = 0 ; i < 8 ; i++ ) {
            sendByte(p[i]);
        }

        finishSending();
        end();
    }

    void writeChannel(uint8_t ch, uint8_t *data) {
        uint8_t *p = (uint8_t *)data;

        begin();

        sendByte(CMD_WRITE);
        sendByte(ch);
        sendByte(TYPE_BYTES);

        for ( int i = 0 ; i < 8 ; i++ ) {
            sendByte(p[i]);
        }

        finishSending();
        end();
    }

    void readChannel(uint8_t ch, char *type, uint8_t *data) {
        uint8_t *p = (uint8_t *)&data;

        begin();

        sendByte(CMD_READ);
        sendByte(ch);
        finishSending();

        startReceiving(9);
        *(byte *)type = receiveByte();

        for ( int i = 0 ; i < 8 ; i++ ) {
            data[i] = receiveByte();
        }
        finishReceiving();

        end();

    }

    void transmit(uint8_t mode){
        begin();
        sendByte(CMD_TRANSMIT);
        sendByte(mode);

        finishSending();
        end();
    }

    uint8_t getTxChannelStatus(uint8_t ch){
        uint8_t status=0xff;

        begin();

        sendByte(CMD_TXCH_STATUS);
        sendByte(ch);
        finishSending();

        startReceiving(1);
        status = receiveByte();
        finishReceiving();

        end();
        return status;
    }

    uint8_t getRxChannelStatus(uint8_t ch){
        uint8_t status=0xff;

        begin();

        sendByte(CMD_RXCH_STATUS);
        sendByte(ch);
        finishSending();

        startReceiving(1);
        status = receiveByte();
        finishReceiving();

        end();
        return status;
    }

    int getUpdatedChannels(uint8_t *buff, uint8_t len){
        int num = 0;

        begin();
        sendByte(CMD_UPDATED);
        finishSending();

        delayMicroseconds(100);

        startReceiving(len+1);
        num = receiveByte();

        for( int i=0; i<num; i++){
            buff[i]=receiveByte();
        }

        finishReceiving();

        end();
        return num;
    }

    int getUntransmittedChannels(uint8_t *buff, uint8_t len){
        int num = 0;

        begin();
        sendByte(CMD_UNTRANSMITTED);
        finishSending();

        delayMicroseconds(100);

        startReceiving(len+1);
        num = receiveByte();

        for( int i=0; i<num; i++){
            buff[i]=receiveByte();
        }

        finishReceiving();

        end();
        return num;
    }
    
    uint8_t getNetworkStatus(){
        uint8_t status=0x00;
        
        begin();
        
        sendByte(CMD_NETWORKSTATUS);
        finishSending();
        
        startReceiving(1);
        status = receiveByte();
        finishReceiving();
        
        end();
        return status;
    }

};

class SakuraAlphaI2C : public SakuraAlpha{

protected:
    void begin(){
        Wire.beginTransmission(SAKURACC_SLAVE_ADDR);
    }
    void end(){

    }

    void sendByte(uint8_t data){
        Wire.write(data);
    }
    void finishSending(){
        Wire.endTransmission();
    }

    void startReceiving(uint16_t num_recieve){
        Wire.requestFrom(SAKURACC_SLAVE_ADDR, num_recieve);
    }
    uint8_t receiveByte(){
        if( Wire.available() ) return Wire.read();

        return 0;
    }
    void finishReceiving(){

    }

public:
    SakuraAlphaI2C(){
        Wire.begin();
    }
};

class SakuraAlphaSPI : public SakuraAlpha{

protected:

    int cs;

    void begin(){
        digitalWrite(cs, LOW);
    }
    void end(){
        digitalWrite(cs, HIGH);
        delayMicroseconds(10);
    }

    void sendByte(uint8_t data){
        delayMicroseconds(10);
        SPI.transfer(data);
    }
    void finishSending(){
    }

    void startReceiving(uint16_t num_recieve){
    }
    uint8_t receiveByte(){
        delayMicroseconds(10);
        return SPI.transfer(0x00);
    }
    void finishReceiving(){
    }


public:
    SakuraAlphaSPI(int _cs){
        cs = _cs;

        SPI.begin();
        pinMode(cs, OUTPUT);
    }
};

#endif /* _SAKURAALPHA_H_ */
