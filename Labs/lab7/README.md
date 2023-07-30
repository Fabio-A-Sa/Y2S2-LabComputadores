# Serial Port

## Tópicos

- [Setup do Minix](#setup-do-minix)
- [TODO](#todo)

## Setup do Minix

(Drafts de https://paginas.fe.up.pt/~pfs/aulas/lcom2018/proj/ser_port_test_setup.html)

Machine > Clone > Full Clone

New VM > Settings > Network > Adva nced > Port Forwarding > Na porta colocar um qualquer número superior a 1023 e diferente de 2222 (ou do que lá estava)

Machine > Settings > Serial Port > Port 1 > Enable Serial Port > Port Mode: TCP > Path/Address: 4321 (ou qualquer número entre 1023 e 65536) > Deixar Connect to existing pipe/socket desativado

Na outra fazer o mesmo mas selecionar o "connect to existing pipe/socket" e fazer localhost:<porta selecionada antes>

