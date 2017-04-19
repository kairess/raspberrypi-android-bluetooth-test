#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>

int main() {
    int l2cap_socket, hci_device_id, hci_socket, hci_device_address, client_socket, len, afds;

    struct sockaddr_l2 l2cap_address = { 0 };
    struct fd_set afds = { 0 };
    char buffer[1024] = { 0 };

    /* create L2CAP socket, and bind it to the local adapter */
    l2cap_socket = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

    hci_device_id = hci_get_route(NULL);
    hci_socket = hci_open_dev(hci_device_id);
    memset(&l2cap_address, sizeof(l2cap_address));
    l2cap_address.l2_family = AF_BLUETOOTH;
    l2cap_address.l2_bdaddr = hci_device_address;
    l2cap_address.l2_cid = htobs(ATT_CID);

    bind(l2cap_socket, (struct sockaddr*)&l2cap_address, sizeof(l2cap_address));
    listen(l2cap_socket, 1);

    while (1) {
        /* now select and accept() client connections. */
        select(l2cap_socket + 1, &afds, NULL, NULL, &tv);
        client_socket = accept(l2cap_socket, (struct sockaddr *)&l2cap_address, &len);

        /* you can now read() what the client sends you */
        int ret = read(client_socket, buffer, sizeof(buffer));
        printf("data len: %d\n", ret);
        for (i = 0; i < ret; i++) {
            printf("%02x", ((int)buffer[i]) & 0xff);
        }
        printf("\n");
        close(client_socket);
    }
}