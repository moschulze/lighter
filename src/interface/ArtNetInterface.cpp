#include "ArtNetInterface.h"
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void ArtNetInterface::sendDmxUniverse(DmxUniverse *universe) {
    char buffer[18+512];

    buffer[0] = 'A';
    buffer[1] = 'r';
    buffer[2] = 't';
    buffer[3] = '-';
    buffer[4] = 'N';
    buffer[5] = 'e';
    buffer[6] = 't';
    buffer[7] = 0x00;

    //OpCode
    buffer[8] = 0x00;
    buffer[9] = 0x50;

    //ProtocolVersion
    buffer[10] = 0x00;
    buffer[11] = 0x0E;

    //SequenceNumber
    buffer[12] = this->sequenceNumber;

    //InputPort
    buffer[13] = 0x01;

    //PortAddress
    int portAddress = 0;
    buffer[14] = (char)portAddress;
    buffer[15] = (char)(portAddress >> 8);

    //Length
    buffer[16] = (char) (512 >> 8);
    buffer[17] = (char) 512;

    //Data
    for(int i = 0; i < 512; i++) {
        buffer[i+18] = (char) universe->getSlot(i);
    }

    if(sendto(sock, buffer, sizeof(buffer), 0, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen) == -1) {
        //Todo error handling
    }

    if(this->sequenceNumber == 0xFF) {
        this->sequenceNumber = 0;
    } else {
        this->sequenceNumber++;
    }
}

void ArtNetInterface::init() {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    int r = getaddrinfo(this->address.c_str(), std::to_string(this->port).c_str(), &hints, &f_addrinfo);
    if(r != 0 || f_addrinfo == NULL)
    {
        //Todo error handling
    }
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock == -1) {
        //Todo error handling
    }
    int enable = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));
}
