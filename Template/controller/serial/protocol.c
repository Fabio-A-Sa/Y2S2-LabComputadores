#include "protocol.h"

int player;
Queue *send_queue;
Queue *recv_queue;

int init_protocol() {
    send_queue = create_queue();
    recv_queue = create_queue();
    return handshake();
}

int handshake() {
    //TODO -> chamar isto no setup() das coisas
    player = 1;
    return 0;
}

int exit_protocol() {
    //TODO -> protocolo de término de ligação
    destroy_queue(send_queue);
    destroy_queue(recv_queue);
    return 0;
}
