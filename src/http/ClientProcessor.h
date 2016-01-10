#ifndef LIGHTER_CLIENTPROCESSOR_H
#define LIGHTER_CLIENTPROCESSOR_H

#define INPUT_BUFFER_SIZE 1024


class ClientProcessor {
public:
    void process();
    void setClient(int client);

private:
    int client;
};


#endif //LIGHTER_CLIENTPROCESSOR_H
