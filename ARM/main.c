/*
 * main.c
 *
 *  Created on: Jul 22, 2009
 *      Author: steven
 * This is the main program on the ARM board
 */
#include "main.h"


int main()
{
		int res,fd,i;
		unsigned char buf[9];
		Packet  CurrentPacket;
		i=0;
		fd=0;
		res=0;

		for(i=0; i<9; i++)
		{
			buf[i]=0;
		}

		struct termios newtio;


		/* 	Open port for reading and writing and not as controlling tty
			because we don't want the program to end because of line noise sends CTRL-C.
		 */
		//fd = open(PORT, O_RDWR | O_NOCTTY | O_NONBLOCK);
		fd = open(PORT, O_RDWR | O_NOCTTY | O_NDELAY);
		if (fd <0) {perror(PORT); exit(-1); }

		fcntl(fd, F_SETFL, 0);

		bzero(&newtio, sizeof(newtio));/* clear struct for new port settings */
		/*
		   BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
		   CS8     : 8n1 (8bit,no parity,1 stopbit)
		   CLOCAL  : local connection, no modem contol
		   CREAD   : enable receiving characters
		 */
		newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;

		/*
		 * iflag  :  Processes input before the read()
		  IGNPAR  : ignore bytes with parity errors
		  ICRNL   : map CR to NL (otherwise a CR input on the other computer
					will not terminate input)
		  otherwise make device raw (no other input processing)
		*/

		 //Raw output.
		newtio.c_oflag &= ~OPOST;

		/* set input mode (non-canonical, no echo,...) */
	    newtio.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);

		newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
		newtio.c_cc[VMIN]     = 9;   /* blocking read until 1 chars received */


		/*
		  now clean the modem line and activate the settings for the port
		*/

		tcflush(fd, TCIFLUSH);
		tcsetattr(fd,TCSANOW,&newtio);


		while (1)
		{
			/* loop for input */
			res = read(fd,buf,9);   /* returns after X chars have been input */
			//printf("%c",buf[0]);
			CurrentPacket = Incoming_Packet_Parser(buf, sizeof(buf));
			print_packet(CurrentPacket);
			MeleeSwing(CurrentPacket);
		}
		return 0;




}
void print_packet(Packet Current_Packet)
{
	printf("%x, ",Current_Packet.COM);
	printf("%x, ",Current_Packet.Team);
	printf("%x, ",Current_Packet.Player);
	printf("%x, ",Current_Packet.Subroutine);
	printf("%x, ",Current_Packet.Process);
	printf("%x, ",Current_Packet.ReturnValue);
	printf("%x\n",Current_Packet.Checksum);
}
