all: a3server1 a3client1 a3client2 a3server2
a3server1: a3server1.c
	gcc a3server1.c -lnsl -lpthread -o a3server1
a3client1: a3client1.c
	gcc a3client1.c -o a3client1
a3server2: a3server2.c
	gcc a3server2.c -lnsl -lpthread -o a3server2
a3client2: a3client2.c
	gcc a3client2.c -o a3client2

